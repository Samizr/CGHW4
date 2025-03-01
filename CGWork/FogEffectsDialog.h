#pragma once


// FogEffectsDialog dialog
#include "Fog.h"
class FogEffectsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FogEffectsDialog)

private:

public:
	FogEffectsDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FogEffectsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOGEFFECTS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFogColor();
	FogParams fog;
};
