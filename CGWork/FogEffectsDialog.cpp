// FogEffectsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "FogEffectsDialog.h"
#include "afxdialogex.h"


// FogEffectsDialog dialog

IMPLEMENT_DYNAMIC(FogEffectsDialog, CDialogEx)

FogEffectsDialog::FogEffectsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FOGEFFECTS, pParent)
{

}

FogEffectsDialog::~FogEffectsDialog()
{
}

void FogEffectsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FOG_BEGIN_Y, fog.z_near);
	DDX_Text(pDX, IDC_FOG_END_Y, fog.z_far);

	int helper = (int)fog.active;
	DDX_Check(pDX, IDC_ENABLE_FOG, helper);
	fog.active = (bool)helper;
}


BEGIN_MESSAGE_MAP(FogEffectsDialog, CDialogEx)
	ON_BN_CLICKED(IDC_FOG_COLOR, &FogEffectsDialog::OnBnClickedFogColor)
END_MESSAGE_MAP()


// FogEffectsDialog message handlers


void FogEffectsDialog::OnBnClickedFogColor()
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK) {
		fog.color = dlg.GetColor();
	}
}
