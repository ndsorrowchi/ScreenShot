
// ScreenShotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenShot.h"
#include "ScreenShotDlg.h"
#include "locale.h"
#include "ShotDlg.h"
#include "SettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MY_WM_NOTIFYICON (WM_USER+1001)

#define WM_MYHOTMSG_1 WM_USER+1002
#define WM_MYHOTMSG_2 WM_USER+1003

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



//

CString GetAppPath()
{
	 CString strAppPath;  // ������
	 TCHAR szModuleFileName[MAX_PATH]; // ȫ·����
	 TCHAR drive[_MAX_DRIVE];  // �̷����ƣ�����˵C�̣�D��
	 TCHAR dir[_MAX_DIR]; // Ŀ¼
	 TCHAR fname[_MAX_FNAME];  // ��������
	 TCHAR ext[_MAX_EXT]; //��׺��һ��Ϊexe������dll
	 if (NULL == GetModuleFileName(NULL, szModuleFileName, MAX_PATH)) //��õ�ǰ���̵��ļ�·��
	 {
	  return _T("");
	 }
	 _tsplitpath_s( szModuleFileName, drive, dir, fname, ext );  //�ָ��·�����õ��̷���Ŀ¼���ļ�������׺��
	 strAppPath = drive;
	 strAppPath += dir;
	 return strAppPath;
}
// CScreenShotDlg �Ի���




CScreenShotDlg::CScreenShotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenShotDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);//(IDR_MAINFRAME);
	start=hasdc=false;tcount=pw=ph=0;limit=5;Act=enlarge=0;AutoOk=FALSE;hasFolder=noQuitMsg=noBubble=saveTwo=FALSE;
	rb.x=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rb.y=GetSystemMetrics(SM_CYVIRTUALSCREEN);
	//rb.x=GetSystemMetrics(SM_CXSCREEN);
	//rb.y=GetSystemMetrics(SM_CYSCREEN);

	lt.x=GetSystemMetrics(SM_XVIRTUALSCREEN);
	lt.y=GetSystemMetrics(SM_YVIRTUALSCREEN);;
}

CScreenShotDlg::~CScreenShotDlg()
{
	if(hasdc)
	{
		picdc.SelectObject(pold);
		bmp.DeleteObject();
		picdc.DeleteDC();
	}
}

void CScreenShotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START, btn1);
	DDX_Control(pDX, IDC_SAVEFILE, btn2);
	DDX_Control(pDX, IDC_SAVEBORD, btn3);
	DDX_Control(pDX, IDCANCEL, btn4);
//	DDX_Control(pDX, IDC_VIEW, view);
}

BEGIN_MESSAGE_MAP(CScreenShotDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CScreenShotDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CScreenShotDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_START, &CScreenShotDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_SAVEFILE, &CScreenShotDlg::OnBnClickedSavefile)
	ON_BN_CLICKED(IDC_SAVEBORD, &CScreenShotDlg::OnBnClickedSavebord)
	ON_MESSAGE(MY_WM_NOTIFYICON, &CScreenShotDlg::OnNotifyIcon)
	ON_COMMAND(ID_HELPME, &CScreenShotDlg::OnHelpme)
	ON_COMMAND(ID_SET, &CScreenShotDlg::OnSet)
	ON_STN_CLICKED(IDC_Set, &CScreenShotDlg::OnStnClickedSet)
	ON_STN_CLICKED(IDC_ShowHelp, &CScreenShotDlg::OnStnClickedShowhelp)
	ON_COMMAND(ID_ESCAPE, &CScreenShotDlg::OnEscape)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CScreenShotDlg ��Ϣ�������

