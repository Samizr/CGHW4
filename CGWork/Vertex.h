#pragma once
#include <vector>
#include "Vec4.h"
#include "Mat4.h"

using std::vector;
class Face;
class Vertex
{
private:
	float _xCoord, _yCoord, _zCoord;
	vector<Face*> faces;
public:
	Vertex();
	Vertex(float x, float y, float z);
	void addFace(Face* face);
	vector<Face*> getFaces();
	Vec4 calculateNormalTarget(Mat4 transformationMatrix);
	float xCoord();
	float yCoord();
	float zCoord();
	~Vertex();
};
