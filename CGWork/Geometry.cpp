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
	this->edges.push_back(edge);
	loaded = true;
}

void Geometry::addFace(const Face& face)
{
	faces.push_back(face);
}

void Geometry::addVertex(Vertex* vertex) {
	maxX = max(vertex->xCoord(), maxX);
	maxY = max(vertex->yCoord(), maxY);
	maxZ = max(vertex->zCoord(), maxZ);
	minX = min(vertex->xCoord(), minX);
	minY = min(vertex->yCoord(), minY);
	minZ = min(vertex->zCoord(), minZ);
	this->verticies.insert(vertex);
}

std::list<Edge*> Geometry::getEdges()
{
	return this->edges;
}

std::set<Vertex*> Geometry::getVertices()
{
	return this->verticies;
}

std::list<Face> Geometry::getFaces()
{
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

// Static Function Implementations:

static float max(float a, float b) {
	return a > b ? a : b;
}

static float min(float a, float b) {
	return a < b ? a : b;
}

