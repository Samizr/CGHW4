//
//  Camera.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "Camera.h"

void Camera::setTransformation(const Mat4& T) {
	this->cTransform = T;
}

void Camera::setProjection(const Mat4& T) {
	this->projection = T;
}

void Camera::LookAt(Vec4& eye, Vec4& at, Vec4& up) {

}

void Camera::Ortho() {
	Vec4 orthoVectors[4] = { Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 0, 0), Vec4(0, 0, 0, 1) };
	this->cTransform = Mat4(orthoVectors);
}

void Camera::Perspective() {
	// TODO implement.
}
