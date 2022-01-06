
#include <fmt/core.h>

namespace app
{
    enum class level
    {
        normal,
        verbose
    };
    inline level log = level::normal;
    template <typename... T>
    void logger(fmt::v8::format_string<T...> fmtr, T &&...args)
    {
        fmt::print(fmtr, args...);
    }

    template <typename... T>
    void logger(level __level, fmt::v8::format_string<T...> fmtr, T &&...args)
    {
        if (__level == level::verbose && app::log == level::verbose)
            fmt::print(fmtr, args...);
    }
}
