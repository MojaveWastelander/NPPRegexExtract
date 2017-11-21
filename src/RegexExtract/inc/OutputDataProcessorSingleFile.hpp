#pragma once
#include "Base.h"
#include "ioutputdataprocessor.hpp"

class OutputDataProcessorSingleFile :
    public IOutputDataProcessor
{
public:
    void AddData(const std::wsmatch& match);
    void AddData(std::wstring&& replaced_match);

    std::vector<std::wstring> GetProcessedData();
    OutputDataProcessorSingleFile(void)
    {
        //LOG(INFO);
    }
    ~OutputDataProcessorSingleFile(void);
    void Reset() 
    {
        m_usUniqueMatches.clear();
        m_vvMatchedData.clear();
        m_vMatchesMaxSize.clear();
        m_bProcessed = false;
        m_uLineMaxSize = 0;
        m_uAllMatchesSize = 0;
    }
    bool IsEmpty() { return m_vvMatchedData.empty(); }
private:
    void ProcessData( std::vector<std::wstring>&& vTemp );
    std::wstring CombineStrings(const std::vector<std::wstring>& vData);
    std::unordered_set<std::wstring> m_usUniqueMatches;
    std::vector<std::vector<std::wstring>> m_vvMatchedData;
};

