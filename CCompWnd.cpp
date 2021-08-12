#include "CCompWnd.h"
#include "UIWnd.h"
#include "ImcHandle.h"
#include "CompositionStringEx.h"

LRESULT CCompWnd::OnWndProc(HWND hWnd,
                            UINT uMsg,
                            WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDC = ::BeginPaint(hWnd, &ps);
      OnPaint(hDC);
      ::EndPaint(hWnd, &ps);
      return 0;
    }
  }
  return __super::OnWndProc(hWnd, uMsg, wParam, lParam);
}

void CCompWnd::OnPaint(HDC hDC) {
  ImcHandle imcHandle(m_pUIWnd->GetHimc());
  CompositionStringEx* pComp = imcHandle.LockComp();
  TCHAR szText[1024] = {0};
  int ccText =
      _stprintf_s(szText, 1024, _T("输入窗：%s"), pComp->GetCompString());
  RECT rect;
  GetWindowRect(m_hWnd, &rect);
  rect.right = rect.right - rect.left;
  rect.bottom = rect.bottom - rect.top;
  rect.left = 0;
  rect.top = 0;
  FillRect(hDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));
  DrawText(hDC, szText, ccText, &rect, DT_VCENTER | DT_CENTER);
}

void CCompWnd::CalcSize(SIZE& size) {
  if (m_pUIWnd == NULL)
    return;
  ImcHandle imcHandle(m_pUIWnd->GetHimc());
  CompositionStringEx* pComp = imcHandle.LockComp();
  TCHAR szText[1024] = {0};
  int ccText =
      _stprintf_s(szText, 1024, _T("输入窗：%s"), pComp->GetCompString());
  HDC hDC = GetDC(m_hWnd);
  GetTextExtentPoint32(hDC, szText, ccText, &size);
  ReleaseDC(m_hWnd, hDC);
}

void CCompWnd::UpdateWindowPos(ImcHandle& imcHandle) {
  SIZE size = {0};
  CalcSize(size);

  POINT pt = {0};
  GetCaretPos(&pt);
  ::ClientToScreen(imcHandle.GetMainWnd(), &pt);
  pt.y += imcHandle.GetMainWndCaretHeight();
  ::MoveWindow(GetWnd(), pt.x, pt.y, size.cx, size.cy, TRUE);
}
