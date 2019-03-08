#pragma once

#include "DCPool.h"
// CShotDlg 对话框
#include <fstream>

class CShotDlg : public CDialog
{
	DECLARE_DYNAMIC(CShotDlg)

public:
	CShotDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShotDlg();
	
// 对话框数据
	enum { IDD = IDD_SHOT };
protected:
	CDC dc;// original cdc
	CBitmap bmp;
	CBitmap* pold;

	
	CDC memdc;//doublebuff cdc
	CBitmap membitmap;
	CBitmap *pbuffbit;
	bool buff;
	
	CDC blenddc;
	CBitmap blendbmp;
	CBitmap* pblendbit;

	CPoint opt;
	CPoint lastmove;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	CDC* GetBmpDC(){return &dc;}
	bool start;//bool
	bool captured;//	bool 
	int LIMITE;
	BOOL AutoOk;
	CPoint GetLt(){int ox=(opt.x<lastmove.x)?opt.x:lastmove.x;int oy=(opt.y<lastmove.y)?opt.y:lastmove.y;return CPoint(ox,oy);}
	CPoint GetRb(){int dx=(opt.x>lastmove.x)?opt.x:lastmove.x;int dy=(opt.y>lastmove.y)?opt.y:lastmove.y;return CPoint(dx,dy);}

	HDCPool m_hdcpool;

	std::fstream logger;

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
