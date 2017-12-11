/// \file   Options.hpp
///
/// Declares the options class.

#pragma once
#include <iosfwd>
#include <vector>
#include <string>
#include <locale>
#include <type_traits>
#include <fstream>
#include <experimental\filesystem>


template <class S, class E>
typename std::enable_if<std::is_enum<E>::value, S>::type& operator<<(S& stream, E f)
{
    return stream << static_cast<int>(f);
}

template <class S, class E>
typename std::enable_if<std::is_enum<E>::value, E >::type& operator>>(S& stream, E& mode)
{
    int m;
    stream >> m;
    return (mode = static_cast<E>(m));
}

/// Options singleton class. All other classes can see and use it.
class Options
{
public:
    /// Radiobuttons enumerations.
    /// Values that represent main extract modes.
    enum class en_ExtractMode
    {
        ExtractInDifferentFile,
        ExtractInSingleFile,
        ExtractWithReplace
    };
    /// Values that represent columns separation mode for single files.

    enum class en_ExtractModeSingleFile
    {
        WithSeparator,  ///< Use a user defined separator.
        PrettyPrint ///< Automatically separate columns so they will have a spreadsheet style.
    };

    /// Values that represent columns separation mode for single files.
    /// Values that represent main save modes.
    enum class en_SaveMode
    {
        ExtractToNotepad,   ///< Data will be extracted to Notepad++.
        SaveAsNewFile   ///< Data will be extracted to files on disk.
    };
   
    /// Values that represent columns separation mode for single files.
    /// Values that represent case conversion options.
    enum class en_ExtractCaseConversion
    {
        NoConversion,
        AllUppercase,
        AllLowercase,
        FirstUppercase
    };

    /// Values that represent columns separation mode for single files.
    /// Values that represent sort mode used on search.
    enum class en_SortMode
    {
        NoSort,
        SortAscending,
        SortDescending
    };

    /// Values that represent columns separation mode for single files.

    /// Values that represent from where to get text data to search in.
    enum class en_DataLocation
    {
        CurrentFile,
        AllOpenedFiles,
        SpecificFiles
    };

    /// Values that represent columns separation mode for single files.

    /// Gets the unique static object.
    ///
    /// \return Options object.

    static Options& get();

    /// All functions below return references to available options variables

    static std::vector<std::wstring>& find_history() { return get().m_find_history; }
    static std::vector<std::wstring>& replace_history() { return get().m_replace_history; }
    static Options::en_ExtractMode& extract_mode() { return get().m_extract_mode; }    
    static Options::en_ExtractModeSingleFile& extract_mode_single_file() { return get().m_extract_mode_single_file; }
    static std::wstring& separator() { return get().m_separator; }
    static bool& add_header() { return get().m_add_header; }
    static Options::en_SaveMode& save_mode() { return get().m_save_mode; }
    static std::vector<std::wstring>& base_path() { return get().m_base_path; }
    static std::wstring& template_name() { return get().m_template; }
    static bool& open_files_in_notepad() { return get().m_open_files_in_notepad; }
    static Options::en_ExtractCaseConversion& extract_case_conversion() { return get().m_extract_case_conversion; }
    static bool& skip_whole_match() { return get().m_skip_whole_match; }
    static Options::en_SortMode& sort_mode() { return get().m_sort_mode; }
    static bool& filter_unique() { return get().m_filter_unique; }
    static bool& case_insensitive() { return get().m_case_insensitive; }
    static bool& dot_match_newline() { return get().m_dot_match_newline; }
    static bool& in_selection() { return get().m_in_selection; }
    static Options::en_DataLocation& data_location() { return get().m_data_location; }
    static std::vector<std::wstring>& files_masks() { return get().m_files_masks; }
    static std::vector<std::wstring>& files_paths() { return get().m_files_paths; }

    /// Loads options from XML file.
    ///
    /// \return true if it succeeds, false if it fails.
    static bool load_options();

    /// Saves current options to XML file.
    ///
    /// \return true if it succeeds, false if it fails.
    static bool save_options();

    /// Initialize XML file used for loading/saving options.
    ///
    /// \param  wsOptionsFilePath           Full pathname to the desired XML file.
    /// \param  bCreateDefaultIfNotExist    (optional) Create default XML if it doesn't exist.
    ///
    /// \return true if it succeeds, false if it fails.

    static bool initialize(const std::wstring& wsOptionsFilePath, bool bCreateDefaultIfNotExist = true);
    static const std::wstring& options_file_path() { return get().m_wsOptionsFilePath; }

    /// Restore a default configuration in case of loading errors or no actual options present.
    static void restore_default_settings();
private:
    /// All functions below made private to prohibit multiple object creation.
    Options() = default;
    Options(const Options& ref) = delete;
    Options& operator=(const Options& ref) = delete;

    static const size_t m_LIST_ITEMS_MAX_COUNT = 16;
    // Options file path
    std::wstring m_wsOptionsFilePath;
    // Find combobox
    std::vector<std::wstring> m_find_history; //< Find combobox history items

    // Replace combobox
    std::vector<std::wstring> m_replace_history; //< Replace combobox history items
    // Extract options
    //  Extract mode
    en_ExtractMode m_extract_mode = en_ExtractMode::ExtractInSingleFile;
    en_ExtractModeSingleFile m_extract_mode_single_file = en_ExtractModeSingleFile::PrettyPrint;
    std::wstring m_separator; 
    bool m_add_header = false; 
    //  Save mode
    en_SaveMode m_save_mode = en_SaveMode::ExtractToNotepad; 
    std::vector<std::wstring> m_base_path; 
    std::wstring  m_template;
    bool m_open_files_in_notepad = false;
    //  Extract case conversion
    en_ExtractCaseConversion m_extract_case_conversion = en_ExtractCaseConversion::NoConversion;  
    //--
    bool m_skip_whole_match = false; 
    // Search options
    //  Sort options
    en_SortMode m_sort_mode = en_SortMode::NoSort; 
    //--
    bool m_filter_unique = false;  
    bool m_case_insensitive = false;
    bool m_dot_match_newline = false;	
    // Data location
    en_DataLocation m_data_location = en_DataLocation::CurrentFile; 
    //--
    bool m_in_selection = false;
    // Mask combobox
    std::vector<std::wstring> m_files_masks;  
    // Path combobox
    std::vector<std::wstring> m_files_paths;  

};

/// Class used to convert strings from edit boxes, XML files etc. to C++ equivalents.
/// E.g. edit box string with text "\r\n" in memory look like "\\r\\n", with this class it will be converted from "\\r\\n" to "\n\r".
class UnescapeFormatter
{
public:
    template<typename FindResultT>
    std::wstring operator()(const FindResultT& match) const;
};

/// Class used to make opposite conversion of UnescapeFormatter.
/// It will convert internal string representation to user friendly one.
class EscapeFormatter
{
public:
    template<typename FindResultT>
    std::wstring operator()(const FindResultT& match) const;
};

/// Unescape string by using UnescapeFormatter class.
///
/// \param  s   String to process.
///
/// \return Processed string.
std::wstring UnescapeString(const std::wstring& s);
std::wstring UnescapeString(std::wstring&& s);
/// Escape string by using EscapeFormatter class.
///
/// \param  s   String to process.
///
/// \return Processed string.
std::wstring EscapeString(const std::wstring& s);
std::wstring EscapeString(std::wstring&& s);
void StringVectorCleanEmptyStrings(std::vector<std::wstring>& vData);