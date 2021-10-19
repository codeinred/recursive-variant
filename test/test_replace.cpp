#include <catch2/catch_test_macros.hpp>
#include <rva/variant.hpp>
#include <vector>

TEST_CASE("Test replace") {

    STATIC_REQUIRE(std::is_same_v<int, rva::replace_t<char, char, int>>);
    STATIC_REQUIRE(std::is_same_v<int&, rva::replace_t<char&, char, int>>);
    STATIC_REQUIRE(std::is_same_v<int&&, rva::replace_t<char&&, char, int>>);
    STATIC_REQUIRE(
        std::is_same_v<const int, rva::replace_t<const char, char, int>>);
    STATIC_REQUIRE(
        std::is_same_v<const int&, rva::replace_t<const char&, char, int>>);
    STATIC_REQUIRE(
        std::is_same_v<const int&&, rva::replace_t<const char&&, char, int>>);
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
    STATIC_REQUIRE(
        std::is_same_v<
            std::vector<std::vector<int** const**>** const* const*> const****&,
            rva::replace_t<
                std::vector<
                    std::vector<char** const**>** const* const*> const****&,
                char,
                int>>);
    STATIC_REQUIRE(std::is_same_v<
                   int const(&)[5],
                   rva::replace_t<char const(&)[5], char, int>>);

    // Check recursive variants that are members of other recursive variants
    using V1 = rva::variant<int, std::vector<rva::self_t>>;
    using V2 = rva::variant<int, V1, std::vector<rva::self_t>>;

    STATIC_REQUIRE(
        std::is_same_v<V1, std::variant_alternative_t<1, V2>>);
}
