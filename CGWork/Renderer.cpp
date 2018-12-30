//
//  Renderer.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "Renderer.h"
#include "LinePlotter.h"
#include <math.h>
#include <limits>
#include <vector>

using std::pair;

static Mat4 generateViewportMatrix(CRect rect);
static bool pixelIsInPolygon(float x, float y, std::vector<Vec4> poly);
static void drawEdge(COLORREF* bitArr, CRect rect, Edge* edge, Mat4 finalMatrix, Mat4 afterCamera, COLORREF lineclr, int windowWidth);
static void drawFaceSolid(COLORREF* bitArr, CRect rect, Face* face, Mat4 finalMatrix, Mat4 afterCamera, COLORREF lineclr, int windowWidth);
float getDepthAtPoint(int x, int y, std::vector<Vec4> poly);
static bool isSilhouetteEdge(Edge* edge, Mat4 transformationMatrix);
static int extractColorFromPng(int xCoord, int yCoord, PngWrapper* png);
static bool beamIntersects(Vec4 p1, Vec4 p2, float x, float y);
static bool pointOnLine(Vec4 p1, Vec4 p2, int x, int y);
static bool pointOnLineScreen(Vec4 p1, Vec4 p2, int x, int y);
vector<pair<float, COLORREF>> findIntersectionsColor(int line, vector<pair<Vec4, Vec4>> polyEdges, vector<pair<COLORREF, COLORREF>> polyEdgesColors, int maxX, CRect rect);
vector<pair<float, Vec4>> findIntersectionsNormal(int line, vector<pair<Vec4, Vec4>> polyEdges, vector<pair<Vec4, Vec4>> polyEdgesNormals);
static COLORREF getInterpolatedColor(double a, COLORREF clr1, COLORREF clr2);
static Vec4 getInterpolatedNormal(double a, Vec4 nrm1, Vec4 nrm2);
static float getIntersectionPoint(Vec4 p1, Vec4 p2, float y);



static double distance(int x1, int y1, int x2, int y2);
float distanceBetweenPoints(float x1, float y1, float x2, float y2);
Vec4 getIntersectionWithLine(Vec4 p1, Vec4 p2, float x, float y);
std::vector<Vec4> getTriangleWithPointInside(float x, float y, std::vector<Vec4> poly);
static bool beamIntersectsScanLine(Vec4 p1, Vec4 p2, float x, float y);
#define NORMAL_LENGTH_FACTOR 13


// this sets all the matricies to be identity;
Renderer::Renderer() {
	withBounding = false;
	withPolygonNormals = false;
	vertexNormals = NONE;
	invertPolygonNormals = false;
	invertVertexNormals = false;
	lightingMode = FLAT;
	normalClr = STANDARD_NORMAL_COLOR;
	silhouetteClr = STANDARD_NORMAL_COLOR;
}

// TODO SWITCH BACK NAMES WITH DRAWWIREFRAME
void Renderer::drawWireframe(COLORREF* bitArr, CRect rect, Model* model) {

	Geometry* geometry = &model->getGeometry();

	windowMatrix = generateViewportMatrix(rect);
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	Mat4 restMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix))));
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	for (Edge* edge : geometry->getEdges()) {
		drawEdge(bitArr, rect, edge, finalMatrix, afterCamera, geometry->getLineClr(), rect.Width());
	}
	if (withPolygonNormals) {
		drawPolygonNormals(bitArr, rect, geometry, restMatrix, objectWorldMatrix);
	}
	if (vertexNormals == CALCULATED) {
		drawCalculatedVertexNormals(bitArr, rect, geometry, restMatrix, objectWorldMatrix);
	}
	if (vertexNormals == IMPORTED) {
		drawImportedVertexNormals(bitArr, rect, geometry, restMatrix, objectWorldMatrix);
	}
//	drawCenterAxis(bitArr, rect, geometry, finalMatrix);
}

void Renderer::drawWireframeBackfaceCulling(COLORREF * bitArr, CRect rect, Model * model) {
	Geometry* geometry = &model->getGeometry();
	windowMatrix = generateViewportMatrix(rect);
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	Mat4 restMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix))));
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	for (Face* face : geometry->getFaces()) {
		if (face->calculateNormal(objectWorldMatrix)[2] > 0) {
			for (Edge* edge : face->getEdges()) {
				drawEdge(bitArr, rect, edge, finalMatrix, afterCamera, geometry->getLineClr(), rect.Width());
			}
			Edge *edge = new Edge(face->getVerticies()[0], face->getVerticies()[face->getVerticies().size() - 1]);
			drawEdge(bitArr, rect, edge, finalMatrix, afterCamera, geometry->getLineClr(), rect.Width());
			delete(edge);
		}
	}
}

void Renderer::drawSolidBackfaceCulling(COLORREF * bitArr, CRect rect, Model * model) {
	Geometry* geometry = &model->getGeometry();
	windowMatrix = generateViewportMatrix(rect);
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	Mat4 restMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix))));
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	for (Face* face : geometry->getFaces()) {
		if (face->calculateNormal(objectWorldMatrix)[2] > 0) {
			drawFaceSolid(bitArr, rect, face, finalMatrix, afterCamera, geometry->getLineClr(), rect.Width());
		}
	}
}

static void drawFaceSolid(COLORREF* bitArr, CRect rect, Face* face, Mat4 finalMatrix, Mat4 afterCamera, COLORREF lineclr, int windowWidth) {
	int minX = rect.Width();
	int minY = rect.Height();
	int maxX = 0;
	int maxY = 0;
	std::vector<Vec4> poly;
	for (Vertex* vertex : face->getVerticies()) {
		Vec4 currPoint = finalMatrix * vertex->getVec4Coords();
		currPoint = currPoint * (1 / currPoint.wCoord());
		poly.push_back(currPoint);
		minX = min(currPoint.xCoord(), minX);
		minY = min(currPoint.yCoord(), minY);
		maxX = max(currPoint.xCoord(), maxX);
		maxY = max(currPoint.yCoord(), maxY);
	}
	// Go over all inside frame from above pixels, only work with ones inside projected polygon.
	for (int i = minY; i < maxY; i++) {
		for (int j = minX; j < maxX; j++) {
			if (pixelIsInPolygon(j, i, poly)) {
				float currentDepth = getDepthAtPoint(j, i, poly);
				bitArr[i * rect.Width() + j] = lineclr;
			}
		}
	}
}

