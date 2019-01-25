// ParametricTexturesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "ParametricTexturesDialog.h"
#include "afxdialogex.h"


// ParametricTexturesDialog dialog

IMPLEMENT_DYNAMIC(ParametricTexturesDialog, CDialogEx)

ParametricTexturesDialog::ParametricTexturesDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARAMETRIC_TEXTURES, pParent)
	, modelNum(0)
	, pngTextureImage(nullptr)
	, enableParametricTextures(FALSE)
{

}

ParametricTexturesDialog::~ParametricTexturesDialog()
{
}

void ParametricTexturesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MODULE_NUMBER, modelNum);
	DDV_MinMaxInt(pDX, modelNum, 0, INT_MAX);
	DDX_Check(pDX, IDC_ENABLE_TEXTURES, enableParametricTextures);
}


BEGIN_MESSAGE_MAP(ParametricTexturesDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ParametricTexturesDialog::OnBnLoadRasterImage)
END_MESSAGE_MAP()


// ParametricTexturesDialog message handlers


void ParametricTexturesDialog::OnBnLoadRasterImage()
{
	TCHAR szFilters[] = _T("PNG Image Files (*.png)|*.png|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	if (dlg.DoModal() == IDOK) {
		rasterImageFileName = dlg.GetPathName();
		pngTextureImage = new PngWrapper();
		CStringA charstr(rasterImageFileName);
		const char* fileName = charstr;
		OutputDebugStringA(fileName);
		pngTextureImage->SetFileName(fileName);
		pngTextureImage->ReadPng();
	}
}
