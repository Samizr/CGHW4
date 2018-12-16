// FileRenderingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "FileRenderingDialog.h"
#include "afxdialogex.h"


// FileRenderingDialog dialog

IMPLEMENT_DYNAMIC(FileRenderingDialog, CDialogEx)

FileRenderingDialog::FileRenderingDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_RENDERING, pParent)
	, desiredWidth(0)
	, desiredHeight(0)
{

}

FileRenderingDialog::~FileRenderingDialog()
{
}

void FileRenderingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH_CONTROL, desiredWidth);
	DDV_MinMaxInt(pDX, desiredWidth, 1, INT_MAX);
	DDX_Text(pDX, IDC_HEIGHT_CONTROL, desiredHeight);
	DDV_MinMaxInt(pDX, desiredHeight, 1, INT_MAX);
}


BEGIN_MESSAGE_MAP(FileRenderingDialog, CDialogEx)
END_MESSAGE_MAP()


// FileRenderingDialog message handlers
