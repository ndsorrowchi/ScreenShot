#pragma once
#include "afxwin.h"


// CBarDialog �Ի���

class CBarDialog : public CDialog
{
	DECLARE_DYNAMIC(CBarDialog)

public:
	CBarDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBarDialog();

// �Ի�������
	enum { IDD = IDD_BARDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	/*afx_msg void OnStnClickedIcon1();
	afx_msg void OnStnClickedStatic2();
	afx_msg void OnStnClickedText1();
	afx_msg void OnStnClickedText2();*/
	virtual BOOL OnInitDialog();
	CPoint tl;
	CPoint br;
	CButton m_btn1;
	CButton m_btn2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
