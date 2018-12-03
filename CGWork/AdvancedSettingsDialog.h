#pragma once


// AdvancedSettingsDialog dialog

class AdvancedSettingsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AdvancedSettingsDialog)

public:
	AdvancedSettingsDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AdvancedSettingsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADVANCED_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int maxSubobject;
	CButton wholeButton;
	CButton subButton;
	unsigned int objectID;
};
