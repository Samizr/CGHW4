//
//  Camera.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright � 2018 firas habib. All rights reserved.
//

#include "Camera.h"



Camera::Camera() {
	this->cTransform = Mat4::Identity();
	this->projection = Mat4::Identity();
	this->projection[2][2] = 0; // sets it to Orthogonal view.
}

void Camera::setTransformation(const Mat4& T) {
	this->cTransform = T;
}

void Camera::setProjection(const Mat4& T) {
	this->projection = T;
}

void Camera::LookAt(Vec4& eye, Vec4& at, Vec4& up) {
	// implementation as seen in the slides.
	Vec4 n = (eye - at).normalize();
	Vec4 u = up.crossProduct(n).normalize();
	Vec4 v = n.crossProduct(u).normalize();
	Vec4 t = Vec4(0, 0, 0, 1);
	Mat4 c = Mat4(u, v, n, t);
	cTransform = c * Mat4::Translate(eye * (-1));
}

void Camera::Ortho() {
	Vec4 orthoVectors[4] = { Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 0, 0), Vec4(0, 0, 0, 1) };
	this->cTransform = Mat4(orthoVectors);
}

void Camera::Perspective() {
	// TODO implement.
}

Mat4 Camera::getTransformationMatrix() {
	return cTransform;
}

Mat4 Camera::getProjectionMatrix() {
	return projection;
}
