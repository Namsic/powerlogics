
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
	, m_mRadio_HSV(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CImgPrcsTestDlg::~CImgPrcsTestDlg()
{
}

void CImgPrcsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN_DISP, m_DispCtrl);
	DDX_Radio(pDX, IDC_Radio_HSV_Origin, m_mRadio_HSV);
}

BEGIN_MESSAGE_MAP(CImgPrcsTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Button_OpenFile, &CImgPrcsTestDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_Radio_HSV_Origin, &CImgPrcsTestDlg::OnBnClickedRadioHSV)
	ON_BN_CLICKED(IDC_Radio_HSV_Hue, &CImgPrcsTestDlg::OnBnClickedRadioHSV)
	ON_BN_CLICKED(IDC_Radio_HSV_Saturation, &CImgPrcsTestDlg::OnBnClickedRadioHSV)
	ON_BN_CLICKED(IDC_Radio_HSV_Value, &CImgPrcsTestDlg::OnBnClickedRadioHSV)
END_MESSAGE_MAP()


// CImgPrcsTestDlg �޽��� ó����

BOOL CImgPrcsTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	
	// Initialize pointer
	m_pDisplayImgBuf = NULL;
	m_pMainImgBuf = NULL;
	m_pHueImgBuf = NULL;
	m_pSatImgBuf = NULL;
	m_pValImgBuf = NULL;


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

BOOL CImgPrcsTestDlg::DestroyWindow()
{
	cvReleaseImage(&m_pMainImgBuf);
	cvReleaseImage(&m_pHueImgBuf);
	cvReleaseImage(&m_pSatImgBuf);
	cvReleaseImage(&m_pValImgBuf);

	return CDialog::DestroyWindow();
}


void CImgPrcsTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
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
		tempImage = cvCreateImage(cvSize(pImage->width, pImage->height), IPL_DEPTH_8U, 3);
		cvCvtColor(pImage, tempImage, CV_HSV2BGR);
		//tempImage = cvCloneImage(pImage);
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
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, 
		_T("image|*.jpg;*.png|all|*.*|"));

	if(IDOK != dlg.DoModal())
	{
		AfxMessageBox("������ �������� �ʾҽ��ϴ�.");
		return;
	}

	CString ext = dlg.GetFileExt().MakeLower();
	if( ext != "jpg" && ext != "png" )
	{
		AfxMessageBox("���� ������ �ƴմϴ�.");
		return;
	}

	// Release Image
	cvReleaseImage(&m_pMainImgBuf);
	cvReleaseImage(&m_pHueImgBuf);
	cvReleaseImage(&m_pSatImgBuf);
	cvReleaseImage(&m_pValImgBuf);
	
	m_pMainImgBuf = cvLoadImage(dlg.GetPathName());
	
	cvCvtColor(m_pMainImgBuf, m_pMainImgBuf, COLOR_BGR2HSV);
	
	m_pHueImgBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	m_pSatImgBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	m_pValImgBuf = cvCreateImage(cvGetSize(m_pMainImgBuf), IPL_DEPTH_8U, 1);
	cvSplit(m_pMainImgBuf, m_pHueImgBuf, m_pSatImgBuf, m_pValImgBuf, NULL);

	m_mRadio_HSV = 0;
	UpdateData(0);

	m_pDisplayImgBuf = m_pMainImgBuf;
	DisplayImage(m_pDisplayImgBuf);
}

void CImgPrcsTestDlg::OnBnClickedRadioHSV()
{
	UpdateData(TRUE);
	switch(m_mRadio_HSV)
	{
	case 0:
		m_pDisplayImgBuf = m_pMainImgBuf;
		break;
	case 1:
		m_pDisplayImgBuf = m_pHueImgBuf;
		break;
	case 2:
		m_pDisplayImgBuf = m_pSatImgBuf;
		break;
	case 3:
		m_pDisplayImgBuf = m_pValImgBuf;
		break;
	}
	cvInRange(m_pMainImgBuf, &cvScalar(0, 30, 30), &cvScalar(10, 255, 255), m_pDisplayImgBuf);
	DisplayImage(m_pDisplayImgBuf);
}
