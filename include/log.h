#pragma once

#include <printv.h>

#define NEW_LINE "\r\n"
#define CONFIG_DEBUG

#ifdef CONFIG_DEBUG
#define LOGI(...) printv("[" $(__FILE__) ":" $(__LINE__) ":" $((char *)__func__) "] "__VA_ARGS__, NEW_LINE)
#define LOGD(...) printv("[" $(__FILE__) ":" $(__LINE__) ":" $((char *)__func__) "] \e[32m"__VA_ARGS__, "\e[0m" NEW_LINE)
#define LOGW(...) printv("[" $(__FILE__) ":" $(__LINE__) ":" $((char *)__func__) "] \e[33m"__VA_ARGS__, "\e[0m" NEW_LINE)
#define LOGE(...) printv("[" $(__FILE__) ":" $(__LINE__) ":" $((char *)__func__) "] \e[31m"__VA_ARGS__, "\e[0m" NEW_LINE)
#define LOGF(...) printv("[" $(__FILE__) ":" $(__LINE__) ":" $((char *)__func__) "] \e[35m"__VA_ARGS__, "\e[0m" NEW_LINE)
#define PANIC(...) ({LOGF("[PANIC] ",__VA_ARGS__); while(1); })
#else
#define LOGI(...)
#define LOGD(...)
#define LOGW(...) printv("[" $(__FILE__) ":" $(__LINE__) ":" $((char *)__func__) "] \e[33m"__VA_ARGS__, "\e[0m" NEW_LINE)
#define LOGE(...) printv("[" $(__FILE__) ":" $(__LINE__) ":" $((char *)__func__) "] \e[31m"__VA_ARGS__, "\e[0m" NEW_LINE)
#define LOGF(...) printv("[" $(__FILE__) ":" $(__LINE__) ":" $((char *)__func__) "] \e[35m"__VA_ARGS__, "\e[0m" NEW_LINE)
#define PANIC(...) ({LOGF("[PANIC] ",__VA_ARGS__); while(1); })
#endif