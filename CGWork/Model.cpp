//
//  Model.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "Model.h"

Model::Model(Geometry& geometry) {
	this->mainGeometry = geometry;
	this->mMainTransform = Mat4::Identity();
}

Geometry& Model::getMainGeometry(){
	return this->mainGeometry;
}

Geometry & Model::getSubGeometry(int id)
{
	return this->subGeometries[id].first;
}

void Model::setMainGeometry(Geometry & geometry)
{
	mainGeometry = geometry;
}

int Model::getSubGeometriesNum()
{
	return subGeometries.size();
}

int Model::addSubGeometry(Geometry & geometry)
{
	pair<Geometry, Mat4> pair;
	pair.first = geometry;
	pair.second = Mat4::Identity();
	subGeometries.push_back(pair);
	return subGeometries.size() - 1;
}

Mat4 Model::getTransformationMatrix(){
	return this->mMainTransform;
}

Mat4 Model::getSubGeometryTransformationMatrix(int id)
{
	return this->subGeometries[id].second;
}

void Model::setTransformation(const Mat4& matrix) {
	this->mMainTransform = matrix;
}

void Model::setSubgeometryTransformation(int id, const Mat4 & matrix)
{
	this->subGeometries[id].second = matrix;
}

void Model::appendToTransformation(const Mat4& matrix) {
	this->mMainTransform = this->mMainTransform * matrix;
}

void Model::prependToTransformation(const Mat4& matrix) {
	this->mMainTransform = matrix * this->mMainTransform;
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

void Model::clear()
{
	mainGeometry.clear();
	mMainTransform = Mat4::Identity();
	subGeometries.clear();
}
