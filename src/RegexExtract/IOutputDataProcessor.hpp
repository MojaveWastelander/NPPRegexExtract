#pragma once
#include "Base.h"
#include "IDataKind.hpp"

/// Output data processor interface. Child classes process matches according to current options.
class IOutputDataProcessor
{
public:

    /// Gets processed data in a formatted way. Each child class defines specific methods to return data ready to be written to output file.
    virtual std::vector<std::wstring> GetProcessedData() = 0;

    virtual void AddData(const std::wsmatch& match) = 0;
    virtual void AddData(std::wstring&& replaced_match) {replaced_match = replaced_match;}
    virtual void Reset() = 0;
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
        void CalculateMatchesSize(const std::wsmatch& match);
        void CalculateMatchesSize(const std::wstring& replaced_match);

        /// Gets output line maximum size. Made as function to calculate size only one time when needed.
        ///
        /// \return The line maximum size. Equal to sum of the longest matches for each matched group.
        size_t GetLineMaxSize();
        size_t m_uAllMatchesSize;
        size_t m_uLineMaxSize;
        bool m_bLineMaxSize;
        bool m_bProcessed;
        std::vector<size_t> m_vMatchesMaxSize;
};

