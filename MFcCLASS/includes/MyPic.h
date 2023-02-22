#pragma once
#pragma warning(disable:4996)

// CMyPic


BOOL FreeMyResource(UINT uiResouceName, const char* lpszResourceType, const char* lpszSaveFileName);
//文件是否存在
int  file_exist(const char* str);
//返回值:当前EXE目录
CString GetIniDLLpath();
