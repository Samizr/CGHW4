//
//  vec4.h
//  graphics
//
//  Created by Firas Habib on 08/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#ifndef vec4_h
#define vec4_h

#include <stdio.h>
#include <stdexcept>
#include "vec4.h"
class Vec4 {
private:
	float _xCoord;
	float _yCoord;
	float _zCoord;
	float _wCoord;

public:
	Vec4();
	Vec4(float xCoord, float yCoord, float zCoord, float wCoord);
	Vec4 operator+(const Vec4& other) const;
	Vec4 operator-(const Vec4& other) const;
	Vec4 operator*(const float& a) const;
	// inner multiplication.
	Vec4 operator*(const Vec4& other) const;
	// multiplication of row and col. useful for matrix calculations
	float operator^(const Vec4& other) const;
	// cross product
	Vec4 crossProduct(const Vec4& other) const;
	float& operator[](const int& i);
	bool operator==(const Vec4 vec) const;
	bool operator!=(const Vec4 vec) const;
	Vec4 normalize();
	float cosineAngle(const Vec4& other);
	float xCoord() const;
	float yCoord() const;
	float zCoord() const;
	float wCoord() const;
};

#endif /* vec4_h */
