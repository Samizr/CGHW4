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

Vec4 Face::calculateFaceNormalTarget(Vec4 origin, Mat4 transformationMatrix, bool invert) {
	
	Vec4 sharedVertex(edges[0]->getB()->xCoord(), edges[0]->getB()->yCoord(), edges[0]->getB()->zCoord(), 1);
	Vec4 firstVertex(edges[0]->getA()->xCoord(), edges[0]->getA()->yCoord(), edges[0]->getA()->zCoord(), 1);
	Vec4 secondVertex(edges[1]->getB()->xCoord(), edges[1]->getB()->yCoord(), edges[1]->getB()->zCoord(), 1);
	
	sharedVertex = transformationMatrix * sharedVertex;
	firstVertex = transformationMatrix * firstVertex;
	secondVertex = transformationMatrix * secondVertex;

	Vec4 U(firstVertex + (sharedVertex * -1.0));
	Vec4 V(secondVertex + (sharedVertex * -1.0));
	Vec4 normal = V.crossProduct(U).normalize();
	if (invert) {
		normal = normal * (-1);
		normal[3] = (-1) * normal[3];
	}

	Mat4 translate = Mat4::Translate(Vec4(origin.xCoord(), origin.yCoord(), origin.zCoord(), 1));
	Mat4 scaling = Mat4::Scale(Vec4(0.2, 0.2, 0.2, 1));

	return translate * (scaling * (normal));
}


Vec4 Face::calculateMidpoint()
{
	Vec4 midpoint(0,0,0,0);
	float numOfVertices = edges.size() + 1;
	//Add a the first vertex from each edge into the calculation:
	for (Edge* edge : edges) {
		midpoint = midpoint + Vec4(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
	}
	//Add the last vertex in the last edge into the calculation:
	int lastEdge = edges.size() - 1;
	midpoint = midpoint + Vec4(edges[lastEdge]->getB()->xCoord(), edges[lastEdge]->getB()->yCoord(), edges[lastEdge]->getB()->zCoord(), 1);
	midpoint = midpoint * (1 / numOfVertices);
	return midpoint;
}



Face::~Face()
{
}
