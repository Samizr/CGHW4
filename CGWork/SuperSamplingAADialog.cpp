// SuperSamplingAADialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "SuperSamplingAADialog.h"
#include "afxdialogex.h"


// SuperSamplingAADialog dialog

IMPLEMENT_DYNAMIC(SuperSamplingAADialog, CDialog)

SuperSamplingAADialog::SuperSamplingAADialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SUPER_SAMPLING, pParent)
	, isFilter3(FALSE)
	, isFilter5(FALSE)
	//, //boxFilter(FALSE)
	, filterType(0)
{
}

SuperSamplingAADialog::~SuperSamplingAADialog()
{
}

void SuperSamplingAADialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_filter3, isFilter3);
	DDX_Check(pDX, IDC_filter5, isFilter5);
	//DDX_Radio(pDX, IDC_boxFilter, filterG);
	//	DDX_Radio(pDX, IDC_triangleFilter, filterType[1]);
	//	DDX_Radio(pDX, IDC_gaussianFilter, filterType[2]);
	//	DDX_Radio(pDX, IDC_sincFilter, filterType[3]);
	DDX_Radio(pDX, IDC_boxFilter, filterType);
}


BEGIN_MESSAGE_MAP(SuperSamplingAADialog, CDialog)
END_MESSAGE_MAP()


// SuperSamplingAADialog message handlers
