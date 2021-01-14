
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
	, m_Edit_LowerHue(0)
	, m_Edit_UpperHue(180)
	, m_Edit_LowerSat(0)
	, m_Edit_UpperSat(255)
	, m_Edit_LowerVal(0)
	, m_Edit_UpperVal(255)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgPrcsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN_DISP, m_DispCtrl);
	DDX_Radio(pDX, IDC_Radio_HSV0, m_Radio_HSV);
	DDX_Text(pDX, IDC_Edit_LowerHue, m_Edit_LowerHue);
	DDX_Text(pDX, IDC_Edit_UpperHue, m_Edit_UpperHue);
	DDX_Text(pDX, IDC_Edit_LowerSat, m_Edit_LowerSat);
	DDX_Text(pDX, IDC_Edit_UpperSat, m_Edit_UpperSat);
	DDX_Text(pDX, IDC_Edit_LowerVal, m_Edit_LowerVal);
	DDX_Text(pDX, IDC_Edit_UpperVal, m_Edit_UpperVal);
}

BEGIN_MESSAGE_MAP(CImgPrcsTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Button_OpenFile, &CImgPrcsTestDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_Radio_HSV0, &CImgPrcsTestDlg::OnBnClickedRadioHsv)
	ON_BN_CLICKED(IDC_Radio_HSV1, &CImgPrcsTestDlg::OnBnClickedRadioHsv)
	ON_EN_CHANGE(IDC_Edit_LowerHue, &CImgPrcsTestDlg::FilterImage)
	ON_EN_CHANGE(IDC_Edit_UpperHue, &CImgPrcsTestDlg::FilterImage)
	ON_EN_CHANGE(IDC_Edit_LowerSat, &CImgPrcsTestDlg::FilterImage)
	ON_EN_CHANGE(IDC_Edit_UpperSat, &CImgPrcsTestDlg::FilterImage)
	ON_EN_CHANGE(IDC_Edit_LowerVal, &CImgPrcsTestDlg::FilterImage)
	ON_EN_CHANGE(IDC_Edit_UpperVal, &CImgPrcsTestDlg::FilterImage)
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
			break;
		case 0x53:  // 's'
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
	cvReleaseImage(&m_pFilterImgBuf);
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

void CImgPrcsTestDlg::FilterImage()
{
	UpdateData(1);
	if(m_Edit_LowerHue > 180) m_Edit_LowerHue = 0;
	if(m_Edit_LowerSat > 255) m_Edit_LowerSat = 0;
	if(m_Edit_LowerVal > 255) m_Edit_LowerVal = 0;
	if(m_Edit_UpperHue > 180) m_Edit_UpperHue = 180;
	if(m_Edit_UpperSat > 255) m_Edit_UpperSat = 255;
	if(m_Edit_UpperVal > 255) m_Edit_UpperVal = 255;
	m_Radio_HSV = 1;
	UpdateData(0);

	cvReleaseImage(&m_pFilterImgBuf);
	m_pFilterImgBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);

	for(int r=0; r<m_pFilterImgBuf->height; r++)
		for(int c=0; c<m_pFilterImgBuf->width; c++)
		{
			int m_index = r * m_pMainImgBuf->widthStep + c * 3;
			int f_index = r * m_pFilterImgBuf->widthStep + c;

			unsigned char hue = m_pMainImgBuf->imageData[m_index];
			unsigned char sat = m_pMainImgBuf->imageData[m_index+1];
			unsigned char val = m_pMainImgBuf->imageData[m_index+2];
			if( hue < m_Edit_LowerHue || hue > m_Edit_UpperHue ||
				sat < m_Edit_LowerSat || sat > m_Edit_UpperSat || 
				val < m_Edit_LowerVal || val > m_Edit_UpperVal )
			{
				m_pFilterImgBuf->imageData[f_index] = 0;
			}
			else
			{
				m_pFilterImgBuf->imageData[f_index] = -1;
			}
		}
	LabelBlob();

	m_pDisplayImgBuf = m_pFilterImgBuf;
	DisplayImage(m_pDisplayImgBuf);
}

void CImgPrcsTestDlg::EnableWidget(bool enable)
{
	GetDlgItem(IDC_Radio_HSV0)->EnableWindow(enable);
	GetDlgItem(IDC_Radio_HSV1)->EnableWindow(enable);

	m_Edit_LowerHue = 0;
	m_Edit_LowerSat = 0;
	m_Edit_LowerVal = 0;
	m_Edit_UpperHue = 180;
	m_Edit_UpperSat = 255;
	m_Edit_UpperVal = 255;

	GetDlgItem(IDC_Edit_LowerHue)->EnableWindow(enable);
	GetDlgItem(IDC_Edit_UpperHue)->EnableWindow(enable);
	GetDlgItem(IDC_Edit_LowerSat)->EnableWindow(enable);
	GetDlgItem(IDC_Edit_UpperSat)->EnableWindow(enable);
	GetDlgItem(IDC_Edit_LowerVal)->EnableWindow(enable);
	GetDlgItem(IDC_Edit_UpperVal)->EnableWindow(enable);
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
	
	int count = 0;

	for(int r=0; r<tempImage->height; r++)
		for(int c=0; c<tempImage->width; c++)
		{
			if(!tempImage->imageData[r * tempImage->widthStep + c])
				continue;

			tempImage->imageData[r * tempImage->widthStep + c] = 0;
			m_blobData[r * tempImage->width + c] = ++count;
			std::queue<std::pair<int, int>> blobQueue;
			blobQueue.push(std::make_pair(r, c));

			while(!blobQueue.empty())
			{
				int row = blobQueue.front().first;
				int col = blobQueue.front().second;
				extendBlob(tempImage, row-1, col-1, count, blobQueue);
				extendBlob(tempImage, row-1, col, count, blobQueue);
				extendBlob(tempImage, row-1, col+1, count, blobQueue);
				extendBlob(tempImage, row, col-1, count, blobQueue);
				extendBlob(tempImage, row, col+1, count, blobQueue);
				extendBlob(tempImage, row+1, col-1, count, blobQueue);
				extendBlob(tempImage, row+1, col, count, blobQueue);
				extendBlob(tempImage, row+1, col+1, count, blobQueue);
				blobQueue.pop();
			}
		}
	DisplayImage(tempImage);
	cvReleaseImage(&tempImage);
}

void CImgPrcsTestDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_blobData)
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
	unsigned int v = m_blobData[y * m_pMainImgBuf->width + x];
	temp.Format("(%4d, %4d) / blob: %3u", x, y, v);
	SetDlgItemText(IDC_Static_Cursor, temp);

	CDialog::OnMouseMove(nFlags, point);
}
