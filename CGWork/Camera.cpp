//
//  Camera.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "Camera.h"



Camera::Camera() {
	this->cTransform = Mat4::Identity();
	this->cTransform[2][2] = -1;
	this->projection = Mat4::Identity();
	this->Ortho();
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
	Vec4 u = (up.crossProduct(n)).normalize();
	Vec4 v = (n.crossProduct(u)).normalize();
	Vec4 t = Vec4(0, 0, 0, 1);
	Mat4 c = Mat4(u, v, n, t);
	cTransform = c * Mat4::Translate(eye * (-1));
}

void Camera::Ortho() {
	float deltaX = 24;
	float deltaY = 13.5;
	float deltaZ = 6;
	float sumX = 0;
	float sumY = 0;
	float sumZ = 0;
	Vec4 vecNM[4] = { Vec4(2 / deltaX, 0, 0, -sumX / deltaX), Vec4(0, 2 / deltaY, 0, -sumY / deltaY), Vec4(0, 0, 2 / deltaZ, -sumZ / deltaZ), Vec4(0, 0, 0, 1) };
	this->projection = Mat4(vecNM);
}

void Camera::Perspective() {
	float mNear, mFar;
	mNear = 0;
	mFar = 10;
	float alpha = -(mNear + mFar) / (mFar - mNear);
	float beta = (2 * mNear * mFar) / (mNear - mFar);
	//Vec4 perspectiveVectors[4] = { Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, alpha, beta), Vec4(0, 0, -1, 0) };
	Vec4 perspectiveVectors[4] = { Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, -1/10, 0) };
	//Vec4 reScaleVectors[4] = { Vec4(-0.666, 0, 0, 0), Vec4(0, -1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 0, 1) };
	this->projection = Mat4(perspectiveVectors);// *Mat4(reScaleVectors);
}

Mat4 Camera::getTransformationMatrix() {
	return cTransform;
}

Mat4 Camera::getProjectionMatrix() {
	return projection;
}
