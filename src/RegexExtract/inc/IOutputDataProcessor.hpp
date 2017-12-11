#pragma once
#include "Base.h"
#include "IDataKind.hpp"

/// Output data processor interface. Child classes process matches according to current options.
class IOutputDataProcessor
{
public:

    /// Gets processed data in a formatted way. Each child class defines specific methods to return data ready to be written to output file.
    /// Each string in returned vector is a fully formatted text (e.g. pretty printed) ready to be sent to notepad++ or a file
    virtual std::vector<std::wstring> GetProcessedData() = 0;

    /// Match class is used temporarily and data processor should store matched strings
    /// It is implied that RegexSearch class is used thus there will always same amount of groups processed
    /// with each call
    virtual void AddData(const std::wsmatch& match) = 0;

    /// When "Extract with replace" is selected class will store only replaced strings
    /// How these strings are formatted is defined by replace expression passed to 
    /// RegexSearch class
    virtual void AddData(std::wstring&& replaced_match) {}

    /// 
    void header_items(const std::wsmatch& match)
    {
        if (m_header_items.empty())
        {
            if (!Options::skip_whole_match())
            {
                m_header_items.emplace_back(L"Match");
            }
            for (size_t idx = 1; idx < match.size(); ++idx)
            {
                m_header_items.emplace_back(fmt::format(L"gr_{}", idx - 1));
            }
        }
    }
    virtual void Reset()   = 0;
    virtual bool IsEmpty() = 0;
    IOutputDataProcessor() :
        m_bLineMaxSize(false), m_bProcessed(false) {;}
    virtual ~IOutputDataProcessor(void) {;}
    protected:

        /// Format line when "Pretty print" option is specified.
        ///
        /// \param  vLineData   A vector containing matched strings.
        ///
        /// \return The formatted line.
        std::wstring FormatLine(const std::vector<std::wstring>&vLineData);

        /// Case conversion if matched strings.
        ///
        /// \param [in,out] vLineData   A vector containing matched strings.
        static void CaseConversion(std::vector<std::wstring>&vLineData);

        /// Calculates matches characteristics. Used for pretty print and output strings preallocation values.
        ///
        /// \param  match   Specifies the match.
        void calculate_matches_size(const std::wsmatch& match);
        void calculate_matches_size(const std::wstring& replaced_match);

        /// Gets output line maximum size. Made as function to calculate size only one time when needed.
        ///
        /// \return The line maximum size. Equal to sum of the longest matches for each matched group.
        size_t GetLineMaxSize();
        size_t m_uAllMatchesSize = 0;
        size_t m_uLineMaxSize    = 0;
        bool m_bLineMaxSize      = false;
        bool m_bProcessed        = false;
        std::vector<size_t> m_vMatchesMaxSize;
        std::vector<std::wstring> m_header_items;
};

