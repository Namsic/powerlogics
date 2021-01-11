
// PracticeOpenCVDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPracticeOpenCVDlg ��ȭ ����
class CPracticeOpenCVDlg : public CDialog
{
// �����Դϴ�.
public:
	CPracticeOpenCVDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PRACTICEOPENCV_DIALOG };

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
	void DrawImage(Mat mat_image);
	void DisplayVideo(VideoCapture capture);
	void EnableVideoControl(bool enable);

	CStatic m_StaticDisplay;
	Mat m_matDisplay;
	VideoCapture m_mainCapture;
	CSliderCtrl m_sldVideo;
public:
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedButtonprev();
	afx_msg void OnBnClickedButtonnext();
	afx_msg void OnNMReleasedcaptureSliderVideo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
