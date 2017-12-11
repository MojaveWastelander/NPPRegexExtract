#include "main.h"
#include <IDataKind.hpp>
#include <DataPath.hpp>
#include <DataString.hpp>
#include <RegexSearch.hpp>
#include <OutputDataProcessorSingleFile.hpp>
#include <regex>
#include <catch.hpp>




using namespace std::string_literals;
TEST_CASE("OutputDataProcessorSingleFile tests", "[OutputDataProcessorSingleFile]")
{
    using namespace std::string_literals;
    RegexSearch rxs;
    DataKindString dks{LR"(123 abc 456 def 789 xyz 123 abc 456 def 789)"};
    REQUIRE(rxs.GetMatchesCount() == 0);

    SECTION("Multiple lines, whole match")
    {
        OutputDataProcessorSingleFile osf{};
        rxs.ChangeExpression(LR"(\d+)");
        Options::skip_whole_match() = false;  
        REQUIRE(rxs.ParseData(&dks, &osf));
        REQUIRE(rxs.GetMatchesCount() == 6);
        REQUIRE(osf.GetProcessedData().back() == L"123\r\n456\r\n789\r\n123\r\n456\r\n789"s);
    }

    SECTION("Multiple lines, whole match, unique")
    {
        OutputDataProcessorSingleFile osf{};
        rxs.ChangeExpression(LR"(\d+)");
        Options::skip_whole_match() = false;  
        Options::filter_unique() = true;  
        REQUIRE(rxs.ParseData(&dks, &osf));
        REQUIRE(rxs.GetMatchesCount() == 6); // all matches count before filtering
        REQUIRE(osf.GetProcessedData().back() == L"123\r\n456\r\n789"s);
    }

    SECTION("Multiple lines, whole match, unique, sort descending")
    {
        OutputDataProcessorSingleFile osf{};
        rxs.ChangeExpression(LR"(\d+)");
        Options::skip_whole_match() = false;  
        Options::filter_unique() = true;  
        Options::sort_mode() = Options::en_SortMode::SortDescending;
        REQUIRE(rxs.ParseData(&dks, &osf));
        REQUIRE(rxs.GetMatchesCount() == 6); // all matches count before filtering
        REQUIRE(osf.GetProcessedData().back() == L"789\r\n456\r\n123"s);
    }

    SECTION("Multiple match groups, multiple lines")
    {
        OutputDataProcessorSingleFile osf{};
        REQUIRE(!rxs.ParseData(&dks, &osf)); // default class have empty regex
        rxs.ChangeExpression(LR"((\d+) (\w+) (\d+))"s);
        Options::skip_whole_match() = true;  
        Options::sort_mode() = Options::en_SortMode::NoSort;
        Options::filter_unique() = false;  
        REQUIRE(rxs.ParseData(&dks, &osf));
        REQUIRE(rxs.GetMatchesCount() == 3);
        REQUIRE(!osf.IsEmpty());
        REQUIRE(osf.GetProcessedData().back() == L"123    abc    456\r\n789    xyz    123\r\n456    def    789"s);

    }

    Options::restore_default_settings();
}