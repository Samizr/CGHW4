#pragma once

#include <vector>
#include "Edge.h"
#include "Mat4.h"
using std::vector;
class Face
{
private:
	vector<Edge*> edges; 
public:
	Face();
	Face(Edge* firstEdge, Edge* secondEdge, Edge* thirdEdge, Edge* fourthEdge);
	void addEdge(Edge* edge);
	//Vec4 calculateNormal(Mat4 transformationMatrix);
	Vec4 calculateFaceNormalTarget(Vec4 origin, Mat4 transformationMatrix);
	Vec4 calculateMidpoint();
	~Face();
};

