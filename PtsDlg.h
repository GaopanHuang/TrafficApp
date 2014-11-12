#if !defined(AFX_PTSDLG_H__784DBEEE_56C9_4864_B139_E411703C7E38__INCLUDED_)
#define AFX_PTSDLG_H__784DBEEE_56C9_4864_B139_E411703C7E38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PtsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPtsDlg dialog

class CPtsDlg : public CDialog
{
// Construction
public:
	CPtsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPtsDlg)
	enum { IDD = IDD_DIALOG_PTS };
	int		m_EditImgx;
	int		m_EditImgy;
	double	m_EditSpax;
	double	m_EditSpay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPtsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPtsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTSDLG_H__784DBEEE_56C9_4864_B139_E411703C7E38__INCLUDED_)
