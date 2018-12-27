//
//  vec4.cpp
//  graphics
//
//  Created by Firas Habib on 08/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#include "vec4.h"
Vec4::Vec4() :_xCoord(0.0), _yCoord(0.0), _zCoord(0.0), _wCoord(0.0) {

}

Vec4::Vec4(float xCoord, float yCoord, float zCoord, float wCoord)
	: _xCoord(xCoord), _yCoord(yCoord), _zCoord(zCoord), _wCoord(wCoord) {

}

Vec4 Vec4::operator+(const Vec4& other) const {
	return Vec4(this->_xCoord + other._xCoord, this->_yCoord + other._yCoord,
		this->_zCoord + other._zCoord, this->_wCoord + other._wCoord);
}

Vec4 Vec4::operator-(const Vec4& other) const {
	return *this + (other * -1);
}

Vec4 Vec4::operator*(const float& a) const {
	return Vec4(a * this->_xCoord, a * this->_yCoord, a * this->_zCoord, a * this->_wCoord);
}

// dot product
Vec4 Vec4::operator*(const Vec4& other) const {
	return Vec4(this->_xCoord * other._xCoord, this->_yCoord * other._yCoord,
		this->_zCoord * other._zCoord, this->_wCoord * other._wCoord);
}

// multiplication of row and col. useful for matrix calculations
float Vec4::operator^(const Vec4& other) const {
	return(this->_xCoord * other._xCoord + this->_yCoord * other._yCoord
		+ this->_zCoord * other._zCoord + this->_wCoord * other._wCoord);
}

float& Vec4::operator[](const int& i) {
	switch (i) {
	case 0:
		return this->_xCoord;
	case 1:
		return this->_yCoord;
	case 2:
		return this->_zCoord;
	case 3:
		return this->_wCoord;
	default:
		throw new std::invalid_argument("Please enter a valid argument (0 - 3)");
		break;
	}
}

bool Vec4::operator==(const Vec4 vec) const
{
	return vec.xCoord() == xCoord() && vec.yCoord() == yCoord() && vec.zCoord() == zCoord() && vec.wCoord() == wCoord();
}

float Vec4::xCoord() const{
	return this->_xCoord;
}

float Vec4::yCoord() const{
	return this->_yCoord;
}

float Vec4::zCoord() const{
	return this->_zCoord;
}

float Vec4::wCoord() const{
	return this->_wCoord;
}

Vec4 Vec4::normalize() {
	float size = sqrt(pow(_xCoord, 2) + pow(_yCoord, 2) + pow(_zCoord, 2));
	return Vec4(_xCoord / size, _yCoord / size, _zCoord / size, _wCoord);
}

float Vec4::cosineAngle(const Vec4& other)
{
	Vec4 dotProduct = (*this) * other;
	float sum = dotProduct._xCoord + dotProduct._yCoord + dotProduct._zCoord;
	float thisDivisor = sqrt(pow(_xCoord, 2) + pow(_yCoord, 2) + pow(_zCoord, 2));
	float otherDivisor = sqrt(pow(other._xCoord, 2) + pow(other._yCoord, 2) + pow(other._zCoord, 2));
	return abs(sum/(thisDivisor * otherDivisor));
}

Vec4 Vec4::crossProduct(const Vec4& other) const {
	Vec4 returnVector;
	returnVector._xCoord = (this->_yCoord * other._zCoord) - (this->_zCoord * other._yCoord);
	returnVector._yCoord = (this->_zCoord * other._xCoord) - (this->_xCoord * other._zCoord);
	returnVector._zCoord = (this->_xCoord * other._yCoord) - (this->_yCoord * other._xCoord);
	returnVector._wCoord = 1;
	return returnVector;
}