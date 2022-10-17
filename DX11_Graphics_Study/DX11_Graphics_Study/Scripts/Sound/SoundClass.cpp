#include "../DxDefine.h"
#include "SoundClass.h"

SoundClass::SoundClass()
{
	directSound = nullptr;
	primaryBuffer = nullptr;
	secondaryBuffer1 = nullptr;
}

SoundClass::SoundClass(const SoundClass& other)
{
}

SoundClass::~SoundClass()
{
}

bool SoundClass::InitSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	result = DirectSoundCreate8(nullptr, &directSound, nullptr);
	if (FAILED(result))
		return false;

	result = directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = nullptr;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = directSound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, nullptr);
	if (FAILED(result))
		return false;

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	result = primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
		return false;

	return true;
}

void SoundClass::ShutDownSound()
{
	if (primaryBuffer)
	{
		primaryBuffer->Release();
		primaryBuffer = nullptr;
	}

	if (directSound)
	{
		directSound->Release();
		directSound = nullptr;
	}

}

bool SoundClass::LoadWaveFile(char* fileName, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData, *bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
		return false;

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
		return false;

	if (!DataVerification(waveFileHeader))
		return false;

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, nullptr);
	if (FAILED(result))
		return false;

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
		return false;

	tempBuffer->Release();
	tempBuffer = nullptr;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
		return false;

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, nullptr, nullptr, 0);
	if (FAILED(result))
		return false;

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, nullptr, 0);
	if (FAILED(result))
		return false;

	delete[] waveData;
	waveData = nullptr;

	return true;
}

bool SoundClass::DataVerification(WaveHeaderType waveFileHeader)
{
	if (waveFileHeader.chunkId[0] != 'R' || waveFileHeader.chunkId[1] != 'I' ||
		waveFileHeader.chunkId[2] != 'F' || waveFileHeader.chunkId[3] != 'F')
		return false;

	if (waveFileHeader.format[0] != 'W' || waveFileHeader.format[1] != 'A' ||
		waveFileHeader.format[2] != 'V' || waveFileHeader.format[3] != 'E')
		return false;

	if (waveFileHeader.subChunkId[0] != 'f' || waveFileHeader.subChunkId[1] != 'm' ||
		waveFileHeader.subChunkId[2] != 't' || waveFileHeader.subChunkId[3] != ' ')
		return false;

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		return false;

	if (waveFileHeader.numChannels != 2)
		return false;

	if (waveFileHeader.sampleRate != 44100)
		return false;

	if (waveFileHeader.bitsPerSample != 16)
		return false;

	if (waveFileHeader.dataChunkId[0] != 'd' || waveFileHeader.dataChunkId[1] != 'a' ||
		waveFileHeader.dataChunkId[2] != 't' || waveFileHeader.dataChunkId[3] != 'a')
		return false;

	return true;
}

void SoundClass::ShutDownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = nullptr;
	}
}

bool SoundClass::playWaveFile()
{
	HRESULT result;

	result = secondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result))
		return false;

	result = secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
		return false;

	result = secondaryBuffer1->Play(0, 0, 0);
	if (FAILED(result))
		return false;

	return true;
}

bool SoundClass::Init(HWND hwnd)
{
	bool result;

	result = InitSound(hwnd);
	if (!result)
		return false;

	result = LoadWaveFile((char*)"Resources/Sound/sound01.wav", &secondaryBuffer1);
	if (!result)
		return false;

	result = playWaveFile();
	if (!result)
		return false;

	return true;
}

void SoundClass::ShutDown()
{
	ShutDownWaveFile(&secondaryBuffer1);
	ShutDownSound();
}
