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

// For Status Bar access
#include "MainFrm.h"

// Static Functions:
static AXIS sceneAxisTranslator(int guiID);
static void initializeBMI(BITMAPINFO& bminfo, CRect rect);
//static void resetModel(Model* model);
static COLORREF invertRB(COLORREF clr);

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))

#define INITIAL_SENSITIVITY 10
/////////////////////////////////////////////////////////////////////////////
// CCGWorkView

IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	//Common Function Mappings
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
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

	//Shading Function Mapping - (Not related to HW2)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
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
	ON_COMMAND(ID_VIEW_OBJECTSELECTION, &CCGWorkView::OnViewObjectselection)
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
	m_nTranslationSensetivity = INITIAL_SENSITIVITY;
	m_nRotationSensetivity = INITIAL_SENSITIVITY;
	m_nScaleSensetivity = INITIAL_SENSITIVITY / 5;
	m_nSubobject = -1;
	m_nIsSubobjectMode = false;
	m_clrBackground = STANDARD_BACKGROUND_COLOR;

	// Transformations Quantitive Setup:
	translationQuota = 1.2;
	rotationQuota = 0.5;
	scalingQuota = 0.8;


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
	SetTimer(1, 1, NULL);
	int h = r.bottom - r.top;
	int w = r.right - r.left;

	bitArray = new COLORREF[w * h];

	for (int i = 0; i < r.Width(); i++) {
		for (int j = 0; j < r.Height(); j++) {
			bitArray[(i - r.left) + ((r.right - r.left) * (j - r.top))] = STANDARD_BACKGROUND_COLOR;
		}
	}

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
	CDC *pDCToUse = m_pDbDC;
	int h = r.bottom - r.top;
	int w = r.right - r.left;
	initializeBMI(bminfo, r);
	if (bitArray != nullptr) {
		delete bitArray;
		bitArray = new COLORREF[h * w];
	}

	for (int i = r.left; i < r.right; i++) {
		for (int j = r.top; j < r.bottom; j++) {
			bitArray[i + j * r.Width()] = m_clrBackground;
		}
	}


	scene.draw(bitArray, r);
	SetDIBits(*m_pDbDC, m_pDbBitMap, 0, h, bitArray, &bminfo, 0);

	if (pDCToUse != m_pDC)
	{
		m_pDC->BitBlt(r.left, r.top, r.right, r.bottom, pDCToUse, r.left, r.top, SRCCOPY);
	}


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


void CCGWorkView::OnFileLoad()
{
	TCHAR szFilters[] = _T("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		::loadedGeometry.clear();
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;

		loadedScene.clear();

		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		// Open the file and read it.
		
		// Does not reload the model if requested.
		scene = loadedScene;
		scene.setRenderer(renderer);
		auto newModel = new Model(::loadedGeometry);
		scene.setMainModel(newModel); 
		float distance = (::loadedGeometry.getMaxZ() - ::loadedGeometry.getMinZ());
		m_nPerspectiveD = 3 * distance;
		m_nPerspectiveAlpha = distance;
		auto newCamera = new Camera();
		scene.addCamera(newCamera);
		newCamera->LookAt(Vec4(0, 0, 3 * distance, 0), Vec4(0, 0, 0, 0), Vec4(0, 1, 0, 0));

		resetButtons();
		m_clrBackground = STANDARD_BACKGROUND_COLOR;

		if (m_bIsPerspective) {
			newCamera->Perspective(m_nPerspectiveD, m_nPerspectiveAlpha);
		}

		Invalidate();	// force a WM_PAINT for drawing.
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

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective()
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	Camera* activeCamera = scene.getActiveCamera();
	activeCamera->Perspective(m_nPerspectiveD, m_nPerspectiveAlpha);
	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
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

void CCGWorkView::OnUpdateViewBoxFrame(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bBoxFrame);
}

void CCGWorkView::OnActionViewSpace()
{
	m_bIsViewSpace = true;
	Invalidate();
}

void CCGWorkView::OnUpdateActionViewSpace(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bIsViewSpace);
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

void CCGWorkView::OnUpdateViewPolyNormals(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bPolyNormals);
}

