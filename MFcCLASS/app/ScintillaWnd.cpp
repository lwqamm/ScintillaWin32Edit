#include "../pch.h"
#include "ScintillaWnd.h"
#include "MyPic.h"
#include "../resource.h"
#include "SciLexer.h"
#include "Scintilla.h"

// ScintillaWnd

IMPLEMENT_DYNAMIC(ScintillaWnd, CWnd)

ScintillaWnd::ScintillaWnd()
{
	m_hSciLexerDll = NULL;
	CString dllpath = GetIniDLLpath();
	if (!file_exist(dllpath+"\\Scintilla64.dll"))
	{
		FreeMyResource(IDR_DLL1,"DLL", dllpath + "\\Scintilla64.dll");
	}
	if (!file_exist(dllpath + "\\Scintilla64.dll"))
	{
		AfxMessageBox(_T("DLL�ļ��ͷ�ʧ�ܣ�"));
		return;
	}
	m_hSciLexerDll = LoadLibrary(_T(dllpath + "\\Scintilla64.dll"));  //H:\VS1\Scintilla\x64\Release\Project2.dll
	//m_hSciLexerDll = LoadLibrary(_T("H:\\VS1\\Scintilla\\x64\\Release\\Project2.dll"));
	if (NULL == m_hSciLexerDll)
	{
		AfxMessageBox(_T("LoadLibrary SciLexer.dll failure..."));
	}
}

ScintillaWnd::~ScintillaWnd()
{
	if (NULL != m_hWnd)
	{
		DestroyWindow();
	}
	if (m_hSciLexerDll != NULL)
	{
		FreeLibrary(m_hSciLexerDll);
	}
}


BEGIN_MESSAGE_MAP(ScintillaWnd, CWnd)
END_MESSAGE_MAP()



// ScintillaWnd ��Ϣ�������




void ScintillaWnd::Init()
{
	
}

void ScintillaWnd::InitScintillaEdit(int nSize, const TCHAR* face)
{
	setCppSyntax();
	Init();
	SetDefaultColorFont(nSize, face);
	UpdateLineNumberWidth();
}

void ScintillaWnd::SetDefaultColorFont(int nSize, const TCHAR* face)
{
	SendMessage(SCI_SETSELFORE, TRUE, RGB(255, 255, 255));
	//ѡ���е���ɫ
	//SendMessage(SCI_SETSELBACK, TRUE, RGB(10, 36, 106));

	//Ĭ���ı���ɫ
	SendMessage(SCI_STYLESETFORE, STYLE_DEFAULT, RGB(0x00, 0x00, 0x00));
	SendMessage(SCI_STYLESETBACK, STYLE_DEFAULT, RGB(0xff, 0xff, 0xff));
	SendMessage(SCI_STYLESETSIZE, STYLE_DEFAULT, nSize);
	SendMessage(SCI_STYLESETFONT, STYLE_DEFAULT, reinterpret_cast<LPARAM>(face));
}

void ScintillaWnd::SetFold(BOOL bFold)
{

}

void ScintillaWnd::ToggleFold(long line)
{
	SendMessage(SCI_TOGGLEFOLD, static_cast<WPARAM>(line), 0);
}

int ScintillaWnd::GetMarginWidthN(int margin)
{
	return SendMessage(SCI_GETMARGINWIDTHN, static_cast<WPARAM>(margin), 0);
}

void ScintillaWnd::SetMarginWidthN(int margin, int mask)
{
	SendMessage(SCI_SETMARGINWIDTHN, static_cast<WPARAM>(margin), static_cast<LPARAM>(mask));
}

void ScintillaWnd::UpdateLineNumberWidth(void)
{
	//start ��ʾ�к�
	long  iLineMarginWidthNow;
	long  iLineMarginWidthFit;
	long iLineNum = SendMessage(SCI_GETLINECOUNT, 0, 0);
	long iLineNumCount = 1;
	while (iLineNum != 0)
	{
		++iLineNumCount;
		iLineNum /= 10;
	}
	iLineMarginWidthNow = SendMessage(SCI_GETMARGINWIDTHN, 0, 0);
	long charWidth = SendMessage(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)("9"));
	iLineMarginWidthFit = charWidth * iLineNumCount;

	if (iLineMarginWidthNow != iLineMarginWidthFit)
	{
		SendMessage(SCI_SETMARGINWIDTHN, 0, iLineMarginWidthFit);
	}
	//end of ��ʾ�к�
}

