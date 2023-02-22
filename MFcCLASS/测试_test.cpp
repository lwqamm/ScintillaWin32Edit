// dlg3.cpp: 实现文件
//

#include "pch.h"
#include "ViewWizard2.h"
#include "dlg3.h"
#include "afxdialogex.h"
#include <fstream>  
#include <iostream>  
#include "MyPic.h"
#include "X64X86ASM.h"

using namespace std;
#define  EDIT_ASM  10000
#define  EDTER     "\r\n"
#include <TlHelp32.h>
#pragma warning(disable:4996)
// dlg3 对话框
extern DWORD g_pid;
extern  LPVOID g_Mymem ;
extern StrIntRetARR g_StrIntRet;//汇编代码格式死锁

ScintillaWnd            m_SciEdit;//汇编代码输入框
HWND                 g_hwnd = 0;
dlg3* g_dlg3 = NULL;
FARPROC  X64LoadLibraryA = NULL;
ULONG    X86LoadLibraryA = NULL;
FARPROC  X64FreeLibrary = 0;
ULONG  X86FreeLibrary = 0;
BOOL IsDll64Ex(const char* path, BOOL rst);//检测DLL是否64位
BOOL IsDll64ExS(const char* path);//检测DLL是否合法
void SetAsmShowTextEx(LPCSTR str);
CString  g_AsmStrKS = "pop mov sub pus lea xor int test je ja and jmp ror js call cmp jns popad cmove ret retn add set jae jne bt btr jbe syscall cmo lock imu inc retn dec out ins jo arp lod dec fim div rcr neg mul xch sbb db or shr";

Split mylsit;
CString FormatCode(CString str1)
{
	for (size_t i = 0; i < str1.GetLength(); i++)
	{
		if (str1[i]=='\r')
		{
			str1.Delete(i);
		}
	}


	DWORD AsmArrlen = mylsit.GetlistSize();
	if (mylsit.GetlistSize()<=0)
	{
		return "";
	}
	g_StrIntRet.Clear();
	CString str;
	DWORD tlen = 0;
	MyAsmStrcode myasm;
	MyStructcode t_asm;
	for (ULONGLONG i = 0; i < AsmArrlen; i++)
	{
		str = mylsit.GetdxlistStr(i);
		if (str==str1.Mid(0,str.GetLength()))
		{
			return str1;
		}

	}



	for (ULONGLONG i = 0; i < AsmArrlen; i++)
	{
		str = mylsit.GetdxlistStr(i);
		CString stc = str;
		CString stc_1 = str1;
		while (1)
		{

			str = FindStrASM(stc_1, stc, "  , |,\r\n", tlen,&tlen);

			if (str != "")
			{
				str = FindDeleteStr(str," |");
				str = FindDeleteStr(str, "\n");
			
				memset(&t_asm, 0, sizeof(MyStructcode));
				sprintf(t_asm.asmstr, str, str.GetLength());
				t_asm.m_StrPos = tlen;
				myasm.AddAsmstr(&t_asm);             
			}
	
			if (str =="")
			{
				tlen = 0;
				break;
			}

		}	
		
	}

	if (myasm.GetSize()>0)
	{
		return myasm.GetAsmStrCode();
	}


	
	return "";
}

typedef struct _MYPID_DATA
{
	DWORD pid=0;
	char path[MAX_PATH] = { 0 };
	LPVOID  p_i=0;
}MYPID_DATA,*PMYPID_DATA;

IMPLEMENT_DYNAMIC(dlg3, CDialogEx)

dlg3::dlg3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

dlg3::~dlg3()
{
	//FreeLibrary(m_hDll);
}

BEGIN_MESSAGE_MAP(dlg3, CDialogEx)
	ON_BN_CLICKED(IDC_DLLIN2, &dlg3::OnBnClickedDllin2)
	ON_BN_CLICKED(IDC_DLLIN3, &dlg3::OnBnClickedDllin3)
	ON_BN_CLICKED(IDC_DLLIN4, &dlg3::OnBnClickedDllin4)
	ON_BN_CLICKED(IDC_BUTTON11, &dlg3::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &dlg3::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON1, &dlg3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON10, &dlg3::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON2, &dlg3::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_DLLIN, &dlg3::OnBnClickedDllin)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON15, &dlg3::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON14, &dlg3::OnBnClickedButton14)
END_MESSAGE_MAP()



