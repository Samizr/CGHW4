// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "stdafx.h"
#include "CGWork.h"

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"
#include "cstring"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"
#include "MouseSensitivityDialog.h"
#include "FinenessControlDialog.h"
#include "PerspectiveParametersDialog.h"
#include "AdvancedDialog.h"
#include "FileRenderingDialog.h"
#include "SuperSamplingAADialog.h"
#include "MotionBlurDialog.h"
#include "FogEffectsDialog.h"
#include "ParametricTexturesDialog.h"


// For Status Bar access
#include "MainFrm.h"

#define NO_FILTER 0
#define FILTER_3X3 1
#define FILTER_5X5 2
#define NO_MODELS_IN_SCENE -1

// Static Functions:
static AXIS sceneAxisTranslator(int guiID);
static void initializeBMI(BITMAPINFO& bminfo, CRect rect);
static void resetModel(Model* model);
static COLORREF invertRB(COLORREF clr);
static void writeColorRefArrayToPng(COLORREF* bitArr, const char* name, CRect rect);
static void invertRBArray(COLORREF* array, CRect rect);
static CRect determineRenderRect(int AAFilterSize, CRect currentRect);
// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))

#define INITIAL_SENSITIVITY 10
/////////////////////////////////////////////////////////////////////////////
// CCGWorkView


IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)m_nScaleSensetivity
	//Common Function Mappings
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD_OBJECT, OnFileLoadObject)
	ON_COMMAND(ID_FILE_LOADBACKGROUND, OnFileLoadBackground)

	ON_WM_TIMER()

	//TOOLBAR - View Function Mapping
	ON_COMMAND(ID_BOXFRAME, OnViewBoxFrame)
	ON_COMMAND(ID_POLYNORMALS, OnViewPolyNormals)
	ON_COMMAND(ID_VERTEXNORMALS, OnViewVertexNormals)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)

	ON_UPDATE_COMMAND_UI(ID_BOXFRAME, OnUpdateViewBoxFrame)
	ON_UPDATE_COMMAND_UI(ID_POLYNORMALS, OnUpdateViewPolyNormals)
	ON_UPDATE_COMMAND_UI(ID_VERTEXNORMALS, OnUpdateViewVertexNormals)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)

	//TOOLBAR - Action Function Mapping
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_COMMAND(ID_VIEWSPACE, OnActionViewSpace)
	ON_COMMAND(ID_OBJECTSPACE, OnActionObjectSpace)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_UPDATE_COMMAND_UI(ID_VIEWSPACE, OnUpdateActionViewSpace)
	ON_UPDATE_COMMAND_UI(ID_OBJECTSPACE, OnUpdateActionObjectSpace)

	//TOOLBAR - Axis Function Mapping
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)

	//TOOLBAR - Options Function Mapping
	ON_COMMAND(ID_LINECOLOR, OnOptionsLineColor)
	ON_COMMAND(ID_BACKGROUNDCOLOR, OnOptionsBackgroundColor)
	ON_COMMAND(ID_NORMALCOLOR, OnOptionsNormalcolor)
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, OnOptionsMousesensitivity)
	ON_COMMAND(ID_SILHOUETTECOLOR, OnOptionsSilhouettecolor)

	//Shading Function Mapping - (Not related to HW2)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_SHADING_PHONG, OnLightShadingPhong)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_PHONG, OnUpdateLightShadingPhong)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_OPTIONS_FINENESSCONTROL, &CCGWorkView::OnOptionsFinenesscontrol)
	ON_COMMAND(ID_OPTIONS_PERSPECTIVECONTROL, &CCGWorkView::OnOptionsPerspectivecontrol)
	ON_COMMAND(ID_VIEW_RESETVIEW, &CCGWorkView::OnViewResetview)
	ON_COMMAND(ID_VIEW_OBJECTSELECTION, &CCGWorkView::OnViewAdvancedSettings)
	ON_COMMAND(ID_VIEW_BACKFACECULLING, &CCGWorkView::OnViewBackfaceculling)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BACKFACECULLING, &CCGWorkView::OnUpdateViewBackfaceculling)
	ON_COMMAND(ID_VIEW_SILHOUETTE, &CCGWorkView::OnViewSilhouette)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SILHOUETTE, &CCGWorkView::OnUpdateViewSilhouette)
	ON_COMMAND(ID_BACKGROUND_STRECHMODE, &CCGWorkView::OnBackgroundStrechmode)
	ON_COMMAND(ID_BACKGROUND_REPEATMODE, &CCGWorkView::OnBackgroundRepeatmode)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUND_STRECHMODE, &CCGWorkView::OnUpdateBackgroundStrechmode)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUND_REPEATMODE, &CCGWorkView::OnUpdateBackgroundRepeatmode)
	ON_COMMAND(ID_LIGHT_MATERIAL, &CCGWorkView::OnLightMaterial)

	ON_COMMAND(ID_SOLIDRENDERING_TOFILE, &CCGWorkView::OnSolidrenderingTofile)

	ON_COMMAND(ID_SOLIDRENDERING_WIREFRAME, &CCGWorkView::OnWireframeToScreen)
	ON_COMMAND(ID_SOLIDRENDERING_TOSCREEN, &CCGWorkView::OnSolidrenderingToscreen)
	ON_UPDATE_COMMAND_UI(ID_SOLIDRENDERING_WIREFRAME, &CCGWorkView::OnUpdateWireframeToScreen)
	ON_UPDATE_COMMAND_UI(ID_SOLIDRENDERING_TOSCREEN, &CCGWorkView::OnUpdateSolidrenderingToscreen)
	ON_COMMAND(ID_SOLIDRENDERING_WIREFRAMTOFILE, &CCGWorkView::OnWireframTofile)
	ON_COMMAND(ID_PLANE_XY, &CCGWorkView::OnPlaneXy)
	ON_UPDATE_COMMAND_UI(ID_PLANE_XY, &CCGWorkView::OnUpdatePlaneXy)
	ON_COMMAND(ID_FOG_PARAMETERS, &CCGWorkView::OnFogEffects)
	ON_COMMAND(ID_SOLIDRENDERING_SUPERSAMPLINGANTI, &CCGWorkView::OnSolidrenderingSupersamplinganti)
	ON_COMMAND(ID_VIEW_MOTIONBLUR, &CCGWorkView::OnViewMotionblur)
	ON_COMMAND(ID_TEXTURES_LOADTEXTURE, &CCGWorkView::OnTexturesLoadtexture)
	ON_COMMAND(ID_FOG_DISPLAYSCENEDEPTH, &CCGWorkView::OnFogDisplayscenedepth)
