#pragma once


// PerspectiveParametersDialog dialog

class PerspectiveParametersDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PerspectiveParametersDialog)

public:
	PerspectiveParametersDialog(CWnd* pParent = nullptr);   // standard constructor
	PerspectiveParametersDialog(float d, float alpha, CWnd* pParent = nullptr);   // standard constructor

	virtual ~PerspectiveParametersDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PERSPECTIVEPARS_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float d;
	float alpha;
};
