#include "../inc/Options.hpp"
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

namespace fs = std::experimental::filesystem;

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

bool Options::load_options()
{
    using boost::property_tree::wptree;
    using VectorStrings = std::vector<std::wstring>;
    try
    {
        wptree pt;
        std::wifstream opt{Options::options_file_path()};
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
        extract_mode() = pt.get(L"root.ExtractOptions.ExtractMode.<xmlattr>.mode", extract_mode());
        extract_mode_single_file() = pt.get(L"root.ExtractOptions.ExtractMode.SingleFileMode.<xmlattr>.mode", extract_mode_single_file());
        add_header() = pt.get(L"root.ExtractOptions.ExtractMode.AddHeader", add_header());

        save_mode() = pt.get(L"root.ExtractOptions.SaveMode.<xmlattr>.mode", save_mode());
        get_list_values(base_path(), L"root.ExtractOptions.BasePaths");

        template_name() = pt.get(L"root.ExtractOptions.SaveMode.TemplateName", template_name());
        open_files_in_notepad() = pt.get(L"root.ExtractOptions.SaveMode.OpenFilesInNotepad", open_files_in_notepad());

        extract_case_conversion() = pt.get(L"root.ExtractOptions.ExtractCaseConversion", extract_case_conversion());
        skip_whole_match() = pt.get(L"root.ExtractOptions.SkipWholeRegexMatch", skip_whole_match());

        sort_mode() = pt.get(L"root.SearchOptions.SortMode", sort_mode());
        filter_unique() = pt.get(L"root.SearchOptions.FilterUnique", filter_unique());
        case_insensitive() = pt.get(L"root.SearchOptions.CaseInsensitive", case_insensitive());

        separator() = pt.get(L"root.ExtractOptions.ExtractMode.SingleFileMode.Separator", separator());

        get_list_values(find_history(), L"root.History.Find");
        get_list_values(replace_history(), L"root.History.Replace");
        data_location() = pt.get(L"root.DataLocation.<xmlattr>.mode", data_location());
        get_list_values(files_masks(), L"root.DataLocation.Masks");
        get_list_values(files_paths(), L"root.DataLocation.Paths");
        in_selection() = pt.get(L"root.DataLocation.InSelection", in_selection());

    }
    catch (std::exception& e)
    {
        std::cerr << e.what();
        return false;
    }
    return true;
}

void Options::restore_default_settings()
{
    find_history().clear();
    replace_history().clear();
    extract_mode() = en_ExtractMode::ExtractInSingleFile;
    separator() = std::wstring(L"\\t");
    add_header() = false;
    save_mode() = en_SaveMode::ExtractToNotepad;
    base_path().clear();
    template_name() = std::wstring(L"Group_%d.txt");
    open_files_in_notepad() = false;
    extract_case_conversion() = en_ExtractCaseConversion::NoConversion;
    skip_whole_match() = false;
    sort_mode() = en_SortMode::NoSort;
    filter_unique() = false;
    case_insensitive() = false;
    files_masks().clear();
    files_paths().clear();
    data_location() = en_DataLocation::CurrentFile;
    in_selection() = false;
    //TODO: Not present in current C++ regex
    //GetDotMatchNewline() = false;    
}

bool Options::save_options()
{
    using boost::property_tree::wptree;



    try
    {
        wptree pt;
        auto set_list_values = [&](const wchar_t* p_setting_path, const auto& container)
        {
            int i = 0;
            for (auto& s : container)
            {
                if (++i <= Options::m_LIST_ITEMS_MAX_COUNT)
                {
                    pt.add(p_setting_path, s);
                }
            }
        };	    
        
        // Extract options
        pt.put(L"root.ExtractOptions.ExtractMode.<xmlattr>.mode", extract_mode());
        pt.put(L"root.ExtractOptions.ExtractMode.SingleFileMode.<xmlattr>.mode", extract_mode_single_file());
        pt.put(L"root.ExtractOptions.ExtractMode.SingleFileMode.Separator", separator());
        pt.put(L"root.ExtractOptions.ExtractMode.AddHeader", add_header());
    
        pt.put(L"root.ExtractOptions.SaveMode.<xmlattr>.mode", save_mode());
        set_list_values(L"root.ExtractOptions.BasePaths.Path", base_path());

        pt.put(L"root.ExtractOptions.SaveMode.TemplateName", template_name());
        pt.put(L"root.ExtractOptions.SaveMode.OpenFilesInNotepad", open_files_in_notepad());
    
        pt.put(L"root.ExtractOptions.ExtractCaseConversion", extract_case_conversion());
        pt.put(L"root.ExtractOptions.SkipWholeRegexMatch", skip_whole_match());
    
        pt.put(L"root.SearchOptions.SortMode", sort_mode());
        pt.put(L"root.SearchOptions.FilterUnique", filter_unique());
        pt.put(L"root.SearchOptions.CaseInsensitive", case_insensitive());
    
        set_list_values(L"root.History.Find.RegEx", find_history());
        set_list_values(L"root.History.Replace.RegEx", replace_history());
    
        pt.put(L"root.DataLocation.<xmlattr>.mode", data_location());
        set_list_values(L"root.DataLocation.Masks.Mask", files_masks());
        set_list_values(L"root.DataLocation.Paths.Path", files_paths());

        pt.put(L"root.DataLocation.InSelection", in_selection());
        

        fs::path options_file{Options::options_file_path()};
        if (!fs::exists(options_file))
        {
            fs::create_directories(options_file.parent_path());
        }
        std::wofstream opt{Options::options_file_path()};
        if (!opt.good()) return false;
        boost::property_tree::write_xml(opt, pt, boost::property_tree::xml_parser::xml_writer_settings<boost::property_tree::wptree::key_type>(L' ', 4));
    }
    catch (std::exception& e)
    {
        std::cerr << e.what();
        return false;
    }
    return true;
}

bool Options::initialize( const std::wstring& wsOptionsFilePath, bool bCreateDefaultIfNotExist /*= true*/ )
{
   get().m_wsOptionsFilePath = wsOptionsFilePath;
   bool bOptions = load_options();
   if (bCreateDefaultIfNotExist && !bOptions)
   {
       restore_default_settings();
       return save_options();
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