long ScintillaWnd::LineFromPosition(long pos)
{
	return SendMessage(SCI_LINEFROMPOSITION, static_cast<WPARAM>(pos), 0);
}

//const TCHAR cppKeyWords[] = { "and and_eq asm auto bitand bitor bool break case catch char class compl const const_cast continue default delete do double dynamic_cast else enum explicit export extern false float for friend goto if inline int long mutable namespace new not not_eq operator or or_eq private protected public register reinterpret_cast return short signed sizeof static static_cast struct switch template this throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq BOOL PUSH push" };

void ScintillaWnd::setCppSyntax()
{
	SendMessage(SCI_SETLEXER, SCLEX_CPP);

	//SendMessage(SCI_SETKEYWORDS, 0, (LPARAM)cppKeyWords);//SCI_SETKEYWORDS
	//SendMessage(SCI_SETKEYWORDS, SET_EDITDIGITALCOLOR, (LPARAM)RGB(0xF0, 0x20, 0xF0));//����������ɫ
	//SendMessage(SCI_SETKEYWORDS, 0, (long)_T("and and_eq asm auto bitand bitor bool break case catch char class compl const const_cast continue default delete do double dynamic_cast else enum explicit export extern false float for friend goto if inline int long mutable namespace new not not_eq operator or or_eq private protected public register reinterpret_cast return short signed sizeof static static_cast struct switch template this throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq"));
	// �������ø����﷨Ԫ��ǰ��ɫ
	//SendMessage(SCI_STYLESETFORE, SCE_C_WORD, 0x00FF0000);   //�ؼ���
	//SendMessage(SCI_STYLESETFORE, SCE_C_STRING, 0x001515A3); //�ַ���
	//SendMessage(SCI_STYLESETFORE, SCE_C_CHARACTER, 0x001515A3); //�ַ�
	//SendMessage(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x00808080);//Ԥ���뿪�� 	if (wcscmp(L"int", tbuf.buffer)==0)
	//SendMessage(SCI_STYLESETFORE, SCE_C_COMMENT, 0x00008000);//��ע��
	//SendMessage(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x00008000);//��ע��
	//SendMessage(SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x00008000);//�ĵ�ע�ͣ�/**��ͷ��
	//SendMessage(SCI_STYLESETFORE, SCE_C_NUMBER, 0x000010ff);//����
	//SendMessage(SCI_STYLESETFORE, SCE_C_OPERATOR, 0x0000c0f0);//����
	
}

/////////////////////////////////////
// @mfunc Goto givven character position
// @rvalue void | not used
//
void ScintillaWnd::GotoPosition(
	long lPos) //@parm new character position
{
	SendMessage(SCI_GOTOPOS, lPos, 0);
}

void ScintillaWnd::SetFocus()
{
	SendMessage(SCI_SETFOCUS, 0, 0);
}

void ScintillaWnd::Clear()
{
	SendMessage(SCI_CLEARALL, 0, 0);
}

void ScintillaWnd::SetEditTextColor(DWORD dwStyle, DWORD Cor)
{
	switch (dwStyle)
	{
	case SET_EDITHIGHLICOLOR:
	{
		if (Cor < 0)
		{
			return;
		}
	
		SendMessage(SCI_SETKEYWORDS, SET_EDITHIGHLICOLOR, (LPARAM)Cor);//�������������ɫ
		return;
	}
	case SET_EDITDIGITALCOLOR:
	{
		if (Cor < 0)
		{
			return;
		}
		
		SendMessage(SCI_SETKEYWORDS, SET_EDITDIGITALCOLOR, (LPARAM)Cor);//����������ɫ

		return;
	}
	case SET_EDITNOTECOLOR:
	{
		if (Cor < 0)
		{
			return;
		}
		SendMessage(SCI_SETKEYWORDS, SET_EDITDIGITALCOLOR, (LPARAM)Cor);//����ע����ɫ
		return;

	}
	default:
		break;
	}
}

BOOL ScintillaWnd::AddEditStr(const char* str)
{
 return	SendMessage(SCI_SETKEYWORDS, 0, (LPARAM)str);//��Ӹ�������
}

