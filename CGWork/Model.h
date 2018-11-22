//
//  Model.h
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include <stdio.h>
#include "Mat4.h"

class Model {
protected:
	virtual ~Model() {}
	// sends the renderer the geometry in world coordinates.
	void virtual draw() = 0;
	//Geometry T;
	Mat4 mTransform;
};
#endif /* Model_h */
