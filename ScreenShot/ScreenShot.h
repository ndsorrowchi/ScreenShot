
// ScreenShot.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CScreenShotApp:
// �йش����ʵ�֣������ ScreenShot.cpp
//

class CScreenShotApp : public CWinAppEx
{
public:
	CScreenShotApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CScreenShotApp theApp;