// dlg3 消息处理程序
BOOL dlg3::OnInitDialog()
{
	X64LoadLibraryA = (FARPROC)LoadLibraryA;
	X64FreeLibrary= (FARPROC)FreeLibrary;
	mylsit.SplitString(g_AsmStrKS);
	GetDebugPrivilege();
	CRect tabRect;
	if (!dg4)
	{
		//CRect tabRect;
		tabRect.left = 674;
		tabRect.top = 44;
		tabRect.right = 674 + 350;
		tabRect.bottom = 44 + 395;
		//BUTTON2_3_1
		dg4.Create(IDD_DIALOG4,this);
		dg4.MoveWindow(&tabRect);
		dg4.ShowWindow(SW_HIDE);

		//m_button1.EnableWindow(false);
	}
	//m_hDll = LoadLibrary("SciLexer.dll");
	
	tabRect.left = 5;
	tabRect.top = 35;
	tabRect.right = 5 + 560;
	tabRect.bottom = 44 + 370;

//	g_hwnd = m_SciEdit.Create(m_hWnd, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), 10000);
	//m_SciEdit.AddEditStr(g_AsmStrKS);//f020f0
	
	m_SciEdit.Create(_T("Scintilla"), _T("CppCodeEditor"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, tabRect, this, 1011);
	m_SciEdit.AddEditStr("int call push ptr eax ebx ecx edx ebp esp esi edi rax rbx rcx rdx rbp rsp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15 mov rax rcx dword qword test int3");
	m_SciEdit.AddEditStr("pop mov sub pus lea xor int test je ja and jmp ror js call cmp jns popad cmove ret retn add set jae jne bt btr jbe syscall cmo lock imu inc retn dec out ins jo arp lod dec fim div rcr neg mul xch sbb db or shr r8");
	m_SciEdit.AddEditStr("raxd rbxd rdxd rcxd rbpd rspd rsid rdid r8d rbxd r9d r10d r11d r12d r13d r14d r15d rip word byte ds fs ss gs es cs dr0 dr1 dr2 dr3 dr4 dr5 dr6 dr7 ymm0 ymm1 ymm2 ymm3 ymm4 ymm5 ymm6 ymm7 ymm8 ymm9 ymm10 ymm11 ymm12 ymm13 ymm14 ymm15");
	m_SciEdit.AddEditStr("xmm0 xmm1 xmm2 xmm3 xmm4 xmm5 xmm6 xmm7 xmm8 xmm9 xmm10 xmm11 xmm12 xmm13 xmm14 xmm15 mm0 mm1 mm2 mm3 mm4 mm5 mm6 mm7 move movdqu xmmword");
	m_SciEdit.ShowWindow(SW_SHOW);
	m_SciEdit.SetEditTextColor(SET_EDITDIGITALCOLOR, RGB(0xF0, 0x20, 0xF0));
	m_SciEdit.InitScintillaEdit(16, _T("Courier New"));
	CString str;


	g_dlg3 = this;
	MyIniPuls myini;
	myini.CreateIniFilePath("config.ini");
	str = myini.ReadIni("DLL注入路径");
	SetDlgItemText(EDIT_DLL, str);
	SetAsmShowText("《《《注入操作提示窗口！---》》》》");
	return 0;
}

void dlg3::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);

}


void SetAsmShowTextEx(LPCSTR str)
{
	g_dlg3->SetAsmShowText(str);
}

void dlg3::SetAsmShowText(LPCSTR str)
{
	SetDlgItemText(EDIT_ASMSHOW, str);
}

