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
static void drawBoundingBox(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix);
static void drawCenterAxis(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix);
static void drawPolygonNormals(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry/*, COLOREF clr*/, Mat4 finalMatrix, Mat4 transformationMatrix);
static void drawVertexNormals(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry, Mat4 finalMatrix, Mat4 transformationMatrix);

#define NORMAL_LENGTH_FACTOR 13

// this sets all the matricies to be identity;
Renderer::Renderer() {
	
	withBounding = false;
	withPolygonNormals = false;
	withVertexNormals = false;
}


static Mat4 generateNormalizationMatrix(float deltaX, float deltaY, float deltaZ, float sumX, float sumY, float sumZ) {
	Vec4 vecNM[4] = { Vec4(2 / deltaX, 0, 0, -sumX / deltaX), Vec4(0, 2 / deltaY, 0, -sumY / deltaY), Vec4(0, 0, 2 / deltaZ, -sumZ / deltaZ), Vec4(0, 0, 0, 1) };
	return Mat4(vecNM);
}

void Renderer::drawWireframe(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry, COLORREF clr) {

	// for each edge in the geometry, do your thing.
	float sumX, sumY, sumZ;
	sumX = geometry->getMaxX() + geometry->getMinX();
	sumY = geometry->getMaxY() + geometry->getMinY();
	sumZ = geometry->getMaxZ() + geometry->getMinZ();
	Mat4 normalizationMatrix = generateNormalizationMatrix(24, 13.5, 24, sumX, 0, sumZ);
	Mat4 finalMatrix = (windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));

	for (Edge* edge : geometry->getEdges()) {
		Vec4 p1(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
		Vec4 p2(edge->getB()->xCoord(), edge->getB()->yCoord(), edge->getB()->zCoord(), 1);
		p1 = finalMatrix * p1;
		p2 = finalMatrix * p2;
		plotLine(p1.xCoord() / p1.wCoord(), p1.yCoord() / p1.wCoord(), p2.xCoord() / p2.wCoord(), p2.yCoord() / p2.wCoord(), bitArr, rect, clr);
	}
	if (withBounding) {
		drawBoundingBox(bitArr, rect, geometry, clr, finalMatrix);
	}
	if (withPolygonNormals) {
		drawPolygonNormals(bitArr, rect, geometry, finalMatrix, objectWorldMatrix);
	}
	if (withVertexNormals) {
		drawVertexNormals(bitArr, rect, geometry, finalMatrix, objectWorldMatrix);
	}
	drawCenterAxis(bitArr, rect, geometry, clr, finalMatrix);
}

static void drawBoundingBox(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix) {
	float mtop, mbottom, mfar, mnear, mright, mleft;
	float xVals[2] = {geometry->getMinX(), geometry->getMaxX()};
	float yVals[2] = {geometry->getMinY(), geometry->getMaxY()};
	float zVals[2] = {geometry->getMinZ(), geometry->getMaxZ()};
	int p = 0;
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
				
				plotLine(p1.xCoord() / p1.wCoord(), p1.yCoord() / p1.wCoord(), p2.xCoord() / p2.wCoord(), p2.yCoord() / p2.wCoord(), bitArr, rect, clr);
			}
		}
	}
}

