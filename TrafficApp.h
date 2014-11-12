// TrafficApp.h : main header file for the TRAFFICAPP application
//

#if !defined(AFX_TRAFFICAPP_H__0DCF048D_8880_4EDE_8650_E82324B94DF5__INCLUDED_)
#define AFX_TRAFFICAPP_H__0DCF048D_8880_4EDE_8650_E82324B94DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTrafficAppApp:
// See TrafficApp.cpp for the implementation of this class
//

class CTrafficAppApp : public CWinApp
{
public:
	CTrafficAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrafficAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTrafficAppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAFFICAPP_H__0DCF048D_8880_4EDE_8650_E82324B94DF5__INCLUDED_)
