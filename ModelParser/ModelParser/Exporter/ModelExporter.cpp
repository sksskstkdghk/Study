#include "ModelExporter.h"

ModelExporter::ModelExporter()
{
	filePath = "ModelFile/";

	vertexCnt = 0;
	indexCnt = 0;
	normalCnt = 0;
	faceCnt = 0;
}

ModelExporter::~ModelExporter()
{
	
}

bool ModelExporter::ObjExport(bool haveUV)
{
	bool result;

	result = GetObjFile();
	if (!result)
	{
		cout << "�� ������ �ҷ��� �� �����ϴ�.\n";
		return false;
	}

	if(haveUV)
		result = ObjReadNWriteFileTexYes();
	else
		result = ObjReadNWriteFileTexNO();

	if (!result)
	{
		cout << "�� ������ �б�, ���� ����\n";
		return false;
	}

	return true;
}

bool ModelExporter::GetObjFile()
{
	string fileName = "";

	cout << "�� �̸��� �����ּ���: ";
	cin >> fileName;

	filePath += fileName;

	modelFile.open(filePath);
	if (modelFile.fail())
	{
		cout << "\n�� ���� ����";
		return false;
	}

	return true;
}

//�ؽ��� ��ǥ�� ���� ���
bool ModelExporter::ObjReadNWriteFileTexNO()
{
	vector<VertexType*> vertices, normals;
	vector<FaceType*> faces;
	char input, trash;
	int cursor, count = 0;

	ofstream fout;

	//read
	modelFile.get(input);
	while (!modelFile.eof())
	{
		if (input == 'v')
		{
			modelFile.get(input);

			if (input == ' ')
			{
				vertices.emplace_back(new VertexType());
				cursor = vertices.size() - 1;

				modelFile >> vertices[cursor]->x >> vertices[cursor]->y >> vertices[cursor]->z;
				//�޼� ��ǥ��� ��ȯ
				vertices[cursor]->z *= -1.0f;
			}
			else if (input == 'n')
			{
				normals.emplace_back(new VertexType());
				cursor = normals.size() - 1;

				modelFile >> normals[cursor]->x >> normals[cursor]->y >> normals[cursor]->z;
				//�޼� ��ǥ��� ��ȯ
				normals[cursor]->z *= -1.0f;
			}
		}

		if (input == 'f')
		{
			modelFile.get(input);

			if (input == ' ')
			{
				faces.emplace_back(new FaceType());
				cursor = faces.size() - 1;

				modelFile >> faces[cursor]->v3 >> trash >> trash >> faces[cursor]->n3
					>> faces[cursor]->v2 >> trash >> trash >> faces[cursor]->n2
					>> faces[cursor]->v1 >> trash >> trash >> faces[cursor]->n1;

				count += 3;

				modelFile.get(input);

				if (input == ' ')
				{
					modelFile >> faces[cursor]->v0 >> trash >> trash >> faces[cursor]->n0;
					count += 3;
				}
			}
		}

		while (input != '\n')
		{
			modelFile.get(input);
		}

		modelFile.get(input);
	}

	//read end
	modelFile.close();

	//write
	fout.open("model.txt");

	fout << "Vertex Count: " << count << endl << endl;
	fout << "Data:" << endl << endl;

	for (int i = 0; i < faces.size() - 1; i++)
	{
		int v, n;

		v = faces[i]->v1 - 1;
		n = faces[i]->n1 - 1;

		fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
			<< -1 << ' ' << -1 << ' '
			<< normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

		v = faces[i]->v2 - 1;
		n = faces[i]->n2 - 1;

		fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
			<< -1 << ' ' << -1 << ' '
			<< normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

		v = faces[i]->v3 - 1;
		n = faces[i]->n3 - 1;

		fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
			<< -1 << ' ' << -1 << ' '
			<< normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

		if (faces[i]->v0 != 0)
		{
			v = faces[i]->v1 - 1;
			n = faces[i]->n1 - 1;

			fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
				<< -1 << ' ' << -1 << ' '
				<< normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

			v = faces[i]->v3 - 1;
			n = faces[i]->n3 - 1;

			fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
				<< -1 << ' ' << -1 << ' '
				<< normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

			v = faces[i]->v0 - 1;
			n = faces[i]->n0 - 1;

			fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
				<< -1 << ' ' << -1 << ' '
				<< normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;
		}
	}

	//write end
	fout.close();

	for (int i = 0; i < vertices.size(); i++)
		delete vertices[i];

	for (int i = 0; i < normals.size(); i++)
		delete normals[i];

	for (int i = 0; i < faces.size(); i++)
		delete faces[i];

	vertices.clear();
	normals.clear();
	faces.clear();

	return true;
}