END_MESSAGE_MAP()


// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
	GLUquadric *quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluCylinder(quad, radius, 0.0, height, 20, 20);
	gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView() {
	//Set default values
	m_nAxis = ID_AXIS_X;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	m_bBoxFrame = false;
	m_bPolyNormals = false;
	m_bVertexNormals = false;
	m_bIsViewSpace = true;
	m_bDualView = false;
	m_bCalculateVertexNormals = true;
	m_bInvertPolygonNormals = false;
	m_bInvertVertexNormals = false;
	m_bBackfaceCullingActive = false;
	m_bWithSilhouette = false;
	m_bWithParametricTextures = false;

	//Fog Related
	m_fog.active = false;
	m_fog.color = 0;
	m_fog.z_near = 0;
	m_fog.z_far = 0;

	m_bRepeatMode = false;
	m_nTranslationSensetivity = INITIAL_SENSITIVITY;
	m_nRotationSensetivity = INITIAL_SENSITIVITY;
	m_nScaleSensetivity = INITIAL_SENSITIVITY * 2;
	m_bRenderToScreen = true;
	m_bIsWireframe = true;
	m_nSubobject = 0;
	m_nIsSubobjectMode = false;
	m_clrBackground = STANDARD_BACKGROUND_COLOR;
	bitArray = nullptr;

	// Transformations Quantitive Setup:
	translationQuota = 1.2;
	rotationQuota = 0.5;
	scalingQuota = 1;


	//Shading Related
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled = true;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;

	//init the supersampling stuff
	m_nSuperSamplingFilter = 0;
	m_nSuperSamplingSize = 0;

	//init motion blur stuff
	m_bWithMotionBlur = false;
	m_fMotionBlurTValue = 0.25;
	lastFrame = nullptr;
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	return 0;
}

// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);
	scene.setRenderer(renderer);
	scene.setLightingMode(FLAT);
	scene.setLightAmbientVariable(m_lMaterialAmbient);
	scene.setLightDiffuseVariable(m_lMaterialDiffuse);
	scene.setLightSpecularVariable(m_lMaterialSpecular);
	scene.setLightCosineComponent(m_nMaterialCosineFactor);
	scene.setBackgroundColor(m_clrBackground);
	scene.draw(bitArray, r);
	scene.setFogParams(m_fog);
	//activeDebugFeatures3();
	//activeDebugFeatures2();
	//activeDebugFeatures1();
	SetTimer(1, 1, NULL);
	int h = r.bottom - r.top;
	int w = r.right - r.left;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers

void CCGWorkView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth / (GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{
	return TRUE;
}

// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}

BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC)
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.


	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////


