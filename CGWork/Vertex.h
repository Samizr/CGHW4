#pragma once
class Vertex
{
private:
	float _xCoord, _yCoord, _zCoord;

public:
	Vertex();
	Vertex(float x, float y, float z);
	float xCoord();
	float yCoord();
	float zCoord();
	~Vertex();
};
