#pragma once

#include <stdio.h>
#include <macro/for-each.h>

#define PRINT_FUNC(fmt, type) \
    type:                     \
    print_##fmt

#define PRINT_TEMPLATE(fmt, type)              \
    static inline void print_##fmt(type value) \
    {                                          \
        printf("%" #fmt, value);               \
    }
#define PRINT_TEMPLATE2(fmt, type) \
    static inline void print_##fmt(type value)

#define PRINT(value) _Generic(        \
    (value),                          \
    PRINT_FUNC(s, char *),            \
    PRINT_FUNC(s, const char *),      \
    PRINT_FUNC(d, short),             \
    PRINT_FUNC(u, unsigned short),    \
    PRINT_FUNC(d, int),               \
    PRINT_FUNC(u, unsigned int),      \
    PRINT_FUNC(p, long int),          \
    PRINT_FUNC(x, unsigned long int), \
    PRINT_FUNC(f, double),            \
    PRINT_FUNC(f, float),             \
    PRINT_FUNC(p, default))(value);

#define printv(...) ({ FOR_EACH(PRINT, ##__VA_ARGS__) })
#define $(x) , x,

PRINT_TEMPLATE(p, const void *);
PRINT_TEMPLATE(s, const char *);
PRINT_TEMPLATE(d, const int);
PRINT_TEMPLATE(f, const double);
PRINT_TEMPLATE2(u, const unsigned int)
{
    printf("%d", value);
}
PRINT_TEMPLATE2(x, const unsigned long int)
{
    printf("%p", (const void *)value);
}