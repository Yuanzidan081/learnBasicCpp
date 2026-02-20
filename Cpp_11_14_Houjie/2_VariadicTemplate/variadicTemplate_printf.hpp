#include <iostream>
void printf(const char *s)
{
    while (*s)
    {
        if (*s == '%' && *(++s) != '%')
            throw std::logic_error("invalid format string: missing arguments");
        std::cout << *s++;
    }
}
template <typename T, typename... Args>
void printf(const char *s, T value, Args... args)
{
    while (*s)
    {
        if (*s == '%' && *(++s) != '%')
        {
            std::cout << value;
            printf(++s, args...);
            return;
        }
        std::cout << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
}