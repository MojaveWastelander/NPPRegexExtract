#include "DataExtractorSingleFile.hpp"
#include "Encoding.hpp"

bool DataExtractorSingleFile::ExtractData(IOutputDataProcessor* pDataProcessor)
{
    std::ofstream fileOut;
    auto sPath = Options::base_path()[0];
    if (sPath.empty()) sPath = L"ExtractedMatches.txt";
    else if (sPath.back() != '\\') sPath += L"\\ExtractedMatches.txt";
    else sPath += L"ExtractedMatches.txt";
    fileOut.open(sPath, std::ios::binary | std::ios::trunc);
    if (!fileOut.good())
    {
        std::string sError("Could not open output file <");
        sError += utf8util::UTF8FromUTF16(sPath);
        sError += "> for writing!";
        throw std::exception(sError.c_str()); 
    }
    auto sExtractedData = utf8util::UTF8FromUTF16(std::move(pDataProcessor->GetProcessedData()[0]));
    fileOut.write(sExtractedData.c_str(), sExtractedData.size());
    return true;
}