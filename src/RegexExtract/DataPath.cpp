#include "DataPath.hpp"
#include <memory>
#include <exception>

void DataKindPath::GetText()
{
    boost::filesystem::path p(m_wsRef);
    if (boost::filesystem::exists(p) == false) return;
    std::wstring  wsData;
    m_wsRawTextData.reserve(boost::filesystem::file_size(p) / sizeof(std::wstring::value_type));
    TextFile reader;
    
    auto result = reader.Open(m_wsRef.c_str(), TF_READ, reader.GetEncoding());
    if (result != 0) throw std::exception{fmt::format("File read error in {0} for file '{1}", __func__, p.string()).c_str()};

    wchar_t* pLine = nullptr;
    while (reader.ReadLine(nullptr, &pLine) >= 0)
    {
        m_wsRawTextData += pLine;
    }
    reader.Close();
}