void ScintillaWnd::SetText(
	LPCSTR szText) //@parm pointer to new text
{
	LRESULT lResult = 0;
	if (szText != NULL)
	{
		lResult = SendMessage(SCI_SETTEXT, 0, (LPARAM)szText);
		DWORD len = strlen(szText);
		CString str = szText;
		DWORD End = 0;
		for (size_t i = 0; i < len; i++)
		{
			if (str.Mid(i,2)=="\r\n")
			{
				End = i+2;
			}

		}
		GotoPosition(0);
		SetFocus();//SCI_SETFOCUS
	}
	
}

LPSTR ScintillaWnd::GetText()
{
	long lLen = SendMessage(SCI_GETLENGTH, 0, 0) + 1;
	if (lLen > 0)
	{
		TCHAR* pReturn = new TCHAR[lLen];
		if (pReturn != NULL)
		{
			*pReturn = '\0';
			SendMessage(SCI_GETTEXT, lLen, (long)pReturn);
			return pReturn;
		}
	}
	return NULL;
}


BOOL ScintillaWnd::OpenFile(CString lpFileName)
{
	CStdioFile stdFile(lpFileName, CFile::modeRead | CFile::typeBinary);
	ULONG nFileLength = static_cast<ULONG>(stdFile.GetLength());

	char* pBuffer;
	pBuffer = new char[nFileLength + 1];

	nFileLength = stdFile.Read(pBuffer, nFileLength);

	stdFile.Close();

	if (SendMessage(SCI_GETREADONLY, 0, 0))
	{
		SendMessage(SCI_SETREADONLY, FALSE, 0);
	}

	SendMessage(SCI_CANCEL, 0, 0);
	SendMessage(SCI_SETUNDOCOLLECTION, 0, 0);
	SendMessage(SCI_EMPTYUNDOBUFFER, 0, 0);

	//����ı�û��ֻ�����ԣ�������������֡�
	SendMessage(SCI_CLEARALL, 0, 0);

	//����������ɾ����ǣ���markerNumber=-1����ɾ�����б�ǡ�
	SendMessage(SCI_MARKERDELETEALL, (WPARAM)-1, 0);

	SendMessage(SCI_ADDTEXT, nFileLength, (LPARAM)pBuffer); //SetText

	SendMessage(SCI_SETUNDOCOLLECTION, 1, 0);
	SendMessage(EM_EMPTYUNDOBUFFER, 0, 0);
	SendMessage(SCI_SETSAVEPOINT, 0, 0);
	SendMessage(SCI_GOTOPOS, 0, 0);
	SendMessage(SCI_CHOOSECARETX, 0, 0);

	UpdateLineNumberWidth();

	delete[] pBuffer;

	return TRUE;
}

BOOL ScintillaWnd::FindInFile(const char* keyword, int flags, BOOL isSearchDown)
{
	int pos(-1);
	if (isSearchDown)
	{
		int curAnchorPos = SendMessage(SCI_GETSELECTIONEND, 0, 0); //��ǰѡ���ı��Ľ���λ��
		SendMessage(SCI_SETCURRENTPOS, (WPARAM)curAnchorPos);
		SendMessage(SCI_SEARCHANCHOR, 0, 0);
		pos = SendMessage(SCI_SEARCHNEXT, flags, (LPARAM)keyword); //Ĭ����������
	}
	else
	{
		int curAnchorPos = SendMessage(SCI_GETSELECTIONSTART, 0, 0); //��ǰѡ���ı��Ľ���λ��
		SendMessage(SCI_SETCURRENTPOS, (WPARAM)curAnchorPos); //���õ�ǰcursorλ��
		SendMessage(SCI_SEARCHANCHOR, 0, 0);
		pos = SendMessage(SCI_SEARCHPREV, flags, (LPARAM)keyword); //������������
	}

	if (-1 != pos) //�ҵ���
	{
		SendMessage(SCI_TARGETFROMSELECTION, 0, 0);
		SendMessage(SCI_SCROLLCARET, 0, 0); //�ƶ�view����Ӧ��λ��
		return TRUE;
	}
	else //û���ҵ���������Ƶ��ĵ�ͷ
	{
		MessageBox(_T("�ѵ��ĵ���β��û���ҵ���"));
		SendMessage(SCI_SETSELECTIONSTART, 0, 0);
		SendMessage(SCI_SETSELECTIONEND, 0, 0);
		return FALSE;
	}

	return 0;
}

BOOL ScintillaWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

