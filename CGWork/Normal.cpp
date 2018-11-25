#include "Normal.h"



Normal::Normal(Vec4 midpoint, Vec4 direction) :
	midpoint(midpoint), direction(direction)
{
	target = Vec4(midpoint.xCoord() + direction.xCoord(), midpoint.yCoord() + direction.xCoord(),
		midpoint.zCoord() + direction.zCoord(), 0);
}


Normal::~Normal()
{
}
