#pragma once


// MouseSensitivityDialog dialog

class MouseSensitivityDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MouseSensitivityDialog)

public:
	MouseSensitivityDialog(CWnd* pParent = nullptr);   // standard constructor
	MouseSensitivityDialog(int in_translationSensitivity, int in_rotationSensitivity, int in_scalingSensitivity, CWnd* pParent = nullptr);   // standard constructor
	float getTranslationSensitivity();
	float getRotationSensitivity();
	float getScalingSensitivity();
	virtual ~MouseSensitivityDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUSESENSETIVITY_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int translationSensitivity;
	int rotationSensitivity;
	int scalingSensitivity;
	CSliderCtrl t_control;
	CSliderCtrl r_control;
	CSliderCtrl s_control;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
