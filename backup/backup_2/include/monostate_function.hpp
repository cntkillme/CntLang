#pragma once

#include <functional>

// from: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0477r0.pdf

template<auto Callable>
struct monostate_function
{
	using value_type = decltype(Callable);
	static value_type constexpr value = Callable;

	template<class... P>
	constexpr decltype(auto) operator()(P&&... args) const
	noexcept(noexcept(std::invoke(Callable, std::declval<P>()...)))
	{
		return std::invoke(Callable, std::forward<P>(args)...);
	}
};

template<auto Callable>
constexpr bool operator==(monostate_function<Callable>, monostate_function<Callable>) noexcept
{
	return true;
}

template<auto Callable>
constexpr bool operator!=(monostate_function<Callable>, monostate_function<Callable>) noexcept
{
	return false;
}
