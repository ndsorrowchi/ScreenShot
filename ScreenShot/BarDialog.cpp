// BarDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenShot.h"
#include "BarDialog.h"
#include "DCPool.h"
#include <vector>

// CBarDialog 对话框

IMPLEMENT_DYNAMIC(CBarDialog, CDialog)

CBarDialog::CBarDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBarDialog::IDD, pParent)
{
	tl=br=CPoint(0,0);
}

CBarDialog::~CBarDialog()
{
}

void CBarDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
}


BEGIN_MESSAGE_MAP(CBarDialog, CDialog)
	/*ON_STN_CLICKED(IDC_ICON1, &CBarDialog::OnStnClickedIcon1)
	ON_STN_CLICKED(IDC_STATIC2, &CBarDialog::OnStnClickedStatic2)
	ON_STN_CLICKED(IDC_TEXT1, &CBarDialog::OnStnClickedText1)
	ON_STN_CLICKED(IDC_TEXT2, &CBarDialog::OnStnClickedText2)*/
	ON_BN_CLICKED(IDC_BUTTON1, &CBarDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBarDialog::OnBnClickedButton2)
END_MESSAGE_MAP()


// CBarDialog 消息处理程序
/*
void CBarDialog::OnStnClickedIcon1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();//MessageBox(_T(""));
}

void CBarDialog::OnStnClickedStatic2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CBarDialog::OnStnClickedText1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CBarDialog::OnStnClickedText2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
*/
bool in_box(CPoint tl, CPoint br, CPoint p)
{
	bool x = (p.x > tl.x) && (p.x < br.x);
	bool y = (p.y > tl.y) && (p.y < br.y);
	return x && y;
}

bool in_box(RECT r, CPoint p)
{
	return in_box(CPoint(r.left,r.top),CPoint(r.right,r.bottom),p);
}

int in_one_of_boxes(CPoint p, std::vector<RECT> &boxes)
{
	int rs = -1;
	for(unsigned int i=0; i<boxes.size(); i++)
	{
		if(in_box(boxes[i],p))
		{
			rs = i;
			break;
		}
	}
	return rs;
}

BOOL CBarDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	//---------------------------------blend test------------------------------------
	//SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	//SetLayeredWindowAttributes(0,0,LWA_ALPHA);
	//--------------------------------------------------------------------------
	HICON i1,i2;
	i1=AfxGetApp()->LoadIconW(IDI_ICON7);
	i2=AfxGetApp()->LoadIconW(IDI_ICON6);
	m_btn1.SetIcon(i1);
	m_btn2.SetIcon(i2);
	//---------------------------------------------
	int sw=GetSystemMetrics(SM_CXSCREEN);
	int sh=GetSystemMetrics(SM_CYSCREEN);
	int vw=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int vh=GetSystemMetrics(SM_CYVIRTUALSCREEN);
	int px=GetSystemMetrics(SM_XVIRTUALSCREEN);
	int py=GetSystemMetrics(SM_YVIRTUALSCREEN);
	//---------------------------------------------
	CPoint tlwin = tl; // top left
	tlwin.Offset(px,py);
	CPoint brwin = br; // bottom right
	brwin.Offset(px,py);
	CPoint trwin(brwin.x,tlwin.y); // top right
	CPoint blwin(tlwin.x,brwin.y); // bottom left
	HDCPool pool;
	std::vector<RECT> areas = pool.GetRectangles();
	//---------------------------------------------
	CRect r;
	GetWindowRect(&r);
	//---------------------------------------------
	//check points one by one. bottom-right, bottom-left, top-left, top-right
	// at least 2 of the points will satisfy anyways, edge case, the box will be displayed in middle of main screen
	int which;
	if(0 <= (which = in_one_of_boxes(brwin,areas)))
	{
		RECT area = areas[which];
		CPoint p=brwin;
		p.x=brwin.x-r.Width();
		if(p.x<area.left)
		{p.x=area.left;}
		int ny=area.bottom;
		if(p.y+r.Height()>ny)
		{p.y=ny-r.Height();}
		r.OffsetRect(p);
	}
	else if(0 <= (which = in_one_of_boxes(blwin,areas)))
	{
		RECT area = areas[which];
		CPoint p=blwin;
		p.x=blwin.x;
		if(p.x+r.Width()>area.right)
		{p.x=area.right-r.Width();}
		int ny=area.bottom;
		if(p.y+r.Height()>ny)
		{p.y=ny-r.Height();}
		r.OffsetRect(p);
	}
	else if(0 <= (which = in_one_of_boxes(tlwin,areas)))
	{
		RECT area = areas[which];
		CPoint p=tlwin;
		p.x=tlwin.x;
		if(p.x+r.Width()>area.right)
		{p.x=area.right-r.Width();}
		int ny=area.top;
		if(p.y-r.Height()<ny)
		{p.y=ny+r.Height();}
		r.OffsetRect(p);
	}
	else if(0 <= (which = in_one_of_boxes(trwin,areas)))
	{
		RECT area = areas[which];
		CPoint p=brwin;
		p.x=brwin.x-r.Width();
		if(p.x<area.left)
		{p.x=area.left;}
		int ny=area.top;
		if(p.y-r.Height()<ny)
		{p.y=ny+r.Height();}
		r.OffsetRect(p);
	}
	else
	{
		CPoint p(sw/2,sh/2);
		r.OffsetRect(p);
	}
	//---------------------------------------------

	this->MoveWindow(&r,FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBarDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CBarDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
