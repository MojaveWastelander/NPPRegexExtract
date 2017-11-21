#include "main.h"
#include <IDataKind.hpp>
#include <DataPath.hpp>
#include <DataString.hpp>
#include <catch.hpp>

namespace fs = std::experimental::filesystem;
// Create a sample empty class derived from IDataKind

class DataKind :
    public IDataKind
{
    void GetData() { m_wsRawTextData = L"123456"; }
public:
    DataKind(const std::wstring& ws) :
        IDataKind{ws} 
    {
        GetData();
    }
};

TEST_CASE("IDataKind basic tests", "[IDataKind]")
{
    using namespace std::string_literals;
    auto def_val = L"xyz"s;
    std::unique_ptr<IDataKind> up = std::make_unique<DataKind>(def_val);
    REQUIRE(up->GetRef() == def_val);
    REQUIRE(up->GetRawTextData() == L"123456"s);   

    REQUIRE(!up->IsMatched());
    up->SetMatched(true);
    REQUIRE(up->IsMatched());

    REQUIRE(!up->IsParsed());
    up->SetParsed();
    REQUIRE(up->IsParsed());
}

TEST_CASE("DataKindPath tests", "[DataKindPath]")
{
    std::wstring test_val = LR"(//  Use
, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  Use
, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
)";
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
        REQUIRE(dp.GetRawTextData() == test_val);
    }
}

TEST_CASE("DataKindString tests", "[DataKindString]")
{
    std::wstring test_val = LR"(//  Use
, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  Use
, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
)";

    // Test copy construction
    DataKindString ds{test_val};
    REQUIRE(ds.GetRef().empty());
    REQUIRE(ds.GetRawTextData() == test_val);

    // Test move construction
    DataKindString dsm{std::move(test_val)};
    REQUIRE(dsm.GetRef().empty());
    REQUIRE(test_val.empty());
    REQUIRE(dsm.GetRawTextData() == ds.GetRawTextData());
}