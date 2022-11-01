/*
 * QuickJS C library
 * 
 * Copyright (c) 2017-2021 Fabrice Bellard
 * Copyright (c) 2017-2021 Charlie Gordon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/time.h>
#include <limits.h>
#include <SDL.h>
#include <emscripten/html5.h>

#include "cutils.h"
#include "list.h"
#include "quickjs-libc.h"

typedef struct {
    int ref_count;
    struct list_head link;
    int64_t timeout;
    JSValue func;
} JSOSTimer;

typedef struct JSThreadState {
    JSValue os_event_handler;
    struct list_head os_timers; /* list of JSOSTimer.link */
} JSThreadState;

static uint64_t os_pending_signals;
static int (*os_poll_func)(JSContext *ctx);

uint8_t *js_load_file(JSContext *ctx, size_t *pbuf_len, const char *filename)
{
    FILE *f;
    uint8_t *buf;
    size_t buf_len;
    long lret;
    
    f = fopen(filename, "rb");
    if (!f)
        return NULL;
    if (fseek(f, 0, SEEK_END) < 0)
        goto fail;
    lret = ftell(f);
    if (lret < 0)
        goto fail;
    /* XXX: on Linux, ftell() return LONG_MAX for directories */
    if (lret == LONG_MAX) {
        errno = EISDIR;
        goto fail;
    }
    buf_len = lret;
    if (fseek(f, 0, SEEK_SET) < 0)
        goto fail;
    if (ctx)
        buf = js_malloc(ctx, buf_len + 1);
    else
        buf = malloc(buf_len + 1);
    if (!buf)
        goto fail;
    if (fread(buf, 1, buf_len, f) != buf_len) {
        errno = EIO;
        if (ctx)
            js_free(ctx, buf);
        else
            free(buf);
    fail:
        fclose(f);
        return NULL;
    }
    buf[buf_len] = '\0';
    fclose(f);
    *pbuf_len = buf_len;
    return buf;
}

int js_module_set_import_meta(JSContext *ctx, JSValueConst func_val,
                              JS_BOOL use_realpath, JS_BOOL is_main)
{
    JSModuleDef *m;
    char buf[PATH_MAX + 16];
    JSValue meta_obj;
    JSAtom module_name_atom;
    const char *module_name;
    
    assert(JS_VALUE_GET_TAG(func_val) == JS_TAG_MODULE);
    m = JS_VALUE_GET_PTR(func_val);

    module_name_atom = JS_GetModuleName(ctx, m);
    module_name = JS_AtomToCString(ctx, module_name_atom);
    JS_FreeAtom(ctx, module_name_atom);
    if (!module_name)
        return -1;
    if (!strchr(module_name, ':')) {
        strcpy(buf, "file://");
#if !defined(_WIN32)
        /* realpath() cannot be used with modules compiled with qjsc
           because the corresponding module source code is not
           necessarily present */
        if (use_realpath) {
            char *res = realpath(module_name, buf + strlen(buf));
            if (!res) {
                JS_ThrowTypeError(ctx, "realpath failure");
                JS_FreeCString(ctx, module_name);
                return -1;
            }
        } else
#endif
        {
            pstrcat(buf, sizeof(buf), module_name);
        }
    } else {
        pstrcpy(buf, sizeof(buf), module_name);
    }
    JS_FreeCString(ctx, module_name);
    
    meta_obj = JS_GetImportMeta(ctx, m);
    if (JS_IsException(meta_obj))
        return -1;
    JS_DefinePropertyValueStr(ctx, meta_obj, "url",
                              JS_NewString(ctx, buf),
                              JS_PROP_C_W_E);
    JS_DefinePropertyValueStr(ctx, meta_obj, "main",
                              JS_NewBool(ctx, is_main),
                              JS_PROP_C_W_E);
    JS_FreeValue(ctx, meta_obj);
    return 0;
}

