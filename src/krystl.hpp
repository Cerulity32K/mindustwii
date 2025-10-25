#pragma once

#include <optional>
#include "rusty.hpp"

namespace kstl {
template <typename T> class option : public std::optional<T> {
public:
    constexpr option(T&& value) : std::optional<T>{std::forward<T>(value)} {}
    constexpr option() : std::optional<T>{} {}

#pragma region map
    template <typename some_mapper>
    constexpr fn map(some_mapper map_fn
    ) & -> option<decltype(map_fn(this->value()))> {
        return this->has_value() ? map_fn(this->value()) : option{};
    }
    template <typename some_mapper>
    constexpr fn map(some_mapper map_fn
    ) const& -> option<decltype(map_fn(this->value()))> {
        return this->has_value() ? map_fn(this->value()) : option{};
    }
    template <typename some_mapper>
    constexpr fn map(some_mapper map_fn
    ) && -> option<decltype(map_fn(this->value()))> {
        return this->has_value() ? map_fn(std::move(this->value())) : option{};
    }
    template <typename some_mapper>
    constexpr fn map(some_mapper map_fn
    ) const&& -> option<decltype(map_fn(this->value()))> {
        return this->has_value() ? map_fn(std::move(this->value())) : option{};
    }
#pragma endregion

#pragma region then
    template <typename some_mapper>
    constexpr fn then(some_mapper map_fn) & -> void {
        if (this->has_value()) {
            map_fn(this->value());
        }
    }
    template <typename some_mapper>
    constexpr fn then(some_mapper map_fn) const& -> void {
        if (this->has_value()) {
            map_fn(this->value());
        }
    }
    template <typename some_mapper>
    constexpr fn then(some_mapper map_fn) && -> void {
        if (this->has_value()) {
            map_fn(std::move(this->value()));
        }
    }
    template <typename some_mapper>
    constexpr fn then(some_mapper map_fn) const&& -> void {
        if (this->has_value()) {
            map_fn(std::move(this->value()));
        }
    }
#pragma endregion

#pragma region value_or_else
    // TODO: constness/reference specialisations?
    template <typename none_mapper>
    constexpr fn value_or_else(none_mapper none_branch) const -> T
        requires(std::is_convertible_v<decltype(none_branch()), T>)
    {
        return this->has_value() ? this->value() : none_branch();
    }
#pragma endregion

#pragma region match
    template <
        typename some_mapper,
        typename none_mapper,
        typename U = decltype(std::declval<some_mapper>()(std::declval<T&>()))>
    constexpr fn match(some_mapper some_branch, none_mapper none_branch) & -> U
        requires(std::is_convertible_v<decltype(none_branch()), U>)
    {
        return this->has_value() ? some_branch(this->value()) : none_branch();
    }
    template <
        typename some_mapper,
        typename none_mapper,
        typename U =
            decltype(std::declval<some_mapper>()(std::declval<const T&>()))>
    constexpr fn
    match(some_mapper some_branch, none_mapper none_branch) const& -> U
        requires(std::is_convertible_v<decltype(none_branch()), U>)
    {
        return this->has_value() ? some_branch(this->value()) : none_branch();
    }
    template <
        typename some_mapper,
        typename none_mapper,
        typename U = decltype(std::declval<some_mapper>()(std::declval<T&&>()))>
    constexpr fn match(some_mapper some_branch, none_mapper none_branch) && -> U
        requires(std::is_convertible_v<decltype(none_branch()), U>)
    {
        return this->has_value() ? some_branch(std::move(this->value()))
                                 : none_branch();
    }
    template <
        typename some_mapper,
        typename none_mapper,
        typename U =
            decltype(std::declval<some_mapper>()(std::declval<const T&&>()))>
    constexpr fn
    match(some_mapper some_branch, none_mapper none_branch) const&& -> U
        requires(std::is_convertible_v<decltype(none_branch()), U>)
    {
        return this->has_value() ? some_branch(std::move(this->value()))
                                 : none_branch();
    }
#pragma endregion

    constexpr fn operator*() &->T& { return this->value(); }
    constexpr fn operator*() const&->const T& { return this->value(); }
    constexpr fn operator*() &&->T&& { return std::move(this->value()); }
    constexpr fn operator*() const&&->const T&& {
        return std::move(this->value());
    }
};

class none_t {
public:
    template <typename T> constexpr operator option<T>() const {
        return option<T>{};
    }
};

constexpr none_t none{};
template <typename T> constexpr option<T> some(T&& value) {
    return option<T>{std::forward<T>(value)};
}

} // namespace kstl
