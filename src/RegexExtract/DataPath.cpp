#include "DataPath.hpp"
#include <memory>
#include <exception>

// Read file to a string using EZUTF (http://www.codeproject.com/Articles/22046/High-Performance-Unicode-Text-File-I-O-Routines-fo) TextFile class
void DataKindPath::GetText()
{
    boost::filesystem::path p(m_wsRef);
    if (boost::filesystem::exists(p) == false) return;
    std::wstring  wsData;
    m_wsRawTextData.reserve(boost::filesystem::file_size(p));
    TextFile reader;
    
    auto result = reader.Open(m_wsRef.c_str(), TF_READ, reader.GetEncoding());
    if (result != 0) throw std::exception{fmt::format("File read error in {0} for file '{1}", __func__, p.string()).c_str()};

    wchar_t* pLine = nullptr;
    while (reader.ReadLine(nullptr, &pLine) >= 0)
    {
        m_wsRawTextData += pLine;
        m_wsRawTextData += L"\n";
        free_block(pLine);
        pLine = nullptr;
    }
    if (m_wsRawTextData.size() > 2)
    {
        m_wsRawTextData.erase(m_wsRawTextData.size() - 1);
    }
    reader.Close();
}
