#ifndef RECURSIVE_VARIANT_AUTHORITY_VARIANT_HPP
#define RECURSIVE_VARIANT_AUTHORITY_VARIANT_HPP
#include <cstdint>

namespace rva {
/**
 * @brief replace is a template type used to implement replace_t. It provides
 * member, a using declaration named `type`.
 *
 * @tparam T the type to transform.
 * @tparam Find the type to find
 * @tparam Replace the type to replace it with.
 */
template <class T, class Find, class Replace>
struct replace;
/**
 * @brief replace is a template that takes a type T (which itself might be a
 * template), and replaces all instances of *Find* with *Replace*. For example:
 *
 * - `replace_t<char, char, int>` -> `int`
 * - `replace_t<std::vector<char>, char, int>` -> `std::vector<int>`
 * - `replace_t<std::variant<int, std::vector<self_t>>, self_t, rva::variant<int,
 * self_t>>` -> `std::variant<int, std::vector<rva::variant<int, self_t>>>`;
 *
 * @tparam T the type to transform.
 * @tparam Find the type to find
 * @tparam Replace the type to replace it with.
 */
template <class T, class Find, class Replace>
using replace_t = typename replace<T, Find, Replace>::type;


template <class T, class Find, class Replace>
struct replace {
    using type = T;
};
template <class Find, class Replace>
struct replace<Find, Find, Replace> {
    using type = Replace;
};
template <class Find, class Replace>
struct replace<Find*, Find, Replace> {
    using type = Replace*;
};
template <class Find, class Replace>
struct replace<Find&, Find, Replace> {
    using type = Replace&;
};
template <class Find, class Replace>
struct replace<Find&&, Find, Replace> {
    using type = Replace&&;
};
template <class Find, class Replace>
struct replace<Find[], Find, Replace> {
    using type = Replace[];
};
template <class Find, class Replace, std::size_t N>
struct replace<Find[N], Find, Replace> {
    using type = Replace[N];
};
template <class Find, class Replace>
struct replace<const Find, Find, Replace> {
    using type = const Replace;
};
template <class Find, class Replace>
struct replace<const Find*, Find, Replace> {
    using type = const Replace*;
};
template <class Find, class Replace>
struct replace<const Find&, Find, Replace> {
    using type = const Replace&;
};
template <class Find, class Replace>
struct replace<const Find[], Find, Replace> {
    using type = const Replace[];
};
template <class Find, class Replace, std::size_t N>
struct replace<const Find[N], Find, Replace> {
    using type = const Replace[N];
};
template <class T, class Find, class Replace>
struct replace<T*, Find, Replace> {
    using type = replace_t<T, Find, Replace>*;
};
template <class T, class Find, class Replace>
struct replace<T&, Find, Replace> {
    using type = replace_t<T, Find, Replace>&;
};
template <class T, class Find, class Replace>
struct replace<T&&, Find, Replace> {
    using type = replace_t<T, Find, Replace>&&;
};
template <class T, class Find, class Replace>
struct replace<T[], Find, Replace> {
    using type = replace_t<T, Find, Replace>[];
};
template <class T, class Find, class Replace, std::size_t N>
struct replace<T[N], Find, Replace> {
    using type = replace_t<T, Find, Replace>[N];
};
template <class T, class Find, class Replace>
struct replace<const T, Find, Replace> {
    using type = replace_t<T, Find, Replace> const;
};
template <class T, class Find, class Replace>
struct replace<const T*, Find, Replace> {
    using type = replace_t<T, Find, Replace> const*;
};
template <class T, class Find, class Replace>
struct replace<const T&, Find, Replace> {
    using type = replace_t<T, Find, Replace> const&;
};
template <class T, class Find, class Replace>
struct replace<const T[], Find, Replace> {
    using type = replace_t<T, Find, Replace> const[];
};
template <class T, class Find, class Replace, std::size_t N>
struct replace<const T[N], Find, Replace> {
    using type = replace_t<T, Find, Replace> const[N];
};

template <template <class...> class T, class... Ts, class Find, class Replace>
struct replace<T<Ts...>, Find, Replace> {
    using type = T<replace_t<Ts, Find, Replace>...>;
};
} // namespace rva

#endif
