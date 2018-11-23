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
#include <map>
#include "Mat4.h"
#include "Camera.h"
#include "Model.h"
#include "Renderer.h"

class Scene {
	std::map<int, Model*> models;
	std::map<int, Camera*> cameras;
	int activeCamera;
	int activeModel;
	Renderer m_renderer;
	static int cameraIdGenerator;
	static int modelIdGenerator;
public:
	Scene(Renderer& renderer);
	int addModel(Model* model);
	int addCamera(Camera* camera);
	Model& getModel(int id);
	Camera& getCamera(int id);
	void draw(CDC* pdc, CRect rect);
};

int Scene::cameraIdGenerator = 0;
int Scene::modelIdGenerator = 0;
#endif /* Scene_h */