void drawCenterAxis(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix) {
	float deltaXNorm = pow(geometry->getMaxX() - geometry->getMinX(), 2);
	float deltaYNorm = pow(geometry->getMaxY() - geometry->getMinY(), 2);
	float deltaZNorm = pow(geometry->getMaxZ() - geometry->getMinZ(), 2);
	float normalizedSize = sqrt(deltaXNorm + deltaYNorm + deltaZNorm) / 10;
	float centerX = (geometry->getMinX() + geometry->getMaxX()) / 2;
	float centerY = (geometry->getMinY() + geometry->getMaxY()) / 2;
	float centerZ = (geometry->getMinZ() + geometry->getMaxZ()) / 2;
	Vec4 p0, p1, p2, p3;
	p0 = finalMatrix * Vec4(centerX, centerY, centerZ, 1);
	p1 = finalMatrix * Vec4(centerX + normalizedSize, centerY, centerZ, 1);
	p2 = finalMatrix * Vec4(centerX, centerY + normalizedSize, centerZ, 1);
	p3 = finalMatrix * Vec4(centerX, centerY, centerZ+ normalizedSize, 1);
	plotLine(p0.xCoord() / p0.wCoord(), p0.yCoord() / p0.wCoord(), p1.xCoord() / p1.wCoord(), p1.yCoord() / p1.wCoord(), bitArr, rect, RGB(255, 0, 0));
	plotLine(p0.xCoord() / p0.wCoord(), p0.yCoord() / p0.wCoord(), p2.xCoord() / p2.wCoord(), p2.yCoord() / p2.wCoord(), bitArr, rect, RGB(0, 255, 0));
	plotLine(p0.xCoord() / p0.wCoord(), p0.yCoord() / p0.wCoord(), p3.xCoord() / p3.wCoord(), p3.yCoord() / p3.wCoord(), bitArr, rect, RGB(0, 0, 255));
}


void drawVertexNormals(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry, Mat4 finalMatrix, Mat4 transformationMatrix) {
	for (Vertex* vertex : geometry->getVertices()) {
		Vec4 normalSum = Vec4();
		for (Face* face : vertex->getFaces()) {
			normalSum = normalSum + face->calculateNormal(transformationMatrix);
		}
		Vec4 finalNormal = ((normalSum.normalize() * (1.0 / vertex->getFaces().size()))).normalize();
		
		Vec4 onScreenPoint1 = finalMatrix * Vec4(vertex->xCoord(), vertex->yCoord(), vertex->zCoord(), 1);
		Mat4 normalScalingMatrix = Mat4::Scale(Vec4(NORMAL_LENGTH_FACTOR, NORMAL_LENGTH_FACTOR, NORMAL_LENGTH_FACTOR, 1));
		Mat4 normalTranslationMatrix = Mat4::Translate(Vec4(onScreenPoint1.xCoord(), onScreenPoint1.yCoord(), onScreenPoint1.zCoord(), 1));
		Vec4 onScreenPoint2 = normalTranslationMatrix * (normalScalingMatrix * finalNormal);
		float p1Factor = onScreenPoint1.wCoord();
		float p2Factor = onScreenPoint2.wCoord();
		plotLine(onScreenPoint1.xCoord() / p1Factor, onScreenPoint1.yCoord() / p1Factor, onScreenPoint2.xCoord() / p2Factor, onScreenPoint2.yCoord() / p2Factor, bitArr, rect, RGB(0, 255, 0));
	}
}

void drawPolygonNormals(COLORREF* bitArr/*CDC* pdc*/, CRect rect, Geometry * geometry, Mat4 finalMatrix, Mat4 transormationMatrix)
{
	std::list<Face*> faces = geometry->getFaces();
	for (Face* face : faces) {
		Vec4 midpoint = face->calculateMidpoint(finalMatrix);
		Vec4 normal = face->calculateNormal(transormationMatrix);
		Mat4 normalScalingMatrix = Mat4::Scale(Vec4(NORMAL_LENGTH_FACTOR, NORMAL_LENGTH_FACTOR, NORMAL_LENGTH_FACTOR, 1));
		Mat4 normalTranslationMatrix = Mat4::Translate(Vec4(midpoint.xCoord(), midpoint.yCoord(), midpoint.zCoord(), 1));
		Vec4 target = normalTranslationMatrix * (normalScalingMatrix * normal); 
		float p1Factor = midpoint.wCoord();
		float p2Factor = target.wCoord();
		plotLine(midpoint.xCoord() / p1Factor, midpoint.yCoord() / p1Factor, target.xCoord() / p2Factor, target.yCoord() / p2Factor, bitArr, rect, RGB(0, 255, 0));
		
	}
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

void Renderer::setWindowMatrix(Mat4 &matrix) {
	windowMatrix = matrix;
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

void Renderer::enablePolygonNormals()
{
	withPolygonNormals = true;
}

void Renderer::enableVertexNormals() {
	this->withVertexNormals = true;
}

void Renderer::disableVertexNormals() {
	this->withVertexNormals = false;
}
