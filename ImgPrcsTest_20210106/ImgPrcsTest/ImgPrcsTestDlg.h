
// ImgPrcsTestDlg.h : 헤더 파일
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

// CImgPrcsTestDlg 대화 상자
class CImgPrcsTestDlg : public CDialog
{
// 생성입니다.
public:
	CImgPrcsTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IMGPRCSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
