#include "CandidateInfoEx.h"

DWORD CandidateList::GetPageSize() const {
  return m_dwPageSize;
}

void CandidateList::SetPageSize(DWORD p_dwPageSize) {
  m_dwPageSize = p_dwPageSize;
}

DWORD CandidateList::GetCount() const {
  return m_dwCount;
}

void CandidateList::SetCount(DWORD p_dwCount) {
  m_dwCount = p_dwCount;
}

DWORD CandidateList::GetPageStart() const {
  return m_dwPageStart;
}

void CandidateList::SetPageStart(DWORD p_dwPageStart) {
  m_dwPageStart = p_dwPageStart;
}

DWORD CandidateList::GetSelection() const {
  return m_dwSelection;
}

void CandidateList::SetSelection(DWORD p_dwSelection) {
  m_dwSelection = p_dwSelection;
}

// #########################################

void CandidateString::ResetStr() {
  memset(m_szCandStr, 0, sizeof(m_szCandStr));
}

LPTSTR CandidateString::GetStr() {
  return m_szCandStr;
}

void CandidateString::SetStr(LPCTSTR p_lpStr) {
  _tcscpy_s(m_szCandStr, _countof(m_szCandStr), p_lpStr);
}

// #########################################

CandidateInfoEx::CandidateInfoEx() {}

CandidateInfoEx::~CandidateInfoEx() {}

void CandidateInfoEx::Init() {
  CANDIDATEINFO* infoBase = reinterpret_cast<CANDIDATEINFO*>(this);
  memset(infoBase, 0, sizeof(CANDIDATEINFO));
  infoBase->dwSize = sizeof(*this);
  infoBase->dwOffset[0] = (LPBYTE)(&(m_candList)) - (LPBYTE)this;

  memset(&m_candList, 0, sizeof(m_candList));
  m_candList.m_dwSize = sizeof(m_candList);

  for (size_t i = 0; i < kMaxCandidateSize; ++i) {
    m_candList.m_dwOffset[i] =
        (LPBYTE)(&m_candStrings[i]) - (LPBYTE)(&m_candList);
    m_candStrings[i].ResetStr();
  }
}

DWORD CandidateInfoEx::GetPageSize() const {
  return m_candList.GetPageSize();
}

void CandidateInfoEx::SetPageSize(DWORD p_dwPageSize) {
  m_candList.SetPageSize(p_dwPageSize);
}

DWORD CandidateInfoEx::GetCount() const {
  return m_candList.GetCount();
}

void CandidateInfoEx::SetCount(DWORD p_dwCount) {
  m_candList.SetCount(p_dwCount);
}

DWORD CandidateInfoEx::GetPageStart() const {
  return m_candList.GetPageStart();
}

void CandidateInfoEx::SetPageStart(DWORD p_dwPageStart) {
  m_candList.SetPageStart(p_dwPageStart);
}

DWORD CandidateInfoEx::GetSelection() const {
  return m_candList.GetSelection();
}

void CandidateInfoEx::SetSelection(DWORD p_dwSelection) {
  m_candList.SetSelection(p_dwSelection);
}

CandidateString* CandidateInfoEx::GetCandStrings() {
  return m_candStrings;
}
