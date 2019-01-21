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
static float* createZBuffer(CRect rect);
Scene::Scene() 
{
	this->activeCamera = -1;
	this->activeModel = -1;
	this->withBackfaceCulling = false;
	this->withBoundingBox = false;
	this->withSilhouette = false;
	this->withPngBackground = false;
	this->repeatMode = false;
	this->wireframeMode = true;
}

int Scene::addModel(Model* model){
	this->models[modelIdGenerator] = model;
	activeModel = modelIdGenerator;
	model->normalizeUVAttributes();
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

void Scene::loadFromScene(const Scene & other)
{
	this->clear();
	this->models = other.models;
	this->cameras = other.cameras;
	this->modelIdGenerator = other.modelIdGenerator;
	this->cameraIdGenerator = other.cameraIdGenerator;
	this->activeModel = other.activeModel;
	this->activeCamera = other.activeCamera;

}

void Scene::setRenderer(Renderer & renderer)
{
	this->m_renderer = renderer;
}

Renderer & Scene::getRenderer()
{
	return m_renderer;
}

void Scene::setWireframeMode(){
	wireframeMode = true;
}

void Scene::setSolidMode(){
	wireframeMode = false;
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
	modelIdGenerator = 0;
	cameraIdGenerator = 0;
	activeModel = -1;
	activeCamera = -1;
}

void Scene::setActiveModelByID(int id)
{
	activeModel = id;
}

int Scene::getActiveModelID()
{
	return activeModel;
}

void Scene::draw(COLORREF* bitArr, CRect rect) {
	//DRAW BACKGROUND
	this->m_renderer.setMainRect(rect);
	if (withPngBackground && !repeatMode) {
		this->m_renderer.drawBackgoundImageStretch(bitArr, rect, pngTextureImage);
	}
	else if (withPngBackground && repeatMode) {
		this->m_renderer.drawBackgoundImageRepeat(bitArr, rect, pngTextureImage);
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
	if (wireframeMode) {
		drawWireframe(bitArr, rect);
	}
	else {
		drawSolid(bitArr, rect);
	}
	drawMiscellaneous(bitArr, rect);
	
	
}

void Scene::drawWireframe(COLORREF* bitArr, CRect rect) {
	for (std::pair<int, Model*> pair : models) {
		this->m_renderer.setNormalizationMatrix(generateNormalizationMatrix(&pair.second->getMainGeometry()));
		if (withBackfaceCulling) {
			m_renderer.drawWireframeBackfaceCulling(bitArr, rect, pair.second);
		}
		else {
			m_renderer.drawWireframe(bitArr, rect, pair.second);
		}
	}
}

void Scene::drawSolid(COLORREF* bitArr, CRect rect) {
	float* zBuffer = createZBuffer(rect);
	double materialComponents[4] = { ambientFraction, diffuseFraction, specularFraction, cosinComponent };
	for (std::pair<int, Model*> pair : models) {
		this->m_renderer.setNormalizationMatrix(generateNormalizationMatrix(&pair.second->getMainGeometry()));
		if (withBackfaceCulling) {
			m_renderer.drawSolidBackfaceCulling(bitArr, rect, pair.second, lightSources, ambientLight, materialComponents);
		}
		else {
			m_renderer.drawSolid(bitArr, zBuffer, rect, pair.second, lightSources, ambientLight, materialComponents);
		}
	}
	free(zBuffer);
}

void Scene::drawMiscellaneous(COLORREF * bitArr, CRect rect)
{
	for (int i = 0; i < models.size(); i++) {
		this->m_renderer.setNormalizationMatrix(generateNormalizationMatrix(&models[i]->getMainGeometry()));
		this->m_renderer.setObjectWorldMatrix(models[i]->getTransformationMatrix());
		Geometry* geometry = &models[i]->getMainGeometry();
		if (withBoundingBox) {
			m_renderer.drawBounding(bitArr, rect, geometry, geometry->getLineClr());
		}
		if (withSilhouette) {
			m_renderer.drawSilhouette(bitArr, rect, geometry);
		}
		
	}
}

float* createZBuffer(CRect rect) {
	float* zBuffer = new float[rect.Width() * rect.Height()];
	for (int i = 0; i < rect.Height(); i++) {
		for (int j = 0; j < rect.Width(); j++) {
			zBuffer[i * rect.Width() + j] = -std::numeric_limits<float>::infinity();;
		}
	}
	return zBuffer;
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

void Scene::enableParametricTextures()
{
	this->m_renderer.enableParametrixTextures();
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

void Scene::disableParametricTextures()
{
	this->m_renderer.disableParametrixTextures();
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

void Scene::setLightingMode(LightMode mode)
{
	this->m_renderer.setLightingMode(mode);
}

void Scene::setLightAmbientVariable(double data)
{
	ambientFraction = data;
}

void Scene::setLightDiffuseVariable(double data)
{
	diffuseFraction = data;
}

void Scene::setLightSpecularVariable(double data)
{
	specularFraction = data;
}

void Scene::setLightCosineComponent(double data)
{
	cosinComponent = data;
}

void Scene::setFogParams(FogParams fog)
{
	this->m_renderer.setFogParams(fog);
}

void Scene::getSceneDepthParams(float * min, float * max)
{
	this->m_renderer.getSceneDepthParams(min, max);
}

void Scene::setPngBackgroundImage(PngWrapper* pngTextureImage) {
	this->pngTextureImage = pngTextureImage;
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