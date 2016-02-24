#include "DataExtractorNotepadMultiple.hpp"



DataExtractorNotepadMultiple::~DataExtractorNotepadMultiple()
{
}

bool DataExtractorNotepadMultiple::ExtractData(IOutputDataProcessor* pDataProcessor)
{
    //logging::log() << L"Create new file result: " <<  
    if (pDataProcessor->GetProcessedData().size() < 1)
    {
        //logging::log() << L"Processed data size invalid: " << pDataProcessor->GetProcessedData().size();
        return false;
    }
    for (auto& wsProcessedData : pDataProcessor->GetProcessedData())
    {
        m_npp.SendNppMsg(NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
        m_sci.setText(utf8util::UTF8FromUTF16(wsProcessedData).c_str());
        //logging::log() << L"Encode in UTF-8 result: " <<  
        m_npp.SendNppMsg(NPPM_MENUCOMMAND, 0, IDM_FORMAT_UTF_8);
    }
    return true;
}