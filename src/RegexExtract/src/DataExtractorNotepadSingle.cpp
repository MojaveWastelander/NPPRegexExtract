#include "../inc/DataExtractorNotepadSingle.hpp"

DataExtractorNotepadSingle::~DataExtractorNotepadSingle(void)
{
}

bool DataExtractorNotepadSingle::ExtractData( IOutputDataProcessor* pDataProcessor )
{
    //logging::log() << L"Create new file result: " <<  
    m_npp.SendNppMsg(NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
    if (pDataProcessor->GetProcessedData().size() != 1)
    {
        //logging::log() << L"Processed data size invalid: " << pDataProcessor->GetProcessedData().size();
        return false;
    }
    m_sci.setText(utf8util::UTF8FromUTF16(pDataProcessor->GetProcessedData()[0]).c_str());
    //logging::log() << L"Encode in UTF-8 result: " <<  
    m_npp.SendNppMsg(NPPM_MENUCOMMAND, 0, IDM_FORMAT_UTF_8);
    return true;
}
