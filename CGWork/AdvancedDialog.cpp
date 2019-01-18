// AdvancedDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "AdvancedDialog.h"
#include "afxdialogex.h"


// AdvancedDialog dialog

IMPLEMENT_DYNAMIC(AdvancedDialog, CDialogEx)

AdvancedDialog::AdvancedDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADVANCED_DLG, pParent)
	, subChecked(FALSE)
	, subobjectID(0)
	, maxSubobject(1)
	, invertVertexNormals(FALSE)
	, invertPolygonNormals(FALSE)
	, importNormals(FALSE)
	, modelId(0)
{

}

AdvancedDialog::~AdvancedDialog()
{
}

void AdvancedDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SUBCHECK, subChecked);
	DDX_Text(pDX, IDC_SUBOBJECT_NUMBER, subobjectID);
	DDX_Check(pDX, IDC_INVERT_VERTEX_NORMALS, invertVertexNormals);
	DDX_Check(pDX, IDC_INVERT_POLYGON_NORMALS, invertPolygonNormals);
	DDX_Check(pDX, IDC_IMPORTNORMALS, importNormals);
	DDX_Text(pDX, IDC_MODEL_NUMBER, modelId);
	DDV_MinMaxInt(pDX, subobjectID, 0, maxSubobject);
	DDV_MinMaxInt(pDX, modelId, 0, maxObject);
}


BEGIN_MESSAGE_MAP(AdvancedDialog, CDialogEx)
END_MESSAGE_MAP()

// AdvancedDialog message handlers
