#pragma once

#include "CommonHeader.h"
#include "ImcHandle.h"
#include <atlwin.h>
#include "CompositionStringEx.h"

class UIWnd;

template <class T>
class ImeBaseWnd {
 public:
  static bool RegisterClass(HINSTANCE hInstance);
  static bool UnRegisterClass(HINSTANCE hInstance);

  ImeBaseWnd(TCHAR* szClassName);
  HWND Create(UIWnd* pUIWnd, HINSTANCE hInstance, RECT& rect);
  HWND GetWnd() { return m_hWnd; }

 protected:
  static const int sMaxClassName = 32;
  static TCHAR sClassName[sMaxClassName];
  static TCHAR* GetClassName() { return sClassName; }
  static LRESULT CALLBACK WndProc(HWND hWnd,
                                  UINT uMsg,
                                  WPARAM wParam,
                                  LPARAM lParam);
  virtual LRESULT OnWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
  LRESULT OnSetCursor(HWND hWnd, UINT nHitTest, UINT uMsg);
  LRESULT OnNcHitTest(int x, int y);
  void OnPaint(HDC hdc);
  void OnMouseMove(UINT nFlags, int x, int y);
  void OnLButtonUp(UINT nFlags, int x, int y);
  void UpdateWindowPos(ImcHandle& imcHandle);
  void CalcSize(SIZE& size);

 protected:
  HWND m_hWnd;
  bool m_bMoving;
  POINT m_ptOldPos;
  UIWnd* m_pUIWnd;
};

template <class T>
TCHAR ImeBaseWnd<T>::sClassName[] = {0};

template <class T>
ImeBaseWnd<T>::ImeBaseWnd(TCHAR* szClassName)
    : m_pUIWnd(NULL), m_bMoving(false) {
  _tcscpy_s(sClassName, sMaxClassName, szClassName);
}

template <class T>
bool ImeBaseWnd<T>::RegisterClass(HINSTANCE hInstance) {
  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_IME;
  wc.lpfnWndProc = ImeBaseWnd<T>::WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = sizeof(DWORD);
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = ImeBaseWnd<T>::GetClassName();
  wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
  wc.hIconSm = NULL;
  return ::RegisterClassEx(&wc);
}

template <class T>
bool ImeBaseWnd<T>::UnRegisterClass(HINSTANCE hInstance) {
  return ::UnregisterClass(ImeBaseWnd<T>::GetClassName(), hInstance);
}

template <class T>
HWND ImeBaseWnd<T>::Create(UIWnd* pUIWnd, HINSTANCE hInstance, RECT& rect) {
  m_hWnd = CreateWindow(ImeBaseWnd<T>::GetClassName(), _T(""),
                        WS_DISABLED | WS_POPUP, rect.left, rect.top,
                        rect.right - rect.left, rect.bottom - rect.top,
                        pUIWnd->GetWnd(), NULL, hInstance, this);
  SetWindowLong(m_hWnd, GWLP_USERDATA, (LONG)this);
  this->m_pUIWnd = pUIWnd;
  return m_hWnd;
}

