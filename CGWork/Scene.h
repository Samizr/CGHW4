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

public:
	Scene();
	int addModel(Model* model);
	int addCamera(Camera* camera);
	std::map<int, Model*>& getAllModels();
	
	void setRenderer(Renderer& renderer);
	Renderer& getRenderer();

	Model* getMainModel();
	void setMainModel(Model* model);
	void setSubobjectMode();
	void setWholeobjectMode();
	Model* getModel(int id);
	Camera* getCamera(int id);
	Model* getActiveModel();
	Model* getSecondActiveModel();
	Camera* getActiveCamera();

	void clear();
	void setActiveModelID(int id);
	void draw(COLORREF* bitArr, CRect rect);
	void enableBoundingBox();
	void enablePolygonNormals();
	void enablePolygonNormalInvert();
	void enableVertexNormalInvert();
	void enableBackfaceCulling();
	void enableSilhouettes();
	void setVertexNormalsMode(VNMode mode);
	void setBackgroundColor(COLORREF clr);
	void setSilhouetteColor(COLORREF clr);
	void disableBoundingBox();
	void disablePolygonNormals();
	void disablePolygonNormalInvert();
	void disableVertexNormalInvert();
	void disableBackfaceCulling();
	void disableSilhouettes();
	//void disableVertexNormals();
	//void enableVertexNormals();
	void enableDualView();
	void disableDualView();

	void setPngBackgroundImage(PngWrapper* pngImage);
	void enableBackgroundImage();
	void disableBackgroundImage();
};
#endif /* Scene_h */