BOOL CScreenShotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//-------Ѱ�������ļ�-------------------------------------
//	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );    
	//setlocale( LC_CTYPE, "chs" );

	CFileFind cff;
	CString setfile=GetAppPath()+L"userprofile.dat";
	if(cff.FindFile(setfile)==TRUE)
	{
		WCHAR ppath[MAX_PATH];
		CFile f(setfile,CFile::modeRead);
		f.Read(&limit,sizeof(limit));
		f.Read(&AutoOk,sizeof(AutoOk));
		f.Read(&Act,sizeof(Act));
		f.Read(&hasFolder,sizeof(hasFolder));
		f.Read(&saveTwo,sizeof(saveTwo));
		f.Read(ppath,MAX_PATH*sizeof(WCHAR));
		f.Read(&noQuitMsg,sizeof(noQuitMsg));
		f.Read(&noBubble,sizeof(noBubble));
		f.Close();
		path=ppath;
	}
	else
	{
		CString apppath=GetAppPath();
		CString deftfolder=apppath+L"captured images";
		if(!PathIsDirectory(deftfolder))   
		{   
		   if(!CreateDirectory(deftfolder,NULL))
		   {   
			    CString strMsg;
				strMsg.Format(L"Creating default path\"%s\"failed!\nPlease manually set default folder in settings.",deftfolder);
				MessageBox(strMsg,L"Error-_-||",MB_OK|MB_ICONERROR);
		   }   
		}  
	}
	//btn images
	HICON btn1ic = AfxGetApp()->LoadIcon(IDI_ICON3);
	btn1.SetIcon(btn1ic);
	HICON btn2ic = AfxGetApp()->LoadIcon(IDI_ICON5);
	btn2.SetIcon(btn2ic);
	HICON btn3ic = AfxGetApp()->LoadIcon(IDI_ICON1);
	btn3.SetIcon(btn3ic);
	HICON btn4ic = AfxGetApp()->LoadIcon(IDI_ICON4);
	btn4.SetIcon(btn4ic);
	//btn1.SetButtonStyle CButton

	btn2.EnableWindow(FALSE);
	btn3.EnableWindow(FALSE);
	//
	m_ntIcon.cbSize = sizeof(NOTIFYICONDATA);                            //�ýṹ������Ĵ�С
    m_ntIcon.hIcon = m_hIcon;//AfxGetApp()->LoadIconW(IDI_ICON4);  //ͼ�꣬ͨ����ԴID�õ�
    m_ntIcon.hWnd = this->m_hWnd;                                                 //��������ͼ��֪ͨ��Ϣ�Ĵ��ھ��
	WCHAR atip[128] = L"ScreenShot Snipping tool";//���������ʱ��ʾ����ʾ
	//strcpy(m_ntIcon.szTip, 128, atip);
	memcpy(m_ntIcon.szTip,atip,128*sizeof(WCHAR));
    m_ntIcon.uCallbackMessage = MY_WM_NOTIFYICON;                //Ӧ�ó��������ϢID��
    m_ntIcon.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;               //ͼ������ԣ����ó�ԱuCallbackMessage��hIcon��szTip��Ч
 
	::Shell_NotifyIconW(NIM_ADD, &m_ntIcon);                                 //��ϵͳ֪ͨ�����������ͼ��
	
	HICON pic1 = AfxGetApp()->LoadIcon(IDI_ICON9);
	(GetDlgItem(IDC_Set))->SetIcon(pic1,FALSE);
	HICON pic2 = AfxGetApp()->LoadIcon(IDI_ICON8);
	(GetDlgItem(IDC_ShowHelp))->SetIcon(pic2,FALSE);
	//hotkey
	::RegisterHotKey( m_hWnd, WM_MYHOTMSG_1, 0,VK_F2);
	::RegisterHotKey( m_hWnd, WM_MYHOTMSG_2, 0,VK_F3);
	//--------
	lastT=CTime::GetCurrentTime();

//	setlocale( LC_CTYPE, old_locale );    
//	free( old_locale );//
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CScreenShotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID==SC_MINIMIZE)
	{
		this->ShowWindow(SW_MINIMIZE);
		ShowDefaultTrayMsg();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScreenShotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CScreenShotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CScreenShotDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(TRUE==noQuitMsg||IDYES==MessageBox(_T("Click NO to minimize to tray"),_T("Confirm to exit?"),MB_ICONQUESTION|MB_YESNO))
	{
		OnCancel();
	}
	else
	{
		this->ShowWindow(SW_MINIMIZE);
		this->ShowWindow(SW_HIDE);
		ShowDefaultTrayMsg();
	}
}

void CScreenShotDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();
	OnBnClickedStart();
}

