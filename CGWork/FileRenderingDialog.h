#pragma once


// FileRenderingDialog dialog

class FileRenderingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FileRenderingDialog)

public:
	FileRenderingDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FileRenderingDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_RENDERING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int desiredWidth;
	int desiredHeight;
};
