#include "Vertex.h"


Vertex::Vertex() {
	this->_xCoord = 0.0;
	this->_yCoord = 0.0;
	this->_zCoord = 0.0;
}

Vertex::Vertex(float x, float y, float z) {
	this->_xCoord = x;
	this->_yCoord = y;
	this->_zCoord = z;
}

void Vertex::addFace(Face * face) {
	this->faces.push_back(face);
}

vector<Face*> Vertex::getFaces() {
	return faces;
}

float Vertex::xCoord() {
	return this->_xCoord;
}

float Vertex::yCoord() {
	return this->_yCoord;
}

float Vertex::zCoord() {
	return this->_zCoord;
}

Vertex::~Vertex()
{
}
