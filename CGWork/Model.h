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

enum AXIS {
	XAXIS = 0, YAXIS = 1, ZAXIS = 2
};

class Model {
protected:
	Geometry geometry;
	Mat4 mTransform;
public:
	Model(Geometry& geometry);
	Geometry& getGeometry();
	Mat4 getTransformationMatrix();
	void setTransformation(const Mat4& matrix);
	void appendToTransformation(const Mat4& matrix);
	void prependToTransformation(const Mat4& matrix);
	void rotateObjectSpace(AXIS axis, float theta);
	void translateObjectSpace(AXIS axis, float amount);
	void scaleObjectSpace(AXIS axis, float amount);
	void rotateViewSpace(AXIS axis, float theta);
	void translateViewSpace(AXIS axis, float amount);
	void scaleViewSpace(AXIS axis, float amount);
};
#endif /* Model_h */
