//https://godbolt.org/z/xMs61rxGx
#include <concepts>
#include <optional>
#include <functional>

struct image{ int i = 0; };
struct raw_image{ int i = 0; };
struct gif{ int i = 0; };
std::optional<raw_image> crop_to_cat(const image& img) { return raw_image{img.i + 1}; }
std::optional<gif> add_bow_tie(const raw_image& img) { return gif{img.i + 1}; }
std::optional<image> make_eyes_sparkle(gif&& img) { return image{img.i + 1}; }
image make_smaller(image&& img) { return image{img.i + 1}; }
std::optional<image> add_rainbow(const image& img) { return image{img.i + 1}; }

//operator| instead of std::invoke at the start of a chain
template<typename T, typename Callable>
requires std::invocable<Callable, const T&> || std::invocable<Callable, T&>
auto operator|(T&& t, Callable&& fn) {
    return std::invoke(std::forward<Callable>(fn), std::forward<T>(t));
}

template<typename T>
concept has_value_optionally = requires(T opt) {
        typename T::value_type;
        { opt.has_value() } -> std::same_as<bool>;
        opt.value();
        *opt;
        //Skipped below - not needed for this code. How this can be specified is beyond me for now!
        //IMHO, member (template) functions taking arbitrary args... is unspecifiable with C++20???
        //opt.value_or(auto...);
    };

template<typename T>
concept Optional = has_value_optionally<T> 
    && std::constructible_from<T, typename T::value_type>
    && std::constructible_from<T, std::nullopt_t>;

//operator| instead of std::optional<T>::and_then() to chain operations
template<Optional Opt, typename Callable>
requires std::invocable<Callable, typename Opt::value_type&&>
    && Optional<std::invoke_result_t<Callable, typename Opt::value_type&&>>
auto operator|(Opt&& opt, Callable&& fn) {
    return opt.has_value()?
        std::invoke(std::forward<Callable>(fn), std::forward<typename Opt::value_type&&>(*std::move(opt))):
        std::nullopt;
}

//operator| instead of std::optional<T>::transform() to chain transformations
template<Optional Opt, typename Callable>
requires std::invocable<Callable, typename Opt::value_type&&> 
    && (!Optional<std::invoke_result_t<Callable, typename Opt::value_type&&>>)
auto operator|(Opt&& opt, Callable&& fn) -> std::optional<std::invoke_result_t<Callable, typename Opt::value_type&&>> {
    return opt.has_value()?
        std::make_optional(std::invoke(std::forward<Callable>(fn), std::forward<typename Opt::value_type&&>(*std::move(opt)))):
        std::nullopt;
}

//operator| instead of std::optional<T>::or_else() to chain operations
template<Optional Opt, typename Callable>
requires std::invocable<Callable>
    && std::convertible_to<std::invoke_result_t<Callable>, typename Opt::value_type>
typename Opt::value_type operator|(Opt&& opt, Callable&& fn) {
    return opt.has_value()? *std::move(opt): std::invoke(std::forward<Callable>(fn));
}

std::optional<image> get_cute_cat(const image& img) {
    return img  | crop_to_cat 
                | add_bow_tie 
                | make_eyes_sparkle 
                | make_smaller 
                | add_rainbow;
}

/*
Working with concrete type std::optional
//operator| instead of std::optional<T>::and_then() to chain operations
template<typename T, typename Callable>
requires std::invocable<Callable, T&&> && Optional<std::invoke_result_t<Callable, T&&>>
auto operator|(std::optional<T>&& opt, Callable&& fn) {
    return opt.has_value()?
        std::invoke(std::forward<Callable>(fn), std::forward<T&&>(*std::move(opt))):
        std::nullopt;
}

//operator| instead of std::optional<T>::transform() to chain transformations
template<typename T, typename Callable>
requires std::invocable<Callable, T&&> && (!Optional<std::invoke_result_t<Callable, T&&>>)
auto operator|(std::optional<T>&& opt, Callable&& fn) -> std::optional<std::invoke_result_t<Callable, T&&>> {
    return opt.has_value()?
        std::make_optional(std::invoke(std::forward<Callable>(fn), std::forward<T&&>(*std::move(opt)))):
        std::nullopt;
}

//operator| instead of std::optional<T>::or_else() to chain operations
template<typename T, typename Callable>
requires std::invocable<Callable> && std::convertible_to<std::invoke_result_t<Callable>, T>
T operator|(std::optional<T>&& opt, Callable&& fn) {
    return opt.has_value()?
        *std::move(opt):
        std::invoke(std::forward<Callable>(fn));
}
*/

/*
Initial attempts & other concepts
template<typename Opt, typename T>
concept is_optional_of = Optional<Opt> && std::same_as<typename Opt::value_type, T>;

template<typename T, typename Callable, typename U>
concept transforms_T_to_U_optionally = std::invocable<T&&, Callable> 
&& requires (T&& t, Callable fn) {
    { fn(t) } -> std::same_as<std::optional<U>>;
};

template<typename T, typename Callable>
requires transforms_T_to_U_optionally<T, Callable, typename std::invoke_result_t<Callable, T>::value_type>
auto operator|(T&& t, Callable&& fn) {
    return std::invoke(std::forward<Callable>(fn), std::forward<T>(t));
}
*/

