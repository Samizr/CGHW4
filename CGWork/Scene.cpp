//
//  Scene.cpp
//  graphics
//
//  Created by Firas Habib on 12/11/2018.
//  Copyright © 2018 firas habib. All rights reserved.
//
// comment
#include "Scene.h"

static Mat4 generateNormalizationMatrix(Geometry* geometry);

Scene::Scene(){
	this->activeCamera = -1;
	this->activeModel = -1;
	this->secondActiveModel = -1;
	this->dualView = false;
	this->subobjectDraw = false;
	this->mainModel = nullptr;
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

std::map<int, Model*>& Scene::getAllModels()
{
	return models;
}

void Scene::setRenderer(Renderer & renderer)
{
	this->m_renderer = renderer;
}

Renderer & Scene::getRenderer()
{
	return m_renderer;
}

Model * Scene::getMainModel()
{
	return mainModel;
}

void Scene::setMainModel(Model * model)
{
	mainModel = model;
}

void Scene::setSubobjectMode()
{
	subobjectDraw = true;
}

void Scene::setWholeobjectMode()
{
	subobjectDraw = false;
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

void Scene::clear()
{
	models.clear();
	cameras.clear();
	mainModel = nullptr;
	modelIdGenerator = 0;
	cameraIdGenerator = 0;
}

void Scene::setActiveModelID(int id)
{
	activeModel = id;
}

void Scene::draw(COLORREF* bitArr, CRect rect) {
	if (activeModel == -1 || activeCamera == -1) {
		return;
	}

	Camera* camera = cameras[activeCamera];
	this->m_renderer.setCameraMatrix(camera->getTransformationMatrix());
	this->m_renderer.setProjectionMatrix(camera->getProjectionMatrix());
	this->m_renderer.setNormalizationMatrix(generateNormalizationMatrix(&mainModel->getGeometry()));
	this->m_renderer.setMainRect(rect);

	for (std::pair<int, Model*> pair : models) {
		//if (subobjectDraw && pair.first == activeModel) {
		//	this->m_renderer.setObjectWorldMatrix(models[activeModel]->getTransformationMatrix());
		//}
		//else {
		//	this->m_renderer.setObjectWorldMatrix(mainModel->getTransformationMatrix());
		//}
		this->m_renderer.setObjectWorldMatrix(models[pair.first]->getTransformationMatrix() * mainModel->getTransformationMatrix());
		m_renderer.drawWireframe(bitArr, rect, pair.second);
	}

	//DUAL SCREEN MODE CODE:
	/*if (dualView) {
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
	}*/
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



static Mat4 generateNormalizationMatrix(Geometry* geometry) {
	float sumX, sumY, sumZ, deltaX, deltaY, deltaZ;
	sumX = geometry->getMaxX() + geometry->getMinX();
	sumY = geometry->getMaxY() + geometry->getMinY();
	sumZ = geometry->getMaxZ() + geometry->getMinZ();
	deltaX = geometry->getMaxX() - geometry->getMinX();
	deltaY = geometry->getMaxY() - geometry->getMinY();
	deltaZ = geometry->getMaxZ() - geometry->getMinZ();
	deltaX *= 2;
	deltaY *= 2;

	Vec4 vecNM[4] = { Vec4(2 / deltaX, 0, 0, -sumX / deltaX), Vec4(0, 2 / deltaY, 0, -sumY / deltaY), Vec4(0, 0, -2 / deltaZ, sumZ / deltaZ), Vec4(0, 0, 0, 1) };
	return Mat4(vecNM);
}
