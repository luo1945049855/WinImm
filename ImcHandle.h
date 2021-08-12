#pragma once
#include "CommonHeader.h"
#include "atlbase.h"

class CompositionStringEx;
class CandidateInfoEx;

class ImcHandle {
 public:
  ImcHandle(HIMC hImc);
  ~ImcHandle();

  bool IsNULL();
  void Init();

  CompositionStringEx* LockComp();
  void UnlockComp();

  CandidateInfoEx* LockCand();
  void UnlockCand();

  void SetActive(bool b);
  bool IsActive();
  HWND GetMainWnd();

  bool GetFont(LOGFONT& font) const;
  int GetMainWndCaretHeight();

 private:
  INPUTCONTEXT* m_pContext;
  HIMC m_hImc;
  CompositionStringEx* m_pComp;
  CandidateInfoEx* m_pCand;
};