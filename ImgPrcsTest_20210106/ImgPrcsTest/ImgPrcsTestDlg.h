
// ImgPrcsTestDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

#include <queue>

#include "zxing/DecodeHints.h"
#include "zxing/BinaryBitmap.h"
#include "zxing/common/GreyscaleLuminanceSource.h"
#include "zxing/common/GlobalHistogramBinarizer.h"
#include "zxing/common/DetectorResult.h"
#include "zxing/common/DecoderResult.h"
#include "zxing/qrcode/decoder/Decoder.h"
#include "zxing/qrcode/detector/Detector.h"

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

	CStatic m_DispCtrl;
	int m_Radio_HSV;
	int m_Edit_DivideHor;
	int m_Edit_DivideVer;
	int m_Edit_ErrorRange;

	void DisplayImage(IplImage* pImage);//, CDC *pDC, CRect& rect);
	void EnableWidget(bool enabled);
	void extendBlob(IplImage* pImage, int row, int col, int flag, std::queue<std::pair<int, int>>& queue);
	void LabelBlob();
	void myAdaptiveTreshold();
	void AdtTreshold();
	void DetectQR();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
