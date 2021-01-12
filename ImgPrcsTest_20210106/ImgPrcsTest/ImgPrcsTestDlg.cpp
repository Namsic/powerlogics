
// ImgPrcsTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ImgPrcsTest.h"
#include "ImgPrcsTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgPrcsTestDlg ��ȭ ����


CImgPrcsTestDlg::CImgPrcsTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImgPrcsTestDlg::IDD, pParent)
	, m_Radio_HSV(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgPrcsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN_DISP, m_DispCtrl);
	DDX_Radio(pDX, IDC_Radio_HSV0, m_Radio_HSV);
}

BEGIN_MESSAGE_MAP(CImgPrcsTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Button_OpenFile, &CImgPrcsTestDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_Radio_HSV0, &CImgPrcsTestDlg::OnBnClickedRadioHsv)
	ON_BN_CLICKED(IDC_Radio_HSV1, &CImgPrcsTestDlg::OnBnClickedRadioHsv)
	ON_BN_CLICKED(IDC_Radio_HSV2, &CImgPrcsTestDlg::OnBnClickedRadioHsv)
	ON_BN_CLICKED(IDC_Radio_HSV3, &CImgPrcsTestDlg::OnBnClickedRadioHsv)
END_MESSAGE_MAP()


// CImgPrcsTestDlg �޽��� ó����

BOOL CImgPrcsTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	m_pDisplayImgBuf = NULL;
	m_pMainImgBuf = NULL;
	m_pHueBuf = NULL;
	m_pSatBuf = NULL;
	m_pValBuf = NULL;

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

BOOL CImgPrcsTestDlg::DestroyWindow()
{
	cvReleaseImage(&m_pMainImgBuf);
	cvReleaseImage(&m_pHueBuf);
	cvReleaseImage(&m_pSatBuf);
	cvReleaseImage(&m_pValBuf);

	return CDialog::DestroyWindow();
}


// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CImgPrcsTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DisplayImage(m_pDisplayImgBuf);
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
	cvReleaseImage(&m_pHueBuf);
	cvReleaseImage(&m_pSatBuf);
	cvReleaseImage(&m_pValBuf);

	m_pMainImgBuf = cvLoadImage(dlg.GetPathName());
	cvCvtColor(m_pMainImgBuf, m_pMainImgBuf, CV_BGR2HSV);

	m_pHueBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	m_pSatBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	m_pValBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	cvSplit(m_pMainImgBuf, m_pHueBuf, m_pSatBuf, m_pValBuf, NULL);

	m_Radio_HSV = 0;
	UpdateData(0);
	m_pDisplayImgBuf = m_pMainImgBuf;
	DisplayImage(m_pDisplayImgBuf);
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
		m_pDisplayImgBuf = m_pHueBuf;
		break;
	case 2:
		m_pDisplayImgBuf = m_pSatBuf;
		break;
	case 3:
		m_pDisplayImgBuf = m_pValBuf;
		break;
	}
	DisplayImage(m_pDisplayImgBuf);
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
		case 0x33:  // '3'
			break;
		case 0x34:  // '4'
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
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
