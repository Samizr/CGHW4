#pragma once
#include "Vertex.h"
#include "Edge.h"
#include <list>
#include <set>
#include "Face.h"
#include "Vec4.h"
#include <unordered_map>

class Geometry
{
	class PointHash {
	public:
		PointHash() = default;
		size_t operator()(const Vec4 point);
	};
	std::set<Vertex*> verticies; 
	std::unordered_map <Vec4, Vertex*, Geometry::PointHash> vertexMap;
	std::list<Edge*> edges;
	std::list<Face*> faces;
	float maxX, maxY, maxZ;
	float minX, minY, minZ;

public:
	bool loaded;
	Geometry();
	~Geometry();
	void addVertex(Vertex* vertex);
	void addEdge(Edge* edge);
	void addFace(Face* face);
	Vertex* getVertex(float x, float y, float z);
	Edge* getEdge(Vertex* x, Vertex* y);
	std::list<Edge*> getEdges();
	std::set<Vertex*> getVertices();
	std::list<Face*> getFaces();

	float getMaxX();
	float getMaxY();
	float getMaxZ();
	float getMinX();
	float getMinY();
	float getMinZ();

	//DEBJUG FUNCTION REMOVE:
	bool findVertexCollisions();
};


