//
//  Model.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "Model.h"

bool Model::UVAttributesValid()
{
	for (auto vertex : mainGeometry.getVertices()) {
		if (!vertex->UVAttributesValid())
			return false;
	}
	return true;
}

bool Model::UVAttributesNormalized()
{
	bool trueOne = false, trueZero = false, trueNegative = false, trueBiggerThanOne = false;
	for (auto vertex : mainGeometry.getVertices()) {
		if (vertex->getUAttribute() == 1 || vertex->getVAttribute() == 1)
			trueOne = true;
		if (vertex->getUAttribute() == 0 || vertex->getVAttribute() == 0)
			trueZero = true;
		if (vertex->getUAttribute() < 0 || vertex->getVAttribute() < 0)
			trueNegative = true;
		if (vertex->getUAttribute() > 1 || vertex->getVAttribute() > 1)
			trueBiggerThanOne = true;
	}
	return trueOne && trueZero && !trueNegative && !trueBiggerThanOne;
}

Model::Model(Geometry& geometry) {
	this->setMainGeometry(geometry);
	this->addSubGeometry(geometry);
	this->mMainTransform = Mat4::Identity();
	this->ParametricTexturePNG = nullptr;
}

Geometry& Model::getMainGeometry() {
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

PngWrapper * Model::getModelTexturePNG()
{
	return ParametricTexturePNG;
}

int Model::addSubGeometry(Geometry & geometry)
{
	pair<Geometry, Mat4> pair;
	pair.first = geometry;
	pair.second = Mat4::Identity();
	subGeometries.push_back(pair);
	return subGeometries.size() - 1;
}

Mat4 Model::getTransformationMatrix() {
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

void Model::setModelTexturePNG(PngWrapper * p)
{
	ParametricTexturePNG = p;
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

void Model::normalizeUVAttributes()
{
	if (!UVAttributesValid())
		return;

	double Umax = DBL_MIN;
	double Vmax = DBL_MIN;
	double Umin = DBL_MAX;
	double Vmin = DBL_MAX;
	for (auto vertex : mainGeometry.getVertices()) {
		Umax = max(Umax, vertex->getUAttribute());
		Vmax = max(Vmax, vertex->getVAttribute());
		Umin = min(Umin, vertex->getUAttribute());
		Vmin = min(Vmin, vertex->getVAttribute());
	}
	//Squeeze both height and width (might test other techniques).
	double deltaU = Umax - Umin;
	double deltaV = Vmax - Vmin;
	ASSERT(deltaU > 0, deltaV > 0);
	for (auto vertex : mainGeometry.getVertices()) {
		vertex->setUV(vertex->getUAttribute() / deltaU, vertex->getVAttribute() / deltaV);
	}
	for (int i = 0; i < subGeometries.size(); i++) {
		for (auto vertex : subGeometries[i].first.getVertices()) {
			vertex->setUV(vertex->getUAttribute() / deltaU, vertex->getVAttribute() / deltaV);
		}
	}
	if (UVAttributesNormalized()) {
		::AfxMessageBox(CString("UV Attributes Normalization Error!"));
	}
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
