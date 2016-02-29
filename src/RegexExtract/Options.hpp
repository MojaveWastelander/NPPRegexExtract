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
//#define __LOG_DLL
/// Very simple logging feature
#ifdef __LOG_DLL

namespace logging
{
    std::wofstream& log();
}
#endif // __LOG_DLL



template <class S, class E>
typename std::enable_if<std::is_enum<E>::value, S&>::type operator<<(S& stream, E f)
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

    static std::vector<std::wstring>& GetFindHistory() { return get().m_vFindHistory; }
    static std::vector<std::wstring>& GetReplaceHistory() { return get().m_vReplaceHistory; }
    static Options::en_ExtractMode& GetExtractMode() { return get().m_ExtractMode; }    
    static Options::en_ExtractModeSingleFile& GetExtractModeSingleFile() { return get().m_ExtractModeSingleFile; }
    static std::wstring& GetSeparator() { return get().m_wsSeparator; }
    static bool& GetAddHeader() { return get().m_bAddHeader; }
    static Options::en_SaveMode& GetSaveMode() { return get().m_SaveMode; }
    static std::vector<std::wstring>& GetBasePath() { return get().m_vBasePath; }
    static std::wstring& GetTemplateName() { return get().m_wsTemplateName; }
    static bool& GetOpenFilesInNotepad() { return get().m_bOpenFilesInNotepad; }
    static Options::en_ExtractCaseConversion& GetExtractCaseConversion() { return get().m_ExtractCaseConversion; }
    static bool& GetSkipWholeMatch() { return get().m_bSkipWholeMatch; }
    static Options::en_SortMode& GetSortMode() { return get().m_SortMode; }
    static bool& GetFilterUnique() { return get().m_bFilterUnique; }
    static bool& GetCaseInsensitive() { return get().m_bCaseInsensitive; }
    static bool& GetDotMatchNewline() { return get().m_bDotMatchNewline; }
    static bool& GetInSelection() { return get().m_bInSelection; }
    static Options::en_DataLocation& GetDataLocation() { return get().m_DataLocation; }
    static std::vector<std::wstring>& GetMask() { return get().m_vMask; }
    static std::vector<std::wstring>& GetPath() { return get().m_vPath; }

    /// Loads options from XML file.
    ///
    /// \return true if it succeeds, false if it fails.
    static bool LoadOptions();

    /// Saves current options to XML file.
    ///
    /// \return true if it succeeds, false if it fails.
    static bool SaveOptions();

    /// Initialize XML file used for loading/saving options.
    ///
    /// \param  wsOptionsFilePath           Full pathname to the desired XML file.
    /// \param  bCreateDefaultIfNotExist    (optional) Create default XML if it doesn't exist.
    ///
    /// \return true if it succeeds, false if it fails.

    static bool Init(const std::wstring& wsOptionsFilePath, bool bCreateDefaultIfNotExist = true);
    static const std::wstring& GetOptionsFilePath() { return get().m_wsOptionsFilePath; }

private:
    /// All functions below made private to prohibit multiple object creation.
    Options() = default;
    Options(const Options& ref) = delete;
    Options& operator=(const Options& ref) = delete;

    static const size_t m_LIST_ITEMS_MAX_COUNT = 16;
    // Options file path
    std::wstring m_wsOptionsFilePath;
    // Find combobox
    std::vector<std::wstring> m_vFindHistory; //< Find combobox history items

    // Replace combobox
    std::vector<std::wstring> m_vReplaceHistory; //< Replace combobox history items
    // Extract options
    //  Extract mode
    en_ExtractMode m_ExtractMode = en_ExtractMode::ExtractInSingleFile;
    en_ExtractModeSingleFile m_ExtractModeSingleFile = en_ExtractModeSingleFile::PrettyPrint;
    std::wstring m_wsSeparator; 
    bool m_bAddHeader = false; 
    //  Save mode
    en_SaveMode m_SaveMode = en_SaveMode::ExtractToNotepad; 
    std::vector<std::wstring> m_vBasePath; 
    std::wstring  m_wsTemplateName;
    bool m_bOpenFilesInNotepad = false;
    //  Extract case conversion
    en_ExtractCaseConversion m_ExtractCaseConversion = en_ExtractCaseConversion::NoConversion;  
    //--
    bool m_bSkipWholeMatch = false; 
    // Search options
    //  Sort options
    en_SortMode m_SortMode = en_SortMode::NoSort; 
    //--
    bool m_bFilterUnique = false;  
    bool m_bCaseInsensitive = false;
    bool m_bDotMatchNewline = false;	
    // Data location
    en_DataLocation m_DataLocation = en_DataLocation::CurrentFile; 
    //--
    bool m_bInSelection = false;
    // Mask combobox
    std::vector<std::wstring> m_vMask;  
    // Path combobox
    std::vector<std::wstring> m_vPath;  

    /// Creates a default configuration in case of loading errors or no actual options present.
    static void CreateDefault();
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