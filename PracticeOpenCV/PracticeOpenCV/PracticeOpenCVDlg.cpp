
// PracticeOpenCVDlg.cpp : 구현 파일
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
	DDX_Control(pDX, IDC_Slider_Video, m_sldVideo);
}

BEGIN_MESSAGE_MAP(CPracticeOpenCVDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Button_OpenFile, &CPracticeOpenCVDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_Button_prev, &CPracticeOpenCVDlg::OnBnClickedButtonprev)
	ON_BN_CLICKED(IDC_Button_next, &CPracticeOpenCVDlg::OnBnClickedButtonnext)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPracticeOpenCVDlg 메시지 처리기

BOOL CPracticeOpenCVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	EnableVideoControl(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPracticeOpenCVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPracticeOpenCVDlg::OnPaint()
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
		DrawImage(m_matDisplay);
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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

void CPracticeOpenCVDlg::EnableVideoControl(bool enable)
{
	GetDlgItem(IDC_Button_prev)->EnableWindow(enable);
	GetDlgItem(IDC_Static_curFrame)->EnableWindow(enable);
	GetDlgItem(IDC_Button_next)->EnableWindow(enable);
	GetDlgItem(IDC_Slider_Video)->EnableWindow(enable);
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
		EnableVideoControl(FALSE);
	}
	// Open Video File
	else if(ext == "mp4")
	{
		m_mainCapture = VideoCapture(strPath);
		m_sldVideo.SetRange(0, m_mainCapture.get(CAP_PROP_FRAME_COUNT));
		m_sldVideo.SetPos(0);
		SetDlgItemInt(IDC_Static_curFrame, m_sldVideo.GetPos());
		
		m_mainCapture >> m_matDisplay;
		DrawImage(m_matDisplay);

		EnableVideoControl(TRUE);
	}
}

void CPracticeOpenCVDlg::OnBnClickedButtonprev()
{
	m_sldVideo.SetPos(m_sldVideo.GetPos() - 1);
	SetDlgItemInt(IDC_Static_curFrame, m_sldVideo.GetPos());
	m_mainCapture.set(CV_CAP_PROP_POS_FRAMES, m_sldVideo.GetPos());
	
	m_mainCapture >> m_matDisplay;
	DrawImage(m_matDisplay);
}

void CPracticeOpenCVDlg::OnBnClickedButtonnext()
{
	m_sldVideo.SetPos(m_sldVideo.GetPos() + 1);
	SetDlgItemInt(IDC_Static_curFrame, m_sldVideo.GetPos());
	m_mainCapture >> m_matDisplay;
	DrawImage(m_matDisplay);
}

void CPracticeOpenCVDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(pScrollBar)
	{
		if(pScrollBar == (CScrollBar*)&m_sldVideo)
		{
			SetDlgItemInt(IDC_Static_curFrame, m_sldVideo.GetPos());
			m_mainCapture >> m_matDisplay;
			DrawImage(m_matDisplay);
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
