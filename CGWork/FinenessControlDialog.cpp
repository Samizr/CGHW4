// FinenessControlDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "FinenessControlDialog.h"
#include "afxdialogex.h"
#include <limits>


// FinenessControlDialog dialog

IMPLEMENT_DYNAMIC(FinenessControlDialog, CDialogEx)

FinenessControlDialog::FinenessControlDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINENESSCONTROL_DLG, pParent)
	, fineness(20)
{

}

FinenessControlDialog::FinenessControlDialog(double currentFineness, CWnd * pParent)
	: CDialogEx(IDD_FINENESSCONTROL_DLG, pParent)
	, fineness(currentFineness)
{

}

FinenessControlDialog::~FinenessControlDialog()
{
}

void FinenessControlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FINENESSVALUE, fineness);
	DDV_MinMaxInt(pDX, fineness, 2, INT_MAX);
}


BEGIN_MESSAGE_MAP(FinenessControlDialog, CDialogEx)
END_MESSAGE_MAP()


// FinenessControlDialog message handlers
