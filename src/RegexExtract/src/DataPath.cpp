#include "../inc/DataPath.hpp"
#include <memory>
#include <exception>
#include <iostream>

// Read file to a string using EZUTF (http://www.codeproject.com/Articles/22046/High-Performance-Unicode-Text-File-I-O-Routines-fo) TextFile class
void DataKindPath::GetData()
{
    std::experimental::filesystem::path p(m_wsRef);
    if (std::experimental::filesystem::exists(p) == false) return;
    std::wstring  wsData;
    m_wsRawTextData.reserve(static_cast<size_t>(std::experimental::filesystem::file_size(p)));
    TextFile reader;
    
    auto result = reader.Open(m_wsRef.c_str(), TF_READ, reader.GetEncoding());
    if (result != 0) throw std::exception{fmt::format("File read error in {0} for file '{1}", __func__, p.string()).c_str()};

    wchar_t c = 0;
    // Switch to by char reading because ReadLine is not suitable 
    // to get exact copy of original file
    while ((c = reader.ReadChar()) != static_cast<wchar_t>(TF_EOF))
    {
        m_wsRawTextData.push_back(c);
    }
    reader.Close();
}
