// PtsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TrafficApp.h"
#include "PtsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPtsDlg dialog


CPtsDlg::CPtsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPtsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPtsDlg)
	m_EditImgx = 0;
	m_EditImgy = 0;
	m_EditSpax = 0.0;
	m_EditSpay = 0.0;
	//}}AFX_DATA_INIT
}


void CPtsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPtsDlg)
	DDX_Text(pDX, IDC_EDIT_IMGX, m_EditImgx);
	DDX_Text(pDX, IDC_EDIT_IMGY, m_EditImgy);
	DDX_Text(pDX, IDC_EDIT_SPAX, m_EditSpax);
	DDX_Text(pDX, IDC_EDIT_SPAY, m_EditSpay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPtsDlg, CDialog)
	//{{AFX_MSG_MAP(CPtsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPtsDlg message handlers