JSModuleDef *js_module_loader(JSContext *ctx,
                              const char *module_name, void *opaque)
{
    JSModuleDef *m;

    size_t buf_len;
    uint8_t *buf;
    JSValue func_val;

    buf = js_load_file(ctx, &buf_len, module_name);
    if (!buf) {
        JS_ThrowReferenceError(ctx, "could not load module filename '%s'",
                                module_name);
        return NULL;
    }
    
    /* compile the module */
    func_val = JS_Eval(ctx, (char *)buf, buf_len, module_name,
                        JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
    js_free(ctx, buf);
    if (JS_IsException(func_val))
        return NULL;
    /* XXX: could propagate the exception */
    js_module_set_import_meta(ctx, func_val, TRUE, FALSE);
    /* the module is already referenced, so we must free it */
    m = JS_VALUE_GET_PTR(func_val);
    JS_FreeValue(ctx, func_val);
    return m;
}

void js_std_set_worker_new_context_func(JSContext *(*func)(JSRuntime *rt))
{
}

static JSValue js_std_exit(JSContext *ctx, JSValueConst this_val,
                           int argc, JSValueConst *argv)
{
    int status;
    if (JS_ToInt32(ctx, &status, argv[0]))
        status = -1;
    exit(status);
    return JS_UNDEFINED;
}

static JSValue js_std_gc(JSContext *ctx, JSValueConst this_val,
                         int argc, JSValueConst *argv)
{
    JS_RunGC(JS_GetRuntime(ctx));
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_std_funcs[] = {
    JS_CFUNC_DEF("exit", 1, js_std_exit ),
    JS_CFUNC_DEF("gc", 0, js_std_gc ),
};
    
static int js_std_init(JSContext *ctx, JSModuleDef *m)
{
    JS_SetModuleExportList(ctx, m, js_std_funcs,
                           countof(js_std_funcs));
    return 0;
}

JSModuleDef *js_init_module_std(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_std_init);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_std_funcs, countof(js_std_funcs));
    return m;
}

/**********************************************************/
/* 'os' object */


/* more portable, but does not work if the date is updated */
static int64_t get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}

static void free_timer(JSRuntime *rt, JSOSTimer *th)
{
    JS_FreeValueRT(rt, th->func);
    if(--th->ref_count <= 0)
    {
        js_free_rt(rt, th);
    }
}

static void unlink_timer(JSRuntime *rt, JSOSTimer *th)
{
    if (th->link.prev) {
        list_del(&th->link);
        th->link.prev = th->link.next = NULL;
        free_timer(rt, th);
    }
}

static JSClassID js_os_timer_class_id;

static void js_os_timer_finalizer(JSRuntime *rt, JSValue val)
{
    JSOSTimer *th = JS_GetOpaque(val, js_os_timer_class_id);
    if (th) {
        free_timer(rt, th);
    }
}

static void js_os_timer_mark(JSRuntime *rt, JSValueConst val,
                             JS_MarkFunc *mark_func)
{
    JSOSTimer *th = JS_GetOpaque(val, js_os_timer_class_id);
    if (th) {
        JS_MarkValue(rt, th->func, mark_func);
    }
}

static JSValue js_os_setTimeout(JSContext *ctx, JSValueConst this_val,
                                int argc, JSValueConst *argv)
{
    JSRuntime *rt = JS_GetRuntime(ctx);
    JSThreadState *ts = JS_GetRuntimeOpaque(rt);
    int64_t delay;
    JSValueConst func;
    JSOSTimer *th;
    JSValue obj;

    func = argv[0];
    if (!JS_IsFunction(ctx, func))
        return JS_ThrowTypeError(ctx, "not a function");
    if (JS_ToInt64(ctx, &delay, argv[1]))
        return JS_EXCEPTION;
    obj = JS_NewObjectClass(ctx, js_os_timer_class_id);
    if (JS_IsException(obj))
        return obj;
    th = js_mallocz(ctx, sizeof(*th));
    if (!th) {
        JS_FreeValue(ctx, obj);
        return JS_EXCEPTION;
    }

    th->ref_count = 1;
    th->timeout = get_time_ms() + delay;
    th->func = JS_DupValue(ctx, func);

    JS_DupValue(ctx, func);
    th->ref_count++;
    list_add_tail(&th->link, &ts->os_timers);
    JS_SetOpaque(obj, th);
    return obj;
}

