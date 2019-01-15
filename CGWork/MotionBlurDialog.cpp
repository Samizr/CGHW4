// MotionBlurDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "MotionBlurDialog.h"
#include "afxdialogex.h"


// MotionBlurDialog dialog

IMPLEMENT_DYNAMIC(MotionBlurDialog, CDialog)

MotionBlurDialog::MotionBlurDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MOTION_BLUR_DLG, pParent)
	, motionBlurActive(FALSE)
	, motionBlurTValue(0.25){

}

MotionBlurDialog::~MotionBlurDialog()
{
}

void MotionBlurDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, motionBlurActive);
	DDX_Text(pDX, IDC_EDIT1, motionBlurTValue);
}


BEGIN_MESSAGE_MAP(MotionBlurDialog, CDialog)
END_MESSAGE_MAP()


// MotionBlurDialog message handlers
