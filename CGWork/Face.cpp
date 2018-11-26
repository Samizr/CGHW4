#include "Face.h"
#include "Vec4.h"
Face::Face()
{
}

Face::Face(Edge firstEdge, Edge secondEdge, Edge thirdEdge, Edge fourthEdge)
{
	edges.push_back(firstEdge);
	edges.push_back(secondEdge);
	edges.push_back(thirdEdge);
	edges.push_back(fourthEdge);

	updateNormalDirection();
}

void Face::addEdge(Edge edge)
{
	edges.push_back(edge);
}

Normal & Face::getNormal()
{
	return normal;
}

void Face::updateNormalDirection() {
	float xU = edges[0].getA()->xCoord() - edges[0].getB()->xCoord();
	float yU = edges[0].getA()->yCoord() - edges[0].getB()->yCoord();
	float zU = edges[0].getA()->zCoord() - edges[0].getB()->zCoord();
	float xV = edges[1].getA()->xCoord() - edges[1].getB()->xCoord();
	float yV = edges[1].getA()->yCoord() - edges[1].getB()->yCoord();
	float zV = edges[1].getA()->zCoord() - edges[1].getB()->zCoord();
	Vec4 A(xU, yU, zU, 1);
	Vec4 B(xV, yV, zV, 1);
	//Vec4 midpoint = Vec4(edges[0].getA()->xCoord(), edges[0].getA()->yCoord(), edges[0].getA()->zCoord(), 0);//(xMid, yMid, zMid, 0);
	Vec4 direction = A.crossProduct(B);
	direction = direction.normalize();
	this->normal = Normal(normal.midpoint, direction);

}

void Face::updateMidpoint(Vec4 newMidpoint)
{
	normal.midpoint = newMidpoint;
}

void Face::updateTarget()
{
	normal.target = Vec4(normal.midpoint.xCoord() + 15*normal.direction.xCoord(), normal.midpoint.yCoord() + 
		15*normal.direction.yCoord(), normal.midpoint.zCoord() + 15*normal.direction.zCoord(), 0);
}

void Face::setNormal(Normal newNormal)
{
	normal = newNormal;
}

vector<Edge> Face::getEdges()
{
	return vector<Edge>(edges);
}


Face::~Face()
{
}
