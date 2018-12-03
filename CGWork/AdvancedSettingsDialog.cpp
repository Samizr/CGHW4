// AdvancedSettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "AdvancedSettingsDialog.h"
#include "afxdialogex.h"


// AdvancedSettingsDialog dialog

IMPLEMENT_DYNAMIC(AdvancedSettingsDialog, CDialogEx)

AdvancedSettingsDialog::AdvancedSettingsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADVANCED_DIALOG, pParent)
	, objectID(0)
{

}

AdvancedSettingsDialog::~AdvancedSettingsDialog()
{
}

void AdvancedSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WHOLE_RADIO, wholeButton);
	DDX_Control(pDX, IDC_SUBOBJECT_RADIO, subButton);
	DDX_Text(pDX, IDC_SUBOBJECT_NUMBER, objectID);
	DDV_MinMaxUInt(pDX, objectID, 0, maxSubobject);
}


BEGIN_MESSAGE_MAP(AdvancedSettingsDialog, CDialogEx)
END_MESSAGE_MAP()


// AdvancedSettingsDialog message handlers
