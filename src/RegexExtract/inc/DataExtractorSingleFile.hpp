#pragma once
#include "idataextractor.hpp"

class DataExtractorSingleFile :
    public IDataExtractor
{
public:
    bool ExtractData(IOutputDataProcessor* pDataProcessor);
private:
};

