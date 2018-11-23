//
//  Scene.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//
// comment
#include "Scene.h"

int Scene::addModel(Model* model){
	//TODO make sure vector function called insert.
	this->models[modelIdGenerator] = model;
	activeModel = modelIdGenerator;
	return modelIdGenerator++;
}

int Scene::addCamera(Camera* camera){
	this->cameras[cameraIdGenerator] = camera; 
	activeCamera = cameraIdGenerator;
	return cameraIdGenerator++;
}

Model* Scene::getModel(int id) {
	if (models.find(id) == models.end()) {
		return nullptr;
	}
	return models[id];
}

Camera* Scene::getCamera(int id) {
	if (cameras.find(id) == cameras.end()) {
		return nullptr;
	}
	return cameras[id];
}

Model* Scene::getActiveModel() {
	if (activeModel == -1) {
		return nullptr;
	}
	return models[activeModel];
}

Camera* Scene::getActiveCamera() {
	if (activeCamera == -1) {
		return nullptr;
	}
	return cameras[activeCamera];
}

void Scene::draw(CDC* pdc, CRect rect) {
	if (activeModel == -1 || activeCamera == -1) {
		return;
	}
	Model* model = models[activeModel];
	Camera* camera = cameras[activeCamera];
	this->m_renderer.setCameraMatrix(camera->getTransformationMatrix());
	this->m_renderer.setProjectionMatrix(camera->getTransformationMatrix());
	this->m_renderer.setObjectWorldMatrix(model->getTransformationMatrix());
	float deltaW = rect.right - rect.left;
	float deltaH = rect.top - rect.bottom;
	float sumW = rect.right + rect.left;
	float sumH = rect.top + rect.bottom;
	Vec4 vecVPM[4] = { Vec4(deltaW / 2, 0, 0, sumW / 2), Vec4(0, deltaH / 2, 0, sumH / 2), Vec4(0, 0, 0.5, 0.5), Vec4(0, 0, 0, 1) };
	this->m_renderer.setWindowMatrix(Mat4(vecVPM));
	m_renderer.drawWireframe(pdc, &model->getGeometry());
}

Scene::Scene(Renderer& renderer){
	this->m_renderer = renderer;
	this->activeCamera = -1;
	this->activeModel = -1;
}

int Scene::cameraIdGenerator = 0;
int Scene::modelIdGenerator = 0;