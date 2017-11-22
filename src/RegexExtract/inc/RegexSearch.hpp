#pragma once
#include "Base.h"
#include "IDataKind.hpp"
#include "IOutputDataProcessor.hpp"

/// Regular expression search object.
class RegexSearch
{
public:
    /// \brief 
    /// \param wsExpression Regular expression used for parsing
    RegexSearch(const std::wstring& wsExpression):
        m_rxExpression(wsExpression, std::wregex::optimize | ((Options::case_insensitive())?std::wregex::icase:std::wregex::optimize)), m_wsExpression(wsExpression), m_uMatchesCount(0) {;}

    /// \brief 
    /// \param wsExpression Regular expression used for parsing
    RegexSearch(std::wstring&& wsExpression):
        m_rxExpression(wsExpression, std::wregex::optimize | ((Options::case_insensitive())?std::wregex::icase:std::wregex::optimize)), m_wsExpression(wsExpression), m_uMatchesCount(0) {;}
    RegexSearch() {;}
    void ChangeExpression(std::wstring&& wsExpression, std::wstring&& wsReplaceExpression = {})
    {
        m_uMatchesCount = 0;
        m_rxExpression.assign(std::move(std::wregex(wsExpression, std::wregex::optimize | ((Options::case_insensitive())?std::wregex::icase:std::wregex::optimize))));
        m_wsExpression = wsExpression;
        m_wsReplaceExpression = wsReplaceExpression;

    }

    /// \brief Parse data with assigned regular expression and store results in output data processor object
    /// \return true is there was at least one match 
    bool ParseData(IDataKind* pDataKind, IOutputDataProcessor* pDataProcessor);
    size_t GetMatchesCount() { return m_uMatchesCount; }
private:
    std::wregex m_rxExpression; ///< Regex object
    std::wstring m_wsExpression;	///< Original expression string
    std::wstring m_wsReplaceExpression;	///< Original replace expression string
    size_t m_uMatchesCount;
};

