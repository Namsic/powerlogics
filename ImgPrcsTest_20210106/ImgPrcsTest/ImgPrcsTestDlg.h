
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

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IMGPRCSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonOpenfile();
	DECLARE_MESSAGE_MAP()

private:
	IplImage* m_pMainImgBuf;
	IplImage* m_pHueBuf;
	IplImage* m_pSatBuf;
	IplImage* m_pValBuf;
	CStatic m_DispCtrl;

public:
	void DisplayImage(IplImage* pImage);//, CDC *pDC, CRect& rect);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
