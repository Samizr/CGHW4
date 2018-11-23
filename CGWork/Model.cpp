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

void Model::translateObjectSpace(int axis, float amount) {
	Vec4 translateVector;
	translateVector[axis] = amount;
	Mat4 matrix = Mat4::Translate(translateVector);
	this->appendToTransformation(matrix);
}

void Model::translateViewSpace(int axis, float amount) {
	Vec4 translateVector;
	translateVector[axis] = amount;
	Mat4 matrix = Mat4::Translate(translateVector);
	this->prependToTransformation(matrix);
}

void Model::scaleObjectSpace(int axis, float amount) {
	Vec4 scaleVector(1, 1, 1, 1);
	scaleVector[axis] = amount;
	Mat4 matrix = Mat4::Translate(scaleVector);
	this->appendToTransformation(matrix);
}

void Model::scaleViewSpace(int axis, float amount) { 
	Vec4 scaleVector(1, 1, 1, 1);
	scaleVector[axis] = amount;
	Mat4 matrix = Mat4::Translate(scaleVector);
	this->prependToTransformation(matrix);
}

void Model::rotateObjectSpace(int axis, float theta) {
	Mat4 matrix = Mat4::Rotate(axis, theta);
	this->appendToTransformation(matrix);
}

void Model::rotateViewSpace(int axis, float theta) {
	Mat4 matrix = Mat4::Rotate(axis, theta);
	this->prependToTransformation(matrix);
}