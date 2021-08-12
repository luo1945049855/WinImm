#pragma once

#include "CommonHeader.h"

struct CandidateList {
  DWORD m_dwSize;
  DWORD m_dwStyle;
  DWORD m_dwCount;
  DWORD m_dwSelection;
  DWORD m_dwPageStart;
  DWORD m_dwPageSize;
  DWORD m_dwOffset[kMaxCandidateSize];

  DWORD GetPageSize() const;
  void SetPageSize(DWORD p_dwPageSize);

  DWORD GetCount() const;
  void SetCount(DWORD p_dwCount);

  DWORD GetPageStart() const;
  void SetPageStart(DWORD p_dwPageStart);

  DWORD GetSelection() const;
  void SetSelection(DWORD p_dwSelection);
};

struct CandidateString {
  TCHAR m_szCandStr[kMaxCandidateStringLength];
  void ResetStr();
  LPTSTR GetStr();
  void SetStr(LPCTSTR p_lpStr);
};

class CandidateInfoEx : CANDIDATEINFO {
 public:
  CandidateInfoEx();
  ~CandidateInfoEx();

  void Init();

  DWORD GetPageSize() const;
  void SetPageSize(DWORD p_dwPageSize);

  DWORD GetCount() const;
  void SetCount(DWORD p_dwCount);

  DWORD GetPageStart() const;
  void SetPageStart(DWORD p_dwPageStart);

  DWORD GetSelection() const;
  void SetSelection(DWORD p_dwSelection);

  CandidateString* GetCandStrings();

 private:
  CandidateList m_candList;
  CandidateString m_candStrings[kMaxCandidateSize];
};