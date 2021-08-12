#include "CompositionStringEx.h"

CompositionStringEx::CompositionStringEx() {
  Init();
}

void CompositionStringEx::Init() {
  COMPOSITIONSTRING* compBase = reinterpret_cast<COMPOSITIONSTRING*>(this);
  memset(compBase, 0, sizeof(COMPOSITIONSTRING));
  compBase->dwSize = sizeof(*this);
  compBase->dwCompStrOffset = (LPBYTE)m_szCompString - (LPBYTE)(this);
  compBase->dwResultStrOffset = (LPBYTE)m_szResultString - (LPBYTE)(this);
  memset(m_szResultString, 0, sizeof(m_szResultString));
  memset(m_szCompString, 0, sizeof(m_szCompString));
}