void CScreenShotDlg::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	start=true;
	bool noRestore=false;
	if((this->IsWindowVisible()==FALSE||this->IsIconic()==TRUE)&&Act!=0)
		noRestore=true;
	if(!noRestore)
	{
		this->ShowWindow(SW_MINIMIZE);
		this->ShowWindow(SW_HIDE);
	}
	//----------------------------------------------------------
	if(hasdc)
	{
		picdc.SelectObject(pold);
		bmp.DeleteObject();
		picdc.DeleteDC();
	}
	pw=ph=0;
	
	CShotDlg g;
	g.LIMITE=limit;
	g.AutoOk=AutoOk;
	g.DoModal();	
	if(g.captured)
	{
		CPoint lt=g.GetLt();CPoint rb=g.GetRb();
		lt.Offset(-enlarge,-enlarge);
		rb.Offset(enlarge,enlarge);
		int w=rb.x-lt.x;
		int h=rb.y-lt.y;
		pw=w;ph=h;
		CDC* pDC=g.GetBmpDC();
		//----------------------------------------------------------
		picdc.CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC,rb.x-lt.x,rb.y-lt.y);
		pold=picdc.SelectObject(&bmp);
		picdc.BitBlt(0,0,rb.x-lt.x,rb.y-lt.y,g.GetBmpDC(),lt.x,lt.y,SRCCOPY);

		hasdc=true;
		btn2.EnableWindow(TRUE);
		btn3.EnableWindow(TRUE);
		
		if(Act==1)
		{OnSaveAsFile();}
		else if(Act==2)
		{OnBnClickedSavebord();}
		else
		{;}
	}
	else
	{
		hasdc=false;
		btn2.EnableWindow(FALSE);
		btn3.EnableWindow(FALSE);
		//MessageBox(_T("��׽���ɹ�������ע����Ļ�����С\n�����Ѿ�ȡ����׽��"),_T("����"),MB_ICONWARNING|MB_OK);
		MessageBox(_T("Snipping failed��Selected area too small to be identified as valid\nor,you have cancelled snipping."),_T("Snipping Failed"),MB_ICONWARNING|MB_OK);

	}
	//----------------------------------------------------------
	if(!noRestore)
	{
		this->ShowWindow(SW_SHOW);
		this->ShowWindow(SW_RESTORE);
	}
	start=false;
}
void CScreenShotDlg::OnSaveAsFile()
{
	int nwidth=pw;
	int nheight=ph;
	int bpp=24;
	if(hasFolder==FALSE)
	{
		CFileDialog g(FALSE);
		g.m_ofn.lpstrTitle=L"Save snipping as Image";
		g.m_ofn.lpstrDefExt=L"JPG";
		g.m_ofn.lpstrFilter=L"JPG(*.jpg)\0*.jpg\0BMP(*.bmp)\0*.bmp\0GIF(*.gif)\0*.gif\0PNG(*.png)\0*.png\0\0";
		if(IDOK==g.DoModal())
		{
			CImage img;
			img.Create(nwidth,nheight,bpp);
			BOOL X=::BitBlt(img.GetDC(),0,0,nwidth,nheight,picdc.m_hDC,0,0,SRCCOPY);
			HRESULT Y=img.Save(g.GetPathName());
			img.ReleaseDC();
			if(X==TRUE && Y>=0)
			{ShowTrayMessage(_T("Successfully Saved."));}
			else
			{ShowTrayMessage(_T("Failed to save."));}
			}
	}
	else
	{
		CTime time;
		time = CTime::GetCurrentTime();
		if(time==lastT){tcount++;}
		else
		{tcount=0;}
		lastT=time;
		CString sDate,sTime;
		sDate = time.Format(L"%y-%m-%d"); //ex: 12-10-98
		sTime = time.Format(L"%H-%M-%S"); //ex: 9:12:02
		CString file;
		file.Format(L"\\IMG%s-%s%d.jpg",sDate,sTime,tcount);
		CImage img;
		img.Create(nwidth,nheight,bpp);
		BOOL X=::BitBlt(img.GetDC(),0,0,nwidth,nheight,picdc.m_hDC,0,0,SRCCOPY);
		HRESULT Y=img.Save(path+file);
		img.ReleaseDC();
	}
	if(TRUE==saveTwo)
	{
		OnBnClickedSavebord();
	}

}
void CScreenShotDlg::OnBnClickedSavefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nwidth=pw;
	int nheight=ph;
	int bpp=24;
	CFileDialog g(FALSE);
	g.m_ofn.lpstrTitle=L"Save snipping As Image";
	g.m_ofn.lpstrDefExt=L"JPG";
	g.m_ofn.lpstrFilter=L"JPG(*.jpg)\0*.jpg\0BMP(*.bmp)\0*.bmp\0GIF(*.gif)\0*.gif\0PNG(*.png)\0*.png\0\0";
	if(IDOK==g.DoModal())
	{
		CImage img;
		img.Create(nwidth,nheight,bpp);
		BOOL X=::BitBlt(img.GetDC(),0,0,nwidth,nheight,picdc.m_hDC,0,0,SRCCOPY);
		HRESULT Y=img.Save(g.GetPathName());
		img.ReleaseDC();
		if(X==TRUE && Y>=0)
		{ShowTrayMessage(_T("Successfully saved."));}
		else
		{ShowTrayMessage(_T("Failed to save."));}
	}
}

