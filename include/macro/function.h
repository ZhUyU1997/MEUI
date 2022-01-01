#pragma once

#define __KEEP(...) __VA_ARGS__
#define __DISCARD(...)
#define __FUNC_ARGS(...) __VA_ARGS__ __DISCARD
#define __FUNC_BODY(...) __KEEP

#define __function(counter, ret, remain) ({ret Function_##counter(__FUNC_ARGS remain) { __FUNC_BODY remain };Function_##counter;})       
#define function(ret, remain) __function(__COUNTER__,ret, remain)