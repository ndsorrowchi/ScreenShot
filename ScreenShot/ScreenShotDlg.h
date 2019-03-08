
// ScreenShotDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CScreenShotDlg �Ի���
class CScreenShotDlg : public CDialog
{
// ����
public:
	CScreenShotDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CScreenShotDlg();
// �Ի�������
	enum { IDD = IDD_SCREENSHOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;	
	NOTIFYICONDATA m_ntIcon;//
	//=======================================
	CDC picdc;
	CBitmap bmp;
	CBitmap* pold;
	bool hasdc;
	int pw,ph;
	//=======================================
	CTime lastT;
	int tcount;
	//-----------------------------------
	int enlarge;//no current use	

	bool m_bMin;//if minimized
	bool start;	
	

	int limit;//row1
	BOOL AutoOk;//row2	
	int Act;//row3
	BOOL hasFolder;//row4
	BOOL saveTwo;//row5
	CString path;//row6
	BOOL noQuitMsg;//row7
	BOOL noBubble;//row8
	POINT lt;
	POINT rb;
	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CButton btn1;
	CButton btn2;
	CButton btn3;
	CButton btn4;
	void ShowTrayMessage(LPCTSTR msg);
	void ShowDefaultTrayMsg();
	void OnSaveAsFile();
	void OnFullScreenCapture();
	afx_msg LRESULT OnNotifyIcon(WPARAM, LPARAM);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedSavefile();
	afx_msg void OnBnClickedSavebord();
	afx_msg void OnHelpme();
	afx_msg void OnSet();
	afx_msg void OnStnClickedSet();
	afx_msg void OnStnClickedShowhelp();
	afx_msg void OnEscape();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
