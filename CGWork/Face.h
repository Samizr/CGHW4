#pragma once

#include <vector>
#include "Edge.h"
#include "Normal.h"
#include "Mat4.h"
using std::vector;
class Face
{
private:
	Normal normal;
	vector<Edge*> edges; 
public:
	Face();
	Face(Edge* firstEdge, Edge* secondEdge, Edge* thirdEdge, Edge* fourthEdge);
	void addEdge(Edge* edge);
	Normal& getNormal();
	void updateNormalDirection();
	void updateMidpoint(Vec4 newMidpoint);
	void updateTarget();
	void setNormal(Normal newNormal);
	vector<Edge*> getEdges();
	Vec4 calculateNormal(Mat4 transformationMatrix);
	Vec4 calculateMidpoint(Mat4 finalMatrix);
	~Face();
};

