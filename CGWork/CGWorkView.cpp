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
#include "VariableSetterDialog.h"


// For Status Bar access
#include "MainFrm.h"

// Static Functions:
static void testModel(CDC* pDC, const CRect& rect);
static AXIS sceneAxisTranslator(int guiID);


// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))

#define INITIAL_SENSITIVITY 10
#define INITIAL_BACKGROUND_COLOR RGB(5,5,5)
#define INITIAL_OBJECT_COLOR RGB(230,230,230)
#define INITIAL_NORMAL_COLOR RGB(0,200,240)
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
	ON_COMMAND(ID_NORMALSCOLOR, OnOptionsNormalscolor)
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
	ON_COMMAND(ID_OPTIONS_SETSENSITIVITYVARIABLES, &CCGWorkView::OnOptionsSetsensitivityvariables)
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

CCGWorkView::CCGWorkView() :
	scene(renderer)
{
	//Set default values
	m_nAxis = ID_AXIS_X;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	m_bBoxFrame = false;
	m_bPolyNormals = false;
	m_bVertexNormals = false;
	m_bIsViewSpace = true;
	m_nTranslationSensetivity = INITIAL_SENSITIVITY;
	m_nRotationSensetivity = INITIAL_SENSITIVITY;
	m_nScaleSensetivity = INITIAL_SENSITIVITY;
	m_clrBackground = INITIAL_BACKGROUND_COLOR;
	m_clrLines = INITIAL_OBJECT_COLOR;
	m_clrNormals = INITIAL_NORMAL_COLOR;

	// Transformations Quantitive Setup:
	rotationQuota = 1;
	translationQuota = 1;
	scalingQuota = 1;

	//Scene initilization:
	auto newCamera = new Camera();
	newCamera->Ortho();
	cameraIDs.push_back(scene.addCamera(newCamera));
	

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
	SetTimer(1, 1, NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);
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
	CDC *pDCToUse = /*m_pDC*/m_pDbDC;

	//testModel(m_pDbDC, r);

	scene.draw(pDC, r);

	if (pDCToUse != m_pDC)
	{
		m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);
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
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;

		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		// Open the file and read it.

		// Does not reload the model if requested.
		auto newModel = new Model(::loadedGeometry);
		modelIDs.push_back(scene.addModel(newModel));

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
	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}

void CCGWorkView::OnViewBoxFrame()
{
	m_bBoxFrame = !m_bBoxFrame;
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
	Invalidate();
}

void CCGWorkView::OnUpdateViewPolyNormals(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bPolyNormals);
}

void CCGWorkView::OnViewVertexNormals()
{
	m_bVertexNormals = !m_bVertexNormals;
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
		m_clrLines = CD.GetColor();
		Invalidate();
	}
}

void CCGWorkView::OnOptionsBackgroundColor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		m_clrBackground = CD.GetColor();
		Invalidate();
	}
}

void CCGWorkView::OnOptionsNormalscolor()
{
	CColorDialog CD;
	if (CD.DoModal() == IDOK) {
		m_clrNormals = CD.GetColor();
		Invalidate();
	}
}

