// ShotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenShot.h"
#include "ShotDlg.h"
#include "BarDialog.h"

using namespace std;
// CShotDlg 对话框

IMPLEMENT_DYNAMIC(CShotDlg, CDialog)

CShotDlg::CShotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShotDlg::IDD, pParent)
{
	start=captured=false;AutoOk=FALSE;
	LIMITE = 5;

	CDC *pdc=GetDesktopWindow()->GetDC();
	dc.CreateCompatibleDC(pdc);

	
	//int nx=GetSystemMetrics(SM_CXSCREEN);
	//int ny=GetSystemMetrics(SM_CYSCREEN);
	int nx=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int ny=GetSystemMetrics(SM_CYVIRTUALSCREEN);

	int px=GetSystemMetrics(SM_XVIRTUALSCREEN);
	int py=GetSystemMetrics(SM_YVIRTUALSCREEN);

	bmp.CreateCompatibleBitmap(pdc,nx,ny);
	pold=dc.SelectObject(&bmp);
	dc.BitBlt(0,0,nx,ny,pdc,px,py,SRCCOPY);
	pbuffbit=NULL;buff=false;
	
	// 
	BLENDFUNCTION blend;  
	memset( &blend, 0, sizeof( blend) ); 
	blend.BlendOp= AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.AlphaFormat = 0;
	blend.SourceConstantAlpha = 160; // 

	CDC TempDC;
	TempDC.CreateCompatibleDC(pdc);
	CBitmap TempBitmap;
	TempBitmap.CreateCompatibleBitmap(pdc,nx,ny);
	CBitmap* temp=TempDC.SelectObject(&TempBitmap);
	TempDC.FillSolidRect(0,0,nx,ny,RGB(0,0,0));

	blenddc.CreateCompatibleDC(pdc);
	blendbmp.CreateCompatibleBitmap(pdc,nx,ny);
	pblendbit=blenddc.SelectObject(&blendbmp);
	blenddc.BitBlt(0,0,nx,ny,&dc,0,0,SRCCOPY);
	blenddc.AlphaBlend(0,0,nx,ny,&TempDC,0,0,nx,ny,blend);
		
	TempDC.SelectObject(temp);
	TempBitmap.DeleteObject();
	TempDC.DeleteDC();//if(X==TRUE){AfxMessageBox(L"y");}
	//logger.open("debug_logs.txt",ios::out|ios::app);
}

CShotDlg::~CShotDlg()
{
	dc.SelectObject(pold);
	bmp.DeleteObject();
	dc.DeleteDC();
	blenddc.SelectObject(pblendbit);
	blendbmp.DeleteObject();
	blenddc.DeleteDC();
	//logger.close();
}

void CShotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShotDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CShotDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CShotDlg::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CShotDlg 消息处理程序

void CShotDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void CShotDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	captured=false;
	OnCancel();
}

BOOL CShotDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//int nx = GetSystemMetrics(SM_CXSCREEN);
	//int ny = GetSystemMetrics(SM_CYSCREEN);
	int nx=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int ny=GetSystemMetrics(SM_CYVIRTUALSCREEN);
	BITMAP bm;
	bmp.GetBitmap(&bm);
	BOOL X=pDC->BitBlt(0,0,nx,ny,&blenddc,0,0,SRCCOPY);
	//Client area 0,0 should be window pos px,py, so only do px. py when dealing with the real dc
	return X;	
///	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CShotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//this->ShowWindow(SW_SHOW);
	//int nx=GetSystemMetrics(SM_CXSCREEN);
	//int ny=GetSystemMetrics(SM_CYSCREEN);

	int nx = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int ny = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	int px = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int py = GetSystemMetrics(SM_YVIRTUALSCREEN);
	SetWindowPos(&wndTopMost,px,py,nx,ny,SWP_SHOWWINDOW);

	//::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void SortOfPoints(CPoint &org,CPoint &des)
{
	CPoint opt=org;CPoint point=des;
	org.x=opt.x<=point.x?opt.x:point.x;
	org.y=opt.y<=point.y?opt.y:point.y;
	des.x=opt.x>point.x?opt.x:point.x;
	des.y=opt.y>point.y?opt.y:point.y;
}
void CShotDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(start)
	{
		CDC *pDC=GetDC();
		CPen pen(PS_SOLID,1,RGB(0,255,255));
		CPen *oldp=memdc.SelectObject(&pen);
		memdc.SelectStockObject(NULL_BRUSH);

		//int nx=GetSystemMetrics(SM_CXSCREEN);
		//int ny=GetSystemMetrics(SM_CYSCREEN);

		int nx=GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int ny=GetSystemMetrics(SM_CYVIRTUALSCREEN);

		int px = GetSystemMetrics(SM_XVIRTUALSCREEN);
		int py = GetSystemMetrics(SM_YVIRTUALSCREEN);

		if(buff)
		{
			CPoint org(opt),des(point);
			memdc.BitBlt(0,0,nx,ny,&blenddc,0,0,SRCCOPY);

			SortOfPoints(org,des);
			memdc.BitBlt(org.x,org.y,des.x-org.x,des.y-org.y,&dc,org.x,org.y,SRCCOPY);
			memdc.Rectangle(org.x-1,org.y-1,des.x+1,des.y+1);
			pDC->BitBlt(0,0,nx,ny,&memdc,0,0,SRCCOPY);//pDC->BitBlt(px,py,nx,ny,&memdc,0,0,SRCCOPY);
		}
		lastmove=point;

		memdc.SelectObject(oldp);

		ReleaseDC(pDC);		

	}
	CDialog::OnMouseMove(nFlags, point);
}

void CShotDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!start)
	{
		opt=point;lastmove=point;start=true;captured=false;


		CDC *pDC=GetDC();
		
		if(!buff)
		{
			buff=true;
			memdc.CreateCompatibleDC(&dc);
			//int nx=GetSystemMetrics(SM_CXSCREEN);
			//int ny=GetSystemMetrics(SM_CYSCREEN);

			int px=GetSystemMetrics(SM_XVIRTUALSCREEN);
			int py=GetSystemMetrics(SM_YVIRTUALSCREEN);

			int nx=GetSystemMetrics(SM_CXVIRTUALSCREEN);
			int ny=GetSystemMetrics(SM_CYVIRTUALSCREEN);
			
			//logger << "OnLButtonDown() : " << "Virtual Screen Pos : (" << px << ", " << py << ")\n";
			//logger << "OnLButtonDown() : " << "Virtual Screen Size : (" << nx << ", " << ny << ")\n";

			membitmap.CreateCompatibleBitmap(&dc,nx,ny);
			pbuffbit=memdc.SelectObject(&membitmap);
			//memdc.BitBlt(0,0,nx,ny,pDC,0,0,SRCCOPY);
		}
		ReleaseDC(pDC);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CShotDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(start)
	{
		start=false;

		int ox,oy,dx,dy;
		ox=(opt.x<point.x)?opt.x:point.x;
		oy=(opt.y<point.y)?opt.y:point.y;
		dx=(opt.x>point.x)?opt.x:point.x;
		dy=(opt.y>point.y)?opt.y:point.y;
		
		int nresponse=0;
		if(TRUE!=AutoOk)
		{
			CBarDialog dlg;
			dlg.tl=CPoint(ox,oy);
			dlg.br=CPoint(dx,dy);
			nresponse=dlg.DoModal();
		}
		else
		{nresponse=IDOK;}
		CDC *pDC=GetDC();

		if(buff)
		{
			//int nx=GetSystemMetrics(SM_CXSCREEN);
			//int ny=GetSystemMetrics(SM_CYSCREEN);
			int nx=GetSystemMetrics(SM_CXVIRTUALSCREEN);
			int ny=GetSystemMetrics(SM_CYVIRTUALSCREEN);

			int px=GetSystemMetrics(SM_XVIRTUALSCREEN);
			int py=GetSystemMetrics(SM_YVIRTUALSCREEN);
			
			//logger << "OnLButtonUp() : " << "Virtual Screen Pos : (" << px << ", " << py << ")\n";
			//logger << "OnLButtonUp() : " << "Virtual Screen Size : (" << nx << ", " << ny << ")\n";
			
			//logger << "OnLButtonUp() : " << "Area TopLeft : (" << ox << ", " << oy << ")\n";
			//logger << "OnLButtonUp() : " << "Area BottomRight : (" << dx << ", " << dy << ")\n";

			pDC->BitBlt(0,0,nx,ny,&dc,0,0,SRCCOPY); //pDC->BitBlt(px,py,nx,ny,&dc,0,0,SRCCOPY);
			buff=false;
			memdc.SelectObject(pbuffbit);
			membitmap.DeleteObject();
			memdc.DeleteDC();
		}
		lastmove=point;
	
		if(dx-ox>LIMITE&&dy-oy>LIMITE&&nresponse==IDOK)
		{
			captured=true;ReleaseDC(pDC);	
			OnOK();
		}
		else
		{captured=false;ReleaseDC(pDC);OnCancel();}				
		
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CShotDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	start=false;
	captured=false;
	OnCancel();
	CDialog::OnRButtonDown(nFlags, point);
}


BOOL CShotDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	return TRUE;
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
