#include <catch2/catch_test_macros.hpp>
#include <rva/variant.hpp>
#include <vector>

TEST_CASE("Test replace") {

    STATIC_REQUIRE(std::is_same_v<int, rva::replace_t<char, char, int>>);
    STATIC_REQUIRE(std::is_same_v<int&, rva::replace_t<char&, char, int>>);
    STATIC_REQUIRE(std::is_same_v<int&&, rva::replace_t<char&&, char, int>>);
    STATIC_REQUIRE(std::is_same_v<const int, rva::replace_t<const char, char, int>>);
    STATIC_REQUIRE(std::is_same_v<const int&, rva::replace_t<const char&, char, int>>);
    STATIC_REQUIRE(std::is_same_v<const int&&, rva::replace_t<const char&&, char, int>>);
    STATIC_REQUIRE(std::is_same_v<
                   int const(&)[],
                   rva::replace_t<char const(&)[], char, int>>);
    STATIC_REQUIRE(
        std::is_same_v<int const&, rva::replace_t<char const&, char, int>>);
    STATIC_REQUIRE(
        std::is_same_v<int const*, rva::replace_t<char const*, char, int>>);
    STATIC_REQUIRE(
        std::is_same_v<int const*&, rva::replace_t<char const*&, char, int>>);
    STATIC_REQUIRE(
        std::is_same_v<int const**, rva::replace_t<char const**, char, int>>);
    STATIC_REQUIRE(
        std::is_same_v<int const**&, rva::replace_t<char const**&, char, int>>);
    STATIC_REQUIRE(std::is_same_v<
                   std::vector<int> const****&,
                   rva::replace_t<std::vector<char> const****&, char, int>>);
    STATIC_REQUIRE(std::is_same_v<
                   int const(&)[5],
                   rva::replace_t<char const(&)[5], char, int>>);
}
TEST_CASE("Catch2 Set up") { REQUIRE(true); }
