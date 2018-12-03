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
#define STANDARD_NORMAL_COLOR RGB(0,255,0)
#define STANDARD_BACKGROUND_COLOR RGB(5,5,5)
//TODO: NORMAL COLOR IS SAME AS OBJECT COLOR???

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
	std::vector<double> objectColor;
	COLORREF lineClr;
	COLORREF normalClr;
	COLORREF backgroundClr;

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

	void setLineClr(COLORREF clr);
	void setNormalClr(COLORREF clr);
	void setBackgroundClr(COLORREF clr);

	COLORREF getLineClr();
	COLORREF getNormalClr();
	COLORREF getBackgroundClr();

	//DEBUG FUNCTION:
	bool findVertexCollisions();
};


