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
	Vec4 calculateVertexNormalTarget(Mat4 transformationMatrix);
	Vec4 getVec4Coords();
	float xCoord();
	float yCoord();
	float zCoord();
	~Vertex();
};
