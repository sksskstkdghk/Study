#pragma once
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef struct
{
    float x, y, z;
}VertexType;

typedef struct
{
    int v0, v1, v2, v3; //정점
    int t0, t1, t2, t3; //uv
    int n0, n1, n2, n3; //법선
}FaceType;

class ModelExporter
{
private:
    string filePath;
    int vertexCnt, indexCnt, normalCnt, faceCnt;

    ifstream modelFile;

public:
    ModelExporter();
    ~ModelExporter();

    bool ObjExport(bool haveUV = true);
    bool GetObjFile();
    bool ObjReadNWriteFileTexYes();
    bool ObjReadNWriteFileTexNO();
};