void CCGWorkView::OnDraw(CDC* pDC)
{
	static float theta = 0.0f;
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//Pre Render Processing
	GetClientRect(&drawRect);
	renderRect = determineRenderRect(m_nSuperSamplingSize, drawRect);
	if (!m_bRenderToScreen) {
		renderRect = outputRect;
		drawRect = outputRect;
	}
	CDC *pDCToUse = m_pDbDC;
	int renderH = renderRect.bottom - renderRect.top;
	int renderW = renderRect.right - renderRect.left;
	int drawH = drawRect.bottom - drawRect.top;
	int drawW = drawRect.right - drawRect.left;
	initializeBMI(bminfo, drawRect);
	if (bitArray != nullptr) {
		if (m_bWithMotionBlur) {
			free(lastFrame);
			lastFrame = bitArray;
		}
		else {
			delete bitArray;
		}
	}

	// Main Render Processing
	bitArray = new COLORREF[renderH * renderW];
	scene.draw(bitArray, renderRect);

	// Post Render Processing
	if (scene.getActiveModelID() != NO_MODELS_IN_SCENE) {
		if (m_nSuperSamplingSize != NO_FILTER) {
			int filterSize = m_nSuperSamplingSize == 1 ? 3 : 5;
			COLORREF* superSampled = bitArray;
			bitArray = new COLORREF[drawH * drawW];
			scene.getRenderer().superSampleImage(superSampled, bitArray, renderRect, drawRect, filterSize, m_nSuperSamplingFilter);
		}
		if (m_bWithMotionBlur) {
			scene.getRenderer().interpolateFrames(lastFrame, bitArray, drawRect, m_fMotionBlurTValue);
		}
	}

	// Block Transfer and Draw Processing
	invertRBArray(bitArray, drawRect);
	SetDIBits(*m_pDbDC, m_pDbBitMap, 0, drawH, bitArray, &bminfo, 0);
	if (m_bRenderToScreen && pDCToUse != m_pDC) {
		m_pDC->BitBlt(drawRect.left, drawRect.top, drawRect.right, drawRect.bottom, pDCToUse, drawRect.left, drawRect.top, SRCCOPY);
	}
	else {
		writeColorRefArrayToPng(bitArray, pngSavePath, drawRect);
		m_bRenderToScreen = true;
	}
}

static void writeColorRefArrayToPng(COLORREF* bitArr, const char* name, CRect rect) {
	PngWrapper pngWrapper(name, rect.Width(), rect.Height());
	pngWrapper.InitWritePng();
	invertRBArray(bitArr, rect);
	for (int i = 0; i < rect.Height(); i++) {
		for (int j = 0; j < rect.Width(); j++) {
			int red = GetRValue(bitArr[j + rect.Width() * ((rect.Height() - 1) - i)]);
			int green = GetGValue(bitArr[j + rect.Width() * ((rect.Height() - 1) - i)]);
			int blue = GetBValue(bitArr[j + rect.Width() * ((rect.Height() - 1) - i)]);
			pngWrapper.SetValue(j, i, SET_RGB(red, green, blue));
		}
	}
	pngWrapper.WritePng();
	invertRBArray(bitArr, rect);

}
/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy()
{
	CView::OnDestroy();

	// delete the DC
	if (m_pDC) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoadObject()
{
	TCHAR szFilters[] = _T("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {

		//Load New Objects & Models
		::loadedGeometry.clear();
		::loadedModel.clear();
		m_strItdFileName = dlg.GetPathName();	// Full path and filename
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);

		//Main Model Initiation 
		Model* newModel = new Model(loadedModel);
		scene.addModel(newModel);
		//::loadedGeometry.setLineClr(newModel->getSubGeometry(0).getLineClr()); //SETS THE MAIN MODEL COLOR TO BE SIMILAR TO THE FIRST COLOR IN SCENE

		//New Camera Initiation (IF none exist):
		if (scene.getActiveCamera() == nullptr) {
			float distance = (::loadedGeometry.getMaxZ() - ::loadedGeometry.getMinZ());
			m_nPerspectiveD = 3 * distance;
			m_nPerspectiveAlpha = distance;
			Camera* newCamera = new Camera();
			newCamera->LookAt(Vec4(0, 0, 3 * distance, 0), Vec4(0, 0, 0, 0), Vec4(0, 1, 0, 0));
			if (m_bIsPerspective) {
				newCamera->Perspective(m_nPerspectiveD, m_nPerspectiveAlpha);
			}
			scene.addCamera(newCamera);
		}

		//resetButtons();
		m_clrBackground = STANDARD_BACKGROUND_COLOR;
		scene.setBackgroundColor(m_clrBackground);

		Invalidate();	// force a WM_PAINT for drawing.
	}

}

void CCGWorkView::OnFileLoadBackground()
{
	TCHAR szFilters[] = _T("PNG Image Files (*.png)|*.png|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	if (dlg.DoModal() == IDOK) {
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper* p = new PngWrapper();
		CStringA charstr(m_strItdFileName);
		const char* fileName = charstr;
		OutputDebugStringA(fileName);
		p->SetFileName(fileName);
		p->ReadPng();
		scene.setPngBackgroundImage(p);
		scene.enableBackgroundImage();
		Invalidate();
	}
}


// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic()
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	Camera* activeCamera = scene.getActiveCamera();
	if (!activeCamera)
		return;
	activeCamera->Ortho();
	Invalidate();		// redraw using the new view.
}


void CCGWorkView::OnViewPerspective()
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	Camera* activeCamera = scene.getActiveCamera();
	if (!activeCamera)
		return;
	activeCamera->Perspective(m_nPerspectiveD, m_nPerspectiveAlpha);
	Invalidate();
}

void CCGWorkView::OnViewBoxFrame()
{
	m_bBoxFrame = !m_bBoxFrame;
	if (m_bBoxFrame) {
		scene.enableBoundingBox();
	}
	else {
		scene.disableBoundingBox();
	}
	Invalidate();
}

