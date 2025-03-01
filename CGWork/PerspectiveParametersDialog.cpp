// PerspectiveParametersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "PerspectiveParametersDialog.h"
#include "afxdialogex.h"
#include <limits>

// PerspectiveParametersDialog dialog

IMPLEMENT_DYNAMIC(PerspectiveParametersDialog, CDialogEx)

PerspectiveParametersDialog::PerspectiveParametersDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PERSPECTIVEPARS_DLG, pParent)
	, d(0)
	, alpha(0)
{

}

PerspectiveParametersDialog::PerspectiveParametersDialog(float d, float alpha, CWnd * pParent)
	: CDialogEx(IDD_PERSPECTIVEPARS_DLG, pParent)
	, d(d)
	, alpha(alpha)
{
}

PerspectiveParametersDialog::~PerspectiveParametersDialog()
{
}

void PerspectiveParametersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PERS_D_PARAM, d);
	DDV_MinMaxFloat(pDX, d, 0, FLT_MAX);
	DDX_Text(pDX, IDC_PERS_ALPHA_PARAM, alpha);
	DDV_MinMaxFloat(pDX, alpha, 0, FLT_MAX);
}


BEGIN_MESSAGE_MAP(PerspectiveParametersDialog, CDialogEx)
END_MESSAGE_MAP()


// PerspectiveParametersDialog message handlers
