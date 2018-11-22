//
//  Scene.hpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#include <stdio.h>
#include <vector>
#include "Mat4.h"
#include "Camera.h"
#include "Model.h"
#include "Renderer.h"

class Scene {
	std::vector<Model*> models;
	std::vector<Camera*> cameras;
	Renderer* m_renderer;


};
#endif /* Scene_h */
