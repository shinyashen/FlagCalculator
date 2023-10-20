
// FlagCalculatorDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FlagCalculator.h"
#include "FlagCalculatorDlg.h"
#include "afxdialogex.h"
#include <string>
#include <bitset>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFlagCalculatorDlg 对话框

int FlagSet[6] = {
	IDC_CFS, IDC_OFS, IDC_PFS, IDC_AFS, IDC_ZFS, IDC_SFS
};

CFlagCalculatorDlg::CFlagCalculatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FLAGCALCULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlagCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, RADIO_1);
	DDX_Control(pDX, IDC_RADIO2, RADIO_2);
	DDX_Control(pDX, IDC_EDIT1, NUM_1);
	DDX_Control(pDX, IDC_EDIT2, NUM_2);
}

BEGIN_MESSAGE_MAP(CFlagCalculatorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_CAL, &CFlagCalculatorDlg::OnBnClickedCal)
	ON_BN_CLICKED(ID_EXIT, &CFlagCalculatorDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_RADIO1, &CFlagCalculatorDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFlagCalculatorDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CFlagCalculatorDlg 消息处理程序

BOOL CFlagCalculatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	DWORD dwStyle = GetStyle();
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle & ~WS_MAXIMIZEBOX);

	// 设置此对话框的图标。
	// 当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetDlgItemText(IDC_RES, TEXT(""));

	// 默认选择8位计算，同时限制输入长度为8位。
	CButton* radio = (CButton*)GetDlgItem(IDC_RADIO1);
	radio->SetCheck(1);

	CEdit* pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT1);
	pEdit->SetLimitText(8);
	pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT2);
	pEdit->SetLimitText(8);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。
// 对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。

void CFlagCalculatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CFlagCalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFlagCalculatorDlg::OnBnClickedCal()
{
	// 判断8位还是16位计算
	bool isEightBits = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();

	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	string num1 = CT2A(str.GetString());
	GetDlgItemText(IDC_EDIT2, str);
	string num2 = CT2A(str.GetString());

	bitset<16> bnum1(string(16 - num1.length(), '0') + num1);
	bitset<16> bnum2(string(16 - num2.length(), '0') + num2);
	bitset<16> result = bnum1.to_ulong() + bnum2.to_ulong();

	// 计算标志

	int bits = 7;
	if (isEightBits) {
		result &= 0xFF;
		str = result.to_string().substr(8, 15).c_str();
	}
	else {
		bits = 15;
		str = result.to_string().c_str();
	}
	bool CF = (bnum1[bits] && bnum2[bits]) || (!result[bits] && (bnum1[bits] || bnum2[bits]));
	bool OF = (bnum1[bits] == bnum2[bits]) && (result[bits] != bnum1[bits]);
	bool PF = !(result.count() % 2);
	bool AF = ((bnum1.to_ulong() & 0xF) + (bnum2.to_ulong() & 0xF)) > 0xF;
	bool ZF = result.none();
	bool SF = result[bits];

	// 输出所有内容
	int FlagDataSet[6] = {CF,OF,PF,AF,ZF,SF};
	GetDlgItem(IDC_RES)->SetWindowTextW(str); // 输出相加结果
	for (int i = 0; i < 6; i++)
	{
		str.Format(_T("%d"), FlagDataSet[i]);
		GetDlgItem(FlagSet[i])->SetWindowTextW(str);
	}
}

void CFlagCalculatorDlg::OnBnClickedExit()
{
	PostMessage(WM_QUIT, 0, 0);
}

//切换位数时清空内容
void CFlagCalculatorDlg::OnBnClickedRadio1()
{
	CEdit* pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT1);
	if (pEdit->GetLimitText() != 8)
	{
		pEdit->SetSel(0, -1);
		pEdit->Clear();
		pEdit->SetLimitText(8);
		pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT2);
		pEdit->SetSel(0, -1);
		pEdit->Clear();
		pEdit->SetLimitText(8);
		SetDlgItemText(IDC_RES, TEXT(""));
		for (int i = 0; i < 6; i++)
			SetDlgItemText(FlagSet[i], TEXT("/"));
	}
}

void CFlagCalculatorDlg::OnBnClickedRadio2()
{
	CEdit* pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT1);
	if (pEdit->GetLimitText() != 16)
	{
		pEdit->SetSel(0, -1);
		pEdit->Clear();
		pEdit->SetLimitText(16);
		pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT2);
		pEdit->SetSel(0, -1);
		pEdit->Clear();
		pEdit->SetLimitText(16);
		SetDlgItemText(IDC_RES, TEXT(""));
		for (int i = 0; i < 6; i++)
			SetDlgItemText(FlagSet[i], TEXT("/"));
	}
}
