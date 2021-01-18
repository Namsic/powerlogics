
// ImgPrcsTestDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

#include <queue>

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
	IplImage* m_pFilterImgBuf;
	unsigned int* m_blobData;
	unsigned int blobCount;

	CStatic m_DispCtrl;
	int m_Radio_HSV;
	int m_Edit_LowerHue;
	int m_Edit_UpperHue;
	int m_Edit_LowerSat;
	int m_Edit_UpperSat;
	int m_Edit_LowerVal;
	int m_Edit_UpperVal;

	void DisplayImage(IplImage* pImage);//, CDC *pDC, CRect& rect);
	void FilterImage();
	void EnableWidget(bool enabled);
	void extendBlob(IplImage* pImage, int row, int col, int flag, std::queue<std::pair<int, int>>& queue);
	void LabelBlob();
	void CImgPrcsTestDlg::centroid(unsigned int label);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