void dlg3::OnBnClickedDllin2()
{
	// TODO: 在此添加控件通知处理程序代码
	char pDllPathBuf[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = "DLL文件\0*.dll\0\0";
	ofn.lpstrFile = pDllPathBuf;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OPEN_EXISTING;
	if (!GetOpenFileName(&ofn))
	{
		//MessageBox(NULL,"请选择DLL文件","提示!",MB_OK|MB_ICONWARNING);
		return;
	}
	char ext[10] = { 0 };
	_splitpath(pDllPathBuf, 0, 0, 0, ext);//将路径名称分解成组件<stdlib.h> 或 <wchar.h>
	toUpper(ext);
	CString str = ".dll";
	if (ext != str || !IsDll64ExS(pDllPathBuf))
	{
		MessageBox("请选择正确的DLL文件！", "提示");
		return;
	}
	MyIniPuls myini;
	myini.CreateIniFilePath("config.ini");
	myini.WriteIni("DLL注入路径", pDllPathBuf);
	SetDlgItemText(EDIT_DLL, pDllPathBuf);
}


void dlg3::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);
	char ext[10] = { 0 };
	_splitpath(szPath, 0, 0, 0, ext);//将路径名称分解成组件<stdlib.h> 或 <wchar.h>
	toUpper(ext);
	CString str = ".dll";
	if (ext!=str||!IsDll64ExS(szPath))
	{
		MessageBox("请选择正确的DLL文件！", "提示");
		return;
	}

	MyIniPuls myini;
	myini.CreateIniFilePath("config.ini");
	myini.WriteIni("DLL注入路径", szPath);

	SetDlgItemText(EDIT_DLL, szPath);

	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL IsDll64Ex(const char* path,BOOL rst)//检测DLL是否64位
{
	HANDLE hFile = CreateFile(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	DWORD fileSize = GetFileSize(hFile, NULL);    //获得文件大小的低两字节
	char* fileBuff = new char[fileSize];    //创建缓冲区
	DWORD realRead = 0;
	BOOL ret = false;
	ret = ReadFile(hFile, fileBuff, fileSize, &realRead, NULL);    //拷贝文件的内容给缓冲区fileBuff
	CloseHandle(hFile);
	if (!ret)
	{
		delete[] fileBuff;
		return FALSE;
	}
	PIMAGE_DOS_HEADER pDosHeaders = (PIMAGE_DOS_HEADER)fileBuff;    //得到DOS头结构体
	PIMAGE_FILE_HEADER ifh = (PIMAGE_FILE_HEADER)(fileBuff+pDosHeaders->e_lfanew+4);
	int n = ifh->Machine;
	if (rst==TRUE)
	{
	
			if (n == 0x014C || n != 0x0200 || n != 0x8664 || pDosHeaders->e_magic != 0x5A4D)
			{
				delete[] fileBuff;
				return FALSE;
			}

		PIMAGE_NT_HEADERS64  pNTheader = (PIMAGE_NT_HEADERS64)((UINT64)pDosHeaders + (UINT64)pDosHeaders->e_lfanew);
		if (pNTheader->Signature != 0x4550)
		{
			AfxMessageBox("不是有效的PE文件");
			delete[] fileBuff;        //删除创建的空间
			return FALSE;
		}
		if ((pNTheader->FileHeader.Characteristics == 0x2022))
		{
			//pNTheader->FileHeader.Characteristics & 0x2000
			//cout << "是DLL文件" << endl;
			delete[] fileBuff;        //删除创建的空间
			return TRUE;
		}
		delete[] fileBuff;        //删除创建的空间
		return FALSE;
	}
	else
	{

			if (n != 0x014C || pDosHeaders->e_magic != 0x5A4D)
			{
				delete[] fileBuff;
				return FALSE;
			}
		PIMAGE_NT_HEADERS32  pNTheader = (PIMAGE_NT_HEADERS32)((UINT64)pDosHeaders + (UINT64)pDosHeaders->e_lfanew);
		if (pNTheader->Signature != 0x4550)
		{
			AfxMessageBox("不是有效的PE文件");
			delete[] fileBuff;        //删除创建的空间
			return FALSE;
		}

		if ((pNTheader->FileHeader.Characteristics == 0x210E || pNTheader->FileHeader.Characteristics == 0x2102))
		{
			//pNTheader->FileHeader.Characteristics & 0x2000
			//cout << "是DLL文件" << endl;

			delete[] fileBuff;        //删除创建的空间
			return TRUE;
		}
		delete[] fileBuff;        //删除创建的空间
		return FALSE;
	}
}



int CrnGetImageFileMachine(const char* lpFileName)
{
	IMAGE_DOS_HEADER idh;
	FILE* f = fopen(lpFileName, "rb");
	fread(&idh, sizeof(idh), 1, f);
	IMAGE_FILE_HEADER ifh;
	fseek(f, idh.e_lfanew + 4, SEEK_SET);
	fread(&ifh, sizeof(ifh), 1, f);
	fclose(f);

	int n = ifh.Machine;
	if (n == 0x014C) //printf("x86\n");//32位
	{
		n = 32;
		return n;
	}
	else if (n == 0x0200)// printf("IA64\n");//纯64位
	{
		n = 64;
		return n;
	}
	else if (n == 0x8664) //printf("x64\n");//64位
	{
		n = 64;
		return n;
	}
	//else //printf("未知\n");
	return 86;

}

BOOL inject(LPVOID p11)
{
	PMYPID_DATA p = (PMYPID_DATA)p11;
	char* pDllPathBuf = p->path;
	if (!pDllPathBuf||!p->p_i)
	{
		return FALSE;
	}
	char ch[MAX_PATH] = { 0 };
	GetPathStrFile(pDllPathBuf, ch);
	CString str;
	if (GetMoHandleEX(p->pid, ch))
	{
		str.Format("该进程下已经存在 %s 模块，不要重复注入！", ch);
		
		g_dlg3->SetAsmShowText("32位程序不能注入64位DLL！");
		//AfxMessageBox(str);
		return FALSE;
	}
	if (p->pid<110)
	{
		//AfxMessageBox("非法进程禁止操作！");
		g_dlg3->SetAsmShowText("非法进程禁止操作！");
		return FALSE;
	}
	DWORD strL = strlen(pDllPathBuf) + 1;
	HANDLE hadle = OpenProcess(PROCESS_ALL_ACCESS| PROCESS_VM_OPERATION, FALSE, p->pid);
	if (hadle <= 0)
	{
		//MessageBox(NULL, "进程打开失败!", "提示", 1);
		g_dlg3->SetAsmShowText("进程打开失败!");
		return FALSE;
	}
	void* pRemoteAdrr = VirtualAllocEx(hadle, NULL, strL, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteAdrr == NULL)
	{
		//AfxMessageBox("在目标进程申请内存失败！");
		g_dlg3->SetAsmShowText("在目标进程申请内存失败！");
		CloseHandle(hadle);
		return FALSE;
	}
	
	if (!WriteProcessMemory(hadle, pRemoteAdrr, pDllPathBuf, strL, NULL))
	{
		//AfxMessageBox("在目标进程内存写DLL路径失败！");
		g_dlg3->SetAsmShowText("在目标进程内存写DLL路径失败！");
		CloseHandle(hadle);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	DWORD dwpid = 0;
	HANDLE the1 = CreateRemoteThread(hadle, NULL, 0, (LPTHREAD_START_ROUTINE)p->p_i, pRemoteAdrr, 0, &dwpid);
	WaitForSingleObject(the1, INFINITE);
	//GetExitCodeThread(the1, &g_undll);
	if (VirtualFreeEx(hadle, pRemoteAdrr, 0, MEM_RELEASE))
	{
		dwpid = dwpid;
	}
	CloseHandle(the1);
	CloseHandle(hadle);
	g_dlg3->SetAsmShowText("注入成功！");
	return TRUE;
}

/*DLL注入*/
BOOL Ntinject(PMYPID_DATA p11)
{
	if(!p11)
	{
		//AfxMessageBox("参数指针错误！");
		g_dlg3->SetAsmShowText("参数指针错误！");
		return FALSE;
	}
	PMYPID_DATA p = (PMYPID_DATA)p11;
	char ch[MAX_PATH] = { 0 };
	GetPathStrFile(p->path, ch);
	CString str;
	if (GetMoHandleEX(p->pid, ch))
	{
		str.Format("该进程下已经存在 %s 模块，不要重复注入！", ch);
		//AfxMessageBox(str);
		g_dlg3->SetAsmShowText(str);
		return FALSE;
	}
	
	HANDLE hProcess = NULL;
	if ((hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION, FALSE, p->pid)) == NULL)
	{
		return 0;
	}
	if (p->pid < 110)
	{
		//AfxMessageBox("非法进程禁止操作！");
		g_dlg3->SetAsmShowText("非法进程禁止操作！");
		CloseHandle(hProcess);
		return FALSE;
	}

	//在要被注入的进程中创建内存，用于存放注入dll的路径
	LPVOID Buff = NULL;
	SIZE_T write_len = 0;
	DWORD DllPathLen = strlen(p->path) + 1;
	Buff = VirtualAllocEx(hProcess, NULL, DllPathLen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (Buff == NULL)
	{
		//AfxMessageBox("在目标进程申请内存失败！");
		g_dlg3->SetAsmShowText("在目标进程申请内存失败！");
		CloseHandle(hProcess);
		return FALSE;
	}
	//将dll路径写入刚刚创建的内存中
	WriteProcessMemory(hProcess, Buff, p->path, DllPathLen, &write_len);
	if (DllPathLen != write_len)
	{
		//AfxMessageBox("在目标进程内存写DLL路径失败！");
		g_dlg3->SetAsmShowText("在目标进程内存写DLL路径失败！");
		VirtualFreeEx(hProcess, Buff, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}
	//加载ntdll.dll并从中获取内核函数ZwCreateThread，并使用函数指针指向此函数
	HMODULE Ntdll = NULL;
	Ntdll = LoadLibrary("ntdll.dll");
	if (!Ntdll)
	{
		VirtualFreeEx(hProcess, Buff, 0, MEM_RELEASE);
		//AfxMessageBox("获取本地Ntdll模块地址失败！");
		g_dlg3->SetAsmShowText("获取本地Ntdll模块地址失败！");
		CloseHandle(hProcess);
		return FALSE;
	}
	//执行ZwCreateThread函数，在指定进程中创建线程加载要被注入的dll
	DWORD dwStatus = 0;
	HANDLE hRemoteThread = NULL;
	if (Ntdll)
	{
#ifdef _WIN64
		{
			pZwCreateThreadEx ZwCreateThreadEx =
				(pZwCreateThreadEx(GetProcAddress(Ntdll, "ZwCreateThreadEx")));
			dwStatus = ZwCreateThreadEx(
				&hRemoteThread,
				PROCESS_ALL_ACCESS,
				NULL,
				hProcess,
				(LPTHREAD_START_ROUTINE)p->p_i,
				Buff,
				0, 0, 0, 0,
				NULL
			);
			if (ZwCreateThreadEx == NULL)
			{
				VirtualFreeEx(hProcess, Buff, 0, MEM_RELEASE);
				CloseHandle(hProcess);
				//GetDebugPrivilege(backCode, 1);
				g_dlg3->SetAsmShowText("注入失败！");
				return 0;
			}
		}
#else
		{
			typedef_ZwCreateThreadEx ZwCreateThreadEx =
				(typedef_ZwCreateThreadEx(GetProcAddress(Ntdll, "ZwCreateThreadEx")));
			dwStatus = ZwCreateThreadEx(
				&hRemoteThread,
				PROCESS_ALL_ACCESS,
				NULL,
				hProcess,
				(LPTHREAD_START_ROUTINE)p->p_i,
				Buff,
				0, 0, 0, 0,
				NULL
			);
			if (ZwCreateThreadEx == NULL)
			{
				VirtualFreeEx(hProcess, Buff, 0, MEM_RELEASE);
				CloseHandle(hProcess);
				//GetDebugPrivilege(backCode, 1);
				g_dlg3->SetAsmShowText("注入失败！");
				return 0;
			}
		}
#endif


		if (hRemoteThread == NULL)
		{
			VirtualFreeEx(hProcess, Buff, 0, MEM_RELEASE);
			CloseHandle(hProcess);
			//GetDebugPrivilege(backCode, 1);
			g_dlg3->SetAsmShowText("注入失败！");
			return 0;
		}
		FreeModule(Ntdll);
	}
	WaitForSingleObject(hRemoteThread, INFINITE);
	VirtualFreeEx(hProcess, Buff, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	CloseHandle(hRemoteThread);
	g_dlg3->SetAsmShowText("注入成功！");
	return TRUE;
}

MYPID_DATA pmy;
void dlg3::OnBnClickedDllin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(EDIT_DLL, str);
	if (g_pid<110)
	{
		SetAsmShowText("未选择有效进程!");
		return;
	}
	if (str.GetLength() < 8)
	{
		SetAsmShowText("未选择有效DLL路径！");
		return;
	}
	LPVOID t_p = 0;
	//CrnGetImageFileMachine(str);
	if (IsWow64ProcessExS(g_pid))
	{
		//MessageBox("64");
		if (!IsDll64Ex(str, TRUE))
		{
			SetAsmShowText("64位程序不能注入32位DLL！");
			return;
		}
		t_p = X64LoadLibraryA;

	}
	else
	{
		if (!X86LoadLibraryA)
		{
			BOOL rst;
			X86LoadLibraryA = (ULONG)EnumDllByZwQueryVirtualMem(g_pid, "kernel32.dll", "LoadLibraryA", &rst);
			if (!X86LoadLibraryA)
			{
				MessageBox("获取函数失败");
				return;
			}
		}
		//MessageBox("32");
		if (!IsDll64Ex(str, FALSE))
		{
			SetAsmShowText("32位程序不能注入64位DLL！");
			return;
		}
		t_p = (LPVOID)X86LoadLibraryA;
	}
	pmy.pid = g_pid;
	pmy.p_i = t_p;
	sprintf(pmy.path, str.GetBuffer(), str.GetLength());
	str.ReleaseBuffer();
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)inject, &pmy, 0, 0);
	

}

void dlg3::OnBnClickedDllin3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(EDIT_DLL, str);
	if (g_pid < 110)
	{
		SetAsmShowText("未选择有效进程!");
		return;
	}
	if (str.GetLength() < 8)
	{
		SetAsmShowText("无效DLL路径");
		return;
	}
	LPVOID t_p = 0;
	//CrnGetImageFileMachine(str);
	if (IsWow64ProcessExS(g_pid))
	{
		//MessageBox("64");
		if (!IsDll64Ex(str, TRUE))
		{
			SetAsmShowText("64位程序不能注入32位DLL！");
			return;
		}
		t_p = X64LoadLibraryA;

	}
	else
	{
		if (!X86LoadLibraryA)
		{
			BOOL rst;
			X86LoadLibraryA = (ULONG)EnumDllByZwQueryVirtualMem(g_pid, "kernel32.dll", "LoadLibraryA", &rst);
			if (!X86LoadLibraryA)
			{
				//MessageBox("获取函数失败");
				SetAsmShowText("获取函数失败");
				return;
			}
		}
		//MessageBox("32");
		if (!IsDll64Ex(str, FALSE))
		{
			SetAsmShowText("32位程序不能注入64位DLL！");
			return;
		}
		t_p = (LPVOID)X86LoadLibraryA;
	}
	pmy.pid = g_pid;
	pmy.p_i = t_p;
	sprintf(pmy.path, str.GetBuffer(), str.GetLength());
	str.ReleaseBuffer();
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Ntinject, &pmy, 0, 0);
}
BOOL UNinject(PMYPID_DATA p11)
{
	if (!p11)
	{
		//AfxMessageBox("参数错误！");
		g_dlg3->SetAsmShowText("参数错误!");
		return FALSE;
	}
	PMYPID_DATA p = (PMYPID_DATA)p11;
	char ch[MAX_PATH] = { 0 };
	GetPathStrFile(p->path, ch);
	if (p->pid<110)
	{
		//AfxMessageBox("非法进程禁止操作！");
		g_dlg3->SetAsmShowText("非法进程禁止操作!");
		return FALSE;
	}
	DWORD64 dwp = GetMoHandleEX(p->pid, ch);
	if (!dwp)
	{
		//AfxMessageBox("目标进程不存在指定的 DLL 模块！");
		g_dlg3->SetAsmShowText("目标进程不存在指定的 DLL 模块！");
		return FALSE;
	}
	HANDLE ha = OpenProcess(PROCESS_ALL_ACCESS, FALSE, p->pid);
	if (!ha)
	{
		//MessageBox(NULL, "远程进程打开失败!", "提示", 1);
		g_dlg3->SetAsmShowText("远程进程打开失败!");
		return FALSE;
	}
	HANDLE the1 =CreateRemoteThread(ha, NULL, 0, (LPTHREAD_START_ROUTINE)p->p_i, (LPVOID)dwp, 0, 0);
	if (!the1)
	{
		//MessageBox(NULL, "远程线程创建失败!", "提示", 1);
		g_dlg3->SetAsmShowText("远程线程创建失败!");
		CloseHandle(ha);
		return FALSE;
	}
	WaitForSingleObject(the1, INFINITE);
	CloseHandle(the1);
	CloseHandle(ha);
	g_dlg3->SetAsmShowText("卸载成功!");
	return TRUE;

}

