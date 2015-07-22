// SSDT_ServiceDlg.h : header file
//

#if !defined(AFX_SSDT_SERVICEDLG_H__56DBDEAD_A81C_4248_A95C_332635AF0954__INCLUDED_)
#define AFX_SSDT_SERVICEDLG_H__56DBDEAD_A81C_4248_A95C_332635AF0954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSSDT_ServiceDlg dialog

/************************************************************************/
/*  Now is VXer's code                                                  */
/************************************************************************/
typedef struct _VXerInfo{
	ULONG SvcNum; // 服务号
	string FuncName; // 函数名称
	ULONG PresentAddr; // 当前地址
	ULONG OriginalityAddr; // 原始地址
	UCHAR FuncHead[1]; // 函数的前10字节，检测inline hook
} VXerInfo, *PVXerInfo;

// 内核模块信息
typedef struct _KernelModules{
	ULONG ulBaseAddr;
	ULONG ulImageSize;
	char szPath[MAX_PATH];
} KernelModules, *PKernelModules;

typedef enum _SYSDBG_COMMAND {
    sysdbggettraceinformation = 1,
	sysdbgcopymemorychunks_0 = 8,
	SysDbgWriteVirtualMemory = 9,
} SYSDBG_COMMAND, *PSYSDBG_COMMAND;

typedef struct _MEMORY_CHUNKS {
	ULONG Address;
	PVOID Data;
	ULONG Length;
}MEMORY_CHUNKS, *PMEMORY_CHUNKS;

typedef struct _SYSTEM_MODULE_INFORMATION { // Information Class 11
	ULONG Reserved[2];
	PVOID Base; // 基址
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef struct _SSDT_Entry
{
	ULONG *SvcTabBase;
	ULONG *SvcCntTabBase;
	ULONG NumOfSvc;
	UCHAR *ParamTabBase;
} SSDT_Entry, *PSSDT_Entry;

typedef ULONG (NTAPI *PZWQUERYSYSTEMINFORMATION)(ULONG, PVOID, ULONG, PULONG);
typedef ULONG (NTAPI *PZWSYSTEMDEBUGCONTROL)(SYSDBG_COMMAND, PVOID, ULONG, PVOID, ULONG, PULONG);

/************************************************************************/
/*  Now VXer's code is over                                             */
/************************************************************************/

class CSSDT_ServiceDlg : public CDialog
{
// Construction
public:
	CSSDT_ServiceDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CSSDT_ServiceDlg)
	enum { IDD = IDD_SSDT_SERVICE_DIALOG };
	CListCtrl	m_lstSSDT;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDT_ServiceDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSSDT_ServiceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonKrnlmdl();
	afx_msg void OnRclickListSsdt(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemRepair();
	afx_msg void OnMenuitemFresh();
	afx_msg void OnButtonExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	SSDT_Entry KeServiceDescriptorTable;
	BOOL bShowSSDT;
	void OnDrawColorForMyList( NMHDR *pNmHdr, LRESULT *pResult );
	void DbgPrivilege();
	PZWSYSTEMDEBUGCONTROL ZwSystemDebugControl;
	PZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation;
	vector<VXerInfo> vxer;
	vector<KernelModules> krnlMdls;
	void GetInfoFromNtdll();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDT_SERVICEDLG_H__56DBDEAD_A81C_4248_A95C_332635AF0954__INCLUDED_)
