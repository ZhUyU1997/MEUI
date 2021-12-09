#pragma once

#include <macro/base.h>
#include <macro/eval.h>

#define _FOR_EACH(macro, x, ...)     \
    CAT(_FOR_EACH_CUR_, IS_EMPTY(x))(macro, x) CAT(_FOR_EACH_NEXT_, IS_EMPTY(__VA_ARGS__))(macro, __VA_ARGS__)

#define _FOR_EACH_NEXT_0(macro, ...) DEFER(_FOR_EACH_I)()(macro, __VA_ARGS__)
#define _FOR_EACH_NEXT_1(macro, ...)
#define _FOR_EACH_CUR_0(macro, x) macro(x)
#define _FOR_EACH_CUR_1(macro, x)

#define _FOR_EACH_I() _FOR_EACH
#define FOR_EACH(macro, ...) EVAL(_FOR_EACH(macro, __VA_ARGS__))