void CScreenShotDlg::OnBnClickedSavebord()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(hasdc)
	{
		if (OpenClipboard()) 
		{
			EmptyClipboard();//��ռ�����
			SetClipboardData(CF_BITMAP, (HBITMAP)bmp.m_hObject);
			CloseClipboard();//�رռ�����
			ShowTrayMessage(_T("Successfully saved."));
		}
	}
}


LRESULT CScreenShotDlg::OnNotifyIcon(WPARAM wparam, LPARAM lparam)
{
   if(lparam == WM_LBUTTONDOWN)
	{
		if(this->IsWindowVisible() == FALSE||this->IsIconic()==TRUE)
         {
             this->ShowWindow(SW_SHOW);
             this->ShowWindow(SW_RESTORE);
              //����ò��ֻ��д����������ܱ�֤�ָ����ں󣬸ô��ڴ��ڻ״̬������ǰ�棩
            // m_bMin = false;
         }
         else
         {
              this->ShowWindow(SW_MINIMIZE);
			  this->ShowWindow(SW_HIDE);
			  ShowDefaultTrayMsg();
              //m_bMin = true;
         }  
                         //������Ӷ�����������Ĵ������崦�����4��
    }
    else if(lparam == WM_RBUTTONDOWN)
    {
		CMenu popMenu;
        popMenu.LoadMenuW(IDR_MENU1);			//IDR_NPOP����ResourceView�д������༭��һ���˵�
        CMenu* pmenu = popMenu.GetSubMenu(0);   //�����Ĳ˵�ʵ������IDR_MENU_POPUP�˵���ĳ����Ӳ˵��������ǵ�һ��
        CPoint pos;
        GetCursorPos(&pos);            //�����˵���λ�ã�����������ĵ�ǰλ��
        //��ʾ�ò˵�����һ������������ֵ�ֱ��ʾ�������ұ���ʾ����Ӧ����һ�
        pmenu->TrackPopupMenu(TPM_RIGHTALIGN|TPM_RIGHTBUTTON, pos.x, pos.y, AfxGetMainWnd(), 0);  
                        //������Ӷ�����Ҽ�����Ĵ������崦�����5��
    }
    return 0;
}
void CScreenShotDlg::OnHelpme()
{
	// TODO: �ڴ���������������
	MessageBox(_T("1. Click 'Start New' or press F2 to start a rectangular selecting snipping.\n2.Press F3 to start a fixed area snipping (The area can be set in Settings).\n3.Save the snipping as an image or copy it to clipboard\nThe settings can be accessed from the bottom right corner.\n"),_T("Instruction"),MB_ICONINFORMATION|MB_OK);
}

void CScreenShotDlg::OnSet()
{
	// TODO: �ڴ���������������
	CSettingDlg g;
	g.limite=limit;
	g.DefaultOK=AutoOk;
	g.AutoAct=Act;
	g.hasFolder=hasFolder;
	g.m_path=path;
	//g.enlarge=enlarge;
	g.m_saveboard=saveTwo;
	g.m_noquitmsg=noQuitMsg;	
	g.m_nobubblemsg=noBubble;
	
	g.m_frmx=lt.x;
	g.m_frmy=lt.y;
	g.m_tox=rb.x;
	g.m_toy=rb.y;
	if(IDOK==g.DoModal())
	{
		limit=g.limite;
		AutoOk=g.DefaultOK;		//enlarge=g.enlarge;
		Act=g.AutoAct;
		hasFolder=g.hasFolder;
		path=g.m_path;
		saveTwo=g.m_saveboard;
		noQuitMsg=g.m_noquitmsg;		
		noBubble=g.m_nobubblemsg;
		//
		lt.x=g.m_frmx;
		lt.y=g.m_frmy;
		rb.x=g.m_tox;
		rb.y=g.m_toy;
	}
}

void CScreenShotDlg::OnStnClickedSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnSet();
}

void CScreenShotDlg::OnStnClickedShowhelp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnHelpme();
}

void CScreenShotDlg::OnEscape()
{
	// TODO: �ڴ���������������
	OnCancel();
	//::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon); 
}

