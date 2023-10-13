// CEditEx.cpp: 实现文件
//

#include "pch.h"
#include "FlagCalculator.h"
#include "CEditEx.h"


// CEditEx

IMPLEMENT_DYNAMIC(CEditEx, CEdit)

CEditEx::CEditEx()
{

}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_CHAR()
    ON_WM_PASTE()
END_MESSAGE_MAP()



// CEditEx 消息处理程序

void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 输入限制，只能输入0和1和退格键
	if ((nChar == '0' || nChar == '1') || nChar == 0x08)
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CEditEx::OnPaste()
{
    // 复制限制，只接受0和1字符输入
    if (OpenClipboard())
    {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData != NULL)
        {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText)
            {
                CString strClipText(pszText);
                GlobalUnlock(hData);

                CString strFiltered;
                for (int i = 0; i < strClipText.GetLength(); ++i)
                    // 这里过滤，只允许0和1
                    if (strClipText[i] == '0' || strClipText[i] == '1')
                        strFiltered.AppendChar(strClipText[i]);

                if (!strFiltered.IsEmpty())
                    ReplaceSel(strFiltered, TRUE);  // 粘贴过滤后的文本
            }
        }
        CloseClipboard();
    }
}

BOOL CEditEx::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        UINT nCode = pMsg->wParam;
        if (::GetFocus() == m_hWnd && (GetKeyState(VK_CONTROL) & 0xFF00) == 0xFF00)
        {
            if (nCode == 'A' || nCode == 'a')
            {
                this->SetSel(0, -1);
                return true;
            } // 全选
            if (nCode == 'C' || nCode == 'c')
            {
                this->Copy();
                return true;
            } // 复制
            if (nCode == 'X' || nCode == 'x')
            {
                this->Cut();
                return true;
            } // 剪切
            if (nCode == 'V' || nCode == 'v')
            {
                this->Paste();
                return true;
            } // 粘贴
            if (nCode == 'Z' || nCode == 'z')
            {
                this->Undo();
                return true;
            } // 撤销
        }
    }
	return CEdit::PreTranslateMessage(pMsg);
}

void CEditEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	SetSel(0, -1);
}