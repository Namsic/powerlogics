
// ImgPrcsTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ImgPrcsTest.h"
#include "ImgPrcsTestDlg.h"

#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgPrcsTestDlg 대화 상자


CImgPrcsTestDlg::CImgPrcsTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImgPrcsTestDlg::IDD, pParent)
	, m_Radio_HSV(0)
	, m_Edit_DivideHor(1)
	, m_Edit_DivideVer(1)
	, m_Edit_ErrorRange(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgPrcsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN_DISP, m_DispCtrl);
	DDX_Radio(pDX, IDC_Radio_HSV0, m_Radio_HSV);
	DDX_Text(pDX, IDC_Edit_DivideHorizontal, m_Edit_DivideHor);
	DDX_Text(pDX, IDC_Edit_DivideVertical, m_Edit_DivideVer);
	DDX_Text(pDX, IDC_Edit_ErrorRange, m_Edit_ErrorRange);
}

BEGIN_MESSAGE_MAP(CImgPrcsTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Button_OpenFile, &CImgPrcsTestDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_Radio_HSV0, &CImgPrcsTestDlg::OnBnClickedRadioHsv)
	ON_BN_CLICKED(IDC_Radio_HSV1, &CImgPrcsTestDlg::OnBnClickedRadioHsv)
	ON_EN_CHANGE(IDC_Edit_DivideHorizontal, &CImgPrcsTestDlg::myAdaptiveTreshold)
	ON_EN_CHANGE(IDC_Edit_DivideVertical, &CImgPrcsTestDlg::myAdaptiveTreshold)
	ON_EN_CHANGE(IDC_Edit_ErrorRange, &CImgPrcsTestDlg::myAdaptiveTreshold)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CImgPrcsTestDlg 메시지 처리기

