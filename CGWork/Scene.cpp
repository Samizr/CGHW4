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

Scene::Scene() 
{
	this->activeCamera = -1;
	this->activeModel = -1;
	this->secondActiveModel = -1;
	this->dualView = false;
	this->subobjectDraw = false;
	this->mainModel = nullptr;
	this->withBackfaceCulling = false;
	this->withBoundingBox = false;
	this->withSilhouette = false;
	this->withPngBackground = false;
	this->repeatMode = false;
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

void Scene::setAmbientLight(LightParams light)
{
	ambientLight = light;
}

void Scene::setLightSource(LightParams light, int id)
{
	lightSources[id] = light;
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
	//DRAW BACKGROUND
	if (withPngBackground && !repeatMode) {
		this->m_renderer.drawBackgoundImageStretch(bitArr, rect, pngImage);
	}
	else if (withPngBackground && repeatMode) {
		this->m_renderer.drawBackgoundImageRepeat(bitArr, rect, pngImage);
	}
	else {
		this->m_renderer.drawBackgroundColor(bitArr, rect);
	}

	//DRAW MODELS
	if (activeModel == -1 || activeCamera == -1) {
		return;
	}

	Camera* camera = cameras[activeCamera];
	this->m_renderer.setCameraMatrix(camera->getTransformationMatrix());
	this->m_renderer.setProjectionMatrix(camera->getProjectionMatrix());
	this->m_renderer.setNormalizationMatrix(generateNormalizationMatrix(&mainModel->getGeometry()));
	this->m_renderer.setMainRect(rect);

	for (std::pair<int, Model*> pair : models) {
		this->m_renderer.setObjectWorldMatrix(models[pair.first]->getTransformationMatrix() * mainModel->getTransformationMatrix());
		if (withBackfaceCulling) {
		//	m_renderer.drawWireframeBackfaceCulling(bitArr, rect, pair.second);
		}
		else {
			m_renderer.drawWireframe(bitArr, rect, pair.second);
		}
	}
	//DRAW BOUNDING BOX
	if (withBoundingBox && !subobjectDraw) {
		Geometry* geometry = &mainModel->getGeometry();
		m_renderer.drawBounding(bitArr, rect, geometry, geometry->getLineClr());
	}
	//DRAW SILHOUETTE
	if (withSilhouette) {
		Geometry* geometry = &mainModel->getGeometry();

		m_renderer.drawSilhouette(bitArr, rect, geometry);
	}
}

void Scene::disablePolygonNormals()
{ 
	m_renderer.disablePolygonNormals();
}

void Scene::disablePolygonNormalInvert()
{
	this->m_renderer.disablePolygonNormalInvert();
}

void Scene::disableVertexNormalInvert()
{
	this->m_renderer.disableVertexNormalInvert();
}

void Scene::enableBackfaceCulling()
{
	withBackfaceCulling = true;
}

void Scene::enableSilhouettes()
{
	withSilhouette = true;
}

void Scene::enablePNGBackground()
{
	withPngBackground = true;
}

void Scene::enableRepeatMode()
{
	repeatMode = true;
}

void Scene::disableBackfaceCulling()
{
	withBackfaceCulling = false;
}

void Scene::disableSilhouettes()
{
	withSilhouette = false;
}

void Scene::disablePNGBackground()
{
	withPngBackground = false;
}

void Scene::disableRepeatMode()
{
	repeatMode = false;
}

void Scene::enablePolygonNormals()
{
	this->m_renderer.enablePolygonNormals();
}

void Scene::enablePolygonNormalInvert()
{
	this->m_renderer.enablePolygonNormalInvert();
}

void Scene::enableVertexNormalInvert()
{
	this->m_renderer.enableVertexNormalInvert();
}

void Scene::setVertexNormalsMode(VNMode mode)
{
	this->m_renderer.setVertexNormalMode(mode);
}

void Scene::setBackgroundColor(COLORREF clr)
{
	this->m_renderer.setBackgroundClr(clr);
}

void Scene::setSilhouetteColor(COLORREF clr)
{
	this->m_renderer.setSilhouetteClr(clr);
}

//void Scene::enableVertexNormals() {
//	this->m_renderer.enableVertexNormals();
//}
//
//void Scene::disableVertexNormals() {
//	this->m_renderer.disableVertexNormals();
//}

void Scene::enableDualView()
{
	dualView = true;
}

void Scene::disableDualView()
{
	dualView = false;
}

void Scene::setPngBackgroundImage(PngWrapper* pngImage) {
	this->pngImage = pngImage;
}

void Scene::enableBackgroundImage() {
	this->withPngBackground = true;
}

void Scene::disableBackgroundImage() {
	this->withPngBackground = false;
}

void Scene::enableBoundingBox() {
	m_renderer.enableBoundingBox();
	withBoundingBox = true;
}

void Scene::disableBoundingBox() {
	m_renderer.disableBoundingBox();
	withBoundingBox = false;
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
