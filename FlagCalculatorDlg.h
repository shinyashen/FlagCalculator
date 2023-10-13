
// FlagCalculatorDlg.h: 头文件
//

#pragma once
#include "CEditEx.h"


// CFlagCalculatorDlg 对话框
class CFlagCalculatorDlg : public CDialogEx
{
// 构造
public:
	CFlagCalculatorDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLAGCALCULATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCal();
	CButton RADIO_1;
	CButton RADIO_2;
	afx_msg void OnBnClickedExit();
	CEditEx NUM_1;
	CEditEx NUM_2;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
