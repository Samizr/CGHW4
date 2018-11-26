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
static void drawBoundingBox(CDC * pDc, Geometry * geometry, COLORREF clr, Mat4 finalMatrix);
static void drawCenterAxis(CDC * pDc, Geometry * geometry, COLORREF clr, Mat4 finalMatrix);
static void drawPolygonNormals(CDC * pDc, Geometry * geometry/*, COLOREF clr*/, Mat4 finalMatrix, Mat4 worldMatrix);

// this sets all the matricies to be identity;
Renderer::Renderer() {
	
	withBounding = false;
	withPolygonNormals = false;
}



void Renderer::drawWireframe(CDC * pDC, Geometry * geometry, COLORREF clr) {

	// for each edge in the geometry, do your thing.
	Mat4 finalMatrix = (windowMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix)));

	for (Edge* edge : geometry->getEdges()) {
		Vec4 p1(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
		Vec4 p2(edge->getB()->xCoord(), edge->getB()->yCoord(), edge->getB()->zCoord(), 1);
		p1 = finalMatrix * p1;
		p2 = finalMatrix * p2;
		plotLine(p1.xCoord() / p1.wCoord(), p1.yCoord() / p1.wCoord(), p2.xCoord() / p2.wCoord(), p2.yCoord() / p2.wCoord(), pDC, clr);
	}
	if (withBounding) {
		drawBoundingBox(pDC, geometry, clr, finalMatrix);
	}
	if (withPolygonNormals) {
		drawPolygonNormals(pDC, geometry, finalMatrix, Mat4::Identity());
	}
	drawCenterAxis(pDC, geometry, clr, finalMatrix);
	
}

static void drawBoundingBox(CDC * pDc, Geometry * geometry, COLORREF clr, Mat4 finalMatrix) {
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
				
				plotLine(p1.xCoord() / p1.wCoord(), p1.yCoord() / p1.wCoord(), p2.xCoord() / p2.wCoord(), p2.yCoord() / p2.wCoord(), pDc, clr);
			}
		}
	}
}
void drawCenterAxis(CDC * pDc, Geometry * geometry, COLORREF clr, Mat4 finalMatrix) {
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
	plotLine(p0.xCoord(), p0.yCoord(), p1.xCoord(), p1.yCoord(), pDc, RGB(255, 0, 0));
	plotLine(p0.xCoord(), p0.yCoord(), p2.xCoord(), p2.yCoord(), pDc, RGB(0, 255, 0));
	plotLine(p0.xCoord(), p0.yCoord(), p3.xCoord(), p3.yCoord(), pDc, RGB(0, 0, 255));
}
void drawPolygonNormals(CDC * pDc, Geometry * geometry, Mat4 finalMatrix, Mat4 worldMatrix)
{
	std::list<Face> faces = geometry->getFaces();
	for (Face& face : faces) {
		for (Edge& edge : face.edges) {
			Vec4 p1(edge.getA()->xCoord(), edge.getA()->yCoord(), edge.getA()->zCoord(), 0);
			Vec4 p2(edge.getB()->xCoord(), edge.getB()->yCoord(), edge.getB()->zCoord(), 0);
			p1 = worldMatrix * p1;
			p2 = worldMatrix * p2;
			Vertex* newV1 = new Vertex(p1[0], p1[1], p1[2]);
			Vertex* newV2 = new Vertex(p2[0], p2[1], p2[2]);
			Edge newEdge(newV1, newV2);
			edge = newEdge;
		}
		std::vector<Edge> edgesCopy = face.edges;
		bool isQuadrilateral = (edgesCopy.size() == 3);
		Vec4 finalP;
		Vec4 p1(edgesCopy[0].getA()->xCoord(), edgesCopy[0].getA()->yCoord(), edgesCopy[0].getA()->zCoord(), 1);
		Vec4 p2(edgesCopy[1].getA()->xCoord(), edgesCopy[1].getA()->yCoord(), edgesCopy[1].getA()->zCoord(), 1);
		Vec4 p3(edgesCopy[1].getB()->xCoord(), edgesCopy[1].getB()->yCoord(), edgesCopy[1].getB()->zCoord(), 1);
		Vec4 p4;
			p1 = finalMatrix * p1;
			p2 = finalMatrix * p2;
			p3 = finalMatrix * p3;
		if (isQuadrilateral) {
			p4 = Vec4(edgesCopy[2].getB()->xCoord(), edgesCopy[2].getB()->yCoord(), edgesCopy[2].getB()->zCoord(), 1);
			p4 = finalMatrix * p4;
			finalP = (p1 + p2 + p3 + p4) * 0.25;
		}
		else {
			finalP = (p1 + p2 + p3) * 0.3333;
		}
			face.updateMidpoint(finalP);
			face.updateNormalDirection();
			face.updateTarget();
			Normal normal = face.getNormal();
			plotLine(normal.midpoint.xCoord(), normal.midpoint.yCoord(), normal.target.xCoord(), normal.target.yCoord(), pDc, RGB(0, 255, 0));
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