void CCGWorkView::OnViewPolyNormals()
{
	m_bPolyNormals = !m_bPolyNormals;
	if (m_bPolyNormals) {
		scene.enablePolygonNormals();
	}
	else {
		scene.disablePolygonNormals();
	}
	Invalidate();
}

void CCGWorkView::OnViewSilhouette()
{
	m_bWithSilhouette = !m_bWithSilhouette;
	if (m_bWithSilhouette) {
		scene.enableSilhouettes();
	}
	else {
		scene.disableSilhouettes();
	}
	Invalidate();
}


void CCGWorkView::OnViewBackfaceculling()
{
	m_bBackfaceCullingActive = !m_bBackfaceCullingActive;
	if (m_bBackfaceCullingActive) {
		scene.enableBackfaceCulling();
	}
	else {
		scene.disableBackfaceCulling();
	}
	Invalidate();
}

void CCGWorkView::OnActionViewSpace()
{
	m_bIsViewSpace = true;
	Invalidate();
}

void CCGWorkView::OnViewVertexNormals()
{
	m_bVertexNormals = !m_bVertexNormals;
	sceneSetVertexNormalMode();
	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnUpdateViewBoxFrame(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bBoxFrame);
}

void CCGWorkView::OnUpdateActionViewSpace(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bIsViewSpace);
}

void CCGWorkView::OnUpdateViewPolyNormals(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bPolyNormals);
}

void CCGWorkView::OnUpdateViewVertexNormals(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bVertexNormals);
}

void CCGWorkView::OnUpdateViewBackfaceculling(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bBackfaceCullingActive);
}

void CCGWorkView::OnUpdateViewSilhouette(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bWithSilhouette);
}



// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate()
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate()
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale()
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}

void CCGWorkView::OnActionObjectSpace()
{
	m_bIsViewSpace = false;
	Invalidate();
}

void CCGWorkView::OnUpdateActionObjectSpace(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(!m_bIsViewSpace);
}

// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX()
{
	m_nAxis = ID_AXIS_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_X);
}

void CCGWorkView::OnAxisY()
{
	m_nAxis = ID_AXIS_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Y);
}

void CCGWorkView::OnAxisZ()
{
	m_nAxis = ID_AXIS_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Z);
}

void CCGWorkView::OnPlaneXy()
{
	m_nAxis = ID_PLANE_XY;
}


void CCGWorkView::OnUpdatePlaneXy(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_PLANE_XY);
}


// OPTIONS HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnOptionsLineColor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		Model* model = scene.getActiveModel();
		if (m_nIsSubobjectMode) {
			model->getSubGeometry(m_nSubobject).setLineClr(CD.GetColor());
		}
		else {
			model->getMainGeometry().setLineClr(CD.GetColor());
			for (int i = 0; i < model->getSubGeometriesNum(); i++) {
				model->getSubGeometry(i).setLineClr(CD.GetColor());
			}
		}
		Invalidate();
	}
}

void CCGWorkView::OnOptionsBackgroundColor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		m_clrBackground = CD.GetColor();
		scene.setBackgroundColor(m_clrBackground);
		Invalidate();
	}
}

void CCGWorkView::OnOptionsNormalcolor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		scene.getRenderer().setNormalClr(CD.GetColor());
		Invalidate();
	}
}



void CCGWorkView::OnOptionsSilhouettecolor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		scene.setSilhouetteColor(CD.GetColor());
		Invalidate();
	}
}


void CCGWorkView::OnOptionsMousesensitivity()
{
	//Dialog initiation
	MouseSensitivityDialog MSDialog(m_nTranslationSensetivity, m_nRotationSensetivity, m_nScaleSensetivity);

	//Dialog values retrival 
	if (MSDialog.DoModal() == IDOK) {
		m_nTranslationSensetivity = MSDialog.getTranslationSensitivity();
		m_nRotationSensetivity = MSDialog.getRotationSensitivity();
		m_nScaleSensetivity = MSDialog.getScalingSensitivity();
		Invalidate();
	}
}

// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat()
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
	scene.setLightingMode(FLAT);
	Invalidate();
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}

void CCGWorkView::OnLightShadingGouraud()
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
	scene.setLightingMode(GOURAUD);
	Invalidate();
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}

void CCGWorkView::OnLightShadingPhong()
{
	m_nLightShading = ID_LIGHT_SHADING_PHONG;
	scene.setLightingMode(PHONG);
	Invalidate();
}


void CCGWorkView::OnUpdateLightShadingPhong(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_PHONG);
}


// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants()
{
	CLightDialog dlg;
	for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
	{
		dlg.SetDialogData((LightID)id, m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT, m_ambientLight);

	if (dlg.DoModal() == IDOK)
	{
		for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
		{
			m_lights[id] = dlg.GetDialogData((LightID)id);
			//FIX INVERT RB BUG:
			//m_lights[id].colorB = 
			scene.setLightSource(m_lights[id], id);
		}
		m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
		scene.setAmbientLight(m_ambientLight);
	}
	Invalidate();
}


void CCGWorkView::OnLightMaterial()
{
	CMaterialDlg dlg;
	dlg.SetDialogData(IDC_MATERIAL_AMBIENT, m_lMaterialAmbient);
	dlg.SetDialogData(IDC_MATERIAL_DIFFUSE, m_lMaterialDiffuse);
	dlg.SetDialogData(IDC_MATERIAL_SHININESS, m_lMaterialSpecular);
	dlg.SetDialogData(IDC_MATERIAL_SPECULAR, m_nMaterialCosineFactor);

	if (dlg.DoModal() == IDOK) {
		scene.setLightAmbientVariable(dlg.m_ambient);
		scene.setLightDiffuseVariable(dlg.m_diffuse);
		scene.setLightSpecularVariable(dlg.m_specular);
		scene.setLightCosineComponent(dlg.m_cosinComponent);
		m_lMaterialAmbient = dlg.m_ambient;
		m_lMaterialDiffuse = dlg.m_diffuse;
		m_lMaterialSpecular = dlg.m_specular;
		m_nMaterialCosineFactor = dlg.m_cosinComponent;
	}
	Invalidate();
}


void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
	//if (nIDEvent == 1)
	//	Invalidate();  THIS MADE THE GUI VERY LAGGY AND WE COULD NOT CHANGE SETTINGS.
}


// FOG EFFECTS HANDLING ////////////////////////////////////////////
void CCGWorkView::OnFogEffects()
{
	FogEffectsDialog dlg;
	dlg.fog = m_fog;
	if (dlg.DoModal() == IDOK) {
		m_fog = dlg.fog;
		scene.setFogParams(m_fog);
	}
	Invalidate();
}



// MOUSE MOVEMENT HANDLING ///////////////////////////////////////////



//STATIC FUNCTIONS ////////////////////////////////////////////////

#include "Mat4.h"
#include "LinePlotter.h"

AXIS sceneAxisTranslator(int guiID)
{
	switch (guiID) {
	case ID_AXIS_X: return XAXIS;
	case ID_AXIS_Y: return YAXIS;
	case ID_AXIS_Z: return ZAXIS;
	case ID_PLANE_XY: return XAXIS;
	default: throw std::bad_exception();
	}
}

void initializeBMI(BITMAPINFO& bminfo, CRect rect)
{
	bminfo.bmiHeader.biSize = sizeof(bminfo.bmiHeader);
	bminfo.bmiHeader.biWidth = rect.right - rect.left;
	bminfo.bmiHeader.biHeight = rect.bottom - rect.top;
	bminfo.bmiHeader.biPlanes = 1;
	bminfo.bmiHeader.biBitCount = 32;
	bminfo.bmiHeader.biCompression = BI_RGB;
	bminfo.bmiHeader.biSizeImage = 0;
	bminfo.bmiHeader.biXPelsPerMeter = 1;
	bminfo.bmiHeader.biYPelsPerMeter = 1;
	bminfo.bmiHeader.biClrUsed = 0;
	bminfo.bmiHeader.biClrImportant = 0;

}


COLORREF invertRB(COLORREF clr)
{
	int iRed = GetRValue(clr);
	int iBlue = GetBValue(clr);
	int iGreen = GetGValue(clr);
	return RGB(iBlue, iGreen, iRed);
}


void CCGWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bAllowTransformations = true;
	m_lnLastXPos = point.x;
	m_lnLastYPos = point.y;
	if (m_bDualView) {
		CRect r;
		GetClientRect(&r);
		m_bLeftModel = point.x < r.Width() / 2 ? true : false;
	}
	CView::OnLButtonDown(nFlags, point);
}


void CCGWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bAllowTransformations = false;
	CView::OnLButtonUp(nFlags, point);
}


void CCGWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	if (!m_bAllowTransformations) {
		return;
	}
	if (m_lnLastXPos > point.x)
		transform(POSITIVE);
	else if (m_lnLastXPos < point.x)
		transform(NEGATIVE);

	if (m_nAxis == ID_PLANE_XY) {
		m_nAxis = ID_AXIS_Y;
		if (m_lnLastYPos < point.y)
			transform(POSITIVE);
		else if (m_lnLastYPos > point.y)
			transform(NEGATIVE);
		m_nAxis = ID_PLANE_XY;
	}
	m_lnLastYPos = point.y;
	m_lnLastXPos = point.x;
	Invalidate();
}

