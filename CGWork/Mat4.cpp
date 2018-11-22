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
		for (int j = 0; j < 4; j++) {
			// transposing the vector from row into col.
			this->cols[i] = Vec4(rows[0][i], rows[1][i], rows[2][i], rows[3][i]);
		}
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




