#include "DataExtractorMultipleFiles.hpp"



DataExtractorMultipleFiles::~DataExtractorMultipleFiles(void)
{
}


bool DataExtractorMultipleFiles::ExtractData( IOutputDataProcessor* pDataProcessor )
{
    std::ofstream fileOut;
    auto wsPath = Options::GetBasePath()[0];
    std::wstring wsOutfileTemplate(Options::GetTemplateName());
    if (std::count(wsOutfileTemplate.begin(), wsOutfileTemplate.end(), '%') > 1) wsOutfileTemplate = L"Group_%d";

    if (wsPath.empty()) wsPath += L"ExtractedMatches.txt";
    else if (wsPath.back() != '\\') wsPath += L"\\";

    size_t i = 0;
    for (auto& s : pDataProcessor->GetProcessedData())
    {
        std::wstring wsPath(wsPath + (boost::basic_format<wchar_t>(wsOutfileTemplate)%i++).str());
        fileOut.open(wsPath, std::ios::binary | std::ios::trunc);
        if (!fileOut.good())
        {
            throw std::exception("Could not open output file for writing!");
        }
        auto sExtractedData = utf8util::UTF8FromUTF16(std::move(s));
        fileOut.write(sExtractedData.c_str(), sExtractedData.size());
        fileOut.close();
        if (Options::GetOpenFilesInNotepad() == true)
        {
            m_npp.SendNppMsg(NPPM_DOOPEN, 0, (WPARAM)(wsPath.c_str()));
            // Reload file in case is already opened
            m_npp.SendNppMsg(NPPM_RELOADFILE, 0, (WPARAM) (wsPath.c_str()));
        }
    }
    return true;
}
