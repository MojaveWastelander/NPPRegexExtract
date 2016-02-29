// Tests.cpp : Defines the entry point for the console application.
//

#include <catch.hpp>
#include <RegexExtract\Options.hpp>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cppformat\format.h>

namespace fs = std::experimental::filesystem;
std::wstring g_options_file_path = L"./options.xml";

TEST_CASE("Initialization checks", "[initialization]") 
{
    SECTION("Default values in class")
    {
        REQUIRE((bool)(Options::GetExtractMode() == Options::en_ExtractMode::ExtractInSingleFile));
        REQUIRE((bool)(Options::GetExtractModeSingleFile() == Options::en_ExtractModeSingleFile::PrettyPrint));
        REQUIRE((bool)(Options::GetSaveMode() == Options::en_SaveMode::ExtractToNotepad));
        REQUIRE((bool)(Options::GetExtractCaseConversion() == Options::en_ExtractCaseConversion::NoConversion));
        REQUIRE((bool)(Options::GetSortMode() == Options::en_SortMode::NoSort));
        REQUIRE((bool)(Options::GetDataLocation() == Options::en_DataLocation::CurrentFile));
        REQUIRE(Options::GetFindHistory().empty());
        REQUIRE(Options::GetReplaceHistory().empty());
        REQUIRE(Options::GetSeparator().empty());
        REQUIRE(Options::GetBasePath().empty());
        REQUIRE(Options::GetTemplateName().empty());
        REQUIRE(Options::GetMask().empty());
        REQUIRE(Options::GetPath().empty());
        REQUIRE(!Options::GetAddHeader());
        REQUIRE(!Options::GetOpenFilesInNotepad());
        REQUIRE(!Options::GetSkipWholeMatch());
        REQUIRE(!Options::GetFilterUnique());
        REQUIRE(!Options::GetCaseInsensitive());
        REQUIRE(!Options::GetDotMatchNewline());
        REQUIRE(!Options::GetInSelection());

        REQUIRE(Options::GetOptionsFilePath().empty());
    }

    SECTION("No option file initialization")
    {
        fs::remove(g_options_file_path);
        // With second argument as false and no options file exists - function should fail
        REQUIRE(!fs::exists(g_options_file_path));
        REQUIRE(!Options::Init(g_options_file_path, false));
        REQUIRE(!fs::exists(g_options_file_path));

        // in this case it should create a new options file with default settings
        REQUIRE(Options::Init(g_options_file_path));
        REQUIRE(fs::exists(g_options_file_path));
        
        // Check defaults as set by CreateDefault
        REQUIRE(Options::GetSeparator() == std::wstring(L"\\t"));
        REQUIRE(Options::GetTemplateName() == std::wstring(L"Group_%d.txt"));
        REQUIRE((bool)(Options::GetExtractMode() == Options::en_ExtractMode::ExtractInSingleFile));
        REQUIRE((bool)(Options::GetExtractModeSingleFile() == Options::en_ExtractModeSingleFile::PrettyPrint));
        REQUIRE((bool)(Options::GetSaveMode() == Options::en_SaveMode::ExtractToNotepad));
        REQUIRE((bool)(Options::GetExtractCaseConversion() == Options::en_ExtractCaseConversion::NoConversion));
        REQUIRE((bool)(Options::GetSortMode() == Options::en_SortMode::NoSort));
        REQUIRE((bool)(Options::GetDataLocation() == Options::en_DataLocation::CurrentFile));
        REQUIRE(Options::GetFindHistory().empty());
        REQUIRE(Options::GetReplaceHistory().empty());
        REQUIRE(Options::GetBasePath().empty());
        REQUIRE(Options::GetMask().empty());
        REQUIRE(Options::GetPath().empty());
        REQUIRE(!Options::GetAddHeader());
        REQUIRE(!Options::GetOpenFilesInNotepad());
        REQUIRE(!Options::GetSkipWholeMatch());
        REQUIRE(!Options::GetFilterUnique());
        REQUIRE(!Options::GetCaseInsensitive());
        REQUIRE(!Options::GetDotMatchNewline());
        REQUIRE(!Options::GetInSelection());
    }

}