bool ModelExporter::ObjReadNWriteFileTexYes()
{
	vector<VertexType*> vertices, texcoords, normals;
	vector<FaceType*> faces;
	char input, trash;
	int cursor, count = 0;

	ofstream fout;

	//read
	modelFile.get(input);
	while (!modelFile.eof())
	{
		if (input == 'v')
		{
			modelFile.get(input);

			if (input == ' ')
			{
				vertices.emplace_back(new VertexType());
				cursor = vertices.size() - 1;

				modelFile >> vertices[cursor]->x >> vertices[cursor]->y >> vertices[cursor]->z;
				//�޼� ��ǥ��� ��ȯ
				vertices[cursor]->z *= -1.0f;
			}
			else if (input == 't')
			{
				texcoords.emplace_back(new VertexType());
				cursor = texcoords.size() - 1;

				modelFile >> texcoords[cursor]->x >> texcoords[cursor]->y;
				//�޼� ��ǥ��� ��ȯ
				texcoords[cursor]->y *= -1.0f;
			}
			else if (input == 'n')
			{
				normals.emplace_back(new VertexType());
				cursor = normals.size() - 1;

				modelFile >> normals[cursor]->x >> normals[cursor]->y >> normals[cursor]->z;
				//�޼� ��ǥ��� ��ȯ
				normals[cursor]->z *= -1.0f;
			}
		}

		if (input == 'f')
		{
			modelFile.get(input);

			if (input == ' ')
			{
				faces.emplace_back(new FaceType());
				cursor = faces.size() - 1;

				//�ؽ��� ��ǥ�� ���� ���
				modelFile >> faces[cursor]->v3 >> trash >> faces[cursor]->t3 >> trash >> faces[cursor]->n3
						  >> faces[cursor]->v2 >> trash >> faces[cursor]->t2 >> trash >> faces[cursor]->n2
						  >> faces[cursor]->v1 >> trash >> faces[cursor]->t1 >> trash >> faces[cursor]->n1;

				count += 3;

				modelFile.get(input);
				if (input == ' ')
				{
					modelFile >> faces[cursor]->v0 >> trash >> faces[cursor]->t0 >> trash >> faces[cursor]->n0;
					count += 3;
				}
			}
		}

		while (input != '\n')
		{
			modelFile.get(input);
		}

		modelFile.get(input);
	}
	
	//read end
	modelFile.close();

	//write
	fout.open("model.txt");

	fout << "Vertex Count: " << (faces.size() * 3) << endl << endl;
	fout << "Data:" << endl << endl;

	for (int i = 0; i < faces.size() - 1; i++)
	{
		int v, t, n;

		v = faces[i]->v1 - 1;
		t = faces[i]->t1 - 1;
		n = faces[i]->n1 - 1;

		fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
			 << texcoords[t]->x << ' ' << texcoords[t]->y << ' '
			 << normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

		v = faces[i]->v2 - 1;
		t = faces[i]->t2 - 1;
		n = faces[i]->n2 - 1;

		fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
			 << texcoords[t]->x << ' ' << texcoords[t]->y << ' '
			 << normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

		v = faces[i]->v3 - 1;
		t = faces[i]->t3 - 1;
		n = faces[i]->n3 - 1;

		fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
			 << texcoords[t]->x << ' ' << texcoords[t]->y << ' '
			 << normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

		if (faces[i]->v0 != 0)
		{
			v = faces[i]->v1 - 1;
			t = faces[i]->t1 - 1;
			n = faces[i]->n1 - 1;

			fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
				 << texcoords[t]->x << ' ' << texcoords[t]->y << ' '
				 << normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

			v = faces[i]->v3 - 1;
			t = faces[i]->t3 - 1;
			n = faces[i]->n3 - 1;

			fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
				 << texcoords[t]->x << ' ' << texcoords[t]->y << ' '
				 << normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;

			v = faces[i]->v0 - 1;
			t = faces[i]->t0 - 1;
			n = faces[i]->n0 - 1;

			fout << vertices[v]->x << ' ' << vertices[v]->y << ' ' << vertices[v]->z << ' '
				 << texcoords[t]->x << ' ' << texcoords[t]->y << ' '
				 << normals[n]->x << ' ' << normals[n]->y << ' ' << normals[n]->z << endl;
		}
	}

	//write end
	fout.close();

	for (int i = 0; i < vertices.size(); i++)
		delete vertices[i];

	for (int i = 0; i < texcoords.size(); i++)
		delete texcoords[i];

	for (int i = 0; i < normals.size(); i++)
		delete normals[i];

	for (int i = 0; i < faces.size(); i++)
		delete faces[i];

	vertices.clear();
	texcoords.clear();
	normals.clear();
	faces.clear();

	return true;
}
