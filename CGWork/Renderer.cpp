//
//  Renderer.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright � 2018 firas habib. All rights reserved.
//

#include "Renderer.h"
#include "LinePlotter.h"
#include <math.h>
#include <limits>
#include <vector>



static Mat4 generateViewportMatrix(CRect rect);
static bool pixelIsInPolygon(int pixelX, int pixelY, Face* polygon, Mat4 finalMatrix);
static void drawEdge(COLORREF* bitArr, CRect rect, Edge* edge, Mat4 finalMatrix, Mat4 afterCamera, COLORREF lineclr, int windowWidth);
bool edgeIntersectsWithBeam(int yBeam, Edge* edge, Mat4 finalMatrix);
float getDepthAtPoint(int x, int y, Face* polygon, Mat4 finalMatrix);
static bool isSilhouetteEdge(Edge* edge, Mat4 transformationMatrix);

#define NORMAL_LENGTH_FACTOR 13


// this sets all the matricies to be identity;
Renderer::Renderer() {
	withBounding = false;
	withPolygonNormals = false;
	vertexNormals = NONE;
	invertPolygonNormals = false;
	invertVertexNormals = false;
	normalClr = STANDARD_NORMAL_COLOR;
}

void Renderer::drawWireframe(COLORREF* bitArr, CRect rect, Model* model) {

	Geometry* geometry = &model->getGeometry();

	windowMatrix = generateViewportMatrix(rect);
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	Mat4 restMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix))));
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	for (Edge* edge : geometry->getEdges()) {
		Vec4 p1(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
		Vec4 p2(edge->getB()->xCoord(), edge->getB()->yCoord(), edge->getB()->zCoord(), 1);

		Vec4 afterCameraP1 = afterCamera* p1;
		Vec4 afterCameraP2 = afterCamera * p2;
		if (afterCameraP2.zCoord() > 0 || afterCameraP1.zCoord() > 0) {
			continue;
		}
		p1 = finalMatrix * p1;
		p2 = finalMatrix * p2;
		int p1x = p1.xCoord() / p1.wCoord();
		int p2x = p2.xCoord() / p2.wCoord();
		int p1y = p1.yCoord() / p1.wCoord();
		int p2y = p2.yCoord() / p2.wCoord();
		plotLine(p1x, p1y, p2x, p2y, bitArr, rect, mainRect.Width() ,geometry->getLineClr());

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
	drawSilhouette(bitArr, rect, geometry, RGB(0,255,0), finalMatrix, objectWorldMatrix);
//	drawCenterAxis(bitArr, rect, geometry, finalMatrix);
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
		// Go over all pixels, only work with ones inside projected polygon.
		for (int i = 0; i < rect.Height(); i++) {
			for (int j = 0; j < rect.Width(); j++) {
				if (pixelIsInPolygon(i, j, face, finalMatrix)) {
					float currentDepth = getDepthAtPoint(i, j, face, finalMatrix);
					// Remember, depth is in negative values.
					if (currentDepth > zBuffer[i * rect.Width() + j]) {
						zBuffer[i * rect.Width() + j] = currentDepth;
					}
				}
			}
		}
	}
}





static bool pixelIsInPolygon(int x, int y, Face* polygon, Mat4 finalMatrix) {
	int polyCorners = polygon->getVerticies().size();
	std::vector<Vec4> poly;
	for (Vertex* vertex : polygon->getVerticies()) {
		poly.push_back(finalMatrix * vertex->getVec4Coords());
	}
	int   j = polyCorners - 1;
	bool  oddNodes = false;

	for (int i = 0; i < polyCorners; i++) {
		if ((poly[i].yCoord() < y && poly[j].yCoord() >= y
			|| poly[j].yCoord() < y && poly[i].yCoord() >= y)
			&& (poly[i].xCoord() <= x || poly[j].xCoord() <= x)) {
			oddNodes ^= (poly[i].xCoord() + (y - poly[i].yCoord()) / (poly[j].yCoord() - poly[i].yCoord())*(poly[j].xCoord() - poly[i].xCoord()) < x);
		}
		j = i;
	}

	return oddNodes;
}

float getDepthAtPoint(int x, int y, Face* polygon, Mat4 finalMatrix) {
	Vec4 p1 = polygon->getVerticies()[0]->getVec4Coords();
	Vec4 p2 = polygon->getVerticies()[1]->getVec4Coords();
	Vec4 p3 = polygon->getVerticies()[2]->getVec4Coords();
	float za = p1.zCoord() - (p1.zCoord() - p2.zCoord()) * ((p1.yCoord() - y) / (p1.yCoord() - p2.yCoord()));
	float xa = p1.xCoord() - (p1.xCoord() - p2.xCoord()) * ((p1.yCoord() - y) / (p1.yCoord() - p2.yCoord()));
	float zb = p1.zCoord() - (p1.zCoord() - p2.zCoord()) * ((p1.yCoord() - y) / (p1.yCoord() - p3.yCoord()));
	float xb = p1.xCoord() - (p1.xCoord() - p2.xCoord()) * ((p1.yCoord() - y) / (p1.yCoord() - p3.yCoord()));;
	float zp = zb - (zb - za) * ((xb - x) / (xb - xa));
	return zp;
}

void Renderer::drawWireframeBackfaceCulling(COLORREF* bitArr, CRect rect, Model* model, COLORREF background) {
	Geometry* geometry = &model->getGeometry();
	windowMatrix = generateViewportMatrix(rect);
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));
	Mat4 restMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix))));
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	float* zBuffer = createZBuffer(rect);
	for (Edge* edge : geometry->getEdges()) {
		drawEdge(bitArr, rect, edge, finalMatrix, afterCamera, geometry->getLineClr(), mainRect.Width());
	}
	zBuffer = createZBuffer(rect);
	for (Face* face : geometry->getFaces()) {
		
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


void Renderer::drawBackgroundColor(COLORREF * bitArr, CRect rect)
{
	for (int i = rect.left; i < rect.right; i++) {
		for (int j = rect.top; j < rect.bottom; j++) {
			bitArr[i  + j * mainRect.Width()] = backgroundClr;
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
			int r, g, b;
			int c = png->GetValue(xCoord, yCoord);
			if (png->GetNumChannels() == 1) {
				r = c;
				g = c;
				b = c;
			}
			if (png->GetNumChannels() == 3) {
				r = GET_R(c);
				g = GET_G(c);
				b = GET_B(c);
			}
			bitArr[j + screenWidth * ((screenHeight - 1) - i)] = RGB(r, g, b);
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
			int r, g, b;
			int c = png->GetValue(xCoord, yCoord);
			if (png->GetNumChannels() == 1) {
				r = c;
				g = c;
				b = c;
			}
			if (png->GetNumChannels() == 3) {
				r = GET_R(c);
				g = GET_G(c);
				b = GET_B(c);
			}
			bitArr[j + screenWidth * ((screenHeight - 1) - i)] = RGB(r, g, b);
		}
	}
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

void Renderer::drawSilhouette(COLORREF * bitArr, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix, Mat4 transformationMatrix) {
	Mat4 afterCamera = (cameraMatrix * objectWorldMatrix);
	for (Edge* edge : geometry->getEdges()) {
		if (isSilhouetteEdge(edge, transformationMatrix)) {
			drawEdge(bitArr, rect, edge, finalMatrix, afterCamera, clr, rect.Width());
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
//
//void Renderer::enableVertexNormals() {
//	this->withVertexNormals = true;
//}
//
//void Renderer::disableVertexNormals() {
//	this->withVertexNormals = false;
//}
