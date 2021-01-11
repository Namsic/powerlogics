
// ImgPrcsTestDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CImgPrcsTestDlg 대화 상자
class CImgPrcsTestDlg : public CDialog
{
// 생성입니다.
public:
	CImgPrcsTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CImgPrcsTestDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IMGPRCSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	IplImage* m_pDisplayImgBuf;

	IplImage* m_pMainImgBuf;
	IplImage* m_pHueImgBuf;
	IplImage* m_pSatImgBuf;
	IplImage* m_pValImgBuf;

	CStatic m_DispCtrl;
	int m_mRadio_HSV;

public:
	void DisplayImage(IplImage* pImage);//, CDC *pDC, CRect& rect);
	
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedRadioHSV();
	virtual BOOL DestroyWindow();
};
