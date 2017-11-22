// Tests.cpp : Defines the entry point for the console application.
//

#include <catch.hpp>
#include <Options.hpp>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fmt\format.h>
#include <iostream>
#include <sstream>

namespace fs = std::experimental::filesystem;
std::wstring g_options_file_path = L"./config/options.xml";

TEST_CASE("Initialization checks", "[initialization]") 
{
    SECTION("Default values in class")
    {         
        REQUIRE((bool)(Options::extract_mode() == Options::en_ExtractMode::ExtractInSingleFile));
        REQUIRE((bool)(Options::extract_mode_single_file() == Options::en_ExtractModeSingleFile::PrettyPrint));
        REQUIRE((bool)(Options::save_mode() == Options::en_SaveMode::ExtractToNotepad));
        REQUIRE((bool)(Options::extract_case_conversion() == Options::en_ExtractCaseConversion::NoConversion));
        REQUIRE((bool)(Options::sort_mode() == Options::en_SortMode::NoSort));
        REQUIRE((bool)(Options::data_location() == Options::en_DataLocation::CurrentFile));
        REQUIRE(Options::find_history().empty());
        REQUIRE(Options::replace_history().empty());
        REQUIRE(Options::separator().empty());
        REQUIRE(Options::base_path().empty());
        REQUIRE(Options::template_name().empty());
        REQUIRE(Options::files_masks().empty());
        REQUIRE(Options::files_paths().empty());
        REQUIRE(!Options::add_header());
        REQUIRE(!Options::open_files_in_notepad());
        REQUIRE(!Options::skip_whole_match());
        REQUIRE(!Options::filter_unique());
        REQUIRE(!Options::case_insensitive());
        REQUIRE(!Options::dot_match_newline());
        REQUIRE(!Options::in_selection());

        REQUIRE(Options::options_file_path().empty());
    }

    SECTION("No option file initialization")
    {
        fs::remove(g_options_file_path);
        // With second argument as false and no options file exists - function should fail
        REQUIRE(!fs::exists(g_options_file_path));
        REQUIRE(!Options::initialize(g_options_file_path, false));
        REQUIRE(!fs::exists(g_options_file_path));

        // in this case it should create a new options file with default settings
        REQUIRE(Options::initialize(g_options_file_path));
        REQUIRE(fs::exists(g_options_file_path));
        
        // Check defaults as set by restore_default_settings
        REQUIRE(Options::separator() == std::wstring(L"\\t"));
        REQUIRE(Options::template_name() == std::wstring(L"Group_%d.txt"));
        REQUIRE((bool)(Options::extract_mode() == Options::en_ExtractMode::ExtractInSingleFile));
        REQUIRE((bool)(Options::extract_mode_single_file() == Options::en_ExtractModeSingleFile::PrettyPrint));
        REQUIRE((bool)(Options::save_mode() == Options::en_SaveMode::ExtractToNotepad));
        REQUIRE((bool)(Options::extract_case_conversion() == Options::en_ExtractCaseConversion::NoConversion));
        REQUIRE((bool)(Options::sort_mode() == Options::en_SortMode::NoSort));
        REQUIRE((bool)(Options::data_location() == Options::en_DataLocation::CurrentFile));
        REQUIRE(Options::find_history().empty());
        REQUIRE(Options::replace_history().empty());
        REQUIRE(Options::base_path().empty());
        REQUIRE(Options::files_masks().empty());
        REQUIRE(Options::files_paths().empty());
        REQUIRE(!Options::add_header());
        REQUIRE(!Options::open_files_in_notepad());
        REQUIRE(!Options::skip_whole_match());
        REQUIRE(!Options::filter_unique());
        REQUIRE(!Options::case_insensitive());
        REQUIRE(!Options::dot_match_newline());
        REQUIRE(!Options::in_selection());
    }

}

