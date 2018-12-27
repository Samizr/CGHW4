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
#include <array>
#include "Light.h"
#include "Model.h"
#include "stdafx.h"
#include "Mat4.h"
#include "PngWrapper.h"

#define STANDARD_NORMAL_COLOR RGB(0,255,0)
#define NUM_LIGHT_SOURCES 7

enum VNMode {
	NONE, IMPORTED, CALCULATED
};

enum LightMode {
	FLAT, GOURAUD, PHONG
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
	COLORREF silhouetteClr;
	LightMode lightingMode;


	void drawCenterAxis(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 finalMatrix);
	void drawPolygonNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix);
	void drawCalculatedVertexNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix);
	void drawImportedVertexNormals(COLORREF* bitArr, CRect rect, Geometry * geometry, Mat4 restMatrix, Mat4 transformationMatrix);
	void drawBoundingBox(COLORREF* bitArr, CRect rect, Geometry * geometry, COLORREF clr, Mat4 finalMatrix);
public:
	Renderer();
	void drawBackgroundColor(COLORREF* bitArr, CRect rect);
	void drawWireframe(COLORREF* bitArr, CRect rect, Model* model);
	void drawSolid(COLORREF* bitArr, float* zBuffer, CRect rect, Model* model, std::array<LightParams, NUM_LIGHT_SOURCES> lightSources, LightParams ambientLight, double* materialParams);
	void drawWireframeBackfaceCulling(COLORREF* bitArr, CRect rect, Model* model);
	void drawWireframeZBufferDepth(COLORREF* bitArr, CRect rect, Model* model, COLORREF background);
	void drawSilhouette(COLORREF * bitArr, CRect rect, Geometry * geometry);
	void setObjectWorldMatrix(Mat4& matrix);
	void setCameraMatrix(Mat4& matrix);
	void setProjectionMatrix(Mat4& matrix);
	void setNormalizationMatrix(Mat4& matrix);
	void setWindowMatrix(Mat4& matrix);
	void setMainRect(CRect rect);
	void setNormalClr(COLORREF clr);
	void setBackgroundClr(COLORREF clr);
	void setSilhouetteClr(COLORREF clr);
	void setLightingMode(LightMode mode);
	void enableBoundingBox();
	void enablePolygonNormals();
	void enablePolygonNormalInvert();
	void enableVertexNormalInvert();
	void setVertexNormalMode(VNMode mode);
	void disableBoundingBox();
	void disablePolygonNormals();
	void disablePolygonNormalInvert();
	void disableVertexNormalInvert();
	void drawBounding(COLORREF* bitArr, CRect rect, Geometry * geometry, COLORREF clr);
	void drawBackgoundImageStretch(COLORREF* bitArr, CRect rect, PngWrapper* png);
	void drawBackgoundImageRepeat(COLORREF* bitArr, CRect rect, PngWrapper* png);
};
#endif /* Renderer_h */