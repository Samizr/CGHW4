//
//  Model.h
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include <stdio.h>
#include "Mat4.h"
#include "Geometry.h"
#include "PngWrapper.h"

using std::pair;

enum AXIS {
	XAXIS = 0, YAXIS = 1, ZAXIS = 2
};

class Model {
protected:
	Geometry mainGeometry;
	Mat4 mMainTransform;
	vector<pair<Geometry, Mat4>> subGeometries;
	PngWrapper* ParametricTexturePNG;
	bool UVAttributesNormalized(); //DBG function

public:
	Model() = default;
	Model(Geometry& geometry);
	Geometry& getMainGeometry();
	Geometry& getSubGeometry(int id);
	Mat4 getTransformationMatrix();
	Mat4 getSubGeometryTransformationMatrix(int id);
	int getSubGeometriesNum();
	PngWrapper* getModelTexturePNG();
	int addSubGeometry(Geometry& geometry);
	void setMainGeometry(Geometry& geometry);
	void setTransformation(const Mat4& matrix);
	void setSubgeometryTransformation(int id, const Mat4& matrix);
	void setModelTexturePNG(PngWrapper* p);
	void appendToTransformation(const Mat4& matrix);
	void prependToTransformation(const Mat4& matrix);
	void rotateObjectSpace(AXIS axis, float theta);
	void translateObjectSpace(AXIS axis, float amount);
	void scaleObjectSpace(AXIS axis, float amount);
	void rotateViewSpace(AXIS axis, float theta);
	void translateViewSpace(AXIS axis, float amount);
	void scaleViewSpace(AXIS axis, float amount);
	void normalizeUVAttributes();
	void clear();
	bool UVAttributesValid();
};
#endif /* Model_h */
