// MyPic.cpp : 实现文件
//

#include "../pch.h"
#include "MyPic.h"
#include "../Resource.h"
#include "psapi.h"
#include <TlHelp32.h>
#include <iostream>
#include <io.h>

#pragma comment(lib,"psapi.lib")
using namespace std;

#pragma warning(disable:4996)


void myMsg()
{
	MSG m;
	while (PeekMessage(&m, 0, 0, 0, 1) != 0)
	{
		DispatchMessage(&m);
		TranslateMessage(&m);
	}
}

void LwSleep(DWORD64 s)
{
	LARGE_INTEGER j_sj;
	j_sj.QuadPart = -10 * 1000 * s;
	HANDLE	j_jb = CreateWaitableTimerA(0, FALSE, 0);
	if (j_jb)
	{
		SetWaitableTimer(j_jb, &j_sj, 0, 0, 0, FALSE);
		while (MsgWaitForMultipleObjects(1, &j_jb, FALSE, -1, 255) != 0)
		{
			myMsg();
		}
		CloseHandle(j_jb);
	}

}


CString g_str;
/*取运行目录*/
CString QuYunxunmulu()//取运行目录
{

	CString strPath;
	char strBuff[512];
	GetModuleFileName(NULL, strBuff, 512);//取运行目录
	strPath.Format("%s", strBuff);
	int pos = strPath.ReverseFind('\\');
	strPath = strPath.Left(pos);
	return strPath;
}

//返回值:当前EXE目录
CString GetIniDLLpath()
{


	CString iniPath;
	if (g_str == "")
	{
		g_str = QuYunxunmulu();
	}
	iniPath = g_str+"\\Pulg";

	if (!PathIsDirectory(iniPath.GetBuffer()))  // 是否有重名文件夹
	{
		iniPath.ReleaseBuffer();
		::CreateDirectory(iniPath.GetBuffer(), 0);
	}
	iniPath.ReleaseBuffer();

	return iniPath;
}
/*
功能：从资源保存文件
参数1：资源ID
参数2：资源类型
参数3：保存文件名全路径
*/
BOOL FreeMyResource(UINT uiResouceName,const char* lpszResourceType, const char* lpszSaveFileName)
{
	HRSRC hRsrc = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(uiResouceName), CString(lpszResourceType));
	//LPTSTR szBuffer = new TCHAR[1024];//这里是定义缓冲区大小

	if (hRsrc == NULL)
	{
		//FreeRes_ShowError("FindResource");
		return FALSE;
	}
	DWORD dwSize = ::SizeofResource(NULL, hRsrc);
	if (0 >= dwSize)
	{
		//FreeRes_ShowError("SizeofResource");
		return FALSE;
	}

	HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		//FreeRes_ShowError("LoadResource");
		return FALSE;
	}

	LPVOID lpVoid = ::LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		//FreeRes_ShowError("LockResource");
		return FALSE;
	}


	FILE* fp = NULL;
	fopen_s(&fp, lpszSaveFileName, "wb+");
	if (NULL == fp)
	{
		//FreeRes_ShowError("LockResource");
		return FALSE;
	}
	fwrite(lpVoid, sizeof(char), dwSize, fp);
	fclose(fp);

	return TRUE;
}
//文件是否存在
int  file_exist(const char* str)
{

	if (access(str, 0) == NULL)
	{
		return 1;
	}
	return 0;
}

