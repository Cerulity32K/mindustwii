# Mindustwii
A (currently unfinished) factory game for the Wii.

Heavily inspired by Mindustry.

The focus of this project is more on the codebase than on the product. I did quite a few experiments.

## `let`, `mut`, and `fn`: "Fun" C++ Quirks
In the codebase you will find three common macros: `let`, `mut`, and `fn`. In C++, you can declare a variable with `auto` and have a type deduced from the assigned expression. I aliased `let` to `const auto` and `mut` to `auto`, so most variable declarations become a simple `let name = expression;`. Simple. What's more interesting is the `fn` macro.

Take the following template:
```cpp
template<typename L, typename R> ? add(L lhs, R rhs) { return lhs + rhs; }
```
How would you specify a return type? One attempt could be using `decltype`:
```cpp
template<typename L, typename R> decltype(lhs + rhs) add(L lhs, R rhs) { return lhs + rhs; }
```
But this doesn't work. In the position of the `decltype`, `lhs` and `rhs` have not yet been defined. There is a way out in here, though it is verbose:
```cpp
template<typename L, typename R> decltype(std::declval<L>() + std::declval<R>()) add(L lhs, R rhs) { return lhs + rhs; }
```
This can get quite messy with more complex expressions, so instead, syntax was added to C++ to allow return types to be specified *after* function parameters by specifying an `auto` leading return type:
```cpp
template<typename L, typename R> auto add(L lhs, R rhs) -> decltype(lhs + rhs) { return lhs + rhs; }
```
`lhs` and `rhs` are declared at the time of the `decltype`, so it's valid! Though, this syntax can be used without templates or `decltype`s at all:
```cpp
auto add(int x, int y) -> int { return x + y; }
```
So, my `fn` macro aliases to `auto`, giving this syntax:
```cpp
fn answer() -> int {
    return 42;
}
```

Overall, I prefer this style of syntax greatly over traditional declarations, which is why I experimented with it, seeing how far I can take it in this codebase.

## STL Augmentations
In this codebase, there exists a header called `krystl.hpp`. This is my extension of the C++ STL. Few types have been extended, but most notably, `std::optional` is replaced with `kstl::option`. This type contains helper functions such as `map`, `match`, `then`, etc., as well as a safer dereference operator.

One thing I will need to look into more is const and lvalue/rvalue qualifiers (`&`, `&&`, `const&`, and `const&&` member function qualifiers), and where they should and shouldn't be used.

## Compiling
You will need devkitPPC to compile. The codebase uses the `gnu++20` standard. Run the included makefile to compile the game.

## Controls
- Nunchuk joystick (or D-pad if not connected) - Fly
- A - Place small (1x1) block at cursor
- B - Place big (2x2) block at cursor (bottom left corner)
- Minus - Destroy block at cursor
