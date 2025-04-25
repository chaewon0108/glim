// mfc_circle_drawer.h
#pragma once
#include <afxwin.h>
#include <vector>

CPoint GetCenterFromThreePoints(const CPoint& A, const CPoint& B, const CPoint& C, double& radius);
void DrawCircle(HDC hdc, const CPoint& center, double radius, int thickness);
void DrawClickPoint(HDC hdc, const CPoint& pt, int radius);
void RandomizePoints(std::vector<CPoint>& points, CRect& clientRect);
UINT RandomMoveThreadProc(LPVOID pParam);



