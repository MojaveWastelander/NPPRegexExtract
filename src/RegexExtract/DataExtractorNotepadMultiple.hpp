#pragma once
#include <npp\SciMessager.h>
#include <npp\NppMessager.h>
#include "idataextractor.hpp"
class DataExtractorNotepadMultiple :
    public IDataExtractor
{
    CSciMessager& m_sci;
    CNppMessager& m_npp;
public:
    bool ExtractData(IOutputDataProcessor* pDataProcessor);
    DataExtractorNotepadMultiple(CSciMessager& sci, CNppMessager& npp) :
        m_sci(sci), m_npp(npp) {;}
    ~DataExtractorNotepadMultiple(void);
};

