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



static Mat4 generateViewportMatrix(CRect rect);

#define NORMAL_LENGTH_FACTOR 13


// this sets all the matricies to be identity;
Renderer::Renderer() {
	withBounding = false;
	withPolygonNormals = false;
	withVertexNormals = false;
	normalClr = STANDARD_NORMAL_COLOR;
}

void Renderer::drawWireframe(COLORREF* bitArr, CRect rect, Model* model) {

	Geometry* geometry = &model->getGeometry();
	//drawBackground(bitArr, rect, RGB(255, 255, 255));//geometry->getBackgroundClr());

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
	if (withBounding) {
		drawBoundingBox(bitArr, rect, geometry, geometry->getLineClr(), finalMatrix);
	}
	if (withPolygonNormals) {
		drawPolygonNormals(bitArr, rect, geometry, restMatrix, objectWorldMatrix);
	}
	if (withVertexNormals) {
		drawVertexNormals(bitArr, rect, geometry, restMatrix, objectWorldMatrix);
	}
//	drawCenterAxis(bitArr, rect, geometry, finalMatrix);
}

void Renderer::drawBackground(COLORREF * bitArr, CRect rect, COLORREF clr)
{

	for (int i = rect.left; i < rect.right; i++) {
		for (int j = rect.top; j < rect.bottom; j++) {
			bitArr[i  + j * mainRect.Width()] = clr;
		}
	}

}

void Renderer::drawBoundingBox(COLORREF* bitArr, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix) {
	float mtop, mbottom, mfar, mnear, mright, mleft;
	float xVals[2] = {geometry->getMinX(), geometry->getMaxX()};
	float yVals[2] = {geometry->getMinY(), geometry->getMaxY()};
	float zVals[2] = {geometry->getMinZ(), geometry->getMaxZ()};
	int p = 0;
	Mat4 afterCamMatrix = objectWorldMatrix * cameraMatrix;
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


void Renderer::drawVertexNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix) {
	for (Vertex* vertex : geometry->getVertices()) {
		Vec4 vertexVector = Vec4(vertex->xCoord(), vertex->yCoord(), vertex->zCoord(), 1);
		Vec4 currentVertex = transformationMatrix * vertexVector;
		if ((cameraMatrix * currentVertex).zCoord() > 0) {
			continue;
		}
		Vec4 target = vertex->calculateVertexNormalTarget(transformationMatrix);
		if ((cameraMatrix * target).zCoord() > 0) {
			continue;
		}
		currentVertex = restMatrix * currentVertex;
		target = restMatrix * target;
		float p1Factor = currentVertex.wCoord();
		float p2Factor = target.wCoord();
		plotLine(currentVertex.xCoord() / p1Factor, currentVertex.yCoord() / p1Factor, target.xCoord() / p2Factor, target.yCoord() / p2Factor, bitArr, rect, mainRect.Width(), normalClr);
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
		Vec4 target = face->calculateFaceNormalTarget(midpoint, transformationMatrix);
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