float* createZBuffer(CRect rect) {
	float* zBuffer = new float[rect.Width() * rect.Height()];
	for (int i = 0; i < rect.Height(); i++) {
		for (int j = 0; j < rect.Width(); j++) {
			zBuffer[i * rect.Width() + j] = -std::numeric_limits<float>::infinity();;
		}
	}
	return zBuffer;
}

void fillZBuffer(CRect rect, float* zBuffer, Model* model, Mat4 finalMatrix) {
	Geometry* geometry = &model->getGeometry();
	for (Face* face : geometry->getFaces()) {
		int minX = rect.Width();
		int minY = rect.Height();
		int maxX = 0;
		int maxY = 0;
		std::vector<Vec4> poly;
		for (Vertex* vertex : face->getVerticies()) {
			Vec4 currPoint = finalMatrix * vertex->getVec4Coords();
			poly.push_back(currPoint);
			minX = min(currPoint.xCoord(), minX);
			minY = min(currPoint.yCoord(), minY);
			maxX = max(currPoint.xCoord(), maxX);
			maxY = max(currPoint.yCoord(), maxY);
		}
		// Go over all inside frame from above pixels, only work with ones inside projected polygon.
		for (int i = minY; i < maxY; i++) {
			for (int j = minX; j < maxX; j++) {
				if (pixelIsInPolygon(j, i, poly)) {
					float currentDepth = getDepthAtPoint(j, i, poly);
					// Remember, depth is in negative values.
					if (currentDepth > zBuffer[i * rect.Width() + j]) {
						zBuffer[i * rect.Width() + j] = currentDepth;
					}
				}
			}
		}
	}
}

void Renderer::drawSolid(COLORREF* bitArr, float* zBuffer, CRect rect, Model* model, std::array<LightParams, NUM_LIGHT_SOURCES> lightSources, LightParams ambientLight, double* materialParams){
	Geometry* geometry = &model->getGeometry();
	windowMatrix = generateViewportMatrix(rect);
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	Mat4 restMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix))));
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	COLORREF clr = STANDARD_OBJECT_COLOR;
	int shit = 0;
	vector<bool> DBG_histogram;
	DBG_histogram.assign(1000, false);
	int DBG_size_faces = geometry->getFaces().size();
	for (Face* face : geometry->getFaces()) {
		float minX = rect.Width();
		float minY = rect.Height();
		float maxX = 0;
		float maxY = 0;
		double lineDist;
		std::vector<Vec4> poly;
		std::vector<pair<Vec4, Vec4>> polyEdges;
		std::vector<pair<COLORREF, COLORREF>> polyEdgesColors;
		std::vector<pair<Vec4, Vec4>> polyEdgesNormals;
		vector<pair<float, COLORREF>> intersectionPointsCLR;
		vector<pair<float, Vec4>> intersectionPointsNRM;
		for (Vertex* vertex : face->getVerticies()) {
			Vec4 currPoint = finalMatrix * vertex->getVec4Coords();
			Vec4 transformedNormal = objectWorldMatrix * (*(vertex->getNormal()));
			poly.push_back(currPoint);
			minX = min(currPoint.xCoord(), minX);
			minY = min(currPoint.yCoord(), minY);
			maxX = max(currPoint.xCoord(), maxX);
			maxY = max(currPoint.yCoord(), maxY);
		}		
		vector<Edge*> edges = face->getEdges();
		for (int i = 0; i <= edges.size(); i++) {
			Vec4 pointA, pointB, normalA, normalB;
			if (i < edges.size()) {
				pointA = finalMatrix * edges[i]->getA()->getVec4Coords();
				pointB = finalMatrix * edges[i]->getB()->getVec4Coords();
				normalA = objectWorldMatrix * (*(edges[i]->getA()->getNormal()));
				normalB = objectWorldMatrix * (*(edges[i]->getB()->getNormal()));
			}
			else if (edges[i - 1]->getB()->getVec4Coords() != edges[0]->getA()->getVec4Coords()){
				pointA = finalMatrix * edges[i-1]->getB()->getVec4Coords();
				pointB = finalMatrix * edges[0]->getA()->getVec4Coords();
				normalA = objectWorldMatrix * (*(edges[i-1]->getB()->getNormal()));
				normalB = objectWorldMatrix * (*(edges[0]->getA()->getNormal()));
			}
			pair<Vec4, Vec4> outEdge;
			outEdge.first = pointA;
			outEdge.second = pointB;
			polyEdges.push_back(outEdge);
			pair<Vec4, Vec4> outNormals;
			outNormals.first = normalA;
			outNormals.second = normalB;
			polyEdgesNormals.push_back(outNormals);
			pair<COLORREF, COLORREF> outColors;
			outColors.first = getLightingColor(normalA, geometry->getLineClr(), lightSources, ambientLight, materialParams);
			outColors.second = getLightingColor(normalB, geometry->getLineClr(), lightSources, ambientLight, materialParams);
			polyEdgesColors.push_back(outColors);
		}
		if (lightingMode == FLAT) 
			clr = getLightingColor(face->calculateNormal(objectWorldMatrix), geometry->getLineClr(), lightSources, ambientLight, materialParams);
		// Go over all inside frame from above pixels, only work with ones inside projected polygon.
		for (int i = minY; i < maxY; i++) {
			if (lightingMode == GOURAUD) {
				intersectionPointsCLR = findIntersectionsColor(i, polyEdges, polyEdgesColors, maxX, rect);
				lineDist = abs(intersectionPointsCLR[0].first - intersectionPointsCLR[1].first);
			}
			else if (lightingMode == PHONG) {
				intersectionPointsNRM = findIntersectionsNormal(i, polyEdges, polyEdgesNormals);
				lineDist = abs(intersectionPointsNRM[0].first - intersectionPointsNRM[1].first);
			}
			if ((lightingMode == GOURAUD || lightingMode == PHONG) && intersectionPointsNRM.size() == 0 && intersectionPointsCLR.size() == 0)
				continue;
			for (int j = minX; j < maxX; j++) {
				if (i < 0 || j < 0 || j >= rect.Width() || i >= rect.Height()) {
					continue;
				}
				if (pixelIsInPolygon(j, i, poly)) {
					float currentDepth = getDepthAtPoint(j, i, poly);
					if (currentDepth > zBuffer[i * rect.Width() + j]) {
						if (lightingMode == GOURAUD) {
							int minId = intersectionPointsCLR[0].first <= intersectionPointsCLR[1].first ? 0 : 1;
							int maxId = 1 - minId;
							double pointDist = abs(j - intersectionPointsCLR[minId].first);
							double percentage = lineDist == 0 ? 1 : pointDist / lineDist;
							clr = getInterpolatedColor(percentage, intersectionPointsCLR[minId].second, intersectionPointsCLR[maxId].second);
						}
						else if (lightingMode == PHONG) {
							int minId = intersectionPointsNRM[0].first <= intersectionPointsNRM[1].first ? 0 : 1;
							int maxId = 1 - minId;
							double pointDist = abs(j - intersectionPointsNRM[minId].first);
							double percentage = lineDist == 0 ? 1 : pointDist / lineDist;
							Vec4 normal = getInterpolatedNormal(percentage, intersectionPointsNRM[minId].second, intersectionPointsNRM[minId].second);
							clr = getLightingColor(normal, geometry->getLineClr(), lightSources, ambientLight, materialParams);
						}
						zBuffer[i * rect.Width() + j] = currentDepth;
						bitArr[i * rect.Width() + j] = clr;
					}
				}
			}
		}
	}
}

