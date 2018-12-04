#pragma once


// AdvancedDialog dialog

class AdvancedDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AdvancedDialog)

public:
	AdvancedDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AdvancedDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADVANCED_DIALOG_2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int maxSubobject;
	BOOL subChecked;
	int subobjectID;
};
