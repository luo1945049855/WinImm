#include "ImcHandle.h"
#include "CompositionStringEx.h"
#include "CandidateInfoEx.h"

ImcHandle::ImcHandle(HIMC hImc)
    : m_hImc(hImc), m_pContext(NULL), m_pComp(NULL), m_pCand(NULL) {
  if (m_hImc)
    m_pContext = ::ImmLockIMC(m_hImc);
}

ImcHandle::~ImcHandle() {
  if (m_pComp != NULL)
    UnlockComp();
  if (m_pCand != NULL)
    UnlockCand();
  if (m_hImc)
    ::ImmUnlockIMC(m_hImc);
}

bool ImcHandle::IsNULL() {
  return (m_pContext == NULL || m_hImc == NULL);
}

void ImcHandle::Init() {
  ATLASSERT(m_pContext != NULL);
  if (m_pContext == NULL)
    return;

  m_pContext->hCompStr =
      ::ImmReSizeIMCC(m_pContext->hCompStr, sizeof(CompositionStringEx));
  CompositionStringEx* pComp = LockComp();
  if (pComp)
    pComp->Init();
  UnlockComp();

  m_pContext->hCandInfo =
      ::ImmReSizeIMCC(m_pContext->hCandInfo, sizeof(CandidateInfoEx));
  CandidateInfoEx* pCand = LockCand();
  if (pCand)
    pCand->Init();
  UnlockCand();
}

bool ImcHandle::IsActive() {
  if (IsNULL())
    return false;
  if (m_pContext->fOpen)
    return true;
  return false;
}

void ImcHandle::SetActive(bool b) {
  if (IsNULL())
    return;
  m_pContext->fOpen = b;
}

CompositionStringEx* ImcHandle::LockComp() {
  ATLASSERT(m_pContext != NULL);
  if (m_pContext == NULL || m_pContext->hCompStr == NULL)
    return NULL;
  if (m_pComp != NULL)
    return m_pComp;
  m_pComp = (CompositionStringEx*)::ImmLockIMCC(m_pContext->hCompStr);
  return m_pComp;
}

void ImcHandle::UnlockComp() {
  if (m_pComp == NULL)
    return;
  m_pComp = NULL;
  ::ImmUnlockIMCC(m_pContext->hCompStr);
}

CandidateInfoEx* ImcHandle::LockCand() {
  ATLASSERT(m_pContext != NULL);
  if (m_pContext == NULL || m_pContext->hCandInfo == NULL)
    return NULL;
  if (m_pCand != NULL)
    return m_pCand;
  m_pCand = (CandidateInfoEx*)::ImmLockIMCC(m_pContext->hCandInfo);
  return m_pCand;
}

void ImcHandle::UnlockCand() {
  if (m_pCand == NULL)
    return;
  m_pCand = NULL;
  ::ImmUnlockIMCC(m_pContext->hCandInfo);
}

bool ImcHandle::GetFont(LOGFONT& font) const {
  ATLASSERT(m_pContext != NULL);
  if (m_pContext == NULL)
    return false;
  memcpy(&font, &m_pContext->lfFont, sizeof(LOGFONT));
  return true;
}

HWND ImcHandle::GetMainWnd() {
  ATLASSERT(m_pContext != NULL);
  if (m_pContext == NULL)
    return NULL;
  return m_pContext->hWnd;
}

int ImcHandle::GetMainWndCaretHeight() {
  LOGFONT logfont;
  int nHeight = 0;
  GetFont(logfont);
  HFONT hFont = CreateFontIndirect(&logfont);
  if (NULL != hFont) {
    SIZE szText;
    HDC hDC = ::GetDC(GetMainWnd());
    HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
    ::GetTextExtentPoint32(hDC, _T("a"), 1, &szText);
    (HFONT)::SelectObject(hDC, hOldFont);
    nHeight = szText.cy;
    ::ReleaseDC(GetMainWnd(), hDC);
  }
  return nHeight;
}