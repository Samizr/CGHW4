// AdvancedDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "AdvancedDialog.h"
#include "afxdialogex.h"


// AdvancedDialog dialog

IMPLEMENT_DYNAMIC(AdvancedDialog, CDialogEx)

AdvancedDialog::AdvancedDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADVANCED_DIALOG_2, pParent)
	, subChecked(FALSE)
	, subobjectID(0)
{

}

AdvancedDialog::~AdvancedDialog()
{
}

void AdvancedDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SUBCHECK, subChecked);
	DDX_Text(pDX, IDC_SUBOBJECT_NUMBER, subobjectID);
	DDV_MinMaxInt(pDX, subobjectID, 0, maxSubobject);
}


BEGIN_MESSAGE_MAP(AdvancedDialog, CDialogEx)
END_MESSAGE_MAP()


// AdvancedDialog message handlers
