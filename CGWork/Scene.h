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
	bool withBoundingBox;
	bool withBackfaceCulling;

	PngWrapper* pngBackgroundImage;
	bool withPngBackground;
	bool withSilhouette;
	bool repeatMode;
	bool wireframeMode;

	//LIGHTING RELATED:
	std::array<LightParams, NUM_LIGHT_SOURCES> lightSources;
	LightParams ambientLight;
	double ambientFraction;
	double diffuseFraction;
	double specularFraction;
	int cosinComponent;

	//DRAW FUNCTIONS:
	void drawWireframe(COLORREF* bitArr, CRect rect);
	void drawSolid(COLORREF* bitArr, CRect rect);
	void drawMiscellaneous(COLORREF* bitArr, CRect rect);

public:
	Scene();
	int addModel(Model* model);
	int addCamera(Camera* camera);
	std::map<int, Model*>& getAllModels();
	void loadFromScene(const Scene& other);
	void setRenderer(Renderer& renderer);
	Renderer& getRenderer();

	void setWireframeMode();
	void setSolidMode();
	Model* getModel(int id);
	Camera* getCamera(int id);
	Model* getActiveModel();
	Camera* getActiveCamera();
	void setAmbientLight(LightParams light);
	void setLightSource(LightParams light, int id);
	void setActiveModelByID(int id);
	int getActiveModelID();

	void clear();
	void draw(COLORREF* bitArr, CRect rect);
	void enableBoundingBox();
	void enablePolygonNormals();
	void enablePolygonNormalInvert();
	void enableVertexNormalInvert();
	void enableBackfaceCulling();
	void enableSilhouettes();
	void enablePNGBackground();
	void enableRepeatMode();
	void enableParametricTextures();
	void setVertexNormalsMode(VNMode mode);
	void setBackgroundColor(COLORREF clr);
	void setSilhouetteColor(COLORREF clr);
	void setLightingMode(LightMode mode);
	void disableBoundingBox();
	void disablePolygonNormals();
	void disablePolygonNormalInvert();
	void disableVertexNormalInvert();
	void disableBackfaceCulling();
	void disableSilhouettes();
	void disablePNGBackground();
	void disableRepeatMode();
	void disableParametricTextures();
	void setLightAmbientVariable(double data);
	void setLightDiffuseVariable(double data);
	void setLightSpecularVariable(double data);
	void setLightCosineComponent(double data);
	void setFogParams(FogParams fog);
	void getSceneDepthParams(float *min, float *max);
	void setPngBackgroundImage(PngWrapper* pngBackgroundImage);
	void enableBackgroundImage();
	void disableBackgroundImage();
};
#endif /* Scene_h */
