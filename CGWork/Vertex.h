#pragma once
#include <vector>

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
	float xCoord();
	float yCoord();
	float zCoord();
	~Vertex();
};