TEST_CASE("Save\\load checks", "[save_load]")
{
    SECTION("Check that settings are preserved from previous test case")
    {
        REQUIRE(fs::exists(g_options_file_path));

        // Check defaults as set by CreateDefault
        REQUIRE(Options::GetSeparator() == std::wstring(L"\\t"));
        REQUIRE(Options::GetTemplateName() == std::wstring(L"Group_%d.txt"));
    }

    SECTION("Change all fields and save them")
    {
        Options::GetExtractMode() = Options::en_ExtractMode::ExtractWithReplace;
        Options::GetExtractModeSingleFile() = Options::en_ExtractModeSingleFile::WithSeparator;
        Options::GetSaveMode() = Options::en_SaveMode::SaveAsNewFile;
        Options::GetExtractCaseConversion() = Options::en_ExtractCaseConversion::AllLowercase;
        Options::GetSortMode() = Options::en_SortMode::SortAscending;
        Options::GetDataLocation() = Options::en_DataLocation::SpecificFiles;
        long long i = 0;
        Options::GetFindHistory().push_back(std::to_wstring(i++));
        Options::GetReplaceHistory().push_back(std::to_wstring(i++));
        Options::GetSeparator() = std::to_wstring(i++);
        Options::GetBasePath().push_back(std::to_wstring(i++));
        Options::GetTemplateName() = std::to_wstring(i++);
        Options::GetMask().push_back(std::to_wstring(i++));
        Options::GetPath().push_back(std::to_wstring(i++));
        Options::GetAddHeader() = true;
        Options::GetOpenFilesInNotepad() = true;
        Options::GetSkipWholeMatch() = true;
        Options::GetFilterUnique() = true;
        Options::GetCaseInsensitive() = true;
        Options::GetDotMatchNewline() = true;
        Options::GetInSelection() = true;
        Options::SaveOptions();

        // Revert changes
        Options::GetExtractMode() = Options::en_ExtractMode::ExtractInSingleFile;
        Options::GetExtractModeSingleFile() = Options::en_ExtractModeSingleFile::WithSeparator;
        Options::GetSaveMode() = Options::en_SaveMode::ExtractToNotepad;
        Options::GetExtractCaseConversion() = Options::en_ExtractCaseConversion::NoConversion;
        Options::GetSortMode() = Options::en_SortMode::NoSort;
        Options::GetDataLocation() = Options::en_DataLocation::CurrentFile;
        Options::GetFindHistory().clear();
        Options::GetReplaceHistory().clear();
        Options::GetSeparator().clear();
        Options::GetBasePath().clear();
        Options::GetTemplateName().clear();
        Options::GetMask().clear();
        Options::GetPath().clear();
        Options::GetAddHeader() = false;
        Options::GetOpenFilesInNotepad() = false;
        Options::GetSkipWholeMatch() = false;
        Options::GetFilterUnique() = false;
        Options::GetCaseInsensitive() = false;
        Options::GetDotMatchNewline() = false;
        Options::GetInSelection() = false;
    }

    auto saved_values_check = [&]
    {
        REQUIRE((bool)(Options::GetExtractMode() == Options::en_ExtractMode::ExtractWithReplace));
        REQUIRE((bool)(Options::GetExtractModeSingleFile() == Options::en_ExtractModeSingleFile::WithSeparator));
        REQUIRE((bool)(Options::GetSaveMode() == Options::en_SaveMode::SaveAsNewFile));
        REQUIRE((bool)(Options::GetExtractCaseConversion() == Options::en_ExtractCaseConversion::AllLowercase));
        REQUIRE((bool)(Options::GetSortMode() == Options::en_SortMode::SortAscending));
        REQUIRE((bool)(Options::GetDataLocation() == Options::en_DataLocation::SpecificFiles));
        long long i = 0;
        REQUIRE(Options::GetFindHistory().back() == std::to_wstring(i++));
        REQUIRE(Options::GetReplaceHistory().back() == std::to_wstring(i++));
        REQUIRE(Options::GetSeparator() == std::to_wstring(i++));
        REQUIRE(Options::GetBasePath().back() == std::to_wstring(i++));
        REQUIRE(Options::GetTemplateName() == std::to_wstring(i++));
        REQUIRE(Options::GetMask().back() == std::to_wstring(i++));
        REQUIRE(Options::GetPath().back() == std::to_wstring(i++));
        REQUIRE(Options::GetAddHeader());
        REQUIRE(Options::GetOpenFilesInNotepad());
        REQUIRE(Options::GetSkipWholeMatch());
        REQUIRE(Options::GetFilterUnique());
        REQUIRE(Options::GetCaseInsensitive());
        REQUIRE(!Options::GetDotMatchNewline()); // Setting is not saved
        REQUIRE(Options::GetInSelection());
    };

    SECTION("Load saved settings and check that they are as initialy modified")
    {
        Options::LoadOptions();
        saved_values_check();
    }

    SECTION("Remove options file and check that loading fails")
    {
        // Removing options file should not affect current state
        fs::remove(g_options_file_path);
        REQUIRE(!Options::LoadOptions());
        saved_values_check();
    
        // create new empty options file
        // if a field fails to be loaded it will keep old value
        std::wofstream f{g_options_file_path};
        REQUIRE(Options::LoadOptions());
        saved_values_check();

        // Save stored settings, values should stay the same
        Options::SaveOptions();
        saved_values_check();
    }

    SECTION("List type settings should save and load in the same order")
    {
        Options::GetFindHistory().clear();
        
        int i = 0;
        std::generate_n(std::back_inserter(Options::GetFindHistory()), 16, 
                        [&i]
                        {
                            return fmt::format(L"[A-Z]+_{}", i++);
                        });
        auto v = Options::GetFindHistory();
        Options::SaveOptions();
        
        Options::GetFindHistory().clear();
        Options::LoadOptions();
        REQUIRE(Options::GetFindHistory() == v);
    }
}
