#include <catch2/catch_test_macros.hpp>

#include <map>
#include <string>
#include <vector>

#include <rva/variant.hpp>

using json_value = rva::variant<
    std::nullptr_t,
    std::string,
    double,
    bool,
    std::map<std::string_view, rva::self_t>,
    std::vector<rva::self_t>>;
