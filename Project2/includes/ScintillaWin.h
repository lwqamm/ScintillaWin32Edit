// Scintilla source code edit control
/** @file ScintillaWin.h
 ** Define functions from ScintillaWin.cxx that can be called from ScintillaDLL.cxx.
 **/
// Copyright 1998-2018 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCINTILLAWIN_H
#define SCINTILLAWIN_H

namespace Scintilla::Internal {

class ScintillaWin;

int ResourcesRelease(bool fromDllMain) noexcept;
int RegisterClasses(void *hInstance) noexcept;
Scintilla::sptr_t DirectFunction(ScintillaWin *sci, UINT iMessage, Scintilla::uptr_t wParam, Scintilla::sptr_t lParam);

}
#endif

/*��д��Сд*/
void toUpper_(char* src);//��д��Сд


#define SET_EDITHIGHLICOLOR            0x1                    //���������ɫ
#define SET_EDITDIGITALCOLOR           0x2                    //������ɫ
#define SET_EDITNOTECOLOR              0x3                    //ע����ɫ
#define SET_EDITLINECOLOR              0x4                    //�к���ɫ
#define GET_EDIT_LEFT                 2156+1000               //��ȡ��߾���
#define SET_EDITFONTCOLOR              0x5                    //�༭��������ɫ

typedef 	struct _HIGHEDIT //�����ؼ���
{
	char    m_str[41] = { 0 };
	int     m_Strlen = 0;
	int     m_Max = 41;
}HIGHEDIT, * PHIGHEDIT;    //Highlight

typedef struct _HIGHEDITARR
{
public:
	_HIGHEDITARR() {}
	~_HIGHEDITARR() { Clear(); }

	//���������С
	int Getlen() { return m_len; };
	//��������ı�
	int Addm_str(PHIGHEDIT  p)
	{
		if (!p || strlen(p->m_str) <= 1 || !p->m_Strlen)
		{
			return m_len;
		}

		if (p)
		{
			char* t_ch = new char[p->m_Strlen + 1];
			memset(t_ch, 0, p->m_Strlen + 1);
			PCHAR p1 = (PCHAR)p->m_str;
			for (size_t i = 0; i < p->m_Strlen; i++)
			{
				if (p1[i] != '\r' || p1[i] != '\n' && p1[i] != '\0' && p1[i] != ' ')
				{
					t_ch[strlen(t_ch)] = p1[i];
				}
			}
			memset(p->m_str, 0, p->m_Strlen + 1);
			memcpy(p->m_str, t_ch, strlen(t_ch));
			p->m_Strlen = strlen(t_ch);

			delete[] t_ch;
		}



		for (size_t i = 0; i < m_len; i++)
		{
			if (strcmp(array[i].m_str, p->m_str) == 0)
			{
				return 0;
			}
		}

		toUpper_(p->m_str);

		if (m_len == 0)
		{
			m_len++;
			array = new HIGHEDIT[m_len];

			if (p == 0 || p->m_Strlen >= 41)
			{
				array[m_len - 1].m_Strlen = 0;
				return m_len;
			}
			memcpy(array[m_len - 1].m_str, p->m_str, strlen(p->m_str));
			array[m_len - 1].m_Strlen = p->m_Strlen;
			return m_len;
		}
		m_len++;
		m_array = new HIGHEDIT[m_len];
		for (int i = 0; i < m_len; i++)
		{
			if (i == m_len - 1)
			{
				if (p == 0 || p->m_Strlen >= 41)
				{
					m_array[i].m_Strlen = 0;
					continue;
				}
				memcpy(m_array[i].m_str, p->m_str, strlen(p->m_str));
				m_array[i].m_Strlen = p->m_Strlen;
				continue;
			}
			memcpy(m_array[i].m_str, array[i].m_str, strlen(array[i].m_str));
			m_array[i].m_Strlen = array[i].m_Strlen;
		}
		delete[] array;
		array = m_array;

		m_array = 0;

		return m_len;
	}
	//���ø�����ɫ
	void SetEditTextColor(DWORD dwStyle, DWORD Cor)
	{
		switch (dwStyle)
		{
		case SET_EDITHIGHLICOLOR:
		{
			if (Cor < 0)
			{
				return;
			}
			m_HighlightColor = Cor;
			return;
		}
		case SET_EDITDIGITALCOLOR:
		{
			if (Cor < 0)
			{
				return;
			}
			m_DigitalColor = Cor;
			return;
		}
		case SET_EDITNOTECOLOR:
		{
			if (Cor < 0)
			{
				return;
			}
			m_NoteColor = Cor;
			return;

		}
		case SET_EDITLINECOLOR:
		{
			if (Cor < 0)
			{
				return;
			}
			m_LineColor = Cor;
			return;

		}
		case SET_EDITFONTCOLOR:
		{
			if (Cor < 0)
			{
				return;
			}
			m_FontColor = Cor;
			return;

		}

		default:
			break;
		}


	}

	//��ȡ�����ı�
	PHIGHEDIT GetEditStr(DWORD dx)
	{
		if (m_len <= 0)
		{
			return 0;
		}

		return &array[dx];
	}
	//��������ı�
	void Clear()//���
	{
		if (m_array)
		{
			delete[] m_array;
			m_len = 0;
			m_array = 0;
		}
		if (array)
		{
			delete[] array;
			m_len = 0;
			array = 0;
		}

	}
private:
	PHIGHEDIT m_array = 0;
	PHIGHEDIT array = 0;
	int       m_len = 0;
public:
	DWORD  m_HighlightColor = RGB(0x0, 0x0, 0xFF);                      //���������ɫ
	DWORD  m_DigitalColor = 128;                                       //������ɫ
	DWORD  m_NoteColor = RGB(0x20, 0x90, 0x20);                    //ע����ɫ
	DWORD  m_FontColor = 0;                                              //�༭��������ɫ
	DWORD  m_LineColor = RGB(0x20, 0x90, 0x20);                     //�к���ɫ
	HWND   m_hWnd = 0;                                                    //��������
	
}HIGHEDITARR, * PHIGHEDITARR, * LPHIGHEDITARR, * NPHIGHEDITARR;

typedef 	struct _MyCEDIT
{
	char    m_str[MAX_PATH * 4] = { 0 };
	int     m_Strlen = 0;
}MyCEDIT, * PMyCEDIT;
extern HIGHEDITARR   Aem;
//��Ӹ��������ı�
BOOL AddEditStr(const char* str);

