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

// For Status Bar access
#include "MainFrm.h"

// Static Functions:
static AXIS sceneAxisTranslator(int guiID);
static void initializeBMI(BITMAPINFO& bminfo, CRect rect);
static void resetModel(Model* model);
static COLORREF invertRB(COLORREF clr);
static void writeColorRefArrayToPng(COLORREF* bitArr, const char* name, CRect rect);
static void invertRBArray(COLORREF* array, CRect rect);
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
	ON_COMMAND(ID_VIEW_SPLITSCREEN, &CCGWorkView::OnViewSplitscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPLITSCREEN, &CCGWorkView::OnUpdateViewSplitscreen)
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

CCGWorkView::CCGWorkView(){
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
	scene.setLightingMode(FLAT);
	scene.setLightAmbientVariable(m_lMaterialAmbient);
	scene.setLightDiffuseVariable(m_lMaterialDiffuse);
	scene.setLightSpecularVariable(m_lMaterialSpecular);
	scene.setLightCosineComponent(m_nMaterialCosineFactor);	
	scene.setBackgroundColor(m_clrBackground);
	scene.draw(bitArray ,r);
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
	CRect r;

	GetClientRect(&r);
	if (!m_bRenderToScreen) {
		r = outputRect;
	}
	CDC *pDCToUse = m_pDbDC;
	int h = r.bottom - r.top;
	int w = r.right - r.left;
	initializeBMI(bminfo, r);

	if (bitArray != nullptr) {
		delete bitArray;
	}
	bitArray = new COLORREF[h * w];
	
	//DEBUG, REMOVE!
	activeDebugFeatures1();
	//DEBUG END

	scene.draw(bitArray, r);
	invertRBArray(bitArray, r);	//SIGNIFICANT PERFORMANCE SLOWDOWN!
	SetDIBits(*m_pDbDC, m_pDbBitMap, 0, h, bitArray, &bminfo, 0);

	if (m_bRenderToScreen && pDCToUse != m_pDC) {
		m_pDC->BitBlt(r.left, r.top, r.right, r.bottom, pDCToUse, r.left, r.top, SRCCOPY);
	}
	else {
		writeColorRefArrayToPng(bitArray, pngSavePath, r);
		m_bRenderToScreen = true;
	}
}

static void writeColorRefArrayToPng(COLORREF* bitArr, const char* name, CRect rect) {
	PngWrapper pngWrapper(name, rect.Width(), rect.Height());
	pngWrapper.InitWritePng();
	for (int i = 0; i < rect.Height(); i++) {
		for (int j = 0; j < rect.Width(); j++) {
			int red = GetRValue(bitArr[j + rect.Width() * ((rect.Height() - 1) - i)]);
			int green = GetGValue(bitArr[j + rect.Width() * ((rect.Height() - 1) - i)]);
			int blue = GetBValue(bitArr[j + rect.Width() * ((rect.Height() - 1) - i)]);
			pngWrapper.SetValue(j, i, SET_RGB(red, green, blue));
		}
	}
	pngWrapper.WritePng();
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
		::loadedScene.clear();
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		
		//Main Model Initiation 
		::loadedGeometry.setLineClr(::loadedScene.getModel(0)->getGeometry().getLineClr()); //SETS THE MAIN MODEL COLOR TO BE SIMILAR TO THE FIRST COLOR IN SCENE
		Model* mainModel = new Model(::loadedGeometry);
		::loadedScene.setMainModel(mainModel);
		
		//Scene Update
		scene.setRenderer(renderer);
		scene.loadFromScene(::loadedScene);
		
		//New Camera Initiation:
		float distance = (::loadedGeometry.getMaxZ() - ::loadedGeometry.getMinZ());
		m_nPerspectiveD = 3 * distance;
		m_nPerspectiveAlpha = distance;
		Camera* newCamera = new Camera();
		newCamera->LookAt(Vec4(0, 0, 3 * distance, 0), Vec4(0, 0, 0, 0), Vec4(0, 1, 0, 0));
		scene.addCamera(newCamera);

		resetButtons();
		m_clrBackground = STANDARD_BACKGROUND_COLOR;
		scene.setBackgroundColor(m_clrBackground);


		if (m_bIsPerspective) {
			newCamera->Perspective(m_nPerspectiveD, m_nPerspectiveAlpha);
		}

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
	activeCamera->Ortho();
	Invalidate();		// redraw using the new view.
}


void CCGWorkView::OnViewPerspective()
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	Camera* activeCamera = scene.getActiveCamera();
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


