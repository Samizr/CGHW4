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
#include "Geometry.h"
#include "stdafx.h"
#include "Mat4.h"

#define STANDARD_OBJECT_COLOR RGB(230,230,230)
#define STANDARD_NORMAL_COLOR RGB(0,255,0)
#define STANDARD_BACKGROUND_COLOR RGB(5,5,5)

class Renderer {
	Mat4 objectWorldMatrix;
	Mat4 cameraMatrix;
	Mat4 projectionMatrix;
	Mat4 normalizationMatrix;
	Mat4 windowMatrix;
	bool withBounding;
	bool withPolygonNormals;
	bool withVertexNormals;
	COLORREF lineClr;
	COLORREF normalClr;
	COLORREF backgroundClr;
public:
	Renderer();
	void drawWireframe(COLORREF* bitArr, CRect rect, Geometry* geometry);
	void setObjectWorldMatrix(Mat4& matrix);
	void setCameraMatrix(Mat4& matrix);
	void setProjectionMatrix(Mat4& matrix);
	void setWindowMatrix(Mat4& matrix);
	void disableBoundingBox();
	void enableBoundingBox();
	void disablePolygonNormals();
	void enablePolygonNormals();
	void enableVertexNormals();
	void disableVertexNormals();
	void setLineClr(COLORREF clr);
	void setNormalClr(COLORREF clr);
	void setBackgroundClr(COLORREF clr);
};
#endif /* Renderer_h */