void dlg3::OnBnClickedDllin4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(EDIT_DLL, str);
	if (g_pid < 110)
	{
		SetAsmShowText("未选择有效进程!");
		return;
	}
	if (str.GetLength() < 8)
	{
		SetAsmShowText("无效DLL路径");
		return;
	}
	LPVOID t_p = 0;
	//CrnGetImageFileMachine(str);
	if (IsWow64ProcessExS(g_pid))
	{
		//MessageBox("64");
		//if (!IsDll64Ex(str, TRUE))
		//{
		//	SetAsmShowText("64位");
		//	return;
		//}
		t_p = X64FreeLibrary;

	}
	else
	{
		if (!X86FreeLibrary)
		{
			BOOL rst;
			X86FreeLibrary = (ULONG)EnumDllByZwQueryVirtualMem(g_pid, "kernel32.dll", "FreeLibrary", &rst);
			if (!X86FreeLibrary)
			{
				//MessageBox("获取函数失败");
				SetAsmShowText("获取函数失败！");
				return;
			}
		}
		//MessageBox("32");
		//if (!IsDll64Ex(str, FALSE))
		//{
		//	SetAsmShowText("32位！");
		//	return;
		//}
		t_p = (LPVOID)X86FreeLibrary;
	}
	pmy.pid = g_pid;
	pmy.p_i = t_p;
	sprintf(pmy.path, str.GetBuffer(), str.GetLength());
	str.ReleaseBuffer();
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UNinject, &pmy, 0, 0);

}


