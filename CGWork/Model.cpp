//
//  Model.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "Model.h"

Model::Model(Geometry& geometry) {
	this->geometry = geometry;
	this->mTransform = Mat4::Identity();
	//TODO: REMOVE!!
	this->mTransform = Mat4::Rotate(XAXIS, 3.1415926535) * mTransform;
}

Geometry& Model::getGeometry(){
	return this->geometry;
}

Mat4 Model::getTransformationMatrix(){
	return Mat4(this->mTransform);
}

void Model::setTransformation(const Mat4& matrix) {
	this->mTransform = matrix;
}

void Model::appendToTransformation(const Mat4& matrix) {
	this->mTransform = this->mTransform * matrix;
}

void Model::prependToTransformation(const Mat4& matrix) {
	this->mTransform = matrix * this->mTransform;
}

void Model::translateObjectSpace(AXIS axis, float amount) {
	Vec4 translateVector;
	translateVector[axis] = amount;
	Mat4 matrix = Mat4::Translate(translateVector);
	this->appendToTransformation(matrix);
}

void Model::translateViewSpace(AXIS axis, float amount) {
	Vec4 translateVector;
	translateVector[axis] = amount;
	Mat4 matrix = Mat4::Translate(translateVector);
	this->prependToTransformation(matrix);
}

void Model::scaleObjectSpace(AXIS axis, float amount) {
	Vec4 scaleVector(1, 1, 1, 1);
	scaleVector[axis] = amount;
	Mat4 matrix = Mat4::Scale(scaleVector);
	this->appendToTransformation(matrix);
}

void Model::scaleViewSpace(AXIS axis, float amount) { 
	Vec4 scaleVector(1, 1, 1, 1);
	scaleVector[axis] = amount;
	Mat4 matrix = Mat4::Scale(scaleVector);
	this->prependToTransformation(matrix);
}

void Model::rotateObjectSpace(AXIS axis, float theta) {
	Mat4 matrix = Mat4::Rotate(axis, theta);
	this->appendToTransformation(matrix);
}

void Model::rotateViewSpace(AXIS axis, float theta) {
	Mat4 matrix = Mat4::Rotate(axis, theta);
	this->prependToTransformation(matrix);
}