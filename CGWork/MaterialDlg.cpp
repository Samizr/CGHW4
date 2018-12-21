// MaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "MaterialDlg.h"


// CMaterialDlg dialog

IMPLEMENT_DYNAMIC(CMaterialDlg, CDialog)

CMaterialDlg::CMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialDlg::IDD, pParent)
	, m_ambient(0)
	, m_diffuse(0)
	, m_specular(0)
	, m_cosinComponent(0)
{

}

void CMaterialDlg::SetDialogData(int dataID, double data)
{
	if (dataID == IDC_MATERIAL_AMBIENT) {
		m_ambient = data;
	}
	else if (dataID == IDC_MATERIAL_DIFFUSE) {
		m_diffuse = data;
	}
	else if (dataID == IDC_MATERIAL_SHININESS) {
		m_specular = data;
	}
	else if (dataID == IDC_MATERIAL_SPECULAR) {
		m_cosinComponent = data;
	}
}

CMaterialDlg::~CMaterialDlg()
{
}

void CMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MATERIAL_AMBIENT, m_ambient);
    DDX_Text(pDX, IDC_MATERIAL_DIFFUSE, m_diffuse);
    DDX_Text(pDX, IDC_MATERIAL_SHININESS, m_specular);
    DDX_Text(pDX, IDC_MATERIAL_SPECULAR, m_cosinComponent);
    DDV_MinMaxDouble(pDX, m_ambient, 0, 1);
    DDV_MinMaxDouble(pDX, m_diffuse, 0, 1);
    DDV_MinMaxDouble(pDX, m_specular, 0, 1);
}


BEGIN_MESSAGE_MAP(CMaterialDlg, CDialog)
END_MESSAGE_MAP()


// CMaterialDlg message handlers
