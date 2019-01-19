#pragma once


// SuperSamplingAADialog dialog

class SuperSamplingAADialog : public CDialog
{
	DECLARE_DYNAMIC(SuperSamplingAADialog)

public:
	SuperSamplingAADialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SuperSamplingAADialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUPER_SAMPLING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int filterType;
	int filterSize;
};
