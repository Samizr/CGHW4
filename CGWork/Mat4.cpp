//
//  mat4.cpp
//  graphics
//
//  Created by Firas Habib on 08/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "mat4.h"
#include <stdexcept>

Mat4::Mat4() {
	// default constructors are called, everything
}

Mat4::Mat4(Vec4* rows) {
	for (int i = 0; i < 4; i++) {
		this->rows[i] = rows[i];
		this->cols[i] = Vec4(rows[0][i], rows[1][i], rows[2][i], rows[3][i]);
	}
}

Mat4 Mat4::operator+(const Mat4& matrix) const {
	Vec4 newRows[4];
	for (int i = 0; i < 4; i++) {
		newRows[i] = this->rows[i] + matrix.rows[i];
	}
	return Mat4(newRows);
}

Mat4 Mat4::operator*(const float& a) const {
	Vec4 newRows[4];
	for (int i = 0; i < 4; i++) {
		newRows[i] = this->rows[i] * a;
	}
	return Mat4(newRows);
}

Vec4& Mat4::operator[](const int i) {
	if (i < 0 || i > 3) {
		throw std::invalid_argument("Please enter a valid argument (0 - 3)");
	}
	return rows[i];
}

Mat4 Mat4::operator*(const Mat4& matrix) const {
	Mat4 returnMatrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			returnMatrix.rows[i][j] = this->rows[i] ^ matrix.cols[j];
			returnMatrix.cols[j][i] = this->rows[i] ^ matrix.cols[j];
		}
	}
	return returnMatrix;
}

Vec4 Mat4::operator*(const Vec4& vector) const {
	Vec4 returnVector;
	for (int i = 0; i < 4; i++) {
		returnVector[i] = this->rows[i] ^ vector;
	}
	return returnVector;
}

Mat4 Mat4::Identity() {
	Vec4 rows[4] = { Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 0, 1) };
	return Mat4(rows);
}

Mat4::Mat4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3) {
	this->rows[0] = row0;
	this->rows[1] = row1;
	this->rows[2] = row2;
	this->rows[3] = row3;
	for (int i = 0; i < 4; i++) {
		this->cols[i] = Vec4(rows[0][i], rows[1][i], rows[2][i], rows[3][i]);
	}
}

Mat4 Mat4::Translate(Vec4& translationVector) {
	Vec4 rows[4] = {Vec4(1, 0, 0, translationVector[0])
		, Vec4(0, 1, 0, translationVector[1])
		, Vec4(0, 0, 1, translationVector[2])
		, Vec4(0, 0, 0, 1) };
	return Mat4(rows);
}
// angels are in radians.
Mat4 Mat4::Rotate(int axis, float theta) {
	if (axis < 0 || axis > 2) {
		throw new std::invalid_argument("Please enter a valid axis");
	}
	float cosTheta, sinTheta;
	cosTheta = cos(theta);
	sinTheta = sin(theta);
	Vec4 rows[4] = {Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 0, 1)};
	switch (axis) {
	case 0:
		rows[1] = Vec4(0, cosTheta, -sinTheta, 0);
		rows[2] = Vec4(0, sinTheta, cosTheta, 0);
		break;
	case 1:
		rows[0] = Vec4(cosTheta, 0, sinTheta, 0);
		rows[2] = Vec4(-sinTheta, 0, cosTheta, 0);
		break;
	case 2: 
		rows[0] = Vec4(cosTheta, -sinTheta, 0, 0);
		rows[1] = Vec4(sinTheta, cosTheta, 0, 0);
		break;
	}
	return Mat4(rows);
}

Mat4 Mat4::Scale(Vec4& scalingVector) {
	Vec4 rows[4] = { Vec4(scalingVector[0] , 0, 0, 0)
		, Vec4(0, scalingVector[1], 0, 0)
		, Vec4(0, 0, scalingVector[2], 0)
		, Vec4(0, 0, 0, 1) };
	return Mat4(rows);
}

