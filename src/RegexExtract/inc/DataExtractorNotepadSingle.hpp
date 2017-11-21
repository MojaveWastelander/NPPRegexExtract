#pragma once
#include <npp\SciMessager.h>
#include <npp\NppMessager.h>
#include "idataextractor.hpp"

class DataExtractorNotepadSingle :
    public IDataExtractor
{
    CSciMessager& m_sci;
    CNppMessager& m_npp;
public:
    bool ExtractData(IOutputDataProcessor* pDataProcessor);
    DataExtractorNotepadSingle(CSciMessager& sci, CNppMessager& npp):
        m_sci(sci), m_npp(npp) {;}
    ~DataExtractorNotepadSingle(void);
};