//Finds an x parallel line intersection points with a polynom, assumes line is inside polynom, returns pairs of intersection
//points (only the x coord) and their colors.
vector<pair<float, COLORREF>> findIntersectionsColor(int line, vector<pair<Vec4, Vec4>> polyEdges, vector<pair<COLORREF, COLORREF>> polyEdgesColors, int maxX, CRect rect)
{
	vector<pair<float, COLORREF>> out;
	int i = 0;
	for (pair<Vec4, Vec4>& edge : polyEdges) {
		if ((line >= (int)edge.first.yCoord() && line <= (int)edge.second.yCoord()) || (line <= (int)edge.first.yCoord() && line >= (int)edge.second.yCoord())) {
			float x = getIntersectionPoint(edge.first, edge.second, line);
			//SHARED EDGE MIGHT BE COUNTED TWICE, THEREFORE SKIP ONE:
			//if (x < edge.first.xCoord() && x < edge.second.xCoord())
			//	continue;
			double lineDist = distance(edge.first.xCoord(), edge.first.yCoord(), edge.second.xCoord(), edge.second.yCoord());
			double pointDist = distance(x, line, edge.second.xCoord(), edge.second.yCoord());
			double percentage = pointDist / lineDist;
			pair<float, COLORREF> pair;
			pair.first = x;
			pair.second = getInterpolatedColor(percentage, polyEdgesColors[i].first, polyEdgesColors[i].second); //TODO THIS IS WRONG! why first and first?
			out.push_back(pair);
			i++;
		}
	}
	if (out.size() > 2) {
		vector<pair<float, COLORREF>>::iterator lowest = out.begin(), highest = out.begin();
		for (auto it = out.begin(); it != out.end(); it++) {
			lowest = (*lowest).first <= (*it).first ? lowest : it;
			highest = (*highest).first > (*it).first ? highest : it;
		}
		vector<pair<float, COLORREF>> newOut;
		newOut.push_back(*lowest);
		newOut.push_back(*highest);
		return newOut;
	}
	//ASSERT(out.size() < 3);
	ASSERT(out.size() != 1);
	return out;
}

vector<pair<float, Vec4>> findIntersectionsNormal(int line, vector<pair<Vec4, Vec4>> polyEdges, vector<pair<Vec4, Vec4>> polyEdgesNormals)
{
	vector<pair<float, Vec4>> out;
	int i = 0;
	for (pair<Vec4, Vec4>& edge : polyEdges) {
		if ((line >= (int)edge.first.yCoord() && line <= (int)edge.second.yCoord()) || (line <= (int)edge.first.yCoord() && line >= (int)edge.second.yCoord())) {
			float x = getIntersectionPoint(edge.first, edge.second, line);
			double lineDist = distance(edge.first.xCoord(), edge.first.yCoord(), edge.second.xCoord(), edge.second.yCoord());
			double pointDist = distance(x, line, edge.second.xCoord(), edge.second.yCoord());
			double percentage = pointDist / lineDist;
			pair<float, Vec4> pair;
			pair.first = x;
			pair.second = getInterpolatedNormal(percentage, polyEdgesNormals[i].first, polyEdgesNormals[i].first); //TODO THIS IS WRONG! why first and first?
			out.push_back(pair);
			i++;
		}
	}
	if (out.size() > 2) {
		vector<pair<float, Vec4>>::iterator lowest = out.begin(), highest = out.begin();
		for (auto it = out.begin(); it != out.end(); it++) {
			lowest = (*lowest).first <= (*it).first ? lowest : it;
			highest = (*highest).first > (*it).first ? highest : it;
		}
		vector<pair<float, Vec4>> newOut;
		newOut.push_back(*lowest);
		newOut.push_back(*highest);
		return newOut;
	}

	ASSERT(out.size() != 1);
	return out;
}

