#include "Geometry.h"
#include <limits>

#undef max
#undef min

static float geo_max(float a, float b);
static float geo_min(float a, float b);


using std::numeric_limits;

void Geometry::setPointLimits()
{
	maxX = std::numeric_limits<float>::min();
	maxY = std::numeric_limits<float>::min();
	maxZ = std::numeric_limits<float>::min();
	minX = std::numeric_limits<float>::max();
	minY = std::numeric_limits<float>::max();
	minZ = std::numeric_limits<float>::max();
}

Geometry::Geometry() {
	setPointLimits();
}

Geometry::~Geometry() {

}

void Geometry::addEdge(Edge* edge) {
	if (this->getEdge(edge->getA(), edge->getB()) == nullptr) {
		this->edges.push_back(edge);
	}
}

void Geometry::addFace(Face* face)
{
	faces.push_back(face);
}

Vertex * Geometry::getVertex(float x, float y, float z)
{
	auto search = vertexMap.find(Vec4(x, y, z, 0));
	if (search == vertexMap.end()) {
		return nullptr;
	} else {
		return (*search).second;
	}
}

Edge * Geometry::getEdge(Vertex * x, Vertex * y) {
	//for (Edge* edge : edges) {
	//	if (edge->getA() == x && edge->getB() == y) {
	//		return edge;
	//	}
	//}
	return nullptr;
}

void Geometry::addVertex(Vertex* vertex) {
	if (this->getVertex(vertex->xCoord(), vertex->yCoord(), vertex->zCoord()) == nullptr) {
		maxX = geo_max(vertex->xCoord(), maxX);
		maxY = geo_max(vertex->yCoord(), maxY);
		maxZ = geo_max(vertex->zCoord(), maxZ);
		minX = geo_min(vertex->xCoord(), minX);
		minY = geo_min(vertex->yCoord(), minY);
		minZ = geo_min(vertex->zCoord(), minZ);
		this->verticies.insert(vertex);
		vertexMap[Vec4(vertex->xCoord(), vertex->yCoord(), vertex->zCoord(), 0)] = vertex;
	}
}



std::list<Edge*> Geometry::getEdges()
{
	return this->edges;
}

std::set<Vertex*> Geometry::getVertices()
{
	return this->verticies;
}

std::list<Face*> Geometry::getFaces() {
	return faces;
}


float Geometry::getMaxX()
{
	return maxX;
}

float Geometry::getMaxY()
{
	return maxY;
}

float Geometry::getMaxZ()
{
	return maxZ;
}

float Geometry::getMinX()
{
	return minX;
}

float Geometry::getMinY()
{
	return minY;
}

float Geometry::getMinZ()
{
	return minZ;
}

void Geometry::setLineClr(COLORREF clr)
{
	lineClr = clr;
}

void Geometry::setNormalClr(COLORREF clr)
{
	normalClr = clr;
}

void Geometry::setBackgroundClr(COLORREF clr)
{
	backgroundClr = clr;
}

COLORREF Geometry::getLineClr()
{
	return lineClr;
}

COLORREF Geometry::getNormalClr()
{
	return normalClr;
}

COLORREF Geometry::getBackgroundClr()
{
	return backgroundClr;
}

void Geometry::clear()
{
	verticies.clear();
	vertexMap.clear();
	edges.clear();
	faces.clear();
	setPointLimits();

}


bool Geometry::findVertexCollisions()
{
	bool returnVal = false;
	int collisions = 0;
	for (Vertex* v : verticies) {
		for (Vertex* u : verticies) {
			if (u != v && v->xCoord() == u->xCoord() && v->yCoord() == u->yCoord() && v->zCoord() == u->zCoord()) {
				collisions++;

			}
		}
	}
	return returnVal;
}

size_t Geometry::PointHash::operator()(const Vec4 point)
{
	return (int)(137.0 * point.xCoord() + 149.0 * point.yCoord() + 163.0 * point.zCoord());
}

// Static Function Implementations:

static float geo_max(float a, float b) {
	return a > b ? a : b;
}

static float geo_min(float a, float b) {
	return a < b ? a : b;
}
