#include <catch2/catch_test_macros.hpp>

#include <map>
#include <string>
#include <type_traits>
#include <utility>
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

// Regression test for issue #3: a recursive reference inside std::pair (or
// std::map, which stores pairs) failed to compile on MSVC
using pair_value = rva::variant<
    int,
    std::map<std::string, rva::self_t>,
    std::vector<std::pair<std::string, rva::self_t>>>;

TEST_CASE("Test swap with std::pair", "[swap]") {
    // swap must stay noexcept when every alternative is nothrow swappable
    using list_value = rva::variant<int, std::vector<rva::self_t>>;
    STATIC_REQUIRE(noexcept(
        std::declval<list_value&>().swap(std::declval<list_value&>())));

    pair_value v1 = 1;
    pair_value v2 = std::vector<std::pair<std::string, pair_value>> {
        {"one", 1},
        {"two", 2}};
    v1.swap(v2);
    REQUIRE(v2 == 1);
    REQUIRE(std::get<2>(v1).size() == 2);
    REQUIRE(std::get<2>(v1)[1].second == 2);

    // Evaluating the noexcept specification of swap for pair_value is what
    // used to fail; it must match the trait of the underlying std::variant
    STATIC_REQUIRE(
        noexcept(std::declval<pair_value&>().swap(std::declval<pair_value&>()))
        == std::is_nothrow_swappable_v<pair_value::base_type>);

    pair_value m1 = std::map<std::string, pair_value> {{"a", 1}};
    pair_value m2 = std::map<std::string, pair_value> {{"b", 2}, {"c", 3}};
    m1.swap(m2);
    REQUIRE(std::get<1>(m1).size() == 2);
    REQUIRE(std::get<1>(m1).at("c") == 3);
    REQUIRE(std::get<1>(m2).at("a") == 1);
}
