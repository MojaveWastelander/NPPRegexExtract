#include "../inc/IOutputDataProcessor.hpp"

std::wstring IOutputDataProcessor::FormatLine(const std::vector<std::wstring>&vLineData)
{
    if (vLineData.size() == 1) return std::move(std::wstring(vLineData[0] + L"\r\n"));

    std::wstring wsTemp;
    try
    {
        wsTemp.reserve(GetLineMaxSize());
        if (Options::extract_mode_single_file() == Options::en_ExtractModeSingleFile::PrettyPrint)
        {
            auto itColumnMaxSize = m_vMatchesMaxSize.cbegin();
            for (auto& s : vLineData)
            {
                wsTemp += s;
                wsTemp.insert(wsTemp.end(), (*itColumnMaxSize - s.size() + TAB_SIZE), ' ');
                ++itColumnMaxSize;
            }
        } 
        else
        {
            size_t i = vLineData.size();
            for (auto& s : vLineData)
            {
                wsTemp += s;
                if (--i) 
                {
                    wsTemp += UnescapeString(Options::separator());
                }
            }
        }
        wsTemp += L"\r\n";
    }
    catch (std::exception& e)
    {
#ifdef __LOG_DLL
        logging::log() << L"Error in <FormatLine>: " << utf8util::UTF16FromUTF8(e.what());
#endif    
    }
#ifdef __LOG_DLL
    logging::log() << L"wsTemp = " << wsTemp;
#endif
    return std::move(wsTemp);
}


size_t IOutputDataProcessor::GetLineMaxSize()
{
    if (!m_bLineMaxSize)
    {
        m_bLineMaxSize = true;
        if (Options::extract_mode() == Options::en_ExtractMode::ExtractWithReplace)
        {
            m_uLineMaxSize = m_vMatchesMaxSize[0];
        }
        else
        {
            m_uLineMaxSize = std::accumulate(m_vMatchesMaxSize.begin(), m_vMatchesMaxSize.end(), 0, 
                [=](size_t lhs, size_t rhs)
            {
                return lhs + rhs + TAB_SIZE;
            });
        }
    }
    return m_uLineMaxSize;
}

void IOutputDataProcessor::CalculateMatchesSize( const std::wsmatch& match )
{
    if (m_vMatchesMaxSize.empty())
    {
        m_vMatchesMaxSize.resize((Options::skip_whole_match() == false)?match.size():(match.size() - 1), 0);
    }
    auto it = m_vMatchesMaxSize.begin();
    bool bSkipFirst = Options::skip_whole_match();
    for (auto& m : match)
    {
        if (bSkipFirst == true)
        {
            bSkipFirst = false;
            continue;
        }
        *it = max(*it, static_cast<size_t>(m.second - m.first));
        ++it;
    }
}

void IOutputDataProcessor::CalculateMatchesSize( const std::wstring& replaced_match )
{
    if (m_vMatchesMaxSize.empty())
    {
        m_vMatchesMaxSize.resize(1);
    }
    m_vMatchesMaxSize[0] = max(m_vMatchesMaxSize[0], replaced_match.size());
}

void IOutputDataProcessor::CaseConversion( std::vector<std::wstring>&vLineData )
{
    for (auto& s : vLineData)
    {
        if (!s.empty())
        {
            switch (Options::extract_case_conversion())
            {
            case Options::en_ExtractCaseConversion::AllUppercase: boost::to_upper(s); break;
            case Options::en_ExtractCaseConversion::AllLowercase: boost::to_lower(s); break;
            case Options::en_ExtractCaseConversion::FirstUppercase: s[0] = std::toupper(s[0], std::locale()); break;             
            default:
                break;
            }
        }
    }
}
