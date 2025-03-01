#pragma once


// FinenessControlDialog dialog

class FinenessControlDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FinenessControlDialog)

public:
	FinenessControlDialog(CWnd* pParent = nullptr);   // standard constructor
	FinenessControlDialog(double currentFineness, CWnd* pParent = nullptr);   // standard constructor
	virtual ~FinenessControlDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINENESSCONTROL_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int fineness;
};
