//
//  Scene.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//
// comment
#include "Scene.h"

Scene::Scene(Renderer& renderer){
	this->m_renderer = renderer;
	this->activeCamera = -1;
	this->activeModel = -1;
	this->secondActiveModel = -1;
	this->dualView = false;
}

int Scene::addModel(Model* model){
	this->models[modelIdGenerator] = model;
	if (dualView && activeModel > secondActiveModel) {
		secondActiveModel = modelIdGenerator;
	}
	else {
		activeModel = modelIdGenerator;
	}
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

Model * Scene::getSecondActiveModel()
{
	if (secondActiveModel == -1) {
		return nullptr;
	}
	return models[secondActiveModel];
}

Camera* Scene::getActiveCamera() {
	if (activeCamera == -1) {
		return nullptr;
	}
	return cameras[activeCamera];
}

void Scene::draw(COLORREF* bitArr, CRect rect) {
	if (activeModel == -1 || activeCamera == -1) {
		return;
	}
	Model* model = models[activeModel];
	Camera* camera = cameras[activeCamera];
	this->m_renderer.setCameraMatrix(camera->getTransformationMatrix());
	this->m_renderer.setProjectionMatrix(camera->getProjectionMatrix());
	//this->m_renderer.setObjectWorldMatrix(model->getTransformationMatrix());
	this->m_renderer.setMainRect(rect);

	if (dualView) {
		if (secondActiveModel == -1) {
			secondActiveModel = activeModel;
			Model* duplicate = new Model(model->getGeometry());
			addModel(duplicate);
		}
		Model* secondModel = models[secondActiveModel];
		CRect leftRect = rect, rightRect = rect;
		leftRect.right /= 2;
		rightRect.left = leftRect.right;
		m_renderer.drawWireframe(bitArr, leftRect, model);
		m_renderer.drawWireframe(bitArr, rightRect, secondModel);
	}
	else {
		m_renderer.drawWireframe(bitArr, rect, model);
	}
}

void Scene::disablePolygonNormals()
{ 
	m_renderer.disablePolygonNormals();
}

void Scene::enablePolygonNormals()
{
	m_renderer.enablePolygonNormals();
}

void Scene::enableVertexNormals() {
	this->m_renderer.enableVertexNormals();
}

void Scene::disableVertexNormals() {
	this->m_renderer.disableVertexNormals();
}

void Scene::enableDualView()
{
	dualView = true;
}

void Scene::disableDualView()
{
	dualView = false;
}


void Scene::enableBoundingBox() {
	m_renderer.enableBoundingBox();
}


void Scene::disableBoundingBox() {
	m_renderer.disableBoundingBox();
}

int Scene::cameraIdGenerator = 0;
int Scene::modelIdGenerator = 0;