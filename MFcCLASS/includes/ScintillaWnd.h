#pragma once
#include "Scintilla.h"
#include "SciLexer.h"

// ScintillaWnd

class ScintillaWnd : public CWnd
{
	DECLARE_DYNAMIC(ScintillaWnd)

public:
	ScintillaWnd();
	virtual ~ScintillaWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	HMODULE m_hSciLexerDll;

public:
	virtual void Init();
	void InitScintillaEdit(int nSize, const TCHAR* face);
	void SetDefaultColorFont(int nSize, const TCHAR* face);
	void SetFold(BOOL bFold = TRUE);
	void ToggleFold(long line);
	int GetMarginWidthN(int margin);
	void SetMarginWidthN(int margin, int mask);
	void UpdateLineNumberWidth(void);
	long LineFromPosition(long pos);
	void setCppSyntax();
	BOOL OpenFile(CString lpFileName);
	BOOL FindInFile(const char* keyword, int flags, BOOL isSearchDown);
	void SetText(LPCSTR szText); //@parm pointer to new text //设置新文本
	virtual LPSTR GetText();
	void  GotoPosition(long lPos); //@parm new character position
	void  SetFocus();
	void  Clear();
	//设置高亮颜色
	void SetEditTextColor(DWORD dwStyle, DWORD Cor);
	//添加高亮过滤文本
	BOOL AddEditStr(const char* str);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};