void CCGWorkView::OnOptionsMousesensitivity()
{
	//Dialog initiation
	MouseSensitivityDialog MSDialog(m_nTranslationSensetivity, m_nRotationSensetivity, m_nScaleSensetivity);;

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





//////////////////////////////////////////TESTING & DEBUG FUNCTIONS////////////////////////////////////////////////////////

void CCGWorkView::testButtons() {
	CDC *pDCToUse = m_pDbDC;
	CRect r;
	GetClientRect(&r);

}

#include "Mat4.h"
#include "LinePlotter.h"

AXIS sceneAxisTranslator(int guiID)
{
	switch (guiID) {
	case ID_AXIS_X: return XAXIS;
	case ID_AXIS_Y: return YAXIS;
	case ID_AXIS_Z: return ZAXIS;
	}
}

static void testModel(CDC* pDC, const CRect& rect) {

	Geometry geometry = ::loadedGeometry;
	float deltaX = 16; //geometry.getMaxX() - geometry.getMinX();
	float deltaY = 9;  //geometry.getMaxY() - geometry.getMinY();
	float deltaZ = 6;  //geometry.getMaxZ() - geometry.getMinZ();
	float sumX = 0;	 //geometry.getMaxX() + geometry.getMinX();
	float sumY = 0; //geometry.getMaxY() + geometry.getMinY();
	float sumZ = 0; //geometry.getMaxZ() + geometry.getMinZ();
	float deltaW = rect.right - rect.left;
	float deltaH = rect.bottom - rect.top;
	float virtualDeltaW = min(deltaH, deltaH);
	float virtualDeltaH = min(deltaH, deltaH);
	if (16*deltaH > 9*deltaW) {
		virtualDeltaW = deltaW;
		virtualDeltaH = deltaW * 9 / 16;
	} else {
		virtualDeltaW = deltaH * 16 / 9;
		virtualDeltaH = deltaH;
	}
	float sumW = rect.right + rect.left;
	float sumH = rect.top + rect.bottom;
	

	Vec4 vecWM[4] = { Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 0, 1) };
	Vec4 vecCM[4] = { Vec4(1, 0, 0, 0), Vec4(0, -1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 0, 1) };
	Vec4 vecPM[4] = { Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 0, 0), Vec4(0, 0, 0, 1) };
	Vec4 vecNM[4] = { Vec4(2 / deltaX, 0, 0, -sumX / deltaX), Vec4(0, 2 / deltaY, 0, -sumY / deltaY), Vec4(0, 0, 2 / deltaZ, -sumZ / deltaZ), Vec4(0, 0, 0, 1) };

	Vec4 vecVPM[4] = { Vec4(virtualDeltaW / 2, 0, 0, sumW / 2), Vec4(0, virtualDeltaH / 2, 0, sumH / 2), Vec4(0, 0, 0.5, 0.5), Vec4(0, 0, 0, 1) };

	Mat4 worldMatrix(vecWM), cameraMatrix(vecCM), projectionMatrix(vecPM), normalizingMatrix(vecNM);
	Mat4 viewPortMatrix(vecVPM);
	Mat4 finalMatrix = ((((viewPortMatrix * normalizingMatrix) * projectionMatrix) * cameraMatrix) * worldMatrix);


	for (Edge* edge : geometry.getEdges()) {
		Vec4 p1(edge->getA()->xCoord(), edge->getA()->yCoord(), edge->getA()->zCoord(), 1);
		Vec4 p2(edge->getB()->xCoord(), edge->getB()->yCoord(), edge->getB()->zCoord(), 1);

		p1 = finalMatrix * p1;
		p2 = finalMatrix * p2;

		plotLine(p1.xCoord(), p1.yCoord(), p2.xCoord(), p2.yCoord(), pDC);
	}
}


void CCGWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bAllowTransformations = true;
	m_lnLastXPos = point.x;
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

}

//Parses the requested transformation and requests the correct transformation:
void CCGWorkView::transform(Direction direction)
{

	Model* model = scene.getActiveModel();
	if (model == nullptr) {
		return;
	}
	AXIS sceneAxis = sceneAxisTranslator(m_nAxis);
	switch (m_nAction) {

	case (ID_ACTION_ROTATE) : 
		if (m_bIsViewSpace)
			model->appendToTransformationrotateViewSpace(sceneAxis, direction * rotationQuota);
		else
			model->rotateObjectSpace(sceneAxis, direction * rotationQuota);
		break;
	
	case (ID_ACTION_TRANSLATE):
		if (m_bIsViewSpace)
			model->translateViewSpace(sceneAxis, direction * translationQuota);
		else
			model->translateObjectSpace(sceneAxis, direction * translationQuota);
		break;

	case (ID_ACTION_SCALE):
		if (m_bIsViewSpace)
			model->scaleViewSpace(sceneAxis, direction * scalingQuota);
		else
			model->scaleObjectSpace(sceneAxis, direction * scalingQuota);
		break;
	}
}


void CCGWorkView::OnOptionsSetsensitivityvariables()
{
	VariableSetterDialog VSDialog;
	if (VSDialog.DoModal() == IDOK) {
		rotationQuota = VSDialog.rotationMax - VSDialog.rotationMin;
		translationQuota = VSDialog.translationMax - VSDialog.translationMin;
		scalingQuota = VSDialog.scalingMax - VSDialog.scalingMin;
	}
}
