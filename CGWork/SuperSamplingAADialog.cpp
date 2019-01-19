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
	, filterType(0)
	, filterSize(0)
{
}

SuperSamplingAADialog::~SuperSamplingAADialog()
{
}

void SuperSamplingAADialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_BOX_FILTER, filterType);
	DDX_Radio(pDX, IDC_NO_SIZE, filterSize);
}


BEGIN_MESSAGE_MAP(SuperSamplingAADialog, CDialog)
END_MESSAGE_MAP()


// SuperSamplingAADialog message handlers