float getIntersectionPoint(Vec4 p1, Vec4 p2, float y) {
	return getIntersectionWithLine(p1, p2, 0, y).xCoord();
}



int counter = 0;
static COLORREF getInterpolatedColor(double a, COLORREF clr1, COLORREF clr2) {
	if (a > 1) {
		counter++;
	}
	int r = GetRValue(clr2) * (1 - a) + GetRValue(clr1) * a;
	int g = GetGValue(clr2) * (1 - a) + GetGValue(clr1) * a;
	int b = GetBValue(clr2) * (1 - a) + GetBValue(clr1) * a;
	if (r > 255 || g > 255 || b > 255) {
		return RGB(255, 255, 255);
	}
	if (r < 0 || g < 0 || b < 0) {
		return RGB(0, 255, 255);
	}
	return RGB(r, g, b);
}

static Vec4 getInterpolatedNormal(double a, Vec4 nrm1, Vec4 nrm2) {
	return nrm1 * a + nrm2 * (1 - a);
}

static double distance(int x1, int y1, int x2, int y2) {
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

static float* calulateDepthOfPixels(Face* polygon, Mat4 finalMatrix, CRect rect) {
	float* depth = createZBuffer(rect);
	std::vector<Vec4> poly;
	int minX = rect.Width();
	int minY = rect.Height();
	int maxX = 0;
	int maxY = 0;
	for (Vertex* vertex : polygon->getVerticies()) {
		Vec4 currPoint = finalMatrix * vertex->getVec4Coords();
		poly.push_back(currPoint);
		minX = min(currPoint.xCoord(), minX);
		minY = min(currPoint.yCoord(), minY);
		maxX = max(currPoint.xCoord(), maxX);
		maxY = max(currPoint.yCoord(), maxY);
	}
	for (int i = minY; i <= maxY; i++) {
		for (int j = minX; j <= maxX; j++) {
			if (pixelIsInPolygon(j, i, poly)) {
				depth[j + i * rect.Width()] = (getDepthAtPoint(j, i, poly));
			}
		}
	}
	return depth;
}

static bool pixelIsInPolygon(float x, float y, std::vector<Vec4> poly) {
	int polyCorners = poly.size();
	int   j = polyCorners - 1;
	bool oddNodes = false;
	for (int i = 0; i < polyCorners; i++) {
		if ((int)x == (int)poly[i].xCoord() && (int)y == (int)poly[i].yCoord()) {
			return true;
		}
		if (beamIntersectsScanLine(poly[i], poly[j], x, y)) {
			oddNodes = !oddNodes;
		}
		j = i;
	}
	return oddNodes;
}

bool pointOnLine(Vec4 p1, Vec4 p2, int x, int y) {
	float lineMaxY = max(p1.yCoord(), p2.yCoord());
	float lineMinY = min(p1.yCoord(), p2.yCoord());
	float lineMaxX = max(p1.xCoord(), p2.xCoord());
	float lineMinX = min(p1.xCoord(), p2.xCoord());
	if (((int)p1.yCoord()) == y && ((int)p1.xCoord()) == x) {
		return true;
	}
	if (((int)p2.yCoord()) == y && ((int)p2.xCoord()) == x) {
		return true;
	}
	if (p1.yCoord() == p2.yCoord()) {
		return (int)y == (int)p1.yCoord() && x >= (int)lineMinX && x <= lineMaxX;
	}
	if (p1.xCoord() == p2.xCoord()) {
		return (int)x == (int)p1.xCoord() && y >= (int)lineMinY && y <= lineMaxY;
	}
	float deltaY = p2.yCoord() - p1.yCoord();
	float deltaX = p2.xCoord() - p1.xCoord();
	float c = p1.yCoord() - ((deltaY / deltaX) * p1.xCoord());
	return (int)(((deltaY / deltaX) * x) + c) == (int)y;
}

bool pointOnLineScreen(Vec4 p1, Vec4 p2, int x, int y) {
	x = (int)x;
	y = (int)y;
	
	COLORREF arr[2000 * 2000];

	for (int i = 0; i < 2000; i++) {
		for (int j = 0; j < 2000; j++) {
			arr[i + j * 2000] = RGB(0, 0, 0);
		}
	}

	CRect r;
	r.bottom = 2000;
	r.right = 2000;
	plotLine(p1.xCoord(), p1.yCoord(), p2.xCoord(), p2.yCoord(), arr, r, 2000, RGB(0, 0, 255));

	if (RGB(0, 0, 255) == arr[x + y * 2000]) {
		return true;
	}
	return false;
}


static bool beamIntersects(Vec4 p1, Vec4 p2, float x, float y) {
	float lineMaxY = max(p1.yCoord(), p2.yCoord());
	float lineMinY = min(p1.yCoord(), p2.yCoord());
	float lineMaxX = max(p1.xCoord(), p2.xCoord());
	float lineMinX = min(p1.xCoord(), p2.xCoord());
	if (p1.yCoord() >= p2.yCoord()) {
		if (y == ((int)p1.yCoord())) {
			return false;
		}
	}
	if (p2.yCoord() >= p1.yCoord()) {
		if (y == ((int)p2.yCoord())) {
			return false;
		}
	}
	return y <= lineMaxY && y >= (int)lineMinY;
}

static bool beamIntersectsScanLine(Vec4 p1, Vec4 p2, float x, float y) {
	float lineMaxY = max(p1.yCoord(), p2.yCoord());
	float lineMinY = min(p1.yCoord(), p2.yCoord());
	float lineMaxX = max(p1.xCoord(), p2.xCoord());
	float lineMinX = min(p1.xCoord(), p2.xCoord());
	if (((int)p1.yCoord()) == y && ((int)p1.xCoord()) == x) {
		return true;
	}
	if (((int)p2.yCoord()) == y && ((int)p2.xCoord()) == x) {
		return true;
	}
	if (p1.yCoord() == p2.yCoord()) {
		//return y == ((int)p1.yCoord()) && x >= (int)lineMinX && x <= lineMaxX;
		return false;
	}
	if (p1.yCoord() >= p2.yCoord()) {
		if (y == ((int)p1.yCoord())) {
			return false;
		}
		if (y == ((int)p2.yCoord()) && x >= (int)p2.xCoord()) {
			return true;
		}
	}
	if (p2.yCoord() >= p1.yCoord()) {
		if (y == ((int)p2.yCoord())) {
			return false;
		}
		if (y == ((int)p1.yCoord()) && x >= (int)p1.xCoord()) {
			return true;
		}
	}
	if (p1.xCoord() == p2.xCoord()) {
		return x >= ((int)p1.xCoord()) && y >= (int)lineMinY && y <= lineMaxY;
	}
	float deltaY = p2.yCoord() - p1.yCoord();
	float deltaX = p2.xCoord() - p1.xCoord();
	float c = p1.yCoord() - ((deltaY / deltaX) * p1.xCoord());
	float pointOnLine = (y - c) * (deltaX / deltaY);
	if (x < (int)lineMinX) {
		return false;
	}
	return x >= (int)pointOnLine && y <= lineMaxY && y >= (int)lineMinY;
}

float getDepthAtPoint(int x, int y, std::vector<Vec4> poly) {
	Vec4 p1 = poly[0];
	Vec4 p2 = poly[1];
	Vec4 p3 = poly[2];
	Vec4 intersectionPoints[3];
	int i = 0;
	// find the two pairs we intersect with
	if (poly.size() >= 4) {
		std::vector<Vec4> subset = getTriangleWithPointInside(x, y, poly);
		p1 = subset[0];
		p2 = subset[1];
		p3 = subset[2];
		for (int i = 0; i < 3; i++) {
			if (x == (int)subset[i].xCoord() && y == (int)subset[i].yCoord()) {
				return subset[i].zCoord();
			}
		}
	}
	//if (pointOnLine(p1, p2, x, y)) {
	//	return getIntersectionWithLine(p1, p2, x, y).zCoord();
	//}
	//if (pointOnLine(p1, p3, x, y)) {
	//	return getIntersectionWithLine(p1, p3, x, y).zCoord();
	//}
	//if (pointOnLine(p2, p3, x, y)) {
	//	return getIntersectionWithLine(p2, p3, x, y).zCoord();
	//}
	if (beamIntersects(p1, p2, x, y)) {
		intersectionPoints[i] = getIntersectionWithLine(p1, p2, x, y);
		i++;
	}
	if (beamIntersects(p1, p3, x, y)){
		intersectionPoints[i] = getIntersectionWithLine(p1, p3, x, y);
		i++;
	}
	if (beamIntersects(p2, p3, x, y)) {
		intersectionPoints[i] = getIntersectionWithLine(p2, p3, x, y);
		i++;
	}
	std::vector<Vec4> subset;
	float i1X, i1Y, i2X, i2Y, i1Z, i2Z;
	i1X = intersectionPoints[0].xCoord();
	i1Y = intersectionPoints[0].yCoord();
	i1Z = intersectionPoints[0].zCoord();
	i2X = intersectionPoints[1].xCoord();
	i2Y = intersectionPoints[1].yCoord();
	i2Z = intersectionPoints[1].zCoord();
	return i1Z + (i2Z - i1Z)  * ((i1X - x) / (i1X - i2X));
}


std::vector<Vec4> getTriangleWithPointInside(float x, float y, std::vector<Vec4> poly) {
	std::vector<Vec4> subset;
	for (int i = 0; i < poly.size(); i++) {
		for (int j = i + 1; j < poly.size(); j++) {
			for (int k = j + 1; k < poly.size(); k++) {
				subset.push_back(poly[i]);
				subset.push_back(poly[j]);
				subset.push_back(poly[k]);
				if (pixelIsInPolygon(x, y, subset) && pixelIsInPolygon(x, y, poly)) {
					return subset;
				}
				subset.clear();
			}
		}
	}
}
Vec4 getIntersectionWithLine(Vec4 p1, Vec4 p2, float x, float y) {
	Vec4 intersection(0, y, 0, 1);
	float deltaY = p2.yCoord() - p1.yCoord();
	float deltaX = p2.xCoord() - p1.xCoord();
	if (p1.xCoord() == p2.xCoord()) {
		intersection[0] = p1.xCoord();
	}
	else if (p1.yCoord() == p2.yCoord()) {
		intersection[0] = x;
	}
	else {
		float c = p1.yCoord() - ((deltaY / deltaX) * p1.xCoord());
		intersection[0] = (y - c) * (deltaX / deltaY);
	}
	intersection[2] = p1.zCoord() + (p2.zCoord() - p1.zCoord()) * ((p1.yCoord() - y) / (p1.yCoord() - p2.yCoord()));
	return intersection;
}
float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
	float deltaY = y2 - y1;
	float deltaX = x2 - x1;
	return sqrt(pow(deltaY, 2) + pow(deltaX, 2));
}


