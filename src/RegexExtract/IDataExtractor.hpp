#pragma once
#include "base.h"
#include "IOutputDataProcessor.hpp"

class IDataExtractor
{
public:
    IDataExtractor() {;}
    virtual bool ExtractData(IOutputDataProcessor* pDataProcessor) = 0;
    virtual ~IDataExtractor(void) {;}
};
