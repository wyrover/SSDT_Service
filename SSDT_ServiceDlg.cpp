// SSDT_ServiceDlg.cpp : implementation file
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
// CSSDT_ServiceDlg dialog

CSSDT_ServiceDlg::CSSDT_ServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSDT_ServiceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSSDT_ServiceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSSDT_ServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSDT_ServiceDlg)
	DDX_Control(pDX, IDC_LIST_SSDT, m_lstSSDT);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSSDT_ServiceDlg, CDialog)
	//{{AFX_MSG_MAP(CSSDT_ServiceDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_KRNLMDL, OnButtonKrnlmdl)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SSDT, OnRclickListSsdt)
	ON_COMMAND(ID_MENUITEM_REPAIR, OnMenuitemRepair)
	ON_COMMAND(ID_MENUITEM_FRESH, OnMenuitemFresh)
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_LIST_SSDT, OnDrawColorForMyList )		//Ϊ�ı���ɫ��ӵ���Ϣ
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDT_ServiceDlg message handlers

BOOL CSSDT_ServiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_lstSSDT.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	ZwQuerySystemInformation = (PZWQUERYSYSTEMINFORMATION)GetProcAddress(LoadLibrary("ntdll.dll"), "ZwQuerySystemInformation");
	ZwSystemDebugControl = (PZWSYSTEMDEBUGCONTROL)GetProcAddress(LoadLibrary("ntdll.dll"), "ZwSystemDebugControl");
	OnButtonKrnlmdl();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSSDT_ServiceDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSSDT_ServiceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

bool CompareFunc(VXerInfo st1, VXerInfo st2)
{
	if (st1.SvcNum < st2.SvcNum)
		return true;
	return false;
}

