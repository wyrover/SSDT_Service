; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSSDT_ServiceDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ssdt_service.h"
LastPage=0

ClassCount=2
Class1=CSSDT_ServiceApp
Class2=CSSDT_ServiceDlg

ResourceCount=2
Resource1=IDD_SSDT_SERVICE_DIALOG
Resource2=IDR_MENU

[CLS:CSSDT_ServiceApp]
Type=0
BaseClass=CWinApp
HeaderFile=SSDT_Service.h
ImplementationFile=SSDT_Service.cpp
LastObject=CSSDT_ServiceApp

[CLS:CSSDT_ServiceDlg]
Type=0
BaseClass=CDialog
HeaderFile=SSDT_ServiceDlg.h
ImplementationFile=SSDT_ServiceDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=ID_MENUITEM_REPAIR

[DLG:IDD_SSDT_SERVICE_DIALOG]
Type=1
Class=CSSDT_ServiceDlg
ControlCount=6
Control1=IDC_LIST_SSDT,SysListView32,1350631437
Control2=IDC_BUTTON_START,button,1342242816
Control3=IDC_STATIC,static,1342177294
Control4=IDC_BUTTON_KRNLMDL,button,1342242816
Control5=IDC_STATIC_TIPS,static,1342308352
Control6=IDC_BUTTON_EXIT,button,1342242816

[MNU:IDR_MENU]
Type=1
Class=CSSDT_ServiceDlg
Command1=ID_MENUITEM_FRESH
Command2=ID_MENUITEM_REPAIR
CommandCount=2

