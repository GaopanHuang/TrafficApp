; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTrafficAppDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TrafficApp.h"

ClassCount=4
Class1=CTrafficAppApp
Class2=CTrafficAppDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_DIALOG_PTS
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDD_TRAFFICAPP_DIALOG
Class4=CPtsDlg
Resource5=IDR_MAIN_MENU

[CLS:CTrafficAppApp]
Type=0
HeaderFile=TrafficApp.h
ImplementationFile=TrafficApp.cpp
Filter=N

[CLS:CTrafficAppDlg]
Type=0
HeaderFile=TrafficAppDlg.h
ImplementationFile=TrafficAppDlg.cpp
Filter=D
LastObject=CTrafficAppDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=TrafficAppDlg.h
ImplementationFile=TrafficAppDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_TRAFFICAPP_DIALOG]
Type=1
Class=CTrafficAppDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC,button,1342177287
Control4=IDC_EDIT_TIPS,edit,1342249156
Control5=IDC_RADIO_LINE1,button,1342308361
Control6=IDC_RADIO_LINE2,button,1342177289
Control7=IDC_BUTTON_REMOVE_LAST,button,1342242816
Control8=IDC_BUTTON_SAVE,button,1342242816
Control9=IDC_BUTTON_INIT,button,1342242816
Control10=IDC_STATIC,button,1342177287

[MNU:IDR_MAIN_MENU]
Type=1
Class=CTrafficAppDlg
Command1=ID_FILE_OPEN
Command2=ID_ABOUT
CommandCount=2

[DLG:IDD_DIALOG_PTS]
Type=1
Class=CPtsDlg
ControlCount=8
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242817
Control3=IDC_EDIT_IMGX,edit,1350633600
Control4=IDC_EDIT_IMGY,edit,1350633600
Control5=IDC_EDIT_SPAX,edit,1350631552
Control6=IDC_EDIT_SPAY,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[CLS:CPtsDlg]
Type=0
HeaderFile=PtsDlg.h
ImplementationFile=PtsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPtsDlg
VirtualFilter=dWC

