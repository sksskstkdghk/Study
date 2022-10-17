#pragma once

#include <mmeapi.h>
#include <dsound.h>
#include <mmsystem.h>

class SoundClass
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	IDirectSound8* directSound;
	IDirectSoundBuffer* primaryBuffer;
	IDirectSoundBuffer8* secondaryBuffer1;

private:
	bool InitSound(HWND hwnd);
	void ShutDownSound();

	bool LoadWaveFile(char* fileName, IDirectSoundBuffer8** secondaryBuffer);
	bool DataVerification(WaveHeaderType waveFileHeader);
	void ShutDownWaveFile(IDirectSoundBuffer8** secondaryBuffer);

	bool playWaveFile();

public:
	SoundClass();
	SoundClass(const SoundClass& other);
	~SoundClass();

	bool Init(HWND hwnd);
	void ShutDown();
};

