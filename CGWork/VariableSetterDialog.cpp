// VariableSetter.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "VariableSetterDialog.h"
#include "afxdialogex.h"


// VariableSetter dialog

IMPLEMENT_DYNAMIC(VariableSetterDialog, CDialogEx)

VariableSetterDialog::VariableSetterDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VARIABLESETTER, pParent)
	, rotationMin(0)
	, rotationMax(1)
	, translationMin(0)
	, translationMax(1)
	, scalingMin(0)
	, scalingMax(1)
{

}

VariableSetterDialog::VariableSetterDialog(float rmin, float rmax, float tmin, float tmax, float smin, float smax, CWnd * pParent)
{
	rotationMax = rmax;
	rotationMin = rmin;
	translationMax = tmax;	
	translationMin = tmin;
	scalingMax = smax;
	scalingMin = smin;
}

VariableSetterDialog::~VariableSetterDialog()
{
}

void VariableSetterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROTATIONMIN, rotationMin);
	DDV_MinMaxFloat(pDX, rotationMin, 0, 100000);
	DDX_Text(pDX, IDC_ROTATIONMAX, rotationMax);
	DDV_MinMaxFloat(pDX, rotationMax, 0, 100000);
	DDX_Text(pDX, IDC_TRANSLATIONMIN, translationMin);
	DDV_MinMaxFloat(pDX, translationMin, 0, 100000);
	DDX_Text(pDX, IDC_TRANSLATIONMAX, translationMax);
	DDV_MinMaxFloat(pDX, translationMax, 0, 100000);
	DDX_Text(pDX, IDC_SCALINGMIN, scalingMin);
	DDV_MinMaxFloat(pDX, scalingMin, 0, 100000);
	DDX_Text(pDX, IDC_SCALINGMAX, scalingMax);
	DDV_MinMaxFloat(pDX, scalingMax, 0, 100000);
}


BEGIN_MESSAGE_MAP(VariableSetterDialog, CDialogEx)
END_MESSAGE_MAP()


// VariableSetter message handlers

