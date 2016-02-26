#include "Options.hpp"
//#include <tinyxml2.h>
#include <utf8conv\utf8conv.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
/// Test stuff
#include <iostream>
#include <iomanip>

//namespace ti = tinyxml2;

#ifdef __LOG_DLL
#include <chrono>
#include <fstream>
namespace logging
{
    std::wofstream g_log(L"Z:\\RegexExtract.log", std::ios::trunc || std::ios::binary);

     std::wofstream& log()
     {
         time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); 
         g_log << std::endl << L"[" << std::put_time(std::localtime(&t), L"%c") << L"] ";
         return g_log; 
     }
}
#endif // __LOG_DLL

void StringVectorCleanEmptyStrings(std::vector<std::wstring>& vData)
{
    vData.erase(std::remove(vData.begin(), vData.end(), std::wstring(L"")));
}

std::wstring UnescapeString(const std::wstring& s)
{
    static boost::wregex rxFindEscapedChars(L"\\\\([\\\\tnvfr0]|x([[:xdigit:]]{2})|u([[:xdigit:]]{4}))");
    return std::move(boost::find_format_all_copy(s, boost::algorithm::regex_finder(rxFindEscapedChars), UnescapeFormatter()));
}

std::wstring UnescapeString(std::wstring&& s)
{
    static boost::wregex rxFindEscapedChars(L"\\\\([\\\\tnvfr0]|x([[:xdigit:]]{2})|u([[:xdigit:]]{4}))");
    boost::find_format_all(s, boost::algorithm::regex_finder(rxFindEscapedChars), UnescapeFormatter());
    return s;
}

std::wstring EscapeString(const std::wstring& s)
{
    static boost::wregex rxFindEscapedChars(L"([\t\n\v\f\r])");
    return std::move(boost::find_format_all_copy(s, boost::algorithm::regex_finder(rxFindEscapedChars), UnescapeFormatter()));
}

std::wstring EscapeString(std::wstring&& s)
{
    static boost::wregex rxFindEscapedChars(L"([\t\n\v\f\r])");
    boost::find_format_all(s, boost::algorithm::regex_finder(rxFindEscapedChars), UnescapeFormatter());
    return s;
}

Options& Options::get()
{
    static Options instance;
    return instance;
}

bool Options::LoadOptions()
{
    using boost::property_tree::wptree;
    using VectorStrings = std::vector<std::wstring>;
    try
    {
        wptree pt;
        std::wifstream opt{Options::GetOptionsFilePath()};
        if (!opt.good()) return false;
        boost::property_tree::read_xml(opt, pt);
        auto get_list_values = [&pt](std::vector<std::wstring>& v, const wchar_t* path)
        {
            if (pt.get_child_optional(path))
            {
                for (auto&& s : pt.get_child(path))
                {
                    v.emplace_back(s.second.data());
                }
            }
        };   
        // Extract options
        GetExtractMode() = pt.get(L"root.ExtractOptions.ExtractMode.<xmlattr>.mode", GetExtractMode());
        GetExtractModeSingleFile() = pt.get(L"root.ExtractOptions.ExtractMode.SingleFileMode.<xmlattr>.mode", GetExtractModeSingleFile());
        GetAddHeader() = pt.get(L"root.ExtractOptions.ExtractMode.AddHeader", GetAddHeader());

        GetSaveMode() = pt.get(L"root.ExtractOptions.SaveMode.<xmlattr>.mode", GetSaveMode());
        get_list_values(GetBasePath(), L"root.ExtractOptions.BasePaths");

        GetTemplateName() = pt.get(L"root.ExtractOptions.SaveMode.TemplateName", GetTemplateName());
        GetOpenFilesInNotepad() = pt.get(L"root.ExtractOptions.SaveMode.OpenFilesInNotepad", GetOpenFilesInNotepad());

        GetExtractCaseConversion() = pt.get(L"root.ExtractOptions.ExtractCaseConversion", GetExtractCaseConversion());
        GetSkipWholeMatch() = pt.get(L"root.ExtractOptions.SkipWholeRegexMatch", GetSkipWholeMatch());

        GetSortMode() = pt.get(L"root.SearchOptions.SortMode", GetSortMode());
        GetFilterUnique() = pt.get(L"root.SearchOptions.FilterUnique", GetFilterUnique());
        GetCaseInsensitive() = pt.get(L"root.SearchOptions.CaseInsensitive", GetCaseInsensitive());

        GetSeparator() = pt.get(L"root.ExtractOptions.ExtractMode.SingleFileMode.Separator", GetSeparator());

        get_list_values(GetFindHistory(), L"root.History.Find");
        get_list_values(GetReplaceHistory(), L"root.History.Replace");
        GetDataLocation() = pt.get(L"root.DataLocation.<xmlattr>.mode", GetDataLocation());
        get_list_values(GetMask(), L"root.DataLocation.Masks");
        get_list_values(GetPath(), L"root.DataLocation.Paths");
        GetInSelection() = pt.get(L"root.DataLocation.InSelection", GetInSelection());

    }
    catch (std::exception& e)
    {
        //LOG(INFO) << e.what();
        return false;
    }
    return true;
}

