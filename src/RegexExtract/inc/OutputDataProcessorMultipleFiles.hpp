#pragma once
#include "ioutputdataprocessor.hpp"
class OutputDataProcessorMultipleFiles :
    public IOutputDataProcessor
{
public:
    void AddData(const std::wsmatch& match);
    std::vector<std::wstring> GetProcessedData();
    void Reset() 
    {
        m_vUniqueData.clear();
        m_vvMatchedData.clear();
        m_bProcessed = false;
        m_uLineMaxSize = 0;
        m_uAllMatchesSize = 0;
    }
    bool IsEmpty() { return m_vvMatchedData.empty(); }
    OutputDataProcessorMultipleFiles():
        m_bInitialize(false) {/* LOG(INFO);*/ }
private:
    std::vector<std::unordered_set<std::wstring >> m_vUniqueData;
    std::vector<std::vector<std::wstring>> m_vvMatchedData;
    bool m_bInitialize;
};

