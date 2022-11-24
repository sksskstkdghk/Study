#include "../DxDefine.h"
#include "ModelListClass.h"

ModelListClass::ModelListClass()
{
	modelInfoList = nullptr;
}

ModelListClass::ModelListClass(const ModelListClass& other)
{

}

ModelListClass::~ModelListClass()
{

}

bool ModelListClass::Init(int count)
{
	float r, g, b;

	modelCount = count;

	modelInfoList = new ModelInfoType[modelCount];
	if (!modelInfoList)
		return false;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < modelCount; i++)
	{
		//랜덤 컬러 값
		r = (float)rand() / RAND_MAX;
		g = (float)rand() / RAND_MAX;
		b = (float)rand() / RAND_MAX;

		//세팅
		modelInfoList[i].color = XMFLOAT4(r, g, b, 1.0f);
		modelInfoList[i].position = XMFLOAT3
		(
			(((float)rand() - (float)rand()) / RAND_MAX) * 10.0f,
			(((float)rand() - (float)rand()) / RAND_MAX) * 10.0f,
			((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f
		);

		/*modelInfoList[i].position = XMFLOAT3
		(
			-100.0f + (i * 14.0f),
			-50.0f + (i * 7.0f),
			15.0f
		);*/
	}

	return true;
}

void ModelListClass::ShutDown()
{
	if (modelInfoList)
	{
		delete[] modelInfoList;
		modelInfoList = nullptr;
	}
}

int ModelListClass::GetModelCount()
{
	return modelCount;
}

void ModelListClass::GetData(int index, XMFLOAT3& position, XMCOLOR& color)
{
	position = modelInfoList[index].position;
	color = modelInfoList[index].color;
}
