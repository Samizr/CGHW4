#include "Edge.h"


Edge::Edge(Vertex* a, Vertex* b) {
	this->_a = a;
	this->_b = b;
}

Vertex* Edge::getA() {
	return this->_a;
}
Vertex* Edge::getB() {
	return this->_b;
}

std::list<Face*> Edge::getFaces() {
	// this is a solution that works for now.
	std::list<Face*> commonFaces;
	std::vector<Face*> nodeAFaces = _a->getFaces();
	std::vector<Face*> nodeBFaces = _b->getFaces();
	for (Face* aFace : nodeAFaces) {
		for (Face* bFace : nodeBFaces) {
			if (aFace == bFace) {
				commonFaces.push_back(aFace);
			}
		}
	}
	return commonFaces;
}

void Edge::addFace(Face * face){
	faces.push_back(face);
}

Edge::~Edge()
{
}
