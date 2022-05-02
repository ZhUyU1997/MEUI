#include "box.h"
#include "class.h"

#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"

#include <macro/function.h>

#include <FlexLayout.h>
#include <plutovg.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <meui.h>

#ifdef PROFILE
#include <gperftools/profiler.h>
#endif

JSModuleDef *js_init_module_meui(JSContext *ctx, const char *module_name);

static JSContext *JS_NewCustomContext(JSRuntime *rt)
{
    JSContext *ctx = JS_NewContextRaw(rt);
    if (!ctx)
        return NULL;
    JS_AddIntrinsicBaseObjects(ctx);
    JS_AddIntrinsicDate(ctx);
    JS_AddIntrinsicEval(ctx);
    JS_AddIntrinsicStringNormalize(ctx);
    JS_AddIntrinsicRegExp(ctx);
    JS_AddIntrinsicJSON(ctx);
    JS_AddIntrinsicProxy(ctx);
    JS_AddIntrinsicMapSet(ctx);
    JS_AddIntrinsicTypedArrays(ctx);
    JS_AddIntrinsicPromise(ctx);

    /* system modules */
    js_init_module_std(ctx, "std");
    js_init_module_os(ctx, "os");
    js_init_module_meui(ctx, "NativeMEUI");
    return ctx;
}

static int eval_buf(JSContext *ctx, const void *buf, int buf_len,
                    const char *filename, int eval_flags)
{
    JSValue val;
    int ret;

    val = JS_Eval(ctx, buf, buf_len, filename,
                  eval_flags | JS_EVAL_FLAG_COMPILE_ONLY);
    if (!JS_IsException(val))
    {
        js_module_set_import_meta(ctx, val, TRUE, TRUE);
        val = JS_EvalFunction(ctx, val);
    }

    if (JS_IsException(val))
    {
        js_std_dump_error(ctx);
        ret = -1;
    }
    else
    {
        ret = 0;
    }
    JS_FreeValue(ctx, val);
    return ret;
}

static int eval_file(JSContext *ctx, const char *filename)
{
    uint8_t *buf;
    int ret, eval_flags;
    size_t buf_len;

    buf = js_load_file(ctx, &buf_len, filename);
    if (!buf)
    {
        perror(filename);
        exit(1);
    }

    eval_flags = JS_EVAL_TYPE_MODULE;
    ret = eval_buf(ctx, buf, buf_len, filename, eval_flags);
    js_free(ctx, buf);
    return ret;
}

int main(int argc, char **argv)
{
#ifdef PROFILE
    ProfilerStart("test.prof");
    atexit(ProfilerStop);
#endif

    do_class_init();

    JSRuntime *rt;
    JSContext *ctx;
    rt = JS_NewRuntime();
    js_std_set_worker_new_context_func(JS_NewCustomContext);
    js_std_init_handlers(rt);
    JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
    ctx = JS_NewCustomContext(rt);
    js_std_add_helpers(ctx, argc, argv);

    eval_file(ctx, argv[1]);
    js_std_loop(ctx);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return 0;
}