#pragma once

#include "CommonHeader.h"

class CompositionStringEx : public COMPOSITIONSTRING {
 public:
  CompositionStringEx();
  void Init();
  LPTSTR GetCompString() { return m_szCompString; }
  LPTSTR GetResultString() { return m_szResultString; }
  COMPOSITIONSTRING& GetBase() { return *this; }

 public:
  static const int c_MaxCompString = 256;
  static const int c_MaxResultString = 256;
  // static const int c_MaxCompReadString = 256;

 private:
  TCHAR m_szCompString[c_MaxCompString];
  TCHAR m_szResultString[c_MaxResultString];
  // TCHAR m_szCompReadString[c_MaxCompReadString];
};