void CScreenShotDlg::OnDestroy()
{

	BOOL bRet1 = UnregisterHotKey( GetSafeHwnd(), WM_MYHOTMSG_1 );
	BOOL bRet2 = UnregisterHotKey( GetSafeHwnd(), WM_MYHOTMSG_2 );
	::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon); 
	// TODO: �ڴ˴������Ϣ����������
	WCHAR ppath[MAX_PATH];
	CString apath=path;//row6
	wcscpy(ppath,apath.GetBuffer());
	apath.ReleaseBuffer();

	CString setfile=GetAppPath()+L"userprofile.dat";
	CFile f(setfile,CFile::modeCreate|CFile::modeWrite);
	f.Write(&limit,sizeof(limit));
	f.Write(&AutoOk,sizeof(AutoOk));
	f.Write(&Act,sizeof(Act));
	f.Write(&hasFolder,sizeof(hasFolder));
	f.Write(&saveTwo,sizeof(saveTwo));
	f.Write(ppath,MAX_PATH*sizeof(WCHAR));
	f.Write(&noQuitMsg,sizeof(noQuitMsg));
	f.Write(&noBubble,sizeof(noBubble));
	f.Close();	
	
	CDialog::OnDestroy();
}

void CScreenShotDlg::OnFullScreenCapture()
{
	start=true;
	bool noRestore=false;
	if((this->IsWindowVisible()==FALSE||this->IsIconic()==TRUE)&&Act!=0)
		noRestore=true;
	if(!noRestore)
	{
		this->ShowWindow(SW_MINIMIZE);
		this->ShowWindow(SW_HIDE);
	}
	//----------------------------------------------------------
	if(hasdc)
	{
		picdc.SelectObject(pold);
		bmp.DeleteObject();
		picdc.DeleteDC();
	}

	//CShotDlg g;
	//g.LIMITE=limit;
	//g.AutoOk=AutoOk;
	//g.DoModal();
	CDC *desktop=CWnd::GetDesktopWindow()->GetDC();
	if(true)//g.captured)
	{
		//CPoint lt=g.GetLt();CPoint rb=g.GetRb();
		//lt.Offset(-enlarge,-enlarge);
		//rb.Offset(enlarge,enlarge);
		//int w=::GetSystemMetrics(SM_CXSCREEN);
		//int h=::GetSystemMetrics(SM_CYSCREEN);
		pw=rb.x-lt.x;
		ph=rb.y-lt.y;
		CDC* pDC=desktop;
		//----------------------------------------------------------
		picdc.CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC,rb.x-lt.x,rb.y-lt.y);
		pold=picdc.SelectObject(&bmp);
		picdc.BitBlt(0,0,rb.x-lt.x,rb.y-lt.y,pDC,lt.x,lt.y,SRCCOPY);

		hasdc=true;
		btn2.EnableWindow(TRUE);
		btn3.EnableWindow(TRUE);
		
		if(Act==1)
		{OnSaveAsFile();}
		else if(Act==2)
		{OnBnClickedSavebord();}
		else
		{;}
	}
	else
	{
		hasdc=false;
		btn2.EnableWindow(FALSE);
		btn3.EnableWindow(FALSE);
		MessageBox(_T("Snipping failed��Selected area too small to be identified as valid\nor,you have cancelled snipping."),_T("Snipping Failed"),MB_ICONWARNING|MB_OK);
	}
	//----------------------------------------------------------
	if(!noRestore)
	{
		this->ShowWindow(SW_SHOW);
		this->ShowWindow(SW_RESTORE);
	}
	start=false;	// TODO: �ڴ˴������Ϣ����������
}

LRESULT CScreenShotDlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if ( wParam == WM_MYHOTMSG_1)//|| wParam == WM_MYHOTMSG_2)
    {
		if (start) 
			start?1:0;        //x
		else
			OnBnClickedStart();       //startcapture
	}
	else if( wParam == WM_MYHOTMSG_2)//|| wParam == WM_MYHOTMSG_2)
    {
		if (start) 
			start?1:0;        //x
		else
			OnFullScreenCapture();       //startcapture
	}
    return 0;
}

void CScreenShotDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}
void CScreenShotDlg::ShowTrayMessage(LPCTSTR msg)
{
	if(TRUE==noBubble)
		return;
	else
	{
		m_ntIcon.uTimeout = 1;
		m_ntIcon.uFlags|=NIF_INFO;
		m_ntIcon.dwInfoFlags=NIN_BALLOONTIMEOUT;
		wcscpy_s(m_ntIcon.szInfo,msg);
		wcscpy_s(m_ntIcon.szInfoTitle,L"Info");
		::Shell_NotifyIcon(NIM_MODIFY, &m_ntIcon); 
	}
}

void CScreenShotDlg::ShowDefaultTrayMsg()
{
	CString str=L"Minimized to taskbar. Use F2 to rectanglar select. Use F3 to capture fixed area";
	ShowTrayMessage(str);
	
}