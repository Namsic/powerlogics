
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedRadioHsv();
	DECLARE_MESSAGE_MAP()

private:
	IplImage* m_pDisplayImgBuf;
	IplImage* m_pMainImgBuf;
	IplImage* m_pHueBuf;
	IplImage* m_pSatBuf;
	IplImage* m_pValBuf;

	CStatic m_DispCtrl;
	int m_Radio_HSV;

public:
	void DisplayImage(IplImage* pImage);//, CDC *pDC, CRect& rect);
};