//Parses the requested transformation and requests the correct transformation:
void CCGWorkView::transform(Direction direction)
{
	Model* model = scene.getActiveModel();
	if (model == nullptr) {
		return;
	}
	if (m_nIsSubobjectMode) {  //TRICK - MAKE A MODEL OUT OF GEOMETRY SO WE WOULD BE ABLE TO PERFORM TRANSFORMATION ON SUBOBJECT
		Geometry& geometry = model->getSubGeometry(m_nSubobject);
		Model* subModel = new Model(geometry);
		subModel->setTransformation(model->getSubGeometryTransformationMatrix(m_nSubobject));
		model = subModel;
	}


	//CODE FOR TRANSFORMATIONS:
	AXIS sceneAxis = sceneAxisTranslator(m_nAxis);
	float adjustedQuota;
	switch (m_nAction) {

	case (ID_ACTION_ROTATE):
		adjustedQuota = direction * rotationQuota * m_nRotationSensetivity / 100;
		if (m_bIsViewSpace)
			model->rotateViewSpace(sceneAxis, adjustedQuota);
		else
			model->rotateObjectSpace(sceneAxis, adjustedQuota);
		break;

	case (ID_ACTION_TRANSLATE):
		adjustedQuota = direction * translationQuota * m_nTranslationSensetivity / 100;
		if (m_bIsViewSpace)
			model->translateViewSpace(sceneAxis, adjustedQuota);
		else
			model->translateObjectSpace(sceneAxis, adjustedQuota);
		break;

	case (ID_ACTION_SCALE):
		float percentage = m_nScaleSensetivity / 1000;
		adjustedQuota = (direction == NEGATIVE ? 1 / (scalingQuota - percentage) : (scalingQuota - percentage));
		if (m_bIsViewSpace)
			model->scaleViewSpace(sceneAxis, adjustedQuota);
		else
			model->scaleObjectSpace(sceneAxis, adjustedQuota);
		break;
	}
	if (m_nIsSubobjectMode) {
		Model* subModel = model;
		model = scene.getActiveModel();
		model->setSubgeometryTransformation(m_nSubobject, subModel->getTransformationMatrix());
		delete subModel;
	}
}

void CCGWorkView::OnOptionsFinenesscontrol()
{
	FinenessControlDialog dlg(::CGSkelFFCState.FineNess);
	if (dlg.DoModal() == IDOK) {
		::CGSkelFFCState.FineNess = dlg.fineness;
	}
}

void CCGWorkView::OnOptionsPerspectivecontrol()
{
	Camera* cam = scene.getActiveCamera();
	if (cam == nullptr) {
		MessageBox(_T("ERROR: No active camera, please load an object."), _T("Error"),
			MB_ICONERROR | MB_OK);
		return;
	}

	PerspectiveParametersDialog dlg(m_nPerspectiveD, m_nPerspectiveAlpha);
	if (dlg.DoModal() == IDOK) {
		m_nPerspectiveD = dlg.d;
		m_nPerspectiveAlpha = dlg.alpha;
		if (m_bIsPerspective) {
			cam->Perspective(m_nPerspectiveD, m_nPerspectiveAlpha);
			Invalidate();
		}
	}
}

void CCGWorkView::OnViewResetview()
{
	int answer = MessageBox(_T("WARNING: You are about to clear the scene, are you sure?"), _T("WARNING"),
		MB_OKCANCEL | MB_ICONQUESTION);
	if (answer == IDOK) {
		resetButtons();
		scene.clear();
		resetModel(scene.getActiveModel());
		for (std::pair<int, Model*> pair : scene.getAllModels()) {
			resetModel(pair.second);
		}
		scene.disableBackgroundImage();
		scene.disableRepeatMode();
		Invalidate();
	}
}

void resetModel(Model* model) {
	if (model == nullptr)
		return;
	model->setTransformation(Mat4::Identity());
	Geometry& geometry = model->getMainGeometry();
}

void invertRBArray(COLORREF * array, CRect rect)
{
	for (int i = rect.left; i < rect.right; i++) {
		for (int j = rect.top; j < rect.bottom; j++) {
			COLORREF newclr = invertRB(array[i + j * rect.Width()]);
			array[i + j * rect.Width()] = newclr;
		}
	}
}

CRect determineRenderRect(int AAFilterSize, CRect currentRect)
{
	CRect renderRect = currentRect;
	if (AAFilterSize == FILTER_3X3) {
		renderRect.left = 0;
		renderRect.top = 0;
		renderRect.right = 3 * currentRect.right;
		renderRect.bottom = 3 * currentRect.bottom;
	}
	else if (AAFilterSize == FILTER_5X5) { 
		renderRect.left = 0;
		renderRect.top = 0;
		renderRect.right = 5 * currentRect.right;
		renderRect.bottom = 5 * currentRect.bottom;
	}
	return renderRect;
}

void CCGWorkView::sceneSetVertexNormalMode()
{
	if (m_bVertexNormals && m_bCalculateVertexNormals) {
		scene.setVertexNormalsMode(CALCULATED);
	}
	else if (m_bVertexNormals && !m_bCalculateVertexNormals) {
		scene.setVertexNormalsMode(IMPORTED);
	}
	else {
		scene.setVertexNormalsMode(NONE);
	}
}