static JSValue js_os_clearTimeout(JSContext *ctx, JSValueConst this_val,
                                  int argc, JSValueConst *argv)
{
    JSOSTimer *th = JS_GetOpaque2(ctx, argv[0], js_os_timer_class_id);
    if (!th)
        return JS_EXCEPTION;
    unlink_timer(JS_GetRuntime(ctx), th);
    return JS_UNDEFINED;
}

static JSClassDef js_os_timer_class = {
    "OSTimer",
    .finalizer = js_os_timer_finalizer,
    .gc_mark = js_os_timer_mark,
}; 

static void call_handler(JSContext *ctx, JSValueConst func)
{
    JSValue ret, func1;
    /* 'func' might be destroyed when calling itself (if it frees the
       handler), so must take extra care */
    func1 = JS_DupValue(ctx, func);
    ret = JS_Call(ctx, func1, JS_UNDEFINED, 0, NULL);
    JS_FreeValue(ctx, func1);
    if (JS_IsException(ret))
        js_std_dump_error(ctx);
    JS_FreeValue(ctx, ret);
}

static JSValue js_os_setEventHandler(JSContext *ctx, JSValueConst this_val,
                                    int argc, JSValueConst *argv)
{
    JSRuntime *rt = JS_GetRuntime(ctx);
    JSThreadState *ts = JS_GetRuntimeOpaque(rt);
    JSValueConst func;
    
    func = argv[0];
    if (JS_IsNull(func)) {
        if (!JS_IsNull(ts->os_event_handler)) {
            JS_FreeValue(ctx, ts->os_event_handler);
            ts->os_event_handler = JS_NULL;
        }
    } else {
        if (!JS_IsFunction(ctx, func))
            return JS_ThrowTypeError(ctx, "not a function");
        JS_FreeValue(ctx, ts->os_event_handler);
        ts->os_event_handler = JS_DupValue(ctx, func);
    }
    return JS_UNDEFINED;
}

static int js_os_poll(JSContext *ctx)
{
    JSRuntime *rt = JS_GetRuntime(ctx);
    JSThreadState *ts = JS_GetRuntimeOpaque(rt);
    int ret, fd_max, min_delay = 10000000;
    int64_t cur_time, delay;
    struct list_head *el;

    if (!list_empty(&ts->os_timers)) {
        cur_time = get_time_ms();
        min_delay = 10000;
        list_for_each(el, &ts->os_timers) {
            JSOSTimer *th = list_entry(el, JSOSTimer, link);
            delay = th->timeout - cur_time;
            if (delay <= 0) {
                JSValue func;
                /* the timer expired */
                func = JS_DupValue(ctx, th->func);
                unlink_timer(rt, th);

                call_handler(ctx, func);
                JS_FreeValue(ctx, func);
                break;
            } else if (delay < min_delay) {
                min_delay = delay;
            }
        }
    }


    ret = SDL_WaitEventTimeout(NULL, 0);
    if (ret > 0) {
        if(!JS_IsNull(ts->os_event_handler))
        {
            call_handler(ctx, ts->os_event_handler);
        }
    }
    done:
    return 0;
}

#define OS_PLATFORM "SDL2"

#define OS_FLAG(x) JS_PROP_INT32_DEF(#x, x, JS_PROP_CONFIGURABLE )

static const JSCFunctionListEntry js_os_funcs[] = {
    JS_CFUNC_DEF("setTimeout", 2, js_os_setTimeout ),
    JS_CFUNC_DEF("clearTimeout", 1, js_os_clearTimeout ),
    JS_PROP_STRING_DEF("platform", OS_PLATFORM, 0 ),
    JS_CFUNC_DEF("setEventHandler", 1, js_os_setEventHandler),
};

static int js_os_init(JSContext *ctx, JSModuleDef *m)
{
    os_poll_func = js_os_poll;
    
    /* OSTimer class */
    JS_NewClassID(&js_os_timer_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_os_timer_class_id, &js_os_timer_class);
    return JS_SetModuleExportList(ctx, m, js_os_funcs,
                                  countof(js_os_funcs));
}

JSModuleDef *js_init_module_os(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_os_init);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_os_funcs, countof(js_os_funcs));
    return m;
}

/**********************************************************/

