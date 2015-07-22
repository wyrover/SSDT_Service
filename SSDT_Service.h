// SSDT_Service.h : main header file for the SSDT_SERVICE application
//

#if !defined(AFX_SSDT_SERVICE_H__FB5EDA0D_F120_4922_8389_A07560BD8E34__INCLUDED_)
#define AFX_SSDT_SERVICE_H__FB5EDA0D_F120_4922_8389_A07560BD8E34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSSDT_ServiceApp:
// See SSDT_Service.cpp for the implementation of this class
//

class CSSDT_ServiceApp : public CWinApp
{
public:
	CSSDT_ServiceApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDT_ServiceApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSSDT_ServiceApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDT_SERVICE_H__FB5EDA0D_F120_4922_8389_A07560BD8E34__INCLUDED_)
