#include <catch2/catch_test_macros.hpp>

#include <map>
#include <string>
#include <vector>

#include <rva/variant.hpp>

using namespace std::string_view_literals;

using json_value = rva::variant<
    std::nullptr_t,                          // json null
    std::string,                             // json string
    double,                                  // json number
    bool,                                    // json true / false
    std::map<std::string_view, rva::self_t>, // json object
    std::vector<rva::self_t>>;               // json array

TEST_CASE("Test assignment", "[assign]") {
    json_value j1;
    json_value j2;

    j1 = nullptr;
    REQUIRE(j1 == nullptr);

    j1 = std::string("Hello, world!");
    REQUIRE(j1 == "Hello, world!");

    j1 = 1.0;
    REQUIRE(j1 == 1.0);

    j1 = false;
    REQUIRE(j1 == false);
    REQUIRE_FALSE(j1 == true);

    auto some_object = std::map<std::string_view, json_value> {
        {"hello", 10.0},
        {"goodbye", j1}};
    j1 = some_object;
    REQUIRE(j1 == some_object);

    j2 = j1;
    REQUIRE(j1 == j2);
}