void Renderer::drawWireframeZBufferDepth(COLORREF* bitArr, CRect rect, Model* model, COLORREF backgroundClr) {
	Geometry* geometry = &model->getGeometry();
	windowMatrix = generateViewportMatrix(rect);
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	Mat4 restMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix))));
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	float* zBuffer = createZBuffer(rect);
	fillZBuffer(rect, zBuffer, model, finalMatrix);
	for (Face* face : geometry->getFaces()) {
		float* depth = calulateDepthOfPixels(face, finalMatrix, rect);
		for (Edge *edge : face->getEdges()) {
			Vec4 p1(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
			Vec4 p2(edge->getB()->xCoord(), edge->getB()->yCoord(), edge->getB()->zCoord(), 1);

			Vec4 afterCameraP1 = afterCamera * p1;
			Vec4 afterCameraP2 = afterCamera * p2;
			if (afterCameraP2.zCoord() > 0 || afterCameraP1.zCoord() > 0) {
				return;
			}
			p1 = finalMatrix * p1;
			p2 = finalMatrix * p2;
			int p1x = p1.xCoord() / p1.wCoord();
			int p2x = p2.xCoord() / p2.wCoord();
			int p1y = p1.yCoord() / p1.wCoord();
			int p2y = p2.yCoord() / p2.wCoord();
			plotLineWithDepth(p1x, p1y, p2x, p2y,zBuffer, depth, bitArr, rect, rect.Width(), geometry->getLineClr());
		}
		free(depth);
	}
}

