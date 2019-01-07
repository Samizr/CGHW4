//
//  Renderer.h
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright � 2018 firas habib. All rights reserved.
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

using std::pair;
using std::array;
using std::vector;

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
	COLORREF getLightingColor(Vec4 point, Vec4 normal, COLORREF originalClr, const std::array<LightParams, NUM_LIGHT_SOURCES> &lightSources, const LightParams& ambientLight, double* materialParams);
	void drawFaceSolid(COLORREF* bitArr, float* zBuffer, CRect rect, Face* face, COLORREF lineClr, std::array<LightParams, NUM_LIGHT_SOURCES> lightSources, LightParams ambientLight, double* materialParams);	vector<pair<Vec4, Vec4>> getEdgesAsVectorVec4(const vector<Edge*> edges, const Mat4 finalMatrix);
	vector<Vec4> getPolyAsVectorVec4(Face* face, const Mat4 finalMatrix);
	void getPolyBoundaries(vector<Vec4>& poly, int* minX, int* minY, int* maxX, int* maxY);
	vector<pair<Vec4, Vec4>> getEdgesNormals(const vector<Edge*> edges, const Mat4 objectWorldMatrix);
	vector<pair<COLORREF, COLORREF>> getEdgesColors(const vector<Edge*>& edges, const Mat4& objectWorldMatrix, const vector<pair<Vec4, Vec4>>& polyEdgesNormals, COLORREF originalClr, array<LightParams, NUM_LIGHT_SOURCES> lightSources, LightParams ambientLight, double* materialParams);
	COLORREF getColorGouraud(vector<pair<float, COLORREF>> intersectionPointsCLR, int x);
	COLORREF getColorPhong(vector<pair<float, Vec4>> intersectionPointsNRM, int x, int y, int z, COLORREF originalClr, array<LightParams, NUM_LIGHT_SOURCES> lightSources, LightParams ambientLight, double* materialParams);

public:
	Renderer();
	void drawBackgroundColor(COLORREF* bitArr, CRect rect);
	void drawWireframe(COLORREF* bitArr, CRect rect, Model* model);
	void drawSolid(COLORREF* bitArr, float* zBuffer, CRect rect, Model* model, std::array<LightParams, NUM_LIGHT_SOURCES> lightSources, LightParams ambientLight, double* materialParams);
	void drawSolidBackfaceCulling(COLORREF * bitArr, CRect rect, Model * model, std::array<LightParams, NUM_LIGHT_SOURCES> lightSources, LightParams ambientLight, double* materialParams);
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
	void drawWireframeBackfaceCulling(COLORREF * bitArr, CRect rect, Model * model);
};
#endif /* Renderer_h */