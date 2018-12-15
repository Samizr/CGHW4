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
#include "PngWrapper.h"

#define STANDARD_NORMAL_COLOR RGB(0,255,0)

class Renderer {
	Mat4 objectWorldMatrix;
	Mat4 cameraMatrix;
	Mat4 projectionMatrix;
	Mat4 normalizationMatrix;
	Mat4 windowMatrix;
	bool withBounding;
	bool withPolygonNormals;
	bool withVertexNormals;
	CRect mainRect;
	COLORREF normalClr;


	void drawBackground(COLORREF * bitArr, CRect rect, COLORREF clr);
	void drawCenterAxis(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 finalMatrix);
	void drawPolygonNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix);
	void drawVertexNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix);
	void drawBoundingBox(COLORREF* bitArr, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix);
	void drawSilhouette();

public:
	Renderer();
	void drawWireframe(COLORREF* bitArr, CRect rect, Model* model);
	void drawWireframeBackfaceCulling(COLORREF* bitArr, CRect rect, Model* model, COLORREF background);
	void setObjectWorldMatrix(Mat4& matrix);
	void setCameraMatrix(Mat4& matrix);
	void setProjectionMatrix(Mat4& matrix);
	void setNormalizationMatrix(Mat4& matrix);
	void setWindowMatrix(Mat4& matrix);
	void setMainRect(CRect rect);
	void setNormalClr(COLORREF clr);
	void disableBoundingBox();
	void enableBoundingBox();
	void disablePolygonNormals();
	void enablePolygonNormals();
	void enableVertexNormals();
	void disableVertexNormals();
	void drawBounding(COLORREF* bitArr, CRect rect, Geometry * geometry, COLORREF clr);
	void drawBackgoundImageStretch(COLORREF* bitArr, CRect rect, PngWrapper png);
	void drawBackgoundImageRepeat(COLORREF* bitArr, CRect rect, PngWrapper png);
};
#endif /* Renderer_h */