static void drawEdge(COLORREF* bitArr, CRect rect, Edge* edge, Mat4 finalMatrix, Mat4 afterCamera, COLORREF lineclr, int windowWidth) {
	Vec4 p1(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
	Vec4 p2(edge->getB()->xCoord(), edge->getB()->yCoord(), edge->getB()->zCoord(), 1);
	Vec4 afterCameraP1 = afterCamera * p1;
	Vec4 afterCameraP2 = afterCamera * p2;
	if (afterCameraP2.zCoord() > 0 || afterCameraP1.zCoord() > 0) {
		return;
	}
	p1 = finalMatrix * p1;
	p2 = finalMatrix * p2;
	int p1x = p1.xCoord() / p1.wCoord();
	int p2x = p2.xCoord() / p2.wCoord();
	int p1y = p1.yCoord() / p1.wCoord();
	int p2y = p2.yCoord() / p2.wCoord();
	plotLine(p1x, p1y, p2x, p2y, bitArr, rect, rect.Width(), lineclr);
}

void Renderer::drawBackgroundColor(COLORREF * bitArr, CRect rect) {
	for (int i = rect.left; i < rect.right; i++) {
		for (int j = rect.top; j < rect.bottom; j++) {
			bitArr[i  + j * rect.Width()] = backgroundClr;
		}
	}
}

void Renderer::drawBounding(COLORREF * bitArr, CRect rect, Geometry * geometry, COLORREF clr) {
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	drawBoundingBox(bitArr, rect, geometry, clr, finalMatrix);
}

void Renderer::drawBackgoundImageStretch(COLORREF * bitArr, CRect rect, PngWrapper* png) {
	int screenWidth = rect.Width();
	int screenHeight = rect.Height();
	int imageWidth = png->GetWidth();
	int imageHeight = png->GetHeight();
	for (int i = 0; i < screenHeight; i++) {
		for (int j = 0; j < screenWidth; j++) {
			int yCoord = (float(i) / screenHeight) * imageHeight;
			int xCoord = (float(j) / screenWidth) * imageWidth;
			int color = extractColorFromPng(xCoord, yCoord, png);
			bitArr[j + screenWidth * ((screenHeight - 1) - i)] = color;
		}
	}
}

void Renderer::drawBackgoundImageRepeat(COLORREF * bitArr, CRect rect, PngWrapper* png) {
	int screenWidth = rect.Width();
	int screenHeight = rect.Height();
	int imageWidth = png->GetWidth();
	int imageHeight = png->GetHeight();
	for (int i = 0; i < screenHeight; i++) {
		for (int j = 0; j < screenWidth; j++) {
			int xCoord = j % imageWidth;
			int yCoord = i % imageHeight;
			int color = extractColorFromPng(xCoord, yCoord, png);
			bitArr[j + screenWidth * ((screenHeight - 1) - i)] = color;
		}
	}
}

static int extractColorFromPng(int xCoord, int yCoord, PngWrapper* png) {
	int c = png->GetValue(xCoord, yCoord);
	int r = 0, g = 0, b = 0;
	if (png->GetNumChannels() == 1) {
		r = c;
		g = c;
		b = c;
	}
	if (png->GetNumChannels() == 3 || png->GetNumChannels() == 4) {
		r = GET_R(c);
		g = GET_G(c);
		b = GET_B(c);
	}
	return RGB(r, g, b);
}

void Renderer::drawBoundingBox(COLORREF* bitArr, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix) {
	float mtop, mbottom, mfar, mnear, mright, mleft;
	float xVals[2] = {geometry->getMinX(), geometry->getMaxX()};
	float yVals[2] = {geometry->getMinY(), geometry->getMaxY()};
	float zVals[2] = {geometry->getMinZ(), geometry->getMaxZ()};
	int p = 0;
	Mat4 afterCamMatrix = cameraMatrix * objectWorldMatrix;
	Vec4 points[8];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				Vec4 point = Vec4(xVals[i], yVals[j], zVals[k], 1);
				points[p] = point;
				p++;
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int similarPlanes = 0;
			if (points[i].xCoord() == points[j].xCoord()) {
				similarPlanes++;
			}
			if (points[i].yCoord() == points[j].yCoord()) {
				similarPlanes++;
			}
			if (points[i].zCoord() == points[j].zCoord()) {
				similarPlanes++;
			}
			if (similarPlanes == 2) {
				Vec4 p1 = finalMatrix * points[i];
				Vec4 p2 = finalMatrix * points[j];
				if ((afterCamMatrix * points[i]).zCoord() > 0 || (afterCamMatrix * points[j]).zCoord() > 0) {
					continue;
				}
				plotLine(p1.xCoord() / p1.wCoord(), p1.yCoord() / p1.wCoord(), p2.xCoord() / p2.wCoord(), p2.yCoord() / p2.wCoord(), bitArr, rect, mainRect.Width(), clr);
			}
		}
	}
}