template <class T>
LRESULT ImeBaseWnd<T>::WndProc(HWND hWnd,
                               UINT uMsg,
                               WPARAM wParam,
                               LPARAM lParam) {
  ImeBaseWnd<T>* pWndThis = (ImeBaseWnd<T>*)GetWindowLong(hWnd, GWLP_USERDATA);
  if (NULL == pWndThis) {
    if (uMsg == WM_CREATE) {
      LPCREATESTRUCT lpCS = (LPCREATESTRUCT)lParam;
      pWndThis = (ImeBaseWnd<T>*)lpCS->lpCreateParams;
      pWndThis->m_hWnd = hWnd;
    } else {
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
  }
  return pWndThis->OnWndProc(hWnd, uMsg, wParam, lParam);
}

template <class T>
LRESULT ImeBaseWnd<T>::OnWndProc(HWND hWnd,
                                 UINT uMsg,
                                 WPARAM wParam,
                                 LPARAM lParam) {
  switch (uMsg) {
    case WM_CREATE:
      return OnCreate((LPCREATESTRUCT)lParam);

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDC = ::BeginPaint(hWnd, &ps);
      OnPaint(hDC);
      ::EndPaint(hWnd, &ps);
      return 0;
    }

    case WM_MOUSEMOVE:
      OnMouseMove((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      return 0;

    case WM_LBUTTONUP:
      OnLButtonUp((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      return 0;

    case WM_SETCURSOR:
      return OnSetCursor((HWND)wParam, (UINT)LOWORD(lParam),
                         (UINT)HIWORD(lParam));

    case WM_NCHITTEST:
      return OnNcHitTest(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

template <class T>
LRESULT ImeBaseWnd<T>::OnCreate(LPCREATESTRUCT lpCreateStruct) {
  DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
  dwStyle |= (WS_DISABLED | WS_POPUP);
  SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
  return 0;
}

template <class T>
void ImeBaseWnd<T>::OnPaint(HDC hdc) {
  PAINTSTRUCT ps;
  HDC hDC = ::BeginPaint(m_hWnd, &ps);
  ImcHandle imcHandle(m_pUIWnd->GetHimc());
  CompositionStringEx* pComp = imcHandle.LockComp();
  TCHAR szText[1024] = {0};
  int ccText =
      _stprintf_s(szText, 1024, _T("Input Window: %s"), pComp->GetCompString());
  RECT rect;
  GetWindowRect(m_hWnd, &rect);
  rect.right = rect.right - rect.left;
  rect.bottom = rect.bottom - rect.top;
  rect.left = 0;
  rect.top = 0;
  FillRect(hDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));
  DrawText(hDC, szText, ccText, &rect, DT_VCENTER | DT_CENTER);
  ::EndPaint(m_hWnd, &ps);
}

template <class T>
void ImeBaseWnd<T>::CalcSize(SIZE& size) {
  if (m_pUIWnd == NULL)
    return;
  CClientDC dc(m_hWnd);
  CImcHandle imcHandle(m_pUIWnd->GetHimc());
  CompositionStringEx* pComp = imcHandle.LockComp();
  TCHAR szText[1024] = {0};
  int ccText =
      _stprintf_s(szText, 1024, _T("Input Window: %s"), pComp->GetCompString());
  HDC hdc = GetDC(m_hWnd);
  GetTextExtentPoint32(hdc, szText, ccText, &size);
  ReleaseDC(m_hWnd, hdc);
}

template <class T>
void ImeBaseWnd<T>::UpdateWindowPos(ImcHandle& imcHandle) {
  SIZE size = {0};
  CalcSize(size);

  POINT pt = {0};
  GetCaretPos(&pt);
  ::ClientToScreen(imcHandle.GetMainWnd(), &pt);
  pt.y += imcHandle.GetMainWndCaretHeight();
  ::MoveWindow(GetWnd(), pt.x, pt.y, size.cx, size.cy, TRUE);
}

template <class T>
LRESULT ImeBaseWnd<T>::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message) {
  if (message == WM_LBUTTONDOWN) {
    ::GetCursorPos(&m_ptOldPos);
    SetCapture(m_hWnd);
    m_bMoving = true;
  }
  SetCursor(LoadCursor(NULL, IDC_SIZEALL));
  return 0;
}

template <class T>
void ImeBaseWnd<T>::OnMouseMove(UINT nFlags, int x, int y) {
  if (m_bMoving) {
    RECT rt;
    POINT pt;
    GetCursorPos(&pt);
    GetWindowRect(m_hWnd, &rt);
    OffsetRect(&rt, pt.x - m_ptOldPos.x, pt.y - m_ptOldPos.y);
    MoveWindow(m_hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top,
               TRUE);
    m_ptOldPos = pt;
  }
}

template <class T>
void ImeBaseWnd<T>::OnLButtonUp(UINT nFlags, int x, int y) {
  ReleaseCapture();
  m_bMoving = false;
}

template <class T>
LRESULT ImeBaseWnd<T>::OnNcHitTest(int x, int y) {
  return HTCLIENT;
}
