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
#include <array>
#include "Mat4.h"
#include "Camera.h"
#include "Model.h"
#include "Renderer.h"
#include "Light.h"

#define NUM_LIGHT_SOURCES 7

class Scene {
	std::map<int, Model*> models;
	std::map<int, Camera*> cameras;
	Model* mainModel;
	int activeCamera;
	int activeModel;
	int secondActiveModel;
	bool dualView;
	bool subobjectDraw;
	Renderer m_renderer;
	static int cameraIdGenerator;
	static int modelIdGenerator;
	bool withBoundingBox;
	bool withBackfaceCulling;
	PngWrapper* pngImage;
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
	void drawWireframe(COLORREF* bitArr, CRect rect);
	void drawSolid(COLORREF* bitArr, CRect rect);

public:
	Scene();
	int addModel(Model* model);
	int addCamera(Camera* camera);
	std::map<int, Model*>& getAllModels();
	
	void setRenderer(Renderer& renderer);
	Renderer& getRenderer();

	Model* getMainModel();
	void setMainModel(Model* model);
	void setWireframeMode();
	void setSolidMode();
	void setSubobjectMode();
	void setWholeobjectMode();
	Model* getModel(int id);
	Camera* getCamera(int id);
	Model* getActiveModel();
	Model* getSecondActiveModel();
	Camera* getActiveCamera();
	void setAmbientLight(LightParams light);
	void setLightSource(LightParams light, int id);

	void clear();
	void setActiveModelID(int id);
	void draw(COLORREF* bitArr, CRect rect);
	void enableBoundingBox();
	void enablePolygonNormals();
	void enablePolygonNormalInvert();
	void enableVertexNormalInvert();
	void enableBackfaceCulling();
	void enableSilhouettes();
	void enablePNGBackground();
	void enableRepeatMode();
	void enableDualView();
	void setVertexNormalsMode(VNMode mode);
	void setBackgroundColor(COLORREF clr);
	void setSilhouetteColor(COLORREF clr);
	void disableBoundingBox();
	void disablePolygonNormals();
	void disablePolygonNormalInvert();
	void disableVertexNormalInvert();
	void disableBackfaceCulling();
	void disableSilhouettes();
	void disablePNGBackground();
	void disableRepeatMode();
	void disableDualView();
	void setLightAmbientVariable(double data);
	void setLightDiffuseVariable(double data);
	void setLightSpecularVariable(double data);
	void setLightCosineComponent(double data);

	COLORREF getLightingColor(Vec4 normal, COLORREF originalClr);

	void setPngBackgroundImage(PngWrapper* pngImage);
	void enableBackgroundImage();
	void disableBackgroundImage();
};
#endif /* Scene_h */
