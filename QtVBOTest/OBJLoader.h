#pragma once

#include <QVector3D>
#include <vector>

using namespace std;

class OBJLoader
{
protected:
	OBJLoader() {}

public:
	static void load(const char* filename, vector<float>& vertices, vector<float>& normals);
};