void dlg3::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

}


void dlg3::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (dg4.IsWindowVisible())
	{
		dg4.ShowWindow(SW_HIDE);
		CButton* p =(CButton*)GetDlgItem(IDC_BUTTON11);
		
		p->SetWindowText("辅助功能->");
		return;
	}
	dg4.ShowWindow(SW_SHOW);
	CButton* p = (CButton*)GetDlgItem(IDC_BUTTON11);
	p->SetWindowText("<-辅助功能");



	
	
}


void dlg3::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
  CString str= m_SciEdit.GetText();
 
 if (str.GetLength()<=20)
 {
	 return;
 }
  str = FormatCode(str);
	m_SciEdit.SetText(str);
	//m_pScintillaWnd->SetScrollPos(SB_HORZ, 0, TRUE);

}

BOOL IsPid(DWORD pid)
{
	HANDLE hSnapshot = NULL;
	hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST | TH32CS_SNAPPROCESS, pid);
	if(hSnapshot == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	PROCESSENTRY32 procEntry = { 0 };
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hSnapshot, &procEntry);
	if (!bRet)
	{
		CloseHandle(hSnapshot);
		return FALSE;
	}
	while (bRet)
	{
		if (procEntry.th32ProcessID==pid)
		{
			CloseHandle(hSnapshot);
			return TRUE;
		}
		bRet = Process32Next(hSnapshot, &procEntry);
	}

	CloseHandle(hSnapshot);
	return FALSE;
}



