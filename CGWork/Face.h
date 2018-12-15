#pragma once

#include <vector>
#include "Edge.h"
#include "Mat4.h"
using std::vector;
class Face
{
private:
	vector<Edge*> edges; 
	vector<Vertex*> verticies;
public:
	Face();
	Face(Edge* firstEdge, Edge* secondEdge, Edge* thirdEdge, Edge* fourthEdge);
	void addEdge(Edge* edge);
	void addVertex(Vertex* vertex);
	vector<Vertex*> getVerticies();
	vector<Edge*> getEdges();
	Vec4 calculateFaceNormalTarget(Vec4 origin, Mat4 transformationMatrix, bool invert);

	Vec4 calculateMidpoint();
	~Face();
};

