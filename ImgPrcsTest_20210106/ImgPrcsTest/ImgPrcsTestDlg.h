
// ImgPrcsTestDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CImgPrcsTestDlg ��ȭ ����
class CImgPrcsTestDlg : public CDialog
{
// �����Դϴ�.
public:
	CImgPrcsTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CImgPrcsTestDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IMGPRCSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	IplImage* m_pMainImgBuf;
	CvCapture* m_pMainVideoBuf;
	CStatic m_DispCtrl;

public:
	void DisplayImage(IplImage* pImage);//, CDC *pDC, CRect& rect);
	
	afx_msg void OnBnClickedButtonOpenfile();
	virtual BOOL DestroyWindow();
};