void CSSDT_ServiceDlg::OnButtonStart() 
{
	m_lstSSDT.DeleteAllItems();
	vxer.clear();
	int ColCount = m_lstSSDT.GetHeaderCtrl()->GetItemCount(); 
	while (ColCount--)
		m_lstSSDT.DeleteColumn(0);
	bShowSSDT = TRUE;

	int nInlineHookNum = 0; // ��inline hook�ĺ�������
	int nSSDTHookNum = 0; // ��ssdt hook�ĺ�������
	m_lstSSDT.InsertColumn(0, "��������", LVCFMT_LEFT, 60);
	m_lstSSDT.InsertColumn(1, "��������", LVCFMT_LEFT, 280);
	m_lstSSDT.InsertColumn(2, "ԭʼ��ַ", LVCFMT_CENTER, 80);
	m_lstSSDT.InsertColumn(3, "��ǰ��ַ", LVCFMT_CENTER, 80);
	m_lstSSDT.InsertColumn(4, "Inline hook", LVCFMT_CENTER, 80);
	m_lstSSDT.InsertColumn(5, "��ǰ����������ģ��", LVCFMT_LEFT, 250);

	DbgPrivilege();
	GetInfoFromNtdll();
	sort(vxer.begin(), vxer.end(), CompareFunc); // �����������
	DWORD dwNeedLen = 0;
	ZwQuerySystemInformation(11, NULL, 0, &dwNeedLen);
	LPVOID pHeap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeedLen);
	ZwQuerySystemInformation(11, pHeap, dwNeedLen, &dwNeedLen); // �õ��ں��ļ���
	PSYSTEM_MODULE_INFORMATION pMglInfo = (PSYSTEM_MODULE_INFORMATION)((PCHAR)pHeap + 4); // ǰ4BΪģ����Ŀ
	HMODULE hMdl = LoadLibraryEx(pMglInfo->ModuleNameOffset +  pMglInfo->ImageName, NULL, DONT_RESOLVE_DLL_REFERENCES);
	DWORD dwSSDTAddr = (DWORD)GetProcAddress(hMdl, "KeServiceDescriptorTable");
	DWORD dwRva = dwSSDTAddr - (DWORD)hMdl;

	DWORD dwReturnLen = 0;
	MEMORY_CHUNKS QueryBuf = { 0 };
	QueryBuf.Data = &KeServiceDescriptorTable;
	QueryBuf.Length = sizeof(SSDT_Entry);
	QueryBuf.Address = (ULONG)((PCHAR)pMglInfo->Base + dwRva);
	ZwSystemDebugControl(sysdbgcopymemorychunks_0, &QueryBuf, sizeof(MEMORY_CHUNKS), NULL, 0, &dwReturnLen);
	ULONG* pKiSystemTable = (ULONG*)((PCHAR)hMdl + (ULONG)KeServiceDescriptorTable.SvcTabBase - (ULONG)pMglInfo->Base); // ����Ҫ

	DWORD* addresses = new DWORD[KeServiceDescriptorTable.NumOfSvc]; // ��ǰ��ַ����
	QueryBuf.Data = addresses;
	QueryBuf.Length = sizeof(DWORD) * KeServiceDescriptorTable.NumOfSvc;
	QueryBuf.Address = (ULONG)KeServiceDescriptorTable.SvcTabBase;
	ZwSystemDebugControl(sysdbgcopymemorychunks_0, &QueryBuf, sizeof(MEMORY_CHUNKS), NULL, 0, &dwReturnLen);
	for (ULONG i = 0; i < vxer.size(); i++)
	{
		if (vxer[i].SvcNum <= KeServiceDescriptorTable.NumOfSvc)
		{
			vxer[i].PresentAddr = addresses[i];
			vxer[i].OriginalityAddr = pKiSystemTable[i] - 0x00400000 + (ULONG)pMglInfo->Base; // �Ժ��о�
			ULONG ImageBase =((PIMAGE_OPTIONAL_HEADER)((PCHAR)hMdl + ((PIMAGE_DOS_HEADER)hMdl)->e_lfanew + sizeof(ULONG) + sizeof(IMAGE_FILE_HEADER)))->ImageBase;
			strncpy((PCHAR)vxer[i].FuncHead, (PCHAR)hMdl + pKiSystemTable[i] - ImageBase, 1); // ���溯��ͷ��ǰ10B���ڼ��inline hook
		}
	}
	int nItemIndex = 0;
	ULONG aa = vxer[284].SvcNum;
	for (i = 0; i < vxer.size() - 1; i++) // ����ǿ�ƺ���һ��: NtCurrentTeb
	{	
		CString str;
		str.Format("0x%03X", vxer[i].SvcNum);
		m_lstSSDT.InsertItem(nItemIndex, str);
		m_lstSSDT.SetItemText(nItemIndex, 1, vxer[i].FuncName.c_str());

		str.Format("0x%08X", vxer[i].OriginalityAddr);
		m_lstSSDT.SetItemText(nItemIndex, 2, str);

		str.Format("0x%08X", addresses[i]);
		m_lstSSDT.SetItemText(nItemIndex, 3, str);

		// �����ַ����ģ��
		for (int nKrnlMdl = 0; nKrnlMdl < krnlMdls.size(); nKrnlMdl++)
		{
			if (addresses[i] >= krnlMdls[nKrnlMdl].ulBaseAddr && addresses[i] < (krnlMdls[nKrnlMdl].ulBaseAddr + krnlMdls[nKrnlMdl].ulImageSize))
			{
				m_lstSSDT.SetItemText(nItemIndex, 5,  krnlMdls[nKrnlMdl].szPath);
				break;
			}
		}

		// INLINE HOOK ���
		UCHAR szBuffer[1] = { 0 };
		QueryBuf.Data = szBuffer;
		QueryBuf.Length = sizeof(CHAR) * 1;
		QueryBuf.Address = (ULONG)addresses[i];
		ZwSystemDebugControl(sysdbgcopymemorychunks_0, &QueryBuf, sizeof(MEMORY_CHUNKS), NULL, 0, &dwReturnLen);
		if (vxer[i].OriginalityAddr == vxer[i].PresentAddr)
		{
			if (0 == strncmp((PCHAR)vxer[i].FuncHead, (PCHAR)szBuffer, 1))
			{
				m_lstSSDT.SetItemText(nItemIndex, 4, "No.");
			}
			else
			{
				m_lstSSDT.SetItemText(nItemIndex, 4, "Yes.");
				nInlineHookNum += 1;
				ULONG uu = pKiSystemTable[i];
			}
		}
		else
			nSSDTHookNum += 1;
		nItemIndex++;		
	}
	delete [] addresses;
	FreeLibrary(hMdl);
	HeapFree(GetProcessHeap(), NULL, pHeap);
	CString strTips;
	strTips.Format("SSDT HOOK ��������:%d  INLINE HOOK ��������:%d", nSSDTHookNum, nInlineHookNum);
	if (0 == nSSDTHookNum && 0 == nInlineHookNum)
		strTips += "   ���ϵͳʮ�ְ�ȫ.";
	SetDlgItemText(IDC_STATIC_TIPS, strTips);
}

