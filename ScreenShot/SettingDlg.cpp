// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenShot.h"
#include "SettingDlg.h"
#include "ShotDlg.h"

// CSettingDlg �Ի���

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


// CSettingDlg ��Ϣ�������

void CSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	GetDlgItem(IDC_FOLDER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BROWSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLIPBOARD)->EnableWindow(TRUE);
	AutoAct=1;
}

void CSettingDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSettingDlg::OnBnClickedBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	HWND hwnd=GetSafeHwnd();        //�õ����ڶ����ָ����
	CString filePath=L"";
	LPMALLOC pMalloc;  //�񵯳�ϵͳ�򿪺ͱ���Ի���һ��������һ������ļ��жԻ������½����½��ļ��У�
	if(::SHGetMalloc(&pMalloc)==NOERROR)
	{
	  BROWSEINFO  bi;
	  TCHAR       pszBuffer[MAX_PATH];
	  LPITEMIDLIST   pidl;

	  bi.hwndOwner =hwnd;
	  bi.pidlRoot  =NULL;
	  bi.pszDisplayName  =pszBuffer;
	  bi.lpszTitle  =_T("Select Folder");//ѡ��Ŀ¼�Ի�����ϲ��ֵı���
	  //������ļ��а�ť BIF_NEWDIALOGSTYLE
	  bi.ulFlags=BIF_NEWDIALOGSTYLE|
	   BIF_RETURNONLYFSDIRS|
	   BIF_RETURNFSANCESTORS;  //Only return file system ancestors

	  bi.lpfn =NULL;
	  bi.lParam=0;
	  bi.iImage=0;
	  if((pidl=::SHBrowseForFolder(&bi))!=NULL)
	  {
	   if(::SHGetPathFromIDList(pidl,pszBuffer))  //���һ���ļ�ϵͳ·��
	   {
		filePath=pszBuffer;
	   }
	   pMalloc->Free(pidl);   //�ͷ��ڴ�
	   //MessageBox(filePath);
	   m_path=filePath;
	   UpdateData(FALSE);
	  }
	  pMalloc->Release();        //�ͷŽӿ�
	 }
	UpdateData(FALSE);
}
void CSettingDlg::OnBnClickedOpenfolder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (! ((int)ShellExecute(NULL, L"Open", m_path, NULL, NULL, SW_SHOWNORMAL)>=32 ))
	{
		MessageBox(L"Cannot open that folder. Please check if it still exists.",L"Error",MB_OK|MB_ICONERROR);
	}
	//FindDirectory
}

void CSettingDlg::OnBnClickedSetarea()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
