// MFCApplication2Dlg.h: 헤더 파일
//

#pragma once

#include <vector>       // std::vector
#include <afxwin.h>     // CPoint, CRect

// CMFCApplication2Dlg 대화 상자
class CMFCApplication2Dlg : public CDialogEx
{
	// 생성자
public:
	CMFCApplication2Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 대화 상자 데이터
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

	// 구현
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// 마우스 이벤트 핸들러
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// 버튼 클릭 핸들러 (나중에 연결)
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRandom();

	// 랜덤 이동 쓰레드 메시지 수신
	afx_msg LRESULT OnRandomMove(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// 사용자 정의 멤버 변수
private:
	std::vector<CPoint> m_points;	// 클릭된 점들
	bool m_dragging = false;		// 드래그 중 여부
	int m_dragIndex = -1;			// 드래그 중인 점의 인덱스

	CPoint m_center;				// 외접원의 중심
	double m_radius = 0;			// 외접원의 반지름

	int m_pointRadius = 10;			// 점 반지름 (사용자 입력)
	int m_circleThickness = 3;		// 원 테두리 두께 (사용자 입력)
};



