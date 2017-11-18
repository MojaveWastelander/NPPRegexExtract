#pragma once
#include "IDataKind.hpp"

/// Object that holds a simple string (in general it holds data from Notepad++ open files).
/// In this case the reference will be empty and m_wsRawTextData will contain specific data
class DataKindString
    : public IDataKind
{
public:
    DataKindString(const std::wstring& wsRef):
        IDataKind(wsRef) { m_wsRawTextData.assign(std::move(m_wsRef)); }
    DataKindString(std::wstring&& wsRef):
        IDataKind(std::move(wsRef)) { m_wsRawTextData.assign(std::move(m_wsRef)); }
public:
    // Does nothing as data is assigned at construnction
    void GetData() {}
};
