#include "Geometry.h"
#include <limits>
#include <math.h>

static float max(float a, float b);
static float min(float a, float b);
static Normal getNormal(const Face& face);

Geometry::Geometry() {
	maxX = std::numeric_limits<float>::min();
	maxY = std::numeric_limits<float>::min();
	maxZ = std::numeric_limits<float>::min();
	minX = std::numeric_limits<float>::max();
	minY = std::numeric_limits<float>::max();
	minZ = std::numeric_limits<float>::max();
	loaded = false;
}

Geometry::~Geometry() {

}

void Geometry::addEdge(Edge* edge) {
	if (this->getEdge(edge->getA(), edge->getB()) == nullptr) {
		this->edges.push_back(edge);
		loaded = true;
	}
}

void Geometry::addFace(Face* face)
{
	faces.push_back(face);
}

Vertex * Geometry::getVertex(float x, float y, float z)
{
	auto it = vertexMap.find(Vec4(x, y, z, 0));
	if (it == vertexMap.end()) {
		return nullptr;
	}
	else {
		return (*it).second;
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
		maxX = max(vertex->xCoord(), maxX);
		maxY = max(vertex->yCoord(), maxY);
		maxZ = max(vertex->zCoord(), maxZ);
		minX = min(vertex->xCoord(), minX);
		minY = min(vertex->yCoord(), minY);
		minZ = min(vertex->zCoord(), minZ);
		this->verticies.insert(vertex);
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

size_t Geometry::PointHash::operator()(const Vec4 point)
{
	return (int)(137 * point.xCoord() + 149 * point.yCoord() + 163 * point.zCoord());
}

// Static Function Implementations:

static float max(float a, float b) {
	return a > b ? a : b;
}

static float min(float a, float b) {
	return a < b ? a : b;
}