BOOL CImgPrcsTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_pDisplayImgBuf = NULL;
	m_pMainImgBuf = NULL;
	m_pFilterImgBuf = NULL;
	m_blobData = NULL;

	EnableWidget(0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BOOL CImgPrcsTestDlg::DestroyWindow()
{
	cvReleaseImage(&m_pMainImgBuf);
	cvReleaseImage(&m_pFilterImgBuf);
	delete m_blobData;

	return CDialog::DestroyWindow();
}

BOOL CImgPrcsTestDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYUP)
	{
		switch(pMsg->wParam)
		{
		case 0x31:  // '1'
			break;
		case 0x32:  // '2'
			break;
		case 0x41:  // 'a'
			myAdaptiveTreshold();
			break;
		case 0x42:  // 'b'
			AdtTreshold();
			break;
		case 0x43:  // 'c'
			break;
		case 0x53:  // 's'
			DetectQR();
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
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
		DisplayImage(m_pDisplayImgBuf);
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImgPrcsTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImgPrcsTestDlg::OnBnClickedButtonOpenfile()
{
	CFileDialog dlg(1, 0, 0, OFN_HIDEREADONLY,
		_T("img|*.jpg;*.png|file|*.*|"));

	if(dlg.DoModal() != IDOK)
		return;

	CString ext = dlg.GetFileExt().MakeLower();
	if( ext != "jpg" && ext != "png" )
		return;

	cvReleaseImage(&m_pMainImgBuf);
	delete m_blobData;
	m_blobData = NULL;

	m_pMainImgBuf = cvLoadImage(dlg.GetPathName());
	cvCvtColor(m_pMainImgBuf, m_pMainImgBuf, CV_BGR2HSV);

	m_Radio_HSV = 0;
	UpdateData(0);
	m_pDisplayImgBuf = m_pMainImgBuf;
	DisplayImage(m_pDisplayImgBuf);
	EnableWidget(1);
}

void CImgPrcsTestDlg::OnBnClickedRadioHsv()
{
	UpdateData(1);

	switch(m_Radio_HSV)
	{
	case 0:
		m_pDisplayImgBuf = m_pMainImgBuf;
		break;
	case 1:
		m_pDisplayImgBuf = m_pFilterImgBuf;
	}
	DisplayImage(m_pDisplayImgBuf);
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

	IplImage *tempImage = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 3);

	switch (pImage->nChannels)
	{
	case 1:
		cvCvtColor(pImage, tempImage, CV_GRAY2BGR);
		break;
	case 3:
		cvCvtColor(pImage, tempImage, CV_HSV2BGR);
		break;
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

void CImgPrcsTestDlg::EnableWidget(bool enable)
{
	GetDlgItem(IDC_Radio_HSV0)->EnableWindow(enable);
	GetDlgItem(IDC_Radio_HSV1)->EnableWindow(enable);

	m_Edit_DivideHor = 1;
	m_Edit_DivideVer = 1;

	GetDlgItem(IDC_Edit_DivideHorizontal)->EnableWindow(enable);
	GetDlgItem(IDC_Edit_DivideVertical)->EnableWindow(enable);
	GetDlgItem(IDC_Edit_ErrorRange)->EnableWindow(enable);
}

void CImgPrcsTestDlg::extendBlob(IplImage* pImage, int row, int col, int flag, std::queue<std::pair<int, int>>& queue)
{
	// out of range
	if( row < 0 || row >= pImage->height ||
		col < 0 || col >= pImage->width )
		return;

	// pixel is not empty
	if(!pImage->imageData[row * pImage->widthStep + col])
		return;

	pImage->imageData[row * pImage->widthStep + col] = 0;
	m_blobData[row * pImage->width + col] = flag;
	queue.push(std::make_pair(row, col));
}

void CImgPrcsTestDlg::LabelBlob()
{
	IplImage* tempImage = cvCloneImage(m_pFilterImgBuf);

	delete m_blobData;
	m_blobData = new unsigned int[tempImage->height * tempImage->width]();
	
	int blobCount = 0;

	for(int r=0; r<tempImage->height; r++)
		for(int c=0; c<tempImage->width; c++)
		{
			if(!tempImage->imageData[r * tempImage->widthStep + c])
				continue;

			tempImage->imageData[r * tempImage->widthStep + c] = 0;
			m_blobData[r * tempImage->width + c] = ++blobCount;
			std::queue<std::pair<int, int>> blobQueue;
			blobQueue.push(std::make_pair(r, c));

			while(!blobQueue.empty())
			{
				int row = blobQueue.front().first;
				int col = blobQueue.front().second;
				extendBlob(tempImage, row-1, col-1, blobCount, blobQueue);
				extendBlob(tempImage, row-1, col, blobCount, blobQueue);
				extendBlob(tempImage, row-1, col+1, blobCount, blobQueue);
				extendBlob(tempImage, row, col-1, blobCount, blobQueue);
				extendBlob(tempImage, row, col+1, blobCount, blobQueue);
				extendBlob(tempImage, row+1, col-1, blobCount, blobQueue);
				extendBlob(tempImage, row+1, col, blobCount, blobQueue);
				extendBlob(tempImage, row+1, col+1, blobCount, blobQueue);
				blobQueue.pop();
			}
		}
	cvReleaseImage(&tempImage);
}

void CImgPrcsTestDlg::myAdaptiveTreshold()
{
	UpdateData();
	int w = m_Edit_DivideHor;
	int h = m_Edit_DivideVer;
	int err = m_Edit_ErrorRange;

	cvReleaseImage(&m_pFilterImgBuf);
	m_pFilterImgBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	int subW = m_pMainImgBuf->width / w;
	int subH = m_pMainImgBuf->height / h;
	unsigned int sumVal;
	unsigned char avg;

	for(int i=0; i<h; i++)
		for(int j=0; j<w; j++)
		{
			sumVal = 0;

			for(int r=0; r<subH; r++)
				for(int c=0; c<subW; c++)
					sumVal += (unsigned char)m_pMainImgBuf->imageData[(i*subH+r)*m_pMainImgBuf->widthStep + (j*subW+c)*3 + 2];
			avg = sumVal / (subH * subW);

			for(int r=0; r<subH; r++)
				for(int c=0; c<subW; c++)
				{
					unsigned char val = m_pMainImgBuf->imageData[(i*subH+r)*m_pMainImgBuf->widthStep + (j*subW+c)*3 + 2];
					m_pFilterImgBuf->imageData[(i*subH+r)*m_pFilterImgBuf->widthStep + (j*subW+c)] = val < avg-err ? 0 : -1;
				}
		}
	//cvErode(m_pFilterImgBuf, m_pFilterImgBuf, 0, 1);
	//cvDilate(m_pFilterImgBuf, m_pFilterImgBuf, 0, 1);

	DisplayImage(m_pFilterImgBuf);
}

void CImgPrcsTestDlg::AdtTreshold()
{
	cvReleaseImage(&m_pFilterImgBuf);
	m_pFilterImgBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	IplImage* tmpHue = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	IplImage* tmpSat = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	cvSplit(m_pMainImgBuf, tmpHue, tmpSat, m_pFilterImgBuf, NULL);
	cvReleaseImage(&tmpHue);
	cvReleaseImage(&tmpSat);

	cvAdaptiveThreshold(m_pFilterImgBuf, m_pFilterImgBuf, 255, 
		ADAPTIVE_THRESH_GAUSSIAN_C);
	DisplayImage(m_pFilterImgBuf);
}

void CImgPrcsTestDlg::DetectQR()
{
	BYTE *imgData = new BYTE[m_pFilterImgBuf->imageSize];
	memcpy(imgData, m_pFilterImgBuf->imageData, m_pFilterImgBuf->imageSize);
	
	
	using namespace zxing;
	Ref<LuminanceSource> tLs;
	GreyscaleLuminanceSource *GLS = new GreyscaleLuminanceSource(imgData, m_pFilterImgBuf->widthStep, m_pFilterImgBuf->height, 0, 0, m_pFilterImgBuf->widthStep, m_pFilterImgBuf->height);
	tLs = GLS;
	Ref<Binarizer> Br;
	GlobalHistogramBinarizer *GHB = new GlobalHistogramBinarizer (tLs);
	Br = GHB;
	BinaryBitmap* BB = new BinaryBitmap(Br);
	
	DecodeHints dHints;
	dHints.addFormat(BarcodeFormat_QR_CODE);

	using namespace qrcode;
	try
	{
		Ref<DetectorResult> detectorResult = Detector(BB->getBlackMatrix()).detect(dHints);
		Ref<DecoderResult> DecodeResult = Decoder().decode(detectorResult->getBits());
		AfxMessageBox(CString(DecodeResult->getText()->getText().c_str()));
	}catch(zxing::Exception e){
		CString strTemp;
		strTemp.Format(_T("Exception: %s"), e.what());
		AfxMessageBox(strTemp);
	}

	delete[] imgData;
	//delete GLS;
	//delete GHB;
	//delete BB;
}

void CImgPrcsTestDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_pMainImgBuf || !m_blobData)
		return;

	CRect rect;
	m_DispCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	if( point.x < rect.left || point.x >= rect.right ||
		point.y < rect.top || point.y >= rect.bottom )
		return;

	CString temp;
	long x = (point.x - rect.left) * m_pMainImgBuf->width / rect.Width();
	long y = (point.y - rect.top) * m_pMainImgBuf->height / rect.Height();
	int m_index = y * m_pMainImgBuf->widthStep + x * 3;
	unsigned char h = m_pMainImgBuf->imageData[m_index];
	unsigned char s = m_pMainImgBuf->imageData[m_index+1];
	unsigned char v = m_pMainImgBuf->imageData[m_index+2];
	unsigned int b = m_blobData[y * m_pMainImgBuf->width + x];
	temp.Format("(%ld, %ld)\nH: %3u\nS: %3u\nV: %3u\nBlob: %3u", x, y, h, s, v, b);

	SetDlgItemText(IDC_Static_Cursor, temp);

	CDialog::OnMouseMove(nFlags, point);
}
