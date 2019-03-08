// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenShot.h"
#include "SettingDlg.h"
#include "ShotDlg.h"

// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
	, limite(0)
	, enlarge(0)
	, AutoAct(0)
	, DefaultOK(FALSE)
	, m_path(_T(""))
	, hasFolder(FALSE)
	, m_saveboard(FALSE)
	, m_noquitmsg(FALSE)
	, m_nobubblemsg(FALSE)
	, m_frmx(0)
	, m_frmy(0)
	, m_tox(0)
	, m_toy(0)
{
	m_tox=GetSystemMetrics(SM_CXSCREEN);
	m_toy=GetSystemMetrics(SM_CYSCREEN);	
	//m_frmx=GetSystemMetrics(SM_XVIRTUALSCREEN);
	//m_frmy=GetSystemMetrics(SM_YVIRTUALSCREEN);
	//m_tox=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	//m_toy=GetSystemMetrics(SM_CYVIRTUALSCREEN);
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LEN, limite);
	//DDX_Text(pDX, IDC_OUT, enlarge);
	DDX_Check(pDX, IDC_CHECK1, DefaultOK);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Check(pDX, IDC_FOLDER, hasFolder);
	DDX_Check(pDX, IDC_CLIPBOARD, m_saveboard);
	DDX_Check(pDX, IDC_NOQUIT, m_noquitmsg);
	DDX_Check(pDX, IDC_NOBUBBLE, m_nobubblemsg);
	DDX_Text(pDX, IDC_FROMX, m_frmx);
	DDX_Text(pDX, IDC_FROMY, m_frmy);
	DDX_Text(pDX, IDC_TOX, m_tox);
	DDX_Text(pDX, IDC_TOY, m_toy);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CSettingDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSettingDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CSettingDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BROWSE, &CSettingDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_OPENFOLDER, &CSettingDlg::OnBnClickedOpenfolder)
	ON_BN_CLICKED(IDC_SETAREA, &CSettingDlg::OnBnClickedSetarea)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序

void CSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(hasFolder==TRUE&&m_path.GetLength()==0)
	{
		MessageBox(L"Please set target folder",L"Error",MB_ICONERROR|MB_OK);
	}
	else
		OnOK();
}

void CSettingDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	AutoAct=0;
	hasFolder=FALSE;
	GetDlgItem(IDC_FOLDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BROWSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLIPBOARD)->EnableWindow(FALSE);
	hasFolder=FALSE;
	m_saveboard=FALSE;
	UpdateData(FALSE);
}

void CSettingDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	GetDlgItem(IDC_FOLDER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BROWSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLIPBOARD)->EnableWindow(TRUE);
	AutoAct=1;
}

void CSettingDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	AutoAct=2;
	UpdateData();
	GetDlgItem(IDC_FOLDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BROWSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLIPBOARD)->EnableWindow(FALSE);
	hasFolder=FALSE;
	m_saveboard=FALSE;
	UpdateData(FALSE);
}

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_FOLDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BROWSE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_OPENFOLDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLIPBOARD)->EnableWindow(FALSE);

	// TODO:  在此添加额外的初始化
	if(AutoAct==0)
		((CButton*)(GetDlgItem(IDC_RADIO1)))->SetCheck(TRUE);
	else if(AutoAct==1)
	{
		((CButton*)(GetDlgItem(IDC_RADIO2)))->SetCheck(TRUE);	
		GetDlgItem(IDC_FOLDER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BROWSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLIPBOARD)->EnableWindow(TRUE);
	}
	else
		((CButton*)(GetDlgItem(IDC_RADIO3)))->SetCheck(TRUE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSettingDlg::OnBnClickedBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	HWND hwnd=GetSafeHwnd();        //得到窗口对象的指针句柄
	CString filePath=L"";
	LPMALLOC pMalloc;  //像弹出系统打开和保存对话框一样，还有一个浏览文件夹对话框，右下角是新建文件夹，
	if(::SHGetMalloc(&pMalloc)==NOERROR)
	{
	  BROWSEINFO  bi;
	  TCHAR       pszBuffer[MAX_PATH];
	  LPITEMIDLIST   pidl;

	  bi.hwndOwner =hwnd;
	  bi.pidlRoot  =NULL;
	  bi.pszDisplayName  =pszBuffer;
	  bi.lpszTitle  =_T("Select Folder");//选择目录对话框的上部分的标题
	  //添加新文件夹按钮 BIF_NEWDIALOGSTYLE
	  bi.ulFlags=BIF_NEWDIALOGSTYLE|
	   BIF_RETURNONLYFSDIRS|
	   BIF_RETURNFSANCESTORS;  //Only return file system ancestors

	  bi.lpfn =NULL;
	  bi.lParam=0;
	  bi.iImage=0;
	  if((pidl=::SHBrowseForFolder(&bi))!=NULL)
	  {
	   if(::SHGetPathFromIDList(pidl,pszBuffer))  //获得一个文件系统路径
	   {
		filePath=pszBuffer;
	   }
	   pMalloc->Free(pidl);   //释放内存
	   //MessageBox(filePath);
	   m_path=filePath;
	   UpdateData(FALSE);
	  }
	  pMalloc->Release();        //释放接口
	 }
	UpdateData(FALSE);
}
void CSettingDlg::OnBnClickedOpenfolder()
{
	// TODO: 在此添加控件通知处理程序代码
	if (! ((int)ShellExecute(NULL, L"Open", m_path, NULL, NULL, SW_SHOWNORMAL)>=32 ))
	{
		MessageBox(L"Cannot open that folder. Please check if it still exists.",L"Error",MB_OK|MB_ICONERROR);
	}
	//FindDirectory
}

void CSettingDlg::OnBnClickedSetarea()
{
	// TODO: 在此添加控件通知处理程序代码
	CShotDlg g;
	g.AutoOk=FALSE;
	g.LIMITE=0;
	if(IDOK==g.DoModal())
	{
		m_frmx=g.GetLt().x;
		m_frmy=g.GetLt().y;
		m_tox=g.GetRb().x;
		m_toy=g.GetRb().y;

	}
	UpdateData(FALSE);
}