void Renderer::drawSilhouette(COLORREF * bitArr, CRect rect, Geometry * geometry) {
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	for (Edge* edge : geometry->getEdges()) {
		if (isSilhouetteEdge(edge, objectWorldMatrix)) {
			drawEdge(bitArr, rect, edge, finalMatrix, afterCamera, silhouetteClr, rect.Width());
		}
	}
}

static bool isSilhouetteEdge(Edge* edge, Mat4 transformationMatrix) {
	bool negativeZNormal = false;
	bool positiveZNormal = false;
	for (Face* face : edge->getFaces()) {
		Vec4 faceNormal = face->calculateNormal(transformationMatrix);
		if (faceNormal[2] < 0) {
			negativeZNormal = true;
		}
		if (faceNormal[2] > 0) {
			positiveZNormal = true;
		}
	}
	return positiveZNormal && negativeZNormal;
}

void Renderer::drawCenterAxis(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 finalMatrix) {
	float deltaXNorm = pow(geometry->getMaxX() - geometry->getMinX(), 2);
	float deltaYNorm = pow(geometry->getMaxY() - geometry->getMinY(), 2);
	float deltaZNorm = pow(geometry->getMaxZ() - geometry->getMinZ(), 2);
	float normalizedSize = sqrt(deltaXNorm + deltaYNorm + deltaZNorm) / 10;
	float centerX = (geometry->getMinX() + geometry->getMaxX()) / 2;
	float centerY = (geometry->getMinY() + geometry->getMaxY()) / 2;
	float centerZ = (geometry->getMinZ() + geometry->getMaxZ()) / 2;
	Vec4 p0, p1, p2, p3;
	Mat4 afterCamMatrix = cameraMatrix * objectWorldMatrix;
	Mat4 restMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix)));
	p0 = afterCamMatrix * Vec4(centerX, centerY, centerZ, 1);
	p1 = afterCamMatrix * Vec4(centerX + normalizedSize, centerY, centerZ, 1);
	p2 = afterCamMatrix * Vec4(centerX, centerY + normalizedSize, centerZ, 1);
	p3 = afterCamMatrix * Vec4(centerX, centerY, centerZ + normalizedSize, 1);
	if (p0.zCoord() < 0 && p1.zCoord() < 0 && p2.zCoord() < 0 && p3.zCoord() < 0) {
		p0 = restMatrix * p0;
		p1 = restMatrix * p1;
		p2 = restMatrix * p2;
		p3 = restMatrix * p3;
		plotLine(p0.xCoord() / p0.wCoord(), p0.yCoord() / p0.wCoord(), p1.xCoord() / p1.wCoord(), p1.yCoord() / p1.wCoord(), bitArr, rect, mainRect.Width(), RGB(255, 0, 0));
		plotLine(p0.xCoord() / p0.wCoord(), p0.yCoord() / p0.wCoord(), p2.xCoord() / p2.wCoord(), p2.yCoord() / p2.wCoord(), bitArr, rect, mainRect.Width(), RGB(0, 255, 0));
		plotLine(p0.xCoord() / p0.wCoord(), p0.yCoord() / p0.wCoord(), p3.xCoord() / p3.wCoord(), p3.yCoord() / p3.wCoord(), bitArr, rect, mainRect.Width(), RGB(0, 0, 255));
	}
	
}


void Renderer::drawCalculatedVertexNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix) {
	for (Vertex* vertex : geometry->getVertices()) {
		Vec4 origin = transformationMatrix * Vec4(vertex->xCoord(), vertex->yCoord(), vertex->zCoord(), 1);
		if ((cameraMatrix * origin).zCoord() > 0) {
			continue;
		}
		Vec4 target = vertex->calculateVertexNormalTarget(transformationMatrix, invertVertexNormals);
		if ((cameraMatrix * target).zCoord() > 0) {
			continue;
		}
		origin = restMatrix * origin;
		target = restMatrix * target;
		float p1Factor = origin.wCoord();
		float p2Factor = target.wCoord();
		plotLine(origin.xCoord() / p1Factor, origin.yCoord() / p1Factor, target.xCoord() / p2Factor, target.yCoord() / p2Factor, bitArr, rect, mainRect.Width(), normalClr);
	}
}

void Renderer::drawImportedVertexNormals(COLORREF * bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix)
{
	for (Vertex* vertex : geometry->getVertices()) {
		Vec4 origin = transformationMatrix * Vec4(vertex->xCoord(), vertex->yCoord(), vertex->zCoord(), 1);
		Vec4 target = transformationMatrix * (*vertex->getNormal());
		if (invertVertexNormals) {
			target = target * (-1);
			target[3] = (-1) * target[3];
		}
		Mat4 translate = Mat4::Translate(Vec4(origin.xCoord(), origin.yCoord(), origin.zCoord(), 1));
		Mat4 scaling = Mat4::Scale(Vec4(0.2, 0.2, 0.2, 1));
		target = translate * (scaling * (target));

		origin = restMatrix * origin;
		target = restMatrix * target;
		float p1Factor = origin.wCoord();
		float p2Factor = target.wCoord();
		plotLine(origin.xCoord() / p1Factor, origin.yCoord() / p1Factor, target.xCoord() / p2Factor, target.yCoord() / p2Factor, bitArr, rect, mainRect.Width(), normalClr);
	}
}

void Renderer::drawPolygonNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix)
{
	std::list<Face*> faces = geometry->getFaces();
	for (Face* face : faces) {
		Vec4 midpoint = transformationMatrix * face->calculateMidpoint();
		if ((cameraMatrix * midpoint).zCoord() > 0) {
			continue;
		}
		Vec4 target = face->calculateFaceNormalTarget(midpoint, transformationMatrix, invertPolygonNormals);
		if ((cameraMatrix * target).zCoord() > 0) {
			continue;
		}
		midpoint = restMatrix * midpoint;
		target = restMatrix * target;
		float p1Factor = midpoint.wCoord();
		float p2Factor = target.wCoord();
		plotLine(midpoint.xCoord() / p1Factor, midpoint.yCoord() / p1Factor, target.xCoord() / p2Factor, target.yCoord() / p2Factor, bitArr, rect, mainRect.Width(), normalClr);
	}
}


