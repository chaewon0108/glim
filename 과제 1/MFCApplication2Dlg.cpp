
// MFCApplication2Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include "MFC_circleDrawer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_BN_CLICKED(IDC_BTN_RESET, &CMFCApplication2Dlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CMFCApplication2Dlg::OnBnClickedBtnRandom)
	ON_MESSAGE(WM_USER + 1, &CMFCApplication2Dlg::OnRandomMove)


END_MESSAGE_MAP()


BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		// 점 찍기
		for (const auto& pt : m_points)
			DrawClickPoint(dc.m_hDC, pt, m_pointRadius);

		// 정원 그리기
		if (m_points.size() == 3) {
			m_center = GetCenterFromThreePoints(m_points[0], m_points[1], m_points[2], m_radius);
			if (m_center.x >= 0 && m_center.y >= 0)
				DrawCircle(dc.m_hDC, m_center, m_radius, m_circleThickness);
		}
	}
}


void CMFCApplication2Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_points.size() < 3) {
		m_points.push_back(point);
		Invalidate(); // 다시 그리기
	}
	else {
		for (int i = 0; i < 3; ++i) {
			if ((abs(m_points[i].x - point.x) < m_pointRadius) &&
				(abs(m_points[i].y - point.y) < m_pointRadius)) {
				m_dragging = true;
				m_dragIndex = i;
				break;
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCApplication2Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_dragging && m_dragIndex >= 0 && m_dragIndex < 3) {
		m_points[m_dragIndex] = point;
		Invalidate();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCApplication2Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_dragging = false;
	m_dragIndex = -1;
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCApplication2Dlg::OnBnClickedBtnReset()
{
	m_points.clear();
	m_dragging = false;
	m_dragIndex = -1;
	Invalidate();
}

void CMFCApplication2Dlg::OnBnClickedBtnRandom()
{
	AfxBeginThread(RandomMoveThreadProc, this);
}

LRESULT CMFCApplication2Dlg::OnRandomMove(WPARAM, LPARAM)
{
	CRect rect;
	GetClientRect(&rect);

	RandomizePoints(m_points, rect);

	Invalidate();
	return 0;
}


HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

