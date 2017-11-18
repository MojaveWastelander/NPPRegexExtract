#include "OutputDataProcessorMultipleFiles.hpp"

std::vector<std::wstring> OutputDataProcessorMultipleFiles::GetProcessedData()
{
    std::vector<std::wstring> vProcessedData;
    std::wstring wsTemp;
    wsTemp.reserve(*(std::max_element(m_vMatchesMaxSize.begin(), m_vMatchesMaxSize.end())) * m_vvMatchedData[0].size());

    if (!m_bProcessed)
    {
        m_bProcessed = true;
        size_t i = 0;
        if (Options::filter_unique() == true)
        {
            for (auto& usUniqueData : m_vUniqueData )
            {
                m_vvMatchedData[i++].assign(usUniqueData.begin(), usUniqueData.end());
            }
        }

        for (size_t i = 0; i < m_vvMatchedData.size(); ++i)
        {
            if (Options::sort_mode() != Options::en_SortMode::NoSort)
            {
                switch (Options::sort_mode())
                {
                case Options::en_SortMode::SortAscending:
                    {
                        std::sort(m_vvMatchedData[i].begin(), m_vvMatchedData[i].end(), std::less<std::wstring>());
                    } break;
                case Options::en_SortMode::SortDescending:
                    {
                        std::sort(m_vvMatchedData[i].begin(), m_vvMatchedData[i].end(), std::greater<std::wstring>());
                    } break;
                default:
                    break;
                }
            }
        }
    }
    for (auto& v : m_vvMatchedData)
    {
        for (auto& s : v)
        {
            wsTemp += s;
            wsTemp += L"\r\n";
        }
        vProcessedData.push_back(std::move(wsTemp));
    }
    return std::move(vProcessedData);
}

void OutputDataProcessorMultipleFiles::AddData( const std::wsmatch& match )
{
    if (!m_bInitialize)
    {
        m_vvMatchedData.resize((Options::skip_whole_match() == false)?match.size():(match.size() - 1));
        m_vUniqueData.resize(m_vvMatchedData.size());
        m_bInitialize = true;
    }
    m_bProcessed = false;
    std::vector<std::wstring> vTemp;
    vTemp.reserve(m_vvMatchedData.size());
    auto itMatch = (Options::skip_whole_match() == false)?match.begin():(match.begin() + 1);
    for (itMatch; itMatch != match.end(); ++itMatch)
    {
        vTemp.push_back(std::move(itMatch->str()));
    }
    CalculateMatchesSize(match);

    if (Options::extract_case_conversion() != Options::en_ExtractCaseConversion::NoConversion)
    {
        CaseConversion(vTemp);
    }

    if (Options::filter_unique() == true)
    {
        auto itvvMatchedData = m_vUniqueData.begin();
        for (auto& s : vTemp)
        {
            itvvMatchedData->insert(std::move(s));
            ++itvvMatchedData;
        }
    }
    else
    {
        auto itvvMatchedData = m_vvMatchedData.begin();
        for (auto& s : vTemp)
        {
            itvvMatchedData->push_back(std::move(s));
            ++itvvMatchedData;
        }
    }
}