void CCGWorkView::OnViewAdvancedSettings()
{
	if (!scene.getActiveModel()) {
		MessageBox(_T("NOT ALLOWED: Please load a module first before accessing the advanced settings!"), _T("Unallowed!"), MB_ICONEXCLAMATION);
		return;
	}

	AdvancedDialog dlg;
	dlg.subobjectID = m_nSubobject;
	dlg.modelId = scene.getActiveModelID();
	dlg.maxSubobject = scene.getActiveModel()->getSubGeometriesNum() - 1;
	dlg.maxObject = scene.getAllModels().size() - 1;
	dlg.invertVertexNormals = m_bInvertVertexNormals;
	dlg.invertPolygonNormals = m_bInvertPolygonNormals;
	dlg.importNormals = !m_bCalculateVertexNormals;
	dlg.subChecked = m_nIsSubobjectMode;

	if (dlg.DoModal() == IDOK) {
		m_nSubobject = dlg.subobjectID;
		scene.setActiveModelByID(dlg.modelId);
		m_nIsSubobjectMode = dlg.subChecked;
		m_bInvertPolygonNormals = dlg.invertPolygonNormals;
		m_bInvertVertexNormals = dlg.invertVertexNormals;
		m_bInvertPolygonNormals == true ? scene.enablePolygonNormalInvert() : scene.disablePolygonNormalInvert();
		m_bInvertVertexNormals == true ? scene.enableVertexNormalInvert() : scene.disableVertexNormalInvert();
		m_bCalculateVertexNormals = !dlg.importNormals;
		sceneSetVertexNormalMode();
	}
	Invalidate();
}

void CCGWorkView::resetButtons() {
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	m_bBoxFrame = false;
	m_bPolyNormals = false;
	m_bVertexNormals = false;
	m_bIsViewSpace = true;
}


void CCGWorkView::OnBackgroundStrechmode()
{
	m_bRepeatMode = false;
	scene.disableRepeatMode();
	Invalidate();
}


void CCGWorkView::OnBackgroundRepeatmode()
{
	m_bRepeatMode = true;
	scene.enableRepeatMode();
	Invalidate();
}


void CCGWorkView::OnUpdateBackgroundStrechmode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bRepeatMode);
}


void CCGWorkView::OnUpdateBackgroundRepeatmode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRepeatMode);
}

// /////////////// DIFFERENT RENDERING MODES:

void CCGWorkView::OnSolidrenderingToscreen()
{
	m_bRenderToScreen = true;
	m_bIsWireframe = false;
	scene.setSolidMode();
	Invalidate();
}

void CCGWorkView::OnWireframeToScreen()
{
	m_bRenderToScreen = true;
	m_bIsWireframe = true;
	scene.setWireframeMode();
	Invalidate();
}

void CCGWorkView::OnUpdateWireframeToScreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bIsWireframe);
}


void CCGWorkView::OnUpdateSolidrenderingToscreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bIsWireframe);
}


void CCGWorkView::OnSolidrenderingTofile()
{
	FileRenderingDialog dlg;
	CRect currentRect;
	GetClientRect(&currentRect);
	dlg.desiredHeight = currentRect.Height();
	dlg.desiredWidth = currentRect.Width();
	if (dlg.DoModal() == IDOK) {
		outputRect.left = 0;
		outputRect.top = 0;
		outputRect.right = dlg.desiredWidth;
		outputRect.bottom = dlg.desiredHeight;
		CStringA charstr(dlg.desiredPath);
		pngSavePath = strdup(charstr);
		m_bRenderToScreen = false;
		m_bIsWireframe = false;
		Invalidate();
	}
}



void CCGWorkView::OnWireframTofile()
{
	FileRenderingDialog dlg;
	CRect currentRect;
	GetClientRect(&currentRect);
	dlg.desiredHeight = currentRect.Height();
	dlg.desiredWidth = currentRect.Width();
	if (dlg.DoModal() == IDOK) {
		outputRect.left = 0;
		outputRect.top = 0;
		outputRect.right = dlg.desiredWidth;
		outputRect.bottom = dlg.desiredHeight;
		CStringA charstr(dlg.desiredPath);
		pngSavePath = strdup(charstr);
		m_bRenderToScreen = false;
		m_bIsWireframe = true;
		Invalidate();
	}
}


// DEBUG INFRASTRUCTURE /////////////////////////

//PREPARES SCENE FOR DEBUGGING. CALLED EVERY TIME ON DRAW.
void CCGWorkView::activeDebugFeatures1()
{
	//resetButtons();
	//resetModel(scene.getMainModel());
	//for (std::pair<int, Model*> pair : scene.getAllModels()) {
	//	resetModel(pair.second);
	//}
	scene.setLightingMode(PHONG);
	//scene.enableBackfaceCulling();
	::CGSkelFFCState.FineNess = 20;
	m_lights[0].colorR = 255;
	m_lights[0].colorG = 255;
	m_lights[0].colorB = 255;
	m_lights[0].posX = 20;
	m_lights[0].posY = 0;
	m_lights[0].posZ = 0;
	m_lights[0].dirX = 0;
	m_lights[0].dirY = 0;
	m_lights[0].dirZ = -2;
	m_lights[0].enabled = true;
	m_lights[0].type = LIGHT_TYPE_POINT;
	scene.setLightSource(m_lights[0], 0);
	scene.setSolidMode();
}

