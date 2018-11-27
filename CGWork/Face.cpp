#include "Face.h"
#include "Vec4.h"
Face::Face()
{
}

Face::Face(Edge* firstEdge, Edge* secondEdge, Edge* thirdEdge, Edge* fourthEdge)
{
	edges.push_back(firstEdge);
	edges.push_back(secondEdge);
	edges.push_back(thirdEdge);
	edges.push_back(fourthEdge);

	updateNormalDirection();
}

void Face::addEdge(Edge* edge)
{
	edges.push_back(edge);
}

Normal & Face::getNormal()
{
	return normal;
}

void Face::updateNormalDirection() {
	float xA = edges[0]->getA()->xCoord() - edges[0]->getB()->xCoord();
	float yA = edges[0]->getA()->yCoord() - edges[0]->getB()->yCoord();
	float zA = edges[0]->getA()->zCoord() - edges[0]->getB()->zCoord();
	float xB = edges[1]->getB()->xCoord() - edges[0]->getB()->xCoord();
	float yB = edges[1]->getB()->yCoord() - edges[0]->getB()->yCoord();
	float zB = edges[1]->getB()->zCoord() - edges[0]->getB()->zCoord();
	Vec4 A(xA, yA, zA, 1);
	Vec4 B(xB, yB, zB, 1);
	Vec4 direction = B.crossProduct(A);
	direction = direction.normalize();
	this->normal = Normal(normal.midpoint, direction);

}

void Face::updateMidpoint(Vec4 newMidpoint) {
	normal.midpoint = newMidpoint;
}

void Face::updateTarget()
{
	normal.target = Vec4(normal.midpoint.xCoord() + 14*normal.direction.xCoord(), normal.midpoint.yCoord() + 
		14*normal.direction.yCoord(), normal.midpoint.zCoord() + 14*normal.direction.zCoord(), 0);
}

void Face::setNormal(Normal newNormal)
{
	normal = newNormal;
}

vector<Edge*> Face::getEdges()
{
	return vector<Edge*>(edges);
}

Vec4 Face::calculateNormal(Mat4 transformationMatrix) {
	float xU = edges[0]->getB()->xCoord() - edges[0]->getA()->xCoord();
	float yU = edges[0]->getB()->yCoord() - edges[0]->getA()->yCoord();
	float zU = edges[0]->getB()->zCoord() - edges[0]->getA()->zCoord();
	float xV = edges[1]->getA()->xCoord() - edges[1]->getB()->xCoord();
	float yV = edges[1]->getA()->yCoord() - edges[1]->getB()->yCoord();
	float zV = edges[1]->getA()->zCoord() - edges[1]->getB()->zCoord();
	Vec4 A(xU, yU, zU, 1);
	Vec4 B(xV, yV, zV, 1);
	A = transformationMatrix * A;
	B = transformationMatrix * B;
	Vec4 crossProduct = A.crossProduct(B);
	return crossProduct.normalize();
}

Vec4 Face::calculateMidpoint(Mat4 finalMatrix)
{
	Vec4 midpoint(0,0,0,0);
	//Add a the first vertex from each edge into the calculation:
	for (Edge* edge : edges) {
		midpoint = midpoint + finalMatrix * Vec4(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 0);
	}
	//Add the last vertex in the last edge into the calculation:
	int lastEdge = edges.size() - 1;
	midpoint = midpoint + finalMatrix * Vec4(edges[lastEdge]->getB()->xCoord(), edges[lastEdge]->getB()->yCoord(), edges[lastEdge]->getB()->zCoord(), 0);
	midpoint = midpoint * (1 / edges.size());
	return midpoint;
}


Face::~Face()
{
}
