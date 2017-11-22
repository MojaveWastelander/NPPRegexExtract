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
    REQUIRE(rxs.GetMatchesCount() == 0);

    SECTION("Basic parse data tests")
    {
        DataKindString dks{L"123 abc 456 def 789"};
        OutputDataProcessorSingleFile osf{};
        REQUIRE(!rxs.ParseData(&dks, &osf)); // default class have empty regex
        rxs.ChangeExpression(LR"((\d+) (\w+) (\d+))"s);
        Options::skip_whole_match() = true;

        REQUIRE(Options::skip_whole_match());
        REQUIRE(rxs.ParseData(&dks, &osf));
        REQUIRE(rxs.GetMatchesCount() == 1);
        REQUIRE(!osf.IsEmpty());
        REQUIRE(osf.GetProcessedData() == std::vector<std::wstring>{L"123"s, L"abc"s, L"456"});
    }
}