//
//  mat4.h
//  graphics
//
//  Created by Firas Habib on 08/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#ifndef mat4_h
#define mat4_h

#include <stdio.h>
#include "vec4.h"

class Mat4 {
private:
	Vec4 cols[4];
	Vec4 rows[4];
public:
	Mat4();
	Mat4(Vec4* rows);
	Mat4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3);
	Mat4 operator*(const Mat4& matrix) const;
	Mat4 operator*(const float& a) const;
	Mat4 operator+(const Mat4& matrix) const;
	Mat4 getTranspose() const;
	Mat4 getInverse() const;
	Vec4& operator[](const int i);
	Vec4 Mat4::operator*(const Vec4& vector) const;
	static Mat4 Identity();
	static Mat4 Translate(Vec4& translationVector);
	static Mat4 Rotate(int axis, float theta);
	static Mat4 Scale(Vec4& scalingMatrixVector);
};

#endif /* mat4_h */
