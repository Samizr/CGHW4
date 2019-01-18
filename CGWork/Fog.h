#pragma once

#include <Windows.h>

class FogParams {
public:
	COLORREF color;
	float z_near;
	float z_far;
	bool active;
};