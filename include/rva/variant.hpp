#ifndef RECURSIVE_VARIANT_AUTHORITY_VARIANT_HPP
#define RECURSIVE_VARIANT_AUTHORITY_VARIANT_HPP
#include <cstdint>

namespace rva {

template <class T, class Find, class Replace>
struct replace {
    using type = T;
};

template <class T, class Find, class Replace>
using replace_t = typename replace<T, Find, Replace>::type;

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