void dlg3::OnBnClickedButton1() //CALL
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_pid <= 110)
	{

		SetAsmShowText("未选择有效进程!");
		return;
	}

	if (!IsPid(g_pid))
	{
		SetAsmShowText("进程pid无效或者指定进程已经关闭！");
		return;
	}

	CString str= m_SciEdit.GetText();

	PCHAR p = (PCHAR)str.GetBuffer();
	for (size_t i = 0; i < str.GetLength(); i++)
	{
		if (p[i]=='\r')
		{
			str.Delete(i);
		}
		
		if (p[i] == '0'&&( p[i+1] == 'x'|| p[i+1] == 'X'))
		{
			str.Delete(i);
			str.Delete(i);
		}
	}
	str.ReleaseBuffer();


	if (str.GetLength()<=2)
	{
		SetAsmShowText("未检测到汇编指令!");
	return;
	}
     X64X86ASM  AsmStr;
	 SetAsmShowText("");
	if (!AsmStr.CreateAsmStr(g_pid, str))
	{
		return;
	}
	AsmStr.SysCall();

}


void dlg3::OnBnClickedButton12()//Clear
{
	// TODO: 在此添加控件通知处理程序代码

	m_SciEdit.Clear();
}


void dlg3::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SciEdit.SetText("mov eax, dword ptr ds:[ eax ]\r\nmov eax, dword ptr ds:[ eax + 288 ]\r\nmov eax, 123654\r\nmov edi, 123456\r\nmov ebx, [ eax ]\r\npush 5\r\npush 4\r\npush 3\r\npush 2\r\npush 1\r\ncall 0x403390");
}


void dlg3::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SciEdit.SetText("mov rax, [ rax ]\r\nmov rax, [ rax + 330 ]\r\nmov rbx, 0x1654\r\nmov rax, 7\r\nmov [ rsp + 30 ], rax\r\nmov [ rsp + 28 ], 6\r\nmov [ rsp + 20 ], 5\r\nmov r9, 0x4\r\nmov r8, 0x3\r\nmov rdx, 2\r\nmov rcx, 1\r\ncall 0x140003EE0");
}


BOOL dlg3::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类


	return CDialogEx::PreTranslateMessage(pMsg);
}
