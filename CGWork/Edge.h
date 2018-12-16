#pragma once
#include"Vertex.h"
#include<list>

class Edge
{
	Vertex *_a, *_b;
	std::list<Face*> faces;
public:
	Edge() = delete;
	Edge(Vertex* a, Vertex* y);
	Vertex* getA();
	Vertex* getB();
	std::list<Face*> getFaces();
	void addFace(Face* face);
	~Edge();
};