Mat4 generateViewportMatrix(CRect rect)
{
	float deltaW = rect.right - rect.left;
	float deltaH = rect.bottom - rect.top;
	float virtualDeltaW = min(deltaH, deltaH);
	float virtualDeltaH = min(deltaH, deltaH);
	if (16 * deltaH > 9 * deltaW) {
		virtualDeltaW = deltaW;
		virtualDeltaH = deltaW * 9 / 16;
	}
	else {
		virtualDeltaW = deltaH * 16 / 9;
		virtualDeltaH = deltaH;
	}
	float sumW = rect.right + rect.left;
	float sumH = rect.top + rect.bottom;
	Vec4 vecVPM[4] = { Vec4(virtualDeltaW / 2, 0, 0, sumW / 2), Vec4(0, virtualDeltaH / 2, 0, sumH / 2), Vec4(0, 0, 0.5, 0.5), Vec4(0, 0, 0, 1) };
	return Mat4(vecVPM);
}


void Renderer::setObjectWorldMatrix(Mat4 &matrix) {
	objectWorldMatrix = matrix;
}

void Renderer::setCameraMatrix(Mat4 &matrix) {
	cameraMatrix = matrix;
}

void Renderer::setProjectionMatrix(Mat4 &matrix) {
	projectionMatrix = matrix;
}

void Renderer::setNormalizationMatrix(Mat4 & matrix)
{
	normalizationMatrix = matrix;
}

void Renderer::setWindowMatrix(Mat4 &matrix) {
	windowMatrix = matrix;
}

void Renderer::setMainRect(CRect rect)
{
	mainRect = rect;
}

void Renderer::setNormalClr(COLORREF clr)
{
	normalClr = clr;
}

void Renderer::setBackgroundClr(COLORREF clr)
{
	backgroundClr = clr;
}

void Renderer::setSilhouetteClr(COLORREF clr)
{
	silhouetteClr = clr;
}

void Renderer::setLightingMode(LightMode mode)
{
	lightingMode = mode;
}

void Renderer::disableBoundingBox() {
	withBounding = false;
}

void Renderer::enableBoundingBox() {
	withBounding = true;
}

void Renderer::disablePolygonNormals()
{
	withPolygonNormals = false;
}

void Renderer::disablePolygonNormalInvert()
{
	invertPolygonNormals = false;
}

void Renderer::disableVertexNormalInvert()
{
	invertVertexNormals = false;
}

void Renderer::enablePolygonNormals()
{
	withPolygonNormals = true;
}

void Renderer::enablePolygonNormalInvert()
{
	invertPolygonNormals = true;
}

void Renderer::enableVertexNormalInvert()
{
	invertVertexNormals = true;
}



void Renderer::setVertexNormalMode(VNMode mode)
{
	vertexNormals = mode;
}

COLORREF Renderer::getLightingColor(Vec4 normal, COLORREF originalClr, const std::array<LightParams, NUM_LIGHT_SOURCES> &lightSources, const LightParams& ambientLight, double* materialParams)
{
	int origR = GetRValue(originalClr);
	int origG = GetGValue(originalClr);
	int origB = GetBValue(originalClr);
	double ambientFraction = materialParams[0];
	double diffuseFraction = materialParams[1];
	double specularFraction = materialParams[2];
	double cosinComponent = materialParams[3];
		//AMBIENT LIGHT:
	double R = ambientFraction * origR * ambientLight.colorR / 255;
	double G = ambientFraction * origG * ambientLight.colorG / 255;
	double B = ambientFraction * origB * ambientLight.colorB / 255;

	for (LightParams light : lightSources) {
		if (!light.enabled)
			continue;
		if (light.type == LIGHT_TYPE_DIRECTIONAL) {
			Vec4 lightVec(light.dirX, light.dirY, light.dirZ, 0);
			float cos_diffuse = normal.cosineAngle(lightVec);
				//DIFFUSE LIGHT:
			R += diffuseFraction * cos_diffuse * origR * light.colorR / 255;
			G += diffuseFraction * cos_diffuse * origG * light.colorG / 255;
			B += diffuseFraction * cos_diffuse * origB * light.colorB / 255;
				//SPECULAR LIGHT:
//			Vec4 viewerVec(0, 0, 12, 0); //FIX THIS!
//			Vec4 specularVec = normal * 2 * (lightVec ^ normal) - lightVec;
////			Vec4 viewerVec = cameraMatrix.getTranspose() * (-1) * Vec4(1, 1, 1, 0);
//			float cos_specular = pow(specularVec.cosineAngle(viewerVec), cosinComponent);
//			R += specularFraction * cos_specular * light.colorR;
//			G += specularFraction * cos_specular * light.colorG;
//			B += specularFraction * cos_specular * light.colorB;
		}
		if (light.type == LIGHT_TYPE_POINT) {
			//THE QUESTION HERE IS SHOULD I CREATE A DIRECTION FROM CORDS - LIGHT_POINT?
				//IF YES, SHOULD I RECEIVE ONE POINT? BECAUSE IN PHONG IT DEPENDS ON POINT
			//SECOND QUESTION IS SHOULD I REGARD VIEW/LOCAL? WB IN DIRECTIONAL? 
		}


	}
	//disable overflow towards end!
	if (R > 255 || G > 255 || B > 255)
		int pause = 1;
	R = R > 255 ? 255 : R;
	G = G > 255 ? 255 : G;
	B = B > 255 ? 255 : B;
	R = R < 0 ? 0 : R;
	G = G < 0 ? 0 : G;
	B = B < 0 ? 0 : B;
	return RGB(R, G, B);
}

