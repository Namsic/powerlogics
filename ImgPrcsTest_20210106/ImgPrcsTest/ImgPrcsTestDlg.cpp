
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
}

BEGIN_MESSAGE_MAP(CImgPrcsTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Button_OpenFile, &CImgPrcsTestDlg::OnBnClickedButtonOpenfile)
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
	m_pMainImgBuf = NULL;


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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
		DisplayImage(m_pMainImgBuf);

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
		tempImage = cvCloneImage(pImage);
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
		_T("image|*.jpg;*.png|all|*.*|")
		);

	if(IDOK != dlg.DoModal())
	{
		AfxMessageBox("������ �������� �ʾҽ��ϴ�.");
		return;
	}

	CString ext = dlg.GetFileExt().MakeLower();
	// Open Image File
	if( ext == "jpg" ||
		ext == "png" )
	{
		cvReleaseImage(&m_pMainImgBuf);
		m_pMainImgBuf = cvLoadImage(dlg.GetPathName());
		DisplayImage(m_pMainImgBuf);
	}
}

BOOL CImgPrcsTestDlg::DestroyWindow()
{
	cvReleaseImage(&m_pMainImgBuf);

	return CDialog::DestroyWindow();
}
