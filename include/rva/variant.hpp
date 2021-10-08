#ifndef RECURSIVE_VARIANT_AUTHORITY_VARIANT_HPP
#define RECURSIVE_VARIANT_AUTHORITY_VARIANT_HPP
#include <cstdint>
#include <variant>

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
 *
 * @tparam T the type to transform.
 * @tparam Find the type to find
 * @tparam Replace the type to replace it with.
 */
template <class T, class Find, class Replace>
using replace_t = typename replace<T, Find, Replace>::type;

struct self_t {};

template <class... T>
class variant : public std::variant<replace_t<T, self_t, variant<T...>>...> {
   public:
    using base_type = std::variant<replace_t<T, self_t, variant<T...>>...>;
    constexpr static bool
        nothrow_swappable = std::is_nothrow_swappable_v<base_type>;

    using base_type::base_type;

    variant() = default;
    variant(variant const&) = default;
    variant(variant&&) = default;
    variant& operator=(variant const&) = default;
    variant& operator=(variant&&) = default;

    using base_type::swap;
    constexpr void swap(variant& other) noexcept(nothrow_swappable) {
        base_type::swap(other);
    }
    base_type& get_base() & { return *this; }
    base_type const& get_base() const& { return *this; }
    base_type&& get_base() && { return *this; }
    base_type const&& get_base() const&& { return *this; }

    auto operator<=>(variant const&) const = default;
    bool operator==(variant const&) const = default;
};

template <class Visitor, class... Variants>
constexpr decltype(auto) visit(Visitor&& visitor, Variants&&... variants) {
    return std::visit(
        std::forward<Visitor>(visitor),
        std::forward<Variants>(variants).get_base()...);
}
template <class R, class Visitor, class... Variants>
constexpr R visit(Visitor&& visitor, Variants&&... variants) {
    return std::visit<R>(
        std::forward<Visitor>(visitor),
        std::forward<Variants>(variants).get_base()...);
}
} // namespace rva

template <class... T>
struct std::hash<rva::variant<T...>> : std::hash<std::variant<T...>> {
    using base_type = std::hash<std::variant<T...>>;
    using base_type::base_type;
    hash() = default;
    hash(hash const&) = default;
    hash(hash&&) = default;
    size_t operator()(rva::variant<T...> const& v) const {
        return base_type::operator()(v.get_base());
    }
};

// Implementation for replace
namespace rva {
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