// OPTIONS HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnOptionsLineColor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		Model* model;
		if (m_nIsSubobjectMode) {
			Geometry& geometry = scene.getActiveModel()->getGeometry();
			geometry.setLineClr(CD.GetColor());
		}
		else {
			for (std::pair<int, Model*> pair : scene.getAllModels()) {
				Geometry& geometry = pair.second->getGeometry();
				geometry.setLineClr(CD.GetColor());
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
	else if (m_lnLastXPos > point.x) {
		transform(POSITIVE);
	}
	else if (m_lnLastXPos < point.x) {
		transform(NEGATIVE);
	}
	m_lnLastXPos = point.x;
	Invalidate();
}

//Parses the requested transformation and requests the correct transformation:
void CCGWorkView::transform(Direction direction)
{
	Model* model = nullptr;
	if (m_nIsSubobjectMode) {
		model = scene.getModel(m_nSubobject);
	}
	else {
		model = scene.getMainModel();
	}
	if (model == nullptr) {
		return;
	}

	//CODE FOR SENDING SUBOBJECT TO CENTER:
	//Geometry& geometry = model->getGeometry();
	//float centerX = (geometry.getMaxX() + geometry.getMinX()) / 2;
	//float centerY = (geometry.getMaxY() + geometry.getMinY()) / 2;
	//float centerZ = (geometry.getMaxZ() + geometry.getMinZ()) / 2;

	//Vec4 center = scene.getMainModel()->getTransformationMatrix() * model->getTransformationMatrix() * Vec4(centerX, centerY, centerZ, -1);
	//if (m_nIsSubobjectMode) {
	//	model->prependToTransformation(Mat4::Translate(center * (-1)));
	//}

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
		//scalingQuota *= (direction == NEGATIVE ? (float)101/100 : (float)99/100);
		break;
	}

}

void CCGWorkView::OnOptionsFinenesscontrol()
{
	FinenessControlDialog dlg(::CGSkelFFCState.FineNess);
	if (dlg.DoModal() == IDOK) {
		::CGSkelFFCState.FineNess = dlg.fineness;
	}
}


void CCGWorkView::OnViewSplitscreen()
{
	m_bDualView = !m_bDualView;
	m_bDualView == true ? scene.enableDualView() : scene.disableDualView();
	Invalidate();
}


void CCGWorkView::OnUpdateViewSplitscreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bDualView);
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
	resetButtons();
	resetModel(scene.getMainModel());
	for (std::pair<int, Model*> pair : scene.getAllModels()) {
		resetModel(pair.second);
	}
	scene.disableBackgroundImage();
	scene.disableRepeatMode();
	Invalidate();
}

void resetModel(Model* model) {
	if (model == nullptr)
		return;
	model->setTransformation(Mat4::Identity());
	Geometry& geometry = model->getGeometry();
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
	AdvancedDialog dlg;
	dlg.subChecked = m_nIsSubobjectMode;
	dlg.subobjectID = m_nSubobject;
	dlg.maxSubobject = scene.getAllModels().size() - 1;
	dlg.invertVertexNormals = m_bInvertVertexNormals;
	dlg.invertPolygonNormals = m_bInvertPolygonNormals;
	dlg.importNormals = !m_bCalculateVertexNormals;
	
	if (dlg.DoModal() == IDOK) {
		m_nIsSubobjectMode = dlg.subChecked;
		m_nSubobject = dlg.subobjectID;
		scene.setActiveModelID(m_nSubobject);
		m_nIsSubobjectMode == true ? scene.setSubobjectMode() : scene.setWholeobjectMode();
		m_bInvertPolygonNormals = dlg.invertPolygonNormals;
		m_bInvertPolygonNormals == true ? scene.enablePolygonNormalInvert() : scene.disablePolygonNormalInvert();
		m_bInvertVertexNormals = dlg.invertVertexNormals;
		m_bInvertVertexNormals == true ? scene.enableVertexNormalInvert() : scene.disableVertexNormalInvert();
		m_bCalculateVertexNormals = !dlg.importNormals;
		sceneSetVertexNormalMode();
	}
	Invalidate();
}

void CCGWorkView::resetButtons() {
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


//DEBUG FUNCTIONS:
void CCGWorkView::activeDebugFeatures1()
{
	//resetButtons();
	//resetModel(scene.getMainModel());
	//for (std::pair<int, Model*> pair : scene.getAllModels()) {
	//	resetModel(pair.second);
	//}
	//scene.setLightingMode(PHONG);
	::CGSkelFFCState.FineNess = 20;
	//m_lights[0].colorR = 255;
	//m_lights[0].colorG = 255;
	//m_lights[0].colorB = 255;
	//m_lights[0].dirX = 0;
	//m_lights[0].dirY = 0;
	//m_lights[0].dirZ = 2;
	//m_lights[0].enabled = true;
	//m_lights[0].type = LIGHT_TYPE_DIRECTIONAL;
	//scene.setLightSource(m_lights[0], 0);
	scene.setSolidMode();
}