void Options::CreateDefault()
{
    GetFindHistory().clear();
    GetReplaceHistory().clear();
    GetExtractMode() = en_ExtractMode::ExtractInSingleFile;
    GetSeparator() = std::wstring(L"\\t");
    GetAddHeader() = false;
    GetSaveMode() = en_SaveMode::ExtractToNotepad;
    GetBasePath().clear();
    GetTemplateName() = std::wstring(L"Group_%d.txt");
    GetOpenFilesInNotepad() = false;
    GetExtractCaseConversion() = en_ExtractCaseConversion::NoConversion;
    GetSkipWholeMatch() = false;
    GetSortMode() = en_SortMode::NoSort;
    GetFilterUnique() = false;
    GetCaseInsensitive() = false;
    GetMask().clear();
    GetPath().clear();
    GetDataLocation() = en_DataLocation::CurrentFile;
    GetInSelection() = false;
    //TODO: Not present in current C++ regex
    //GetDotMatchNewline() = false;    
}

bool Options::SaveOptions()
{
    using boost::property_tree::wptree;
    try
    {
	    wptree pt;
	    // Extract options
	    pt.put(L"root.ExtractOptions.ExtractMode.<xmlattr>.mode", GetExtractMode());
	    pt.put(L"root.ExtractOptions.ExtractMode.SingleFileMode.<xmlattr>.mode", GetExtractModeSingleFile());
        pt.put(L"root.ExtractOptions.ExtractMode.SingleFileMode.Separator", GetSeparator());
	    pt.put(L"root.ExtractOptions.ExtractMode.AddHeader", GetAddHeader());
	
	    pt.put(L"root.ExtractOptions.SaveMode.<xmlattr>.mode", GetSaveMode());
	    for (auto& s : GetBasePath())
	    {
	        pt.add(L"root.ExtractOptions.BasePaths.Path", s);
	    }
	    pt.put(L"root.ExtractOptions.SaveMode.TemplateName", GetTemplateName());
	    pt.put(L"root.ExtractOptions.SaveMode.OpenFilesInNotepad", GetOpenFilesInNotepad());
	
	    pt.put(L"root.ExtractOptions.ExtractCaseConversion", GetExtractCaseConversion());
	    pt.put(L"root.ExtractOptions.SkipWholeRegexMatch", GetSkipWholeMatch());
	
	    pt.put(L"root.SearchOptions.SortMode", GetSortMode());
	    pt.put(L"root.SearchOptions.FilterUnique", GetFilterUnique());
	    pt.put(L"root.SearchOptions.CaseInsensitive", GetCaseInsensitive());
	
	    for (auto& s : GetFindHistory())
	    {
	        pt.add(L"root.History.Find.RegEx", s);
	    }
	    for (auto& s : GetReplaceHistory())
	    {
	        pt.add(L"root.History.Replace.RegEx", s);
	    }
	
	    pt.put(L"root.DataLocation.<xmlattr>.mode", GetDataLocation());
	    for (auto& s : GetMask())
	    {
	        pt.add(L"root.DataLocation.Masks.Mask", s);
	    }
	    for (auto& s : GetPath())
	    {
	        pt.add(L"root.DataLocation.Paths.Path", s);
	    }

        pt.put(L"root.DataLocation.InSelection", GetInSelection());

	    std::wofstream opt{Options::GetOptionsFilePath()};
        if (!opt.good()) return false;
	    boost::property_tree::write_xml(opt, pt, boost::property_tree::xml_parser::xml_writer_settings<boost::property_tree::wptree::key_type>(L' ', 4));
    }
    catch (std::exception& e)
    {
        // out error
        return false;
    }
    return true;
}

bool Options::Init( const std::wstring& wsOptionsFilePath, bool bCreateDefaultIfNotExist /*= true*/ )
{
   get().m_wsOptionsFilePath = wsOptionsFilePath;
   bool bOptions = LoadOptions();
   if (bCreateDefaultIfNotExist && !bOptions)
   {
       CreateDefault();
       return SaveOptions();
   }
   return bOptions;
}

template<typename FindResultT>
std::wstring UnescapeFormatter::operator()(const FindResultT& match) const
{
    std::wstring wsTemp;
    auto& m = match.match_results();

    if (m[1].length() != 0)
    {
        switch (*(m[1].first))
        {
        case '\\': wsTemp.assign(L"\\"); break;
        case 'r': wsTemp.assign(L"\r"); break;
        case 'n': wsTemp.assign(L"\n"); break;
        case 't': wsTemp.assign(L"\t"); break;
        case 'v': wsTemp.assign(L"\v"); break;
        case 'f': wsTemp.assign(L"\f"); break;
        case '0': wsTemp.assign(L"\0"); break;
        case 'x':
            {
                std::wstring wsDigit(m[2].first, m[2].second);
                wsDigit += L"\0";
                wsTemp.assign(1, static_cast<wchar_t>(wcstoul(&wsDigit[0], nullptr, 16)));
            } break;
        case 'u':
            {
                std::wstring wsDigit(m[3].first, m[3].second);
                wsDigit += L"\0";
                wsTemp.assign(1, static_cast<wchar_t>(wcstoul(&wsDigit[0], nullptr, 16)));
            } break;
        }
    }
    return wsTemp;
}

template<typename FindResultT>
std::wstring EscapeFormatter::operator()(const FindResultT& match) const
{
    std::wstring wsTemp;
    auto& m = match.match_results();
    switch (*(m[1].first))
    {
    case '\r': wsTemp.assign(L"\\r"); break;
    case '\n': wsTemp.assign(L"\\n"); break;
    case '\t': wsTemp.assign(L"\\t"); break;
    case '\v': wsTemp.assign(L"\\v"); break;
    case '\f': wsTemp.assign(L"\\f"); break;
    case '\0': wsTemp.assign(L"\\0"); break;
    }
    return wsTemp;
}