void CSSDT_ServiceDlg::GetInfoFromNtdll()
{
	HMODULE hModule = LoadLibrary("ntdll.dll");
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_OPTIONAL_HEADER pOptHeader = (PIMAGE_OPTIONAL_HEADER)((PCHAR)hModule + pDosHeader->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_EXPORT_DIRECTORY pExpDir = (PIMAGE_EXPORT_DIRECTORY)((PCHAR)hModule + pOptHeader->DataDirectory[0].VirtualAddress);
	PULONG pNameTab = (PULONG)((ULONG)((PCHAR)hModule + pExpDir->AddressOfNames));// �������Ƶ�ַ������ʼ��ַ
	PULONG pAddrTab = (PULONG)((ULONG)((PCHAR)hModule + pExpDir->AddressOfFunctions)); // ������ַ������ʵ��ַ
	PUSHORT pNameOrdTab = (PUSHORT)((ULONG)((PCHAR)hModule + pExpDir->AddressOfNameOrdinals)); // AddressOfNameOrdinals
	for (ULONG i = 0; i < pExpDir->NumberOfNames; i++)
	{
		if (0 == strncmp((PCHAR)hModule + pNameTab[i], "Nt", 2))
		{
			VXerInfo info;
			info.FuncName.append((PCHAR)hModule + pNameTab[i]);// ������
			info.SvcNum = *(PULONG)((PCHAR)hModule + pAddrTab[pNameOrdTab[i]] + 1);// �����
			vxer.push_back(info);
		}
	}
	FreeLibrary(hModule);
}

void CSSDT_ServiceDlg::DbgPrivilege()
{
	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES,&hToken);
	LUID uid;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &uid);
	TOKEN_PRIVILEGES tp = { 0 };
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = uid;
	tp.PrivilegeCount = 1;	
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	CloseHandle(hToken);
}

void CSSDT_ServiceDlg::OnButtonKrnlmdl() 
{
	m_lstSSDT.DeleteAllItems();
	int ColCount = m_lstSSDT.GetHeaderCtrl()->GetItemCount(); 
	while (ColCount--)
		m_lstSSDT.DeleteColumn(0);
	bShowSSDT = FALSE;
	krnlMdls.clear(); // �������

	m_lstSSDT.InsertColumn(0, "ģ������", LVCFMT_LEFT, 100);
	m_lstSSDT.InsertColumn(1, "���ػ�ַ", LVCFMT_CENTER, 75);
	m_lstSSDT.InsertColumn(2, "ӳ���С", LVCFMT_CENTER, 75);
	m_lstSSDT.InsertColumn(3, "��������", LVCFMT_CENTER, 70);
	m_lstSSDT.InsertColumn(4, "���ؼ���", LVCFMT_CENTER, 70);
	m_lstSSDT.InsertColumn(5, "�ں˱�־", LVCFMT_CENTER, 79);
	m_lstSSDT.InsertColumn(6, "ӳ��·��", LVCFMT_LEFT, 358);
	
	DWORD dwNeedLen = 0;// 
	ZwQuerySystemInformation(11, NULL, 0, &dwNeedLen);// ö���ں˼��ص�ģ��
	LPVOID pHeap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeedLen);
	ZwQuerySystemInformation(11, pHeap, dwNeedLen, &dwNeedLen);
	ULONG count = *(PULONG)pHeap; // ģ����
	PSYSTEM_MODULE_INFORMATION pMglInfo = (PSYSTEM_MODULE_INFORMATION)((PCHAR)pHeap + 4);
	int nIndex = 0;
	for (ULONG i = 0; i < count; i++)
	{
		CString str;
		m_lstSSDT.InsertItem(nIndex, pMglInfo->ModuleNameOffset +  pMglInfo->ImageName); // ģ������

		str.Format("0x%08X", (ULONG)pMglInfo->Base); // ���ػ�ַ
		m_lstSSDT.SetItemText(nIndex, 1, str);

		str.Format("0x%08X", pMglInfo->Size); // ӳ���С
		m_lstSSDT.SetItemText(nIndex, 2, str);

		str.Format("%d", i);
		m_lstSSDT.SetItemText(nIndex, 3, str);

		str.Format("%d", pMglInfo->LoadCount);// ���ش���
		m_lstSSDT.SetItemText(nIndex, 4, str); 

		str.Format("0x%08X", pMglInfo->Flags);// ��־
		m_lstSSDT.SetItemText(nIndex, 5, str); 

		m_lstSSDT.SetItemText(nIndex, 6, pMglInfo->ImageName); // ӳ��·��
	
		KernelModules mdl;
		mdl.ulBaseAddr = (ULONG)pMglInfo->Base;
		mdl.ulImageSize = pMglInfo->Size;
		strcpy(mdl.szPath, pMglInfo->ImageName);
		krnlMdls.push_back(mdl);

		pMglInfo++;
		nIndex++;
	}
}


