#pragma once

#include "PngWrapper.h"
// ParametricTexturesDialog dialog

class ParametricTexturesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ParametricTexturesDialog)

public:
	ParametricTexturesDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ParametricTexturesDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETRIC_TEXTURES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int modelNum;
	CString rasterImageFileName;
	PngWrapper* pngBackgroundImage;
	afx_msg void OnBnLoadRasterImage();
	BOOL enableParametricTextures;
};