//SIMULATES A LOADING PROCESS, WITH LOADING m_stdItdFileName. CALLED ON INITIALIZATION
void CCGWorkView::activeDebugFeatures2() {

	//Load New Objects & Models
	::loadedGeometry.clear();
	::loadedModel.clear();
	::CGSkelFFCState.FineNess = 20;
	m_strItdFileName = L"C:\\Users\\sami.zr\\Desktop\\BasicModels\\teapot.itd";	// Full path and filename
	CGSkelProcessIritDataFiles(m_strItdFileName, 1);

	//Main Model Initiation 
	Model* newModel = new Model(loadedModel);
	scene.addModel(newModel);

	//New Camera Initiation (IF none exist):
	if (scene.getActiveCamera() == nullptr) {
		float distance = (::loadedGeometry.getMaxZ() - ::loadedGeometry.getMinZ());
		m_nPerspectiveD = 3 * distance;
		m_nPerspectiveAlpha = distance;
		Camera* newCamera = new Camera();
		newCamera->LookAt(Vec4(0, 0, 3 * distance, 0), Vec4(0, 0, 0, 0), Vec4(0, 1, 0, 0));
		if (m_bIsPerspective) {
			newCamera->Perspective(m_nPerspectiveD, m_nPerspectiveAlpha);
		}
		scene.addCamera(newCamera);
	}

	//if (!AllocConsole())
		//AfxMessageBox((CString)"Failed to create the console!", MB_ICONEXCLAMATION);

	//transform(POSITIVE);

	//m_clrBackground = STANDARD_BACKGROUND_COLOR;
	//scene.setBackgroundColor(m_clrBackground);

	//Invalidate();	// force a WM_PAINT for drawing.
}

//Debugging console:
void CCGWorkView::activeDebugFeatures3() {
	Geometry square;
	Vertex* a = new Vertex(1, 1, -2);
	Vertex* b = new Vertex(-1, 1, -2);
	Vertex* c = new Vertex(1, -1, 2);
	Vertex* d = new Vertex(-1, -1, 2);
	Edge* e1 = new Edge(a, b);
	Edge* e2 = new Edge(b, d);
	Edge* e3 = new Edge(d, c);
	Edge* e4 = new Edge(c, a);
	Face* face = new Face(e1, e2, e3, e4);
	square.addVertex(a);
	square.addVertex(b);
	square.addVertex(c);
	square.addVertex(d);
	square.addEdge(e1);
	square.addEdge(e2);
	square.addEdge(e3);
	square.addEdge(e4);
	a->addFace(face);
	b->addFace(face);
	c->addFace(face);
	d->addFace(face);
	a->setUV(1, 1);
	b->setUV(0, 1);
	c->setUV(1, 0);
	d->setUV(0, 0);

	square.addFace(face);
	square.setLineClr(RGB(255, 0, 0));
	Model* squareModel = new Model(square);
	
	scene.addModel(squareModel);

	float distance = 5;
	m_nPerspectiveD = 3 * distance;
	m_nPerspectiveAlpha = distance;
	Camera* newCamera = new Camera();
	newCamera->LookAt(Vec4(0, 0, 3 * distance, 0), Vec4(0, 0, 0, 0), Vec4(0, 1, 0, 0));
	scene.addCamera(newCamera);
}

void CCGWorkView::OnSolidrenderingSupersamplinganti()
{
	SuperSamplingAADialog dlg;
	dlg.filterType = m_nSuperSamplingFilter;
	dlg.filterSize = m_nSuperSamplingSize;
	if (dlg.DoModal() == IDOK) {
		m_nSuperSamplingSize = dlg.filterSize;
		m_nSuperSamplingFilter = dlg.filterType;
		Invalidate();
	}	

}


void CCGWorkView::OnViewMotionblur() {
	MotionBlurDialog dlg;
	dlg.motionBlurActive = m_bWithMotionBlur;
	dlg.motionBlurTValue = m_fMotionBlurTValue;
	if (dlg.DoModal() == IDOK) {
		m_bWithMotionBlur = dlg.motionBlurActive;
		m_fMotionBlurTValue = dlg.motionBlurTValue;
	}
}

void CCGWorkView::OnTexturesLoadtexture()
{
	if (!scene.getActiveModel()) {
		MessageBox(_T("NOT ALLOWED: Please load a module first before accessing parametric texture settings!"), _T("Unallowed!"), MB_ICONEXCLAMATION);
		return;
	}
	ParametricTexturesDialog dlg;
	dlg.enableParametricTextures = m_bWithParametricTextures;
	dlg.modelNum = scene.getActiveModelID();
	bool modelOk = false;
	while (!modelOk && dlg.DoModal() == IDOK) {
		if (scene.getModel(dlg.modelNum)->UVAttributesValid()) {
			scene.getModel(dlg.modelNum)->setModelTexturePNG(dlg.pngTextureImage);
			scene.setActiveModelByID(dlg.modelNum);
			modelOk = true;
		}
		else {
			::AfxMessageBox(_T("NOT ALLOWED: The model you chose does not have valid parametrization"));
		}
	}
	m_bWithParametricTextures = dlg.enableParametricTextures;
	m_bWithParametricTextures ? scene.enableParametricTextures() : scene.disableParametricTextures();
	Invalidate();
}


void CCGWorkView::OnFogDisplayscenedepth()
{
	float min, max;
	char buff[100];
	scene.getSceneDepthParams(&min, &max);
	sprintf_s(buff, "Minimum Object Depth = %f\n Maximum Object Depth = %f", min, max);
	MessageBox((CString)(buff), _T("Fog Parameters"), MB_ICONINFORMATION);
}
