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
	Vec4* importedNormal;
public:
	Vertex();
	Vertex(float x, float y, float z);
	void addFace(Face* face);
	vector<Face*> getFaces();
	Vec4 calculateVertexNormalTarget(Mat4 transformationMatrix, bool invert);
	Vec4 getVec4Coords();
	Vec4* getNormal();
	void setNormal(Vec4* normal);
	float xCoord();
	float yCoord();
	float zCoord();
	~Vertex();
};
