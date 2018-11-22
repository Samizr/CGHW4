#pragma once
#include"Vertex.h"

class Edge
{
	Vertex *_a, *_b;
public:
	Edge() = delete;
	Edge(Vertex* a, Vertex* y);
	Vertex* getA();
	Vertex* getB();
	~Edge();
};
