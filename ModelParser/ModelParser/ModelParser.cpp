#include <iostream>
#include "Exporter/ModelExporter.h"

using namespace std;

int main()
{
    bool result;
    ModelExporter* exporter = new ModelExporter();

    result = exporter->ObjExport(false);
    if (!result)
    {
        cout << "모델 추출 실패\n";
    }
    else
    {
        cout << "모델 추출 성공\n";
    }

    delete exporter;
}