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

}

void Face::addEdge(Edge* edge)
{
	edges.push_back(edge);
}

Vec4 Face::calculateNormal(Mat4 transformationMatrix) {
	float xU = edges[0]->getA()->xCoord() - edges[0]->getB()->xCoord();
	float yU = edges[0]->getA()->yCoord() - edges[0]->getB()->yCoord();
	float zU = edges[0]->getA()->zCoord() - edges[0]->getB()->zCoord();
	float xV = edges[1]->getB()->xCoord() - edges[1]->getA()->xCoord();
	float yV = edges[1]->getB()->yCoord() - edges[1]->getA()->yCoord();
	float zV = edges[1]->getB()->zCoord() - edges[1]->getA()->zCoord();
	Vec4 A(xU, yU, zU, 1);
	Vec4 B(xV, yV, zV, 1);
	A = transformationMatrix * A;
	B = transformationMatrix * B;
	Vec4 crossProduct = B.crossProduct(A);
	return crossProduct.normalize();
}

Vec4 Face::calculateMidpoint(Mat4 finalMatrix)
{
	Vec4 midpoint(0,0,0,0);
	float numOfVertices = edges.size() + 1;
	//Add a the first vertex from each edge into the calculation:
	for (Edge* edge : edges) {
		midpoint = midpoint + finalMatrix * Vec4(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
	}
	//Add the last vertex in the last edge into the calculation:
	int lastEdge = edges.size() - 1;
	midpoint = midpoint + finalMatrix * Vec4(edges[lastEdge]->getB()->xCoord(), edges[lastEdge]->getB()->yCoord(), edges[lastEdge]->getB()->zCoord(), 1);
	midpoint = midpoint * (1 / numOfVertices);
	return midpoint;
}


Face::~Face()
{
}
