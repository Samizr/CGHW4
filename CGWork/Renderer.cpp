//
//  Renderer.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "Renderer.h"
#include "LinePlotter.h"

// this sets all the matricies to be identity;
Renderer::Renderer() {
	float deltaX = 16;
	float deltaY = 9;
	float deltaZ = 3;
	float sumX = 0;
	float sumY = 0;
	float sumZ = 0;
	Vec4 vecNM[4] = { Vec4(2 / deltaX, 0, 0, -sumX / deltaX), Vec4(0, 2 / deltaY, 0, -sumY / deltaY), Vec4(0, 0, 2 / deltaZ, -sumZ / deltaZ), Vec4(0, 0, 0, 1) };
	normalizationMatrix = Mat4(vecNM);
}

void Renderer::drawWireframe(CDC * pDc, Geometry * geometry) {

	// for each edge in the geometry, do your thing.
	Mat4 finalMatrix =(windowMatrix * (normalizationMatrix * (projectionMatrix * (cameraMatrix * objectWorldMatrix))));

	for (Edge* edge : geometry->getEdges()) {
		Vec4 p1(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
		Vec4 p2(edge->getB()->xCoord(), edge->getB()->yCoord(), edge->getB()->zCoord(), 1);
		p1 = finalMatrix * p1;
		p2 = finalMatrix * p2;
		plotLine(p1.xCoord(), p1.yCoord(), p2.xCoord(), p2.yCoord(), pDc);
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
