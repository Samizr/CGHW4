#pragma once


// MotionBlurDialog dialog

class MotionBlurDialog : public CDialog
{
	DECLARE_DYNAMIC(MotionBlurDialog)

public:
	MotionBlurDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MotionBlurDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOTION_BLUR_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL motionBlurActive;
	double motionBlurTValue;
};
