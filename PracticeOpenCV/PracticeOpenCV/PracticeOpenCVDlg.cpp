
// PracticeOpenCVDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "PracticeOpenCV.h"
#include "PracticeOpenCVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPracticeOpenCVDlg::CPracticeOpenCVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPracticeOpenCVDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPracticeOpenCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Static_Display, m_StaticDisplay);
}

BEGIN_MESSAGE_MAP(CPracticeOpenCVDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Button_OpenFile, &CPracticeOpenCVDlg::OnBnClickedButtonOpenfile)
END_MESSAGE_MAP()


// CPracticeOpenCVDlg �޽��� ó����

BOOL CPracticeOpenCVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	//m_matDisplay = NULL;

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CPracticeOpenCVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPracticeOpenCVDlg::OnPaint()
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
		DrawImage(m_matDisplay);
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CPracticeOpenCVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPracticeOpenCVDlg::DrawImage(Mat mat_image)
{
	CDC *pDC = m_StaticDisplay.GetDC();
	CRect rect;
	m_StaticDisplay.GetClientRect(&rect);

	BITMAPINFO *bmpInfo = new BITMAPINFO;
	//memset(&bmpInfo, 0, sizeof(bmpInfo));
	bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo->bmiHeader.biPlanes = 1;
	bmpInfo->bmiHeader.biCompression = BI_RGB;
	bmpInfo->bmiHeader.biWidth = mat_image.cols;
	bmpInfo->bmiHeader.biHeight = -mat_image.rows;
	bmpInfo->bmiHeader.biBitCount = mat_image.channels() * 8;

	// if(image.channels == 1)

	pDC->SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(pDC->GetSafeHdc(), 
		rect.left, rect.top, rect.right, rect.bottom, 
		0, 0, mat_image.cols, mat_image.rows, 
		mat_image.data, bmpInfo, DIB_RGB_COLORS, SRCCOPY);

	delete bmpInfo;
	m_StaticDisplay.ReleaseDC(pDC);
}

void CPracticeOpenCVDlg::DisplayVideo(VideoCapture capture)
{
	Mat m_mimage;
	while(TRUE)
	{
		capture >> m_mimage;
		if(m_mimage.empty()) break;
		DrawImage(m_mimage);
	}
}

void CPracticeOpenCVDlg::OnBnClickedButtonOpenfile()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("file|*.*|"));

	if(IDOK != dlg.DoModal())
		return;

	// Open Image File
	CString ext = dlg.GetFileExt().MakeLower();
	CString path = dlg.GetPathName();
	CT2CA pstring(path);
	std::string strPath(pstring);
	if( ext == "jpg" ||
		ext == "png" )
	{
		m_matDisplay = imread(strPath);
		DrawImage(m_matDisplay);
	}
	// Open Video File
	else if(ext == "mp4")
	{
		DisplayVideo(VideoCapture(strPath));
	}
}
