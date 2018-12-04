#pragma once
#include <list>
#include <set>
#include <vector>
#include <unordered_map>
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "Vec4.h"
#include "stdafx.h"



#define STANDARD_OBJECT_COLOR RGB(230,230,230)

class Geometry
{
private:
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
	COLORREF lineClr;

protected:
	void setPointLimits();

public:
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

	void setLineClr(COLORREF clr);
	COLORREF getLineClr();
	void clear();

	//DEBUG FUNCTION:
	bool findVertexCollisions();
};


