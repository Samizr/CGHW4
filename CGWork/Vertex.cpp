#include "Vertex.h"
#include "Face.h"

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

Vec4 Vertex::calculateNormalTarget(Mat4 transformationMatrix)
{
	Vec4 targetSum = Vec4();
	Vec4 vertexAsVector = transformationMatrix * Vec4(_xCoord, _yCoord, _zCoord, 1);
	for (Face* face : faces) {
		targetSum = targetSum + face->calculateNormalTarget(vertexAsVector, transformationMatrix);
	}
	targetSum[3] = 1;
	Vec4 finalNormal = (targetSum * (1.0 / faces.size())).normalize();
	finalNormal[3] = 1;
	//Mat4 translate = Mat4::Translate(Vec4(targetSum.xCoord(), targetSum.yCoord(), targetSum.zCoord(), 1));
	//Mat4 scaling = Mat4::Scale(Vec4(0.01, 0.01, 0.01, 1));

	return finalNormal;
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
