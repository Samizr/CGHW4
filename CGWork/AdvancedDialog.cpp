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
	//DDV_MinMaxInt(pDX, subobjectID, 0, maxSubobject);
	DDX_Check(pDX, IDC_INVERT_VERTEX_NORMALS, invertVertexNormals);
	DDX_Check(pDX, IDC_ADVANCED_DIALOG, invertPolygonNormals);
	DDX_Radio(pDX, IDC_IMPORT_NORMALS, importNormals);
}


BEGIN_MESSAGE_MAP(AdvancedDialog, CDialogEx)
END_MESSAGE_MAP()

// AdvancedDialog message handlers
