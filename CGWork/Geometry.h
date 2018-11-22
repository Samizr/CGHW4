#pragma once
#include"Vertex.h"
#include"Edge.h"
#include<list>
#include<set>
class Geometry
{
	std::set<Vertex*> verticies;
	std::list<Edge*> edges;
	float maxX, maxY, maxZ;
	float minX, minY, minZ;

public:
	bool loaded;
	Geometry();
	~Geometry();
	void addEdge(Edge* edge);
	void addVertex(Vertex* vertex);
	std::list<Edge*> getEdges();
	std::set<Vertex*> getVertices();

	float getMaxX();
	float getMaxY();
	float getMaxZ();
	float getMinX();
	float getMinY();
	float getMinZ();
};

