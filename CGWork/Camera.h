//
//  Camera.h
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "Mat4.h"

class Camera {
	Mat4 cTransform;
	Mat4 projection;

public:
	void setTransformation(const Mat4& T);
	void setProjection(const Mat4& T);
	void LookAt(Vec4& eye, Vec4& at, Vec4& up);
	void Ortho();
	void Perspective();
};
#endif /* Camera_h */
