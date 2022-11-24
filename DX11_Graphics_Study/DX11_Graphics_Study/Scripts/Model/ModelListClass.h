#pragma once

#include <time.h>

class ModelListClass
{
private:
	struct ModelInfoType
	{
		XMCOLOR color;
		XMFLOAT3 position;
	};

	int modelCount;
	ModelInfoType* modelInfoList;

public:
	ModelListClass();
	ModelListClass(const ModelListClass& other);
	~ModelListClass();

	bool Init(int count);
	void ShutDown();

	int GetModelCount();
	void GetData(int index, XMFLOAT3& position, XMCOLOR& color);
};