static JSValue js_print(JSContext *ctx, JSValueConst this_val,
                              int argc, JSValueConst *argv)
{
    int i;
    const char *str;
    size_t len;

    for(i = 0; i < argc; i++) {
        if (i != 0)
            putchar(' ');
        str = JS_ToCStringLen(ctx, &len, argv[i]);
        if (!str)
            return JS_EXCEPTION;
        fwrite(str, 1, len, stdout);
        JS_FreeCString(ctx, str);
    }
    putchar('\n');
    return JS_UNDEFINED;
}

void js_std_add_helpers(JSContext *ctx, int argc, char **argv)
{
    JSValue global_obj, console, args;
    int i;

    /* XXX: should these global definitions be enumerable? */
    global_obj = JS_GetGlobalObject(ctx);

    console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, console, "log",
                      JS_NewCFunction(ctx, js_print, "log", 1));
    JS_SetPropertyStr(ctx, global_obj, "console", console);

    /* same methods as the mozilla JS shell */
    if (argc >= 0) {
        args = JS_NewArray(ctx);
        for(i = 0; i < argc; i++) {
            JS_SetPropertyUint32(ctx, args, i, JS_NewString(ctx, argv[i]));
        }
        JS_SetPropertyStr(ctx, global_obj, "scriptArgs", args);
    }
    
    JS_SetPropertyStr(ctx, global_obj, "print",
                      JS_NewCFunction(ctx, js_print, "print", 1));
    JS_FreeValue(ctx, global_obj);
}

void js_std_init_handlers(JSRuntime *rt)
{
    JSThreadState *ts;

    ts = malloc(sizeof(*ts));
    if (!ts) {
        fprintf(stderr, "Could not allocate memory for the worker");
        exit(1);
    }
    memset(ts, 0, sizeof(*ts));
    init_list_head(&ts->os_timers);
    ts->os_event_handler = JS_NULL;
    JS_SetRuntimeOpaque(rt, ts);
}

void js_std_free_handlers(JSRuntime *rt)
{
    JSThreadState *ts = JS_GetRuntimeOpaque(rt);
    struct list_head *el, *el1;

    JS_FreeValueRT(rt, ts->os_event_handler);
    
    list_for_each_safe(el, el1, &ts->os_timers) {
        JSOSTimer *th = list_entry(el, JSOSTimer, link);
        unlink_timer(rt, th);
    }
    free(ts);
    JS_SetRuntimeOpaque(rt, NULL); /* fail safe */
}

static void js_dump_obj(JSContext *ctx, FILE *f, JSValueConst val)
{
    const char *str;
    
    str = JS_ToCString(ctx, val);
    if (str) {
        fprintf(f, "%s\n", str);
        JS_FreeCString(ctx, str);
    } else {
        fprintf(f, "[exception]\n");
    }
}

static void js_std_dump_error1(JSContext *ctx, JSValueConst exception_val)
{
    JSValue val;
    BOOL is_error;
    
    is_error = JS_IsError(ctx, exception_val);
    js_dump_obj(ctx, stderr, exception_val);
    if (is_error) {
        val = JS_GetPropertyStr(ctx, exception_val, "stack");
        if (!JS_IsUndefined(val)) {
            js_dump_obj(ctx, stderr, val);
        }
        JS_FreeValue(ctx, val);
    }
}

void js_std_dump_error(JSContext *ctx)
{
    JSValue exception_val;
    
    exception_val = JS_GetException(ctx);
    js_std_dump_error1(ctx, exception_val);
    JS_FreeValue(ctx, exception_val);
}

void __js_std_loop(void *data)
{
    JSContext *ctx = data;
    JSContext *ctx1;
    int err;

    /* execute the pending jobs */
    for (;;)
    {
        err = JS_ExecutePendingJob(JS_GetRuntime(ctx), &ctx1);
        if (err <= 0)
        {
            if (err < 0)
            {
                js_std_dump_error(ctx1);
            }
            break;
        }
    }

    if (!os_poll_func || os_poll_func(ctx))
        emscripten_cancel_main_loop();
}

void js_cancel_main_loop()
{
    emscripten_cancel_main_loop();
}

/* main loop which calls the user JS callbacks */
void js_std_loop(JSContext *ctx)
{
    emscripten_set_main_loop_arg(__js_std_loop, ctx, 120, 1);
}