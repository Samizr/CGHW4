#pragma once


// VariableSetter dialog

class VariableSetterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(VariableSetterDialog)

public:
	VariableSetterDialog(CWnd* pParent = nullptr);   // standard constructor
	VariableSetterDialog(float rmin, float rmax, float tmin, float tmax, float smin, float smax, CWnd* pParent = nullptr);
	virtual ~VariableSetterDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VARIABLESETTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float rotationMin;
	float rotationMax;
	float translationMin;
	float translationMax;
	float scalingMin;
	float scalingMax;
};
