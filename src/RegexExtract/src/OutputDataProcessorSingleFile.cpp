#include "../inc/OutputDataProcessorSingleFile.hpp"
#include <alphanum\alphanum.hpp>


OutputDataProcessorSingleFile::~OutputDataProcessorSingleFile(void)
{
}

void OutputDataProcessorSingleFile::AddData( const std::wsmatch& match )
{

    std::vector<std::wstring> vTemp;
    header_items(match);
    vTemp.reserve((Options::skip_whole_match() == false)?match.size():(match.size() - 1));
    auto itPos = (Options::skip_whole_match() == false)?match.begin():(match.begin() + 1);

    std::for_each(itPos, match.end(), 
        [&vTemp](const std::wsmatch::_Elem& m)
    {
        vTemp.emplace_back(std::move(m.str()));
    });
    calculate_matches_size(match);
    ProcessData(std::move(vTemp));
}

void OutputDataProcessorSingleFile::AddData( std::wstring&& replaced_match )
{
    std::vector<std::wstring> vTemp;
    vTemp.push_back(replaced_match);
   // logging::log() << "Replaced match: " << vTemp[0];
    calculate_matches_size(replaced_match);
    ProcessData(std::move(vTemp));
}


std::vector<std::wstring> OutputDataProcessorSingleFile::GetProcessedData()
{
#ifdef __LOG_DLL
    logging::log() << L"Matched data vector size: " << m_vvMatchedData.size();
    logging::log() << L"GetLineMaxSize() = " << GetLineMaxSize();
    for (auto& i : m_vMatchesMaxSize)
    {
        logging::log() << L"\t" << i;
    }
#endif 
    std::vector<std::wstring> vProcessedData;
    std::wstring wsTemp;
    m_bLineMaxSize = false;
    m_bProcessed = false;
    try
    {
        wsTemp.reserve(GetLineMaxSize() * m_vvMatchedData.size());
    
    
        if (!m_bProcessed)
        {
            m_bProcessed = true;
            if (Options::sort_mode() != Options::en_SortMode::NoSort)
            {
                switch (Options::sort_mode())
                {
                case Options::en_SortMode::SortAscending:
                    {
                        std::sort(m_vvMatchedData.begin(), m_vvMatchedData.end(),
                            [&](const decltype(m_vvMatchedData[0])& lhs, const decltype(m_vvMatchedData[0])& rhs)
                        {
                            return doj::alphanum_comp(CombineStrings(lhs), CombineStrings(rhs)) < 0;
                        });
                    } break;
                case Options::en_SortMode::SortDescending:
                    {
                        std::sort(m_vvMatchedData.begin(), m_vvMatchedData.end(),
                            [&](const decltype(m_vvMatchedData[0])& lhs, const decltype(m_vvMatchedData[0])& rhs)
                        {
                            return CombineStrings(lhs) > CombineStrings(rhs);
                        });
                    } break;
                default:
                    break;
                }
            }
        }
        if (Options::add_header())
        {
            wsTemp += FormatLine(m_header_items) + L"\r\n";
        }

        size_t idx = m_vvMatchedData.size();
        for (auto& v : m_vvMatchedData)
        {
            wsTemp += FormatLine(v);
            if(--idx)
            {
                wsTemp += L"\r\n";
            }
        }
        vProcessedData.push_back(wsTemp);
    }
    catch (std::exception& e)
    {
#ifdef __LOG_DLL
        logging::log() << L"Exception caught in OutputDataProcessorSingleFile::GetProcessedData():" << utf8util::UTF16FromUTF8(e.what());
#endif   
    }

    return std::move(vProcessedData);
}

std::wstring OutputDataProcessorSingleFile::CombineStrings( const std::vector<std::wstring>& vData )
{
    size_t uMaxSize = 0;
    for (size_t i = 0; i < vData.size(); ++i) uMaxSize += vData[i].size();
    std::wstring wsTemp;
    wsTemp.reserve(uMaxSize);

    for (auto& s : vData) wsTemp += s;
    return std::move(wsTemp);
}

void OutputDataProcessorSingleFile::ProcessData( std::vector<std::wstring>&& vTemp )
{
    bool bInsertMatch = !Options::filter_unique();
#ifdef __LOG_DLL
    logging::log() << L"New data size: " << vTemp.size();
    logging::log() << L"New data contents: ";
    for (auto& s : vTemp)
    {
        logging::log() << L"\t" << s;
    }
#endif 
    if (Options::extract_case_conversion() != Options::en_ExtractCaseConversion::NoConversion)
    {
        CaseConversion(vTemp);
    }

    if (Options::filter_unique() == true)
    {
        if ((Options::extract_mode() == Options::en_ExtractMode::ExtractWithReplace) || (Options::skip_whole_match() == false))
        {
            bInsertMatch = m_usUniqueMatches.insert(vTemp[0]).second;
        }
        else
        {
            std::wstring wsTemp;
            wsTemp.reserve(GetLineMaxSize());
            for (auto& s : vTemp)
            {
                wsTemp += s;
            }
            bInsertMatch = m_usUniqueMatches.insert(wsTemp).second;
        }
    }
    if (bInsertMatch)
    {
        m_vvMatchedData.push_back(vTemp);
    }
}
