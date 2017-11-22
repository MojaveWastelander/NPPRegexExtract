#include "main.h"
#include <IDataKind.hpp>
#include <DataPath.hpp>
#include <DataString.hpp>
#include <RegexSearch.hpp>
#include <OutputDataProcessorSingleFile.hpp>
#include <catch.hpp>




using namespace std::string_literals;
TEST_CASE("RegexSearch tests", "[RegexSearch]")
{
    using namespace std::string_literals;
    RegexSearch rxs;
    REQUIRE(rxs.GetMatchesCount() == 0);

    SECTION("Basic parse data tests")
    {
        DataKindString dks{L"123 𝝰bc 4𝝰6 def 7𝝰9"};
        OutputDataProcessorSingleFile osf{};
        REQUIRE(!rxs.ParseData(&dks, &osf)); // default class have empty regex
        rxs.ChangeExpression(L"𝝰"s);
        REQUIRE(rxs.ParseData(&dks, &osf));
        REQUIRE(rxs.GetMatchesCount() == 3);
    }


}
