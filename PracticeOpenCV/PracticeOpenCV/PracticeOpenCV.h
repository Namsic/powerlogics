
// PracticeOpenCV.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPracticeOpenCVApp:
// �� Ŭ������ ������ ���ؼ��� PracticeOpenCV.cpp�� �����Ͻʽÿ�.
//

class CPracticeOpenCVApp : public CWinAppEx
{
public:
	CPracticeOpenCVApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPracticeOpenCVApp theApp;