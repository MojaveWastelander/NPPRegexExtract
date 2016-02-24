#pragma once
#include "IDataKind.hpp"

/// Object that holds a simple string (in general it holds data from Notepad++ open files).
class DataKindString
    : public IDataKind
{
public:
    DataKindString(const std::wstring& wsRef):
        IDataKind(wsRef) { m_wsRawTextData.assign(m_wsRef); }
    DataKindString(std::wstring&& wsRef):
        IDataKind(wsRef) { m_wsRawTextData.assign(std::move(m_wsRef)); }
public:
    void GetText() {}
    //const std::wstring& GetRawTextData() { return m_wsRef; }
};
