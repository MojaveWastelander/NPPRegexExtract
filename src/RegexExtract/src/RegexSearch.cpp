#include "../inc/RegexSearch.hpp"

bool RegexSearch::ParseData(IDataKind* pDataKind, IOutputDataProcessor* pDataProcessor)
{
    bool bMatch = false;
    std::wsregex_iterator itPos(pDataKind->GetRawTextData().begin(), pDataKind->GetRawTextData().end(), m_rxExpression);
    std::wsregex_iterator itEnd;
    for (; itPos != itEnd; ++itPos)
    {
        if (!bMatch && (itPos->size() > 0))
        {
            bMatch = true;
            pDataKind->SetMatched(true);
        }
        ++m_uMatchesCount;
        //LOG(INFO) << "New match:" << itPos->begin()->str();
        if (Options::extract_mode() == Options::en_ExtractMode::ExtractWithReplace)
        {
            auto& match = *itPos;
            pDataProcessor->AddData(std::regex_replace(match[0].str(), m_rxExpression, m_wsReplaceExpression));
        }
        else
        {
            pDataProcessor->AddData(*itPos);
        }
    }
    pDataKind->SetParsed();  

    return true;
}
