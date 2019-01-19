#include "Vertex.h"
#include "Face.h"

Vertex::Vertex() {
	this->_xCoord = 0.0;
	this->_yCoord = 0.0;
	this->_zCoord = 0.0;
	importedNormal = nullptr;
	validUV = false;
}

Vertex::Vertex(float x, float y, float z) {
	this->_xCoord = x;
	this->_yCoord = y;
	this->_zCoord = z;
	importedNormal = nullptr;
	validUV = false;
}

void Vertex::addFace(Face * face) {
	this->faces.push_back(face);
}

vector<Face*> Vertex::getFaces() {
	return faces;
}

Vec4 Vertex::calculateVertexNormalTarget(Mat4 transformationMatrix, bool invert)
{
	Vec4 targetSum = Vec4();
	Vec4 vertexAsVector = transformationMatrix * Vec4(_xCoord, _yCoord, _zCoord, 1);
	for (Face* face : faces) {
		targetSum = targetSum + face->calculateFaceNormalTarget(vertexAsVector, transformationMatrix, invert);
	}
	Vec4 finalNormal = targetSum * (1.0 / faces.size());
	//finalNormal = finalNormal.normalize();
	finalNormal[3] = 1;
	//Mat4 translate = Mat4::Translate(Vec4(targetSum.xCoord(), targetSum.yCoord(), targetSum.zCoord(), 1));
	//Mat4 scaling = Mat4::Scale(Vec4(0.01, 0.01, 0.01, 1));
	return finalNormal;
}

Vec4 Vertex::getVec4Coords(){
	return Vec4(_xCoord, _yCoord, _zCoord, 1);
}

Vec4 * Vertex::getNormal()
{
	return importedNormal;
}

double Vertex::getUAttribute()
{
	return attrU;
}

double Vertex::getVAttribute()
{
	return attrV;
}

bool Vertex::UVAttributesValid()
{
	return validUV;
}

void Vertex::setNormal(Vec4 * normal)
{
	importedNormal = normal;
}

void Vertex::setUV(double U, double V)
{
	validUV = true;
	attrU = U;
	attrV = V;
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
