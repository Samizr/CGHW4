#pragma once
#include"Vertex.h"
#include"Edge.h"
#include<list>
#include<set>
#include"Face.h"
#include "Vec4.h"
#include "Normal.h"
class Geometry
{
	std::set<Vertex*> verticies;
	std::list<Edge*> edges;
	std::list<Face> faces;
	//std::list<Normal> normals;
	float maxX, maxY, maxZ;
	float minX, minY, minZ;

public:
	bool loaded;
	Geometry();
	~Geometry();
	void addVertex(Vertex* vertex);
	void addEdge(Edge* edge);
	void addFace(const Face& face);
	std::list<Edge*> getEdges();
	std::set<Vertex*> getVertices();
	std::list<Face> getFaces();

	//std::list<Normal>& getNormals();
	//void updateNormals();

	float getMaxX();
	float getMaxY();
	float getMaxZ();
	float getMinX();
	float getMinY();
	float getMinZ();
};

