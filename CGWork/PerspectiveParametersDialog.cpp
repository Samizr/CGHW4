// PerspectiveParametersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "PerspectiveParametersDialog.h"
#include "afxdialogex.h"


// PerspectiveParametersDialog dialog

IMPLEMENT_DYNAMIC(PerspectiveParametersDialog, CDialogEx)

PerspectiveParametersDialog::PerspectiveParametersDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PERSPECTIVEPARS_DLG, pParent)
{

}

PerspectiveParametersDialog::~PerspectiveParametersDialog()
{
}

void PerspectiveParametersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PerspectiveParametersDialog, CDialogEx)
END_MESSAGE_MAP()


// PerspectiveParametersDialog message handlers
