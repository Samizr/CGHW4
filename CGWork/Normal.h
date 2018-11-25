#pragma once

#include "Vec4.h"

class Normal
{
public:
	Vec4 midpoint;
	Vec4 direction;
	Vec4 target;
	Normal() = default;
	Normal(Vec4 midpoint, Vec4 direction);
	~Normal();
};