TEST_CASE("Save\\load checks", "[save_load]")
{
    SECTION("Check that settings are preserved from previous test case")
    {
        REQUIRE(fs::exists(g_options_file_path));

        // Check defaults as set by restore_default_settings
        REQUIRE(Options::separator() == std::wstring(L"\\t"));
        REQUIRE(Options::template_name() == std::wstring(L"Group_%d.txt"));
    }

    SECTION("Change all fields and save them")
    {
        Options::extract_mode() = Options::en_ExtractMode::ExtractWithReplace;
        Options::extract_mode_single_file() = Options::en_ExtractModeSingleFile::WithSeparator;
        Options::save_mode() = Options::en_SaveMode::SaveAsNewFile;
        Options::extract_case_conversion() = Options::en_ExtractCaseConversion::AllLowercase;
        Options::sort_mode() = Options::en_SortMode::SortAscending;
        Options::data_location() = Options::en_DataLocation::SpecificFiles;
        long long i = 0;
        Options::find_history().push_back(std::to_wstring(i++));
        Options::replace_history().push_back(std::to_wstring(i++));
        Options::separator() = std::to_wstring(i++);
        Options::base_path().push_back(std::to_wstring(i++));
        Options::template_name() = std::to_wstring(i++);
        Options::files_masks().push_back(std::to_wstring(i++));
        Options::files_paths().push_back(std::to_wstring(i++));
        Options::add_header() = true;
        Options::open_files_in_notepad() = true;
        Options::skip_whole_match() = true;
        Options::filter_unique() = true;
        Options::case_insensitive() = true;
        Options::dot_match_newline() = true;
        Options::in_selection() = true;
        Options::save_options();

        // Revert changes
        Options::extract_mode() = Options::en_ExtractMode::ExtractInSingleFile;
        Options::extract_mode_single_file() = Options::en_ExtractModeSingleFile::WithSeparator;
        Options::save_mode() = Options::en_SaveMode::ExtractToNotepad;
        Options::extract_case_conversion() = Options::en_ExtractCaseConversion::NoConversion;
        Options::sort_mode() = Options::en_SortMode::NoSort;
        Options::data_location() = Options::en_DataLocation::CurrentFile;
        Options::find_history().clear();
        Options::replace_history().clear();
        Options::separator().clear();
        Options::base_path().clear();
        Options::template_name().clear();
        Options::files_masks().clear();
        Options::files_paths().clear();
        Options::add_header() = false;
        Options::open_files_in_notepad() = false;
        Options::skip_whole_match() = false;
        Options::filter_unique() = false;
        Options::case_insensitive() = false;
        Options::dot_match_newline() = false;
        Options::in_selection() = false;
    }

    auto saved_values_check = [&]
    {
        REQUIRE((bool)(Options::extract_mode() == Options::en_ExtractMode::ExtractWithReplace));
        REQUIRE((bool)(Options::extract_mode_single_file() == Options::en_ExtractModeSingleFile::WithSeparator));
        REQUIRE((bool)(Options::save_mode() == Options::en_SaveMode::SaveAsNewFile));
        REQUIRE((bool)(Options::extract_case_conversion() == Options::en_ExtractCaseConversion::AllLowercase));
        REQUIRE((bool)(Options::sort_mode() == Options::en_SortMode::SortAscending));
        REQUIRE((bool)(Options::data_location() == Options::en_DataLocation::SpecificFiles));
        long long i = 0;
        REQUIRE(Options::find_history().back() == std::to_wstring(i++));
        REQUIRE(Options::replace_history().back() == std::to_wstring(i++));
        REQUIRE(Options::separator() == std::to_wstring(i++));
        REQUIRE(Options::base_path().back() == std::to_wstring(i++));
        REQUIRE(Options::template_name() == std::to_wstring(i++));
        REQUIRE(Options::files_masks().back() == std::to_wstring(i++));
        REQUIRE(Options::files_paths().back() == std::to_wstring(i++));
        REQUIRE(Options::add_header());
        REQUIRE(Options::open_files_in_notepad());
        REQUIRE(Options::skip_whole_match());
        REQUIRE(Options::filter_unique());
        REQUIRE(Options::case_insensitive());
        REQUIRE(!Options::dot_match_newline()); // Setting is not saved
        REQUIRE(Options::in_selection());
    };

    SECTION("Load saved settings and check that they are as initialy modified")
    {
        Options::load_options();
        saved_values_check();
    }

    SECTION("Remove options file and check that loading fails")
    {
        // Removing options file should not affect current state
        fs::remove(g_options_file_path);
        REQUIRE(!Options::load_options());
        saved_values_check();
    
        // create new empty options file
        // if a field fails to be loaded it will keep old value
        std::wofstream f{g_options_file_path};
        REQUIRE(Options::load_options());
        saved_values_check();

        // Save stored settings, values should stay the same
        Options::save_options();
        saved_values_check();
    }

    SECTION("List type settings should save and load in the same order")
    {
        Options::find_history().clear();
        
        int i = 0;
        std::generate_n(std::back_inserter(Options::find_history()), 16, 
                        [&i]
                        {
                            return fmt::format(L"[A-Z]+_{}", i++);
                        });
        auto v = Options::find_history();
        Options::save_options();
        
        Options::find_history().clear();
        Options::load_options();
        REQUIRE(Options::find_history() == v);
    }
}
