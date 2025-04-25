#include "pch.h"
#include "framework.h"
#include "MFCApplication2Dlg.h"
#include <cmath>
#include <thread>
#include <random>

CPoint GetCenterFromThreePoints(const CPoint& A, const CPoint& B, const CPoint& C, double& radius)
{
    double a1 = B.x - A.x;
    double b1 = B.y - A.y;
    double c1 = (a1 * (A.x + B.x) + b1 * (A.y + B.y)) / 2.0;

    double a2 = C.x - A.x;
    double b2 = C.y - A.y;
    double c2 = (a2 * (A.x + C.x) + b2 * (A.y + C.y)) / 2.0;

    double det = a1 * b2 - a2 * b1;
    if (fabs(det) < 1e-6) return CPoint(-1, -1); // 평행한 경우

    double cx = (c1 * b2 - c2 * b1) / det;
    double cy = (a1 * c2 - a2 * c1) / det;

    radius = hypot(cx - A.x, cy - A.y);
    return CPoint(static_cast<int>(cx), static_cast<int>(cy));
}

void DrawCircle(HDC hdc, const CPoint& center, double radius, int thickness)
{
    HPEN hPen = CreatePen(PS_SOLID, thickness, RGB(0, 0, 0));
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Ellipse(hdc, center.x - radius, center.y - radius,
        center.x + radius, center.y + radius);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}

void DrawClickPoint(HDC hdc, const CPoint& pt, int radius)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Ellipse(hdc, pt.x - radius, pt.y - radius, pt.x + radius, pt.y + radius);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

void RandomizePoints(std::vector<CPoint>& points, CRect& clientRect)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(clientRect.left + 20, clientRect.right - 20);
    std::uniform_int_distribution<> disY(clientRect.top + 20, clientRect.bottom - 20);

    for (auto& pt : points)
    {
        pt.x = disX(gen);
        pt.y = disY(gen);
    }
}

UINT RandomMoveThreadProc(LPVOID pParam)
{
    auto* pDlg = reinterpret_cast<CMFCApplication2Dlg*>(pParam);
    for (int i = 0; i < 10; ++i)
    {
        ::PostMessage(pDlg->GetSafeHwnd(), WM_USER + 1, 0, 0);
        Sleep(500);
    }
    return 0;
}



