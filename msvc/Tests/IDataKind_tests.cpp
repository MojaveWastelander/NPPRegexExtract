#include "main.h"
#include <IDataKind.hpp>
#include <DataPath.hpp>
#include <DataString.hpp>
#include <catch.hpp>

namespace fs = std::experimental::filesystem;
// Sample empty class derived from IDataKind for basic tests    
class DataKind :
    public IDataKind
{
    void GetData() override { m_wsRawTextData = L"123456"; }
public:
    DataKind(const std::wstring& ws) :
        IDataKind{ws} 
    {
        GetData();
    }
};

// Sample text
std::wstring test_val = LR"(//  Use
, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  Use
, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
)";

TEST_CASE("IDataKind basic tests", "[IDataKind]")
{
    using namespace std::string_literals;
    auto def_val = L"xyz"s;
    std::unique_ptr<IDataKind> up = std::make_unique<DataKind>(def_val);
    REQUIRE(up->data_location() == def_val);
    REQUIRE(up->raw_text_data() == L"123456"s);   

    REQUIRE(!up->IsMatched());
    up->SetMatched(true);
    REQUIRE(up->IsMatched());

    REQUIRE(!up->IsParsed());
    up->SetParsed();
    REQUIRE(up->IsParsed());
}

TEST_CASE("DataKindPath tests", "[DataKindPath]")
{

    std::wstring file_path = L"test_file.txt";

    SECTION("Prepare test data")
    {
        fs::remove(file_path);
        REQUIRE(!fs::exists(file_path));
        { std::wofstream{file_path} << test_val; }
        REQUIRE(fs::exists(file_path));
    }

    SECTION("Class should return initial data read from file")
    {
        DataKindPath dp{file_path};
        REQUIRE(dp.raw_text_data() == test_val);
    }
}

TEST_CASE("DataKindString tests", "[DataKindString]")
{

    DataKindString ds{test_val};
    SECTION("Default construction from string")
    {
        REQUIRE(ds.data_location().empty());
        REQUIRE(ds.raw_text_data() == test_val);
    }

    SECTION("Move construction")
    {
        DataKindString dsm{std::move(ds)};
        REQUIRE(ds.raw_text_data().empty());
        REQUIRE(dsm.raw_text_data() == test_val);
    }
}