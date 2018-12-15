//
//  Renderer.h
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#ifndef Renderer_h
#define Renderer_h

#include <stdio.h>
#include "Model.h"
#include "stdafx.h"
#include "Mat4.h"

#define STANDARD_NORMAL_COLOR RGB(0,255,0)

enum VNMode {
	NONE, IMPORTED, CALCULATED
};

class Renderer {
	Mat4 objectWorldMatrix;
	Mat4 cameraMatrix;
	Mat4 projectionMatrix;
	Mat4 normalizationMatrix;
	Mat4 windowMatrix;
	bool withBounding;
	bool withPolygonNormals;
	VNMode vertexNormals;
	bool invertPolygonNormals;
	bool invertVertexNormals;
	CRect mainRect;
	COLORREF normalClr;
	COLORREF backgroundClr;


	void drawCenterAxis(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 finalMatrix);
	void drawPolygonNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix);
	void drawVertexNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix);
	void drawBoundingBox(COLORREF* bitArr, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix);

public:
	Renderer();
	void drawBackgroundColor(COLORREF* bitArr, CRect rect);
	void drawWireframe(COLORREF* bitArr, CRect rect, Model* model);
	void setObjectWorldMatrix(Mat4& matrix);
	void setCameraMatrix(Mat4& matrix);
	void setProjectionMatrix(Mat4& matrix);
	void setNormalizationMatrix(Mat4& matrix);
	void setWindowMatrix(Mat4& matrix);
	void setMainRect(CRect rect);
	void setNormalClr(COLORREF clr);
	void setBackgroundClr(COLORREF clr);
	void enableBoundingBox();
	void enablePolygonNormals();
	void enablePolygonNormalInvert();
	void enableVertexNormalInvert();
	void setVertexNormalMode(VNMode mode);
	void disableBoundingBox();
	void disablePolygonNormals();
	void disablePolygonNormalInvert();
	void disableVertexNormalInvert();
	//void disableVertexNormals();
	//void enableVertexNormals();
	void drawBounding(COLORREF* bitArr, CRect rect, Geometry * geometry, COLORREF clr);
};
#endif /* Renderer_h */