// CGWorkView.h : interface of the CCGWorkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
#define AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "gl\gl.h"    // Include the standard CGWork  headers
#include "gl\glu.h"   // Add the utility library

#include "Scene.h"
#include "Light.h"
#include <vector>

using std::vector;
class CCGWorkView : public CView
{
protected: // create from serialization only
	CCGWorkView();
	DECLARE_DYNCREATE(CCGWorkView)

	// Attributes
public:
	CCGWorkDoc* GetDocument();

	// Operations
public:

private:

	//Parameters that define the drawing and viewing of the objects:
	int m_nAxis;				// Axis of Action, X Y or Z
	int m_nAction;				// Rotate, Translate, Scale
	int m_nView;				// Orthographic, perspective
	bool m_bIsPerspective;			// is the view perspective
	bool m_bIsViewSpace;			// are we in view space or object space
	bool m_bPolyNormals;			//Polygon normals activation
	bool m_bVertexNormals;			//Vertex normals activation
	bool m_bBoxFrame;				//Bounding box activation
	bool m_bDualView;				//Split screen or not?
	bool m_bLeftModel;				//Acticating transformations on left model?	
	float m_nTranslationSensetivity;   //Mouse sensetivity for translation
	float m_nRotationSensetivity;		//Mouse sensetivity for rotation
	float m_nScaleSensetivity;			//Mouse sensetivity for scaling
	float m_nPerspectiveD;
	float m_nPerspectiveAlpha;
	int m_nSubobject;	
	bool m_nIsSubobjectMode;		
	COLORREF m_clrBackground;		//Current background color
	CString m_strItdFileName;		// file name of IRIT data

	//Scene, cameras, models and related objects:
	Renderer renderer;
	Scene scene;
	vector<int> cameraIDs;
	vector<int> modelIDs;


	//Mouse movement control section:
	bool m_bAllowTransformations;	//Set when transformations are allowed
	LONG m_lnLastXPos;				//Stores the x pos of the last place the mouse visited
	enum Direction {
		POSITIVE = -1, NEGATIVE = 1
	};
	void transform(Direction direction);

	//Sensetivity test debug configuration:
	float rotationQuota;
	float translationQuota;
	float scalingQuota;

	//Parameters not related for HW2 Implementation:
	double m_lMaterialAmbient;		// The Ambient in the scene
	double m_lMaterialDiffuse;		// The Diffuse in the scene
	double m_lMaterialSpecular;		// The Specular in the scene
	int m_nMaterialCosineFactor;		// The cosine factor for the specular
	int m_nLightShading;			// shading: Flat, Gouraud.
	LightParams m_lights[MAX_LIGHT];	//configurable lights array
	LightParams m_ambientLight;		//ambient light (only RGB is used)


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGWorkView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCGWorkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitializeCGWork();
	BOOL SetupViewingFrustum(void);
	BOOL SetupViewingOrthoConstAspect(void);

	virtual void RenderScene();


	HGLRC    m_hRC;			// holds the Rendering Context
	CDC*     m_pDC;			// holds the Device Context
	BITMAPINFO bminfo;
	int m_WindowWidth;		// hold the windows width
	int m_WindowHeight;		// hold the windows height
	double m_AspectRatio;		// hold the fixed Aspect Ration

	HBITMAP m_pDbBitMap;
	CDC* m_pDbDC;
	COLORREF* bitArray;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCGWorkView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoad();

	//TOOLBAR - View Functions
	afx_msg void OnViewBoxFrame();
	afx_msg void OnViewPolyNormals();
	afx_msg void OnViewVertexNormals();
	afx_msg void OnViewOrthographic();
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewBoxFrame(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPolyNormals(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewVertexNormals(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOrthographic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);

	//TOOLBAR - Action Functions:
	afx_msg void OnActionTranslate();
	afx_msg void OnActionRotate();
	afx_msg void OnActionScale();
	afx_msg void OnActionViewSpace();
	afx_msg void OnActionObjectSpace();
	afx_msg void OnViewSplitscreen();
	afx_msg void OnUpdateActionTranslate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionScale(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionViewSpace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionObjectSpace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSplitscreen(CCmdUI *pCmdUI);

	//TOOLBAR - Axis Functions 
	afx_msg void OnAxisX();
	afx_msg void OnAxisY();
	afx_msg void OnAxisZ();
	afx_msg void OnUpdateAxisX(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAxisY(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAxisZ(CCmdUI* pCmdUI);

	//TOOLBAR - Option Functions
	afx_msg void OnOptionsLineColor();
	afx_msg void OnOptionsBackgroundColor();
	afx_msg void OnOptionsNormalcolor();
	afx_msg void OnOptionsMousesensitivity();

	//LIGHTING OPTIONS (NOT USED IN HW2)
	afx_msg void OnLightShadingFlat();
	afx_msg void OnUpdateLightShadingFlat(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingGouraud();
	afx_msg void OnUpdateLightShadingGouraud(CCmdUI* pCmdUI);
	afx_msg void OnOptionsPerspectivecontrol();
	afx_msg void OnOptionsFinenesscontrol();
	afx_msg void OnLightConstants();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnViewResetview();
	afx_msg void OnViewObjectselection();
};

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
{
	return (CCGWorkDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
