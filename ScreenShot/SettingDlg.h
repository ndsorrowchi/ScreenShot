#pragma once


// CSettingDlg �Ի���

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDlg();

// �Ի�������
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int limite;
	int enlarge;
	afx_msg void OnBnClickedOk();
	int AutoAct;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	virtual BOOL OnInitDialog();
	BOOL DefaultOK;
	afx_msg void OnBnClickedBrowse();
	CString m_path;
	BOOL hasFolder;
	BOOL m_saveboard;
	BOOL m_noquitmsg;
	BOOL m_nobubblemsg;
	afx_msg void OnBnClickedOpenfolder();
	int m_frmx;
	int m_frmy;
	int m_tox;
	int m_toy;
	afx_msg void OnBnClickedSetarea();
};
