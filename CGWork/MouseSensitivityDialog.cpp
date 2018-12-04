// MouseSensitivityDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "MouseSensitivityDialog.h"
#include "afxdialogex.h"

// MouseSensitivityDialog dialog

IMPLEMENT_DYNAMIC(MouseSensitivityDialog, CDialogEx)

MouseSensitivityDialog::MouseSensitivityDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOUSESENSETIVITY_DLG, pParent)
	, translationSensitivity(10)
	, rotationSensitivity(10)
	, scalingSensitivity(2)
{
}



MouseSensitivityDialog::MouseSensitivityDialog(int in_translationSensitivity, int in_rotationSensitivity, int in_scalingSensitivity, CWnd * pParent)
	: CDialogEx(IDD_MOUSESENSETIVITY_DLG, pParent)
	, translationSensitivity(in_translationSensitivity)
	, rotationSensitivity(in_rotationSensitivity)
	, scalingSensitivity(in_scalingSensitivity)
{

}

float MouseSensitivityDialog::getTranslationSensitivity()
{
	return translationSensitivity;
}

float MouseSensitivityDialog::getRotationSensitivity()
{
	return rotationSensitivity;
}

float MouseSensitivityDialog::getScalingSensitivity()
{
	return scalingSensitivity;
}

MouseSensitivityDialog::~MouseSensitivityDialog()
{
}

void MouseSensitivityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRANSLATION_SLIDER, t_control);
	DDX_Control(pDX, IDC_ROTATION_SLIDER, r_control);
	DDX_Control(pDX, IDC_SCALING_SLIDER, s_control);
}


BEGIN_MESSAGE_MAP(MouseSensitivityDialog, CDialogEx)
END_MESSAGE_MAP()


// MouseSensitivityDialog message handlers



BOOL MouseSensitivityDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	t_control.SetRange(5, 40);
	r_control.SetRange(3, 30);
	s_control.SetRange(1, 100);
	t_control.SetPos(translationSensitivity);
	r_control.SetPos(rotationSensitivity);
	s_control.SetPos(scalingSensitivity);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void MouseSensitivityDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	translationSensitivity = t_control.GetPos();
	rotationSensitivity = r_control.GetPos();
	scalingSensitivity = s_control.GetPos();
	CDialogEx::OnOK();
}
