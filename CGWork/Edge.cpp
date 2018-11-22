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

Edge::~Edge()
{
}