void CSSDT_ServiceDlg::OnDrawColorForMyList(NMHDR *pNmHdr, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNmHdr);	
    *pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
        *pResult = CDRF_NOTIFYITEMDRAW;
    else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
    else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{   
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		pLVCD->clrText = RGB(0, 0, 0); // ��������Ϊ��ɫ

		// ���ñ���ɫ
		if(bShowSSDT && vxer[nItem].OriginalityAddr != vxer[nItem].PresentAddr)
			pLVCD->clrTextBk = RGB(255, 0, 0);
		else if(m_lstSSDT.GetItemText(nItem, 4) == "Yes.")
			pLVCD->clrTextBk = RGB(0, 0xCC, 0);
		else		
			pLVCD->clrTextBk = RGB(255, 255, 255);
		*pResult = CDRF_DODEFAULT;
	}
}

void CSSDT_ServiceDlg::OnRclickListSsdt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (bShowSSDT)
	{
		CPoint point; 
		::GetCursorPos(&point); 
		HMENU hMenu = LoadMenu(AfxGetInstanceHandle(), (LPCTSTR)IDR_MENU);
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		TrackPopupMenu(hSubMenu, TPM_LEFTALIGN, point.x, point.y, 0, m_hWnd, NULL);
	}
	*pResult = 0;
}

void CSSDT_ServiceDlg::OnMenuitemRepair() 
{
	POSITION pos = m_lstSSDT.GetFirstSelectedItemPosition();
	int nSel = 	m_lstSSDT.GetNextSelectedItem(pos);
	ULONG SvcNum = strtoul(m_lstSSDT.GetItemText(nSel, 0), '\0', 16); // �����
//	ULONG TrueAddr = strtoul(m_lstSSDT.GetItemText(nSel, 2), '\0', 16); // ��ʵ��ַ
	ULONG TrueAddr = 0; // ��ʵ��ַ
	MEMORY_CHUNKS QueryBuff;
	QueryBuff.Address = (ULONG)&KeServiceDescriptorTable.SvcTabBase[SvcNum];
	QueryBuff.Data = &TrueAddr;
	QueryBuff.Length = sizeof(ULONG);
	DWORD ReturnLength;
	ZwSystemDebugControl(SysDbgWriteVirtualMemory, &QueryBuff, sizeof(MEMORY_CHUNKS), NULL, 0, &ReturnLength);
	OnButtonStart();
}

void CSSDT_ServiceDlg::OnMenuitemFresh() 
{
	if (bShowSSDT)
		OnButtonStart();
	else
		OnButtonKrnlmdl();
}

void CSSDT_ServiceDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
