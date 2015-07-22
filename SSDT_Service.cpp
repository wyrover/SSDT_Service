// SSDT_Service.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SSDT_Service.h"
#include "SSDT_ServiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSDT_ServiceApp

BEGIN_MESSAGE_MAP(CSSDT_ServiceApp, CWinApp)
	//{{AFX_MSG_MAP(CSSDT_ServiceApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDT_ServiceApp construction

CSSDT_ServiceApp::CSSDT_ServiceApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSSDT_ServiceApp object

CSSDT_ServiceApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSSDT_ServiceApp initialization

BOOL CSSDT_ServiceApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSSDT_ServiceDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
