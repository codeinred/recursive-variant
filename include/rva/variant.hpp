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

// See: https://en.cppreference.com/w/cpp/utility/variant
template <class... T>
class variant : public std::variant<replace_t<T, self_t, variant<T...>>...> {
   public:
    using base_type = std::variant<replace_t<T, self_t, variant<T...>>...>;
    constexpr static bool
        nothrow_swappable = std::is_nothrow_swappable_v<base_type>;

    using base_type::base_type;

    // Observers
    using base_type::index;
    using base_type::valueless_by_exception;

    // Modifiers
    using base_type::operator=;
    using base_type::emplace;
    using base_type::swap;

    variant() = default;
    variant(variant const&) = default;
    variant(variant&&) = default;
    variant& operator=(variant const&) = default;
    variant& operator=(variant&&) = default;

    constexpr void swap(variant& other) noexcept(nothrow_swappable) {
        base_type::swap(other);
    }
    constexpr base_type& get_base() & noexcept { return *this; }
    constexpr base_type const& get_base() const& noexcept { return *this; }
    constexpr base_type&& get_base() && noexcept { return *this; }
    constexpr base_type const&& get_base() const&& noexcept { return *this; }

    constexpr base_type* get_pointer_to_base() noexcept { return this; }
    constexpr base_type const* get_pointer_to_base() const noexcept {
        return this;
    }

    auto operator<=>(variant const&) const = default;
    bool operator==(variant const&) const = default;
};

// See: https://en.cppreference.com/w/cpp/utility/variant/visit
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

// See: https://en.cppreference.com/w/cpp/utility/variant/get
template <std::size_t I, class... Types>
constexpr decltype(auto) get(rva::variant<Types...>& v) {
    return std::get<I>(std::forward<decltype(v)>(v).get_base());
}
template <std::size_t I, class... Types>
constexpr decltype(auto) get(rva::variant<Types...>&& v) {
    return std::get<I>(std::forward<decltype(v)>(v).get_base());
}
template <std::size_t I, class... Types>
constexpr decltype(auto) get(const rva::variant<Types...>& v) {
    return std::get<I>(std::forward<decltype(v)>(v).get_base());
}
template <std::size_t I, class... Types>
constexpr decltype(auto) get(const rva::variant<Types...>&& v) {
    return std::get<I>(std::forward<decltype(v)>(v).get_base());
}
template <class T, class... Types>
constexpr T& get(rva::variant<Types...>& v) {
    return std::get<T>(std::forward<decltype(v)>(v).get_base());
}
template <class T, class... Types>
constexpr T&& get(rva::variant<Types...>&& v) {
    return std::get<T>(std::forward<decltype(v)>(v).get_base());
}
template <class T, class... Types>
constexpr const T& get(const rva::variant<Types...>& v) {
    return std::get<T>(std::forward<decltype(v)>(v).get_base());
}
template <class T, class... Types>
constexpr const T&& get(const rva::variant<Types...>&& v) {
    return std::get<T>(std::forward<decltype(v)>(v).get_base());
}

// See: https://en.cppreference.com/w/cpp/utility/variant/get_if
template <std::size_t I, class... Types>
constexpr auto* get_if(rva::variant<Types...>* pv) noexcept {
    return std::get_if<I>(pv->get_pointer_to_base());
}
template <std::size_t I, class... Types>
constexpr auto const* get_if(const rva::variant<Types...>* pv) noexcept {
    return std::get_if<I>(pv->get_pointer_to_base());
}
template <class T, class... Types>
constexpr auto* get_if(rva::variant<Types...>* pv) noexcept {
    return std::get_if<T>(pv->get_pointer_to_base());
}
template <class T, class... Types>
constexpr auto const* get_if(const rva::variant<Types...>* pv) noexcept {
    return std::get_if<T>(pv->get_pointer_to_base());
}

template <class T, class... Types>
constexpr bool holds_alternative(const rva::variant<Types...>& v) noexcept {
    return std::holds_alternative(v.get_base());
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

template <class... Types>
struct std::variant_size<rva::variant<Types...>>
  : std::integral_constant<std::size_t, sizeof...(Types)> {};
template <class... Types>
struct std::variant_size<const rva::variant<Types...>>
  : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <std::size_t I, class... Types>
struct std::variant_alternative<I, rva::variant<Types...>>
  : std::variant_alternative<I, typename rva::variant<Types...>::base_type> {};
template <std::size_t I, class... Types>
struct std::variant_alternative<I, const rva::variant<Types...>>
  : std::variant_alternative<I, typename rva::variant<Types...>::base_type> {};

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

// Add shortcut for rva::variant to avoid replacing into instances of an
// rva::variant that's given as a template parameter to another rva::variant
template <class... Ts, class Find, class Replace>
struct replace<rva::variant<Ts...>, Find, Replace> {
    using type = rva::variant<Ts...>;
};
} // namespace rva

#endif