void CCGWorkView::OnViewVertexNormals()
{
	m_bVertexNormals = !m_bVertexNormals;
	if (m_bVertexNormals) {
		scene.enableVertexNormals();
	}
	else {
		scene.disableVertexNormals();
	}
	Invalidate();
}

void CCGWorkView::OnUpdateViewVertexNormals(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bVertexNormals);
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
			geometry.setLineClr(invertRB(CD.GetColor()));
		}
		else {
			for (std::pair<int, Model*> pair : scene.getAllModels()) {
				Geometry& geometry = pair.second->getGeometry();
				geometry.setLineClr(invertRB(CD.GetColor()));
			}
		}
		Invalidate();
	}
}
void CCGWorkView::OnOptionsBackgroundColor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		//Model* model;
		//if (m_bDualView && !m_bLeftModel) {
		//	model = scene.getSecondActiveModel();
		//}
		//else {
		//	model = scene.getActiveModel();
		//}
		//Geometry& geometry = model->getGeometry();
		//geometry.setBackgroundClr(invertRB(CD.GetColor()));
		m_clrBackground = invertRB(CD.GetColor());
		Invalidate();
	}
}

void CCGWorkView::OnOptionsNormalcolor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		//Model* model;
		//if (m_bDualView && !m_bLeftModel) {
		//	model = scene.getSecondActiveModel();
		//}
		//else {
		//	model = scene.getActiveModel();
		//}
		//Geometry& geometry = model->getGeometry();		
		//geometry.setNormalClr(invertRB(CD.GetColor()));
		scene.getRenderer().setNormalClr(invertRB(CD.GetColor()));
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
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}

void CCGWorkView::OnLightShadingGouraud()
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
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
		}
		m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
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
	Model * model = nullptr;
	if (m_nIsSubobjectMode) {
		model = scene.getModel(m_nSubobject);
	}
	else {
		model = scene.getMainModel();
	}
	if (model == nullptr) {
		return;
	}
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
		float percentage = m_nScaleSensetivity / 100;
		adjustedQuota = (direction == NEGATIVE ? 1 / (scalingQuota + percentage) : (scalingQuota + percentage));
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
//	m_bBoxFrame = false;
//	m_bIsPerspective = false;
//	m_bIsViewSpace = false;
//	m_bPolyNormals = false;
//	m_bVertexNormals = false;
//	m_nAxis = ID_AXIS_X;
//	m_nAction = ID_ACTION_ROTATE;
//	
//	for (int i = 0; i < scene.getAllModels().size(); i++) {
//		Model* model = scene.getAllModels()[i];
//		resetModel(model);
//	}
//
//	Invalidate();
//
}

//static void resetModel(Model* model) {
//	if (model == nullptr)
//		return;
//	model->setTransformation(Mat4::Identity());
//	Geometry& geometry = model->getGeometry();
//	geometry.setBackgroundClr(STANDARD_BACKGROUND_COLOR);
//	geometry.setLineClr(STANDARD_OBJECT_COLOR);
//	geometry.setNormalClr(STANDARD_NORMAL_COLOR);
//}


void CCGWorkView::OnViewObjectselection()
{
	AdvancedDialog dlg;
	dlg.maxSubobject = scene.getAllModels().size();
	dlg.subChecked = m_nIsSubobjectMode;
	if (dlg.DoModal() == IDOK) {
		m_nIsSubobjectMode = dlg.subChecked;
		m_nSubobject = dlg.subobjectID;
		scene.setActiveModelID(m_nSubobject);
		m_nIsSubobjectMode == true ? scene.setSubobjectMode() : scene.setWholeobjectMode();
	}
}

void CCGWorkView::resetButtons() {
	m_bIsPerspective = false;
	m_bBoxFrame = false;
	m_bPolyNormals = false;
	m_bVertexNormals = false;
	m_bIsViewSpace = true;
}
