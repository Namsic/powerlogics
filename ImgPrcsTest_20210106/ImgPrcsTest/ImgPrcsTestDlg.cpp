
// ImgPrcsTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ImgPrcsTest.h"
#include "ImgPrcsTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgPrcsTestDlg 대화 상자


CImgPrcsTestDlg::CImgPrcsTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImgPrcsTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgPrcsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN_DISP, m_DispCtrl);
}

BEGIN_MESSAGE_MAP(CImgPrcsTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Button_OpenFile, &CImgPrcsTestDlg::OnBnClickedButtonOpenfile)
END_MESSAGE_MAP()


// CImgPrcsTestDlg 메시지 처리기

BOOL CImgPrcsTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_pMainImgBuf = NULL;
	m_pHueBuf = NULL;
	m_pSatBuf = NULL;
	m_pValBuf = NULL;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BOOL CImgPrcsTestDlg::DestroyWindow()
{
	cvReleaseImage(&m_pMainImgBuf);
	cvReleaseImage(&m_pHueBuf);
	cvReleaseImage(&m_pSatBuf);
	cvReleaseImage(&m_pValBuf);

	return CDialog::DestroyWindow();
}


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CImgPrcsTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DisplayImage(m_pMainImgBuf);
		CDialog::OnPaint();
	}

}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImgPrcsTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImgPrcsTestDlg::DisplayImage(IplImage* pImage)//, CDC *pDC, CRect& rect)
{
	CDC *pDC = m_DispCtrl.GetDC();
	CRect rect;
	m_DispCtrl.GetClientRect(&rect);

	if (pImage == NULL || pDC == NULL)
		return;



	BITMAPINFO bitmapInfo;
	memset(&bitmapInfo, 0, sizeof(bitmapInfo));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	
	bitmapInfo.bmiHeader.biWidth = pImage->width;
	bitmapInfo.bmiHeader.biHeight = -pImage->height;

	IplImage *tempImage = NULL;

	if (pImage->nChannels == 1)
	{
		tempImage = cvCreateImage(cvSize(pImage->width, pImage->height), IPL_DEPTH_8U, 3);
		cvCvtColor(pImage, tempImage, CV_GRAY2BGR);
	}
	else if (pImage->nChannels == 3)
	{
		//tempImage = cvCloneImage(pImage);
		tempImage = cvCreateImage(cvSize(pImage->width, pImage->height), IPL_DEPTH_8U, 3);
		cvCvtColor(pImage, tempImage, CV_HSV2BGR);
	}

	bitmapInfo.bmiHeader.biBitCount = tempImage->depth * tempImage->nChannels;

	pDC->SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(pDC->GetSafeHdc(),
		rect.left, rect.top, rect.right, rect.bottom,
		0, 0, tempImage->width, tempImage->height,
		tempImage->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	
	m_DispCtrl.ReleaseDC(pDC);
	cvReleaseImage(&tempImage);
}

void CImgPrcsTestDlg::OnBnClickedButtonOpenfile()
{
	CFileDialog dlg(1, 0, 0, OFN_HIDEREADONLY, _T("|file|*.*|"));

	if(dlg.DoModal() != IDOK)
		return;

	CString ext = dlg.GetFileExt().MakeLower();
	if( ext != "jpg" && ext != "png")
		return;

	cvReleaseImage(&m_pMainImgBuf);
	cvReleaseImage(&m_pHueBuf);
	cvReleaseImage(&m_pSatBuf);
	cvReleaseImage(&m_pValBuf);

	m_pMainImgBuf = cvLoadImage(dlg.GetPathName());
	cvCvtColor(m_pMainImgBuf, m_pMainImgBuf, CV_BGR2HSV);

	m_pHueBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	m_pSatBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	m_pValBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	cvSplit(m_pMainImgBuf, m_pHueBuf, m_pSatBuf, m_pValBuf, NULL);

	DisplayImage(m_pMainImgBuf);
}

BOOL CImgPrcsTestDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYUP)
	{
		switch(pMsg->wParam)
		{
		case 0x31:
			DisplayImage(m_pHueBuf);
			break;
		case 0x32:
			DisplayImage(m_pSatBuf);
			break;
		case 0x33:
			DisplayImage(m_pValBuf);
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
