#pragma once
#include <npp\NppMessager.h>
#include "idataextractor.hpp"
class DataExtractorMultipleFiles :
    public IDataExtractor
{
    CNppMessager& m_npp;
public:
    bool ExtractData(IOutputDataProcessor* pDataProcessor);
    DataExtractorMultipleFiles(CNppMessager& npp) :
        m_npp(npp) {;}
    ~DataExtractorMultipleFiles(void);
};

