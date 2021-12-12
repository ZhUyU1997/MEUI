#define _XOPEN_SOURCE 600

#include <meui.h>
#include <box.h>
#include <log.h>

#include <FlexLayout.h>
#include <plutovg.h>

#include <SDL2/SDL.h>
#include <time.h>

#ifdef PROFILE
#include <gperftools/profiler.h>
#endif

#include <stdbool.h>

struct meui_platform_t
{
    SDL_Window *window;
    SDL_Surface *surface;
};

static struct meui_t *meui_instance = NULL;

struct meui_t *meui_start(int width, int height)
{
#ifdef PROFILE
    ProfilerStart("test.prof"); //开启性能分析
    atexit(ProfilerStop);
#endif
    SDL_Init(SDL_INIT_VIDEO);

    struct meui_platform_t *platform = calloc(1, sizeof(struct meui_platform_t));

    if (!platform)
        goto exit0;

    struct meui_t *meui = calloc(1, sizeof(struct meui_t));
    if (!meui)
        goto exit1;

    LOGI("SDL_CreateWindow");

    platform->window = SDL_CreateWindow("MEUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    if (!platform->window)
        goto exit2;

    LOGI("SDL_GetWindowSurface");

    platform->surface = SDL_GetWindowSurface(platform->window);

    if (!platform->surface)
        goto exit2;

    LOGI("plutovg_surface_create_for_data");

    meui->win_surface = plutovg_surface_create_for_data(platform->surface->pixels, width, height, sizeof(int) * width);
    meui->render_context.surface = plutovg_surface_reference(meui->win_surface);
    meui->width = width;
    meui->height = height;
    meui->render_context.root = Flex_newBox();

#if 0
    plutovg_t *pluto = plutovg_create(meui->render_context.surface);
    plutovg_rect(pluto, 0, 0, width, height);
    plutovg_set_source_rgba(pluto, 1, 1, 1, 1);
    plutovg_fill(pluto);
    plutovg_destroy(pluto);
#endif

    Flex_setWidth(meui->render_context.root, width);
    Flex_setHeight(meui->render_context.root, height);

    if (!meui->win_surface)
        goto exit2;

    meui->platform_data = platform;

    return meui;
exit2:
    free(meui);
exit1:
    free(platform);
exit0:
    return NULL;
}

struct meui_t *meui_get_instance()
{
    return meui_instance;
}

void meui_register_callback(struct meui_t *meui, enum MEUI_CALLBACK type, meui_callback_t cb)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }
    if (!cb)
    {
        LOGE("cb == NULL");
        return;
    }
    if (type >= MEUI_CB_MAX)
    {
        LOGE("enum MEUI_CALLBACK type is invalid!");
        return;
    }

    meui->callback[type] = cb;
}

void meui_main_loop(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }

    meui_instance = meui;

    if (meui->callback[MEUI_CB_ON_CREATE])
    {
        LOGD("Invoke MEUI_CB_ON_CREATE");
        meui->callback[MEUI_CB_ON_CREATE](meui);
    }

    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }
    }
}

void meui_update_surface(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }
    struct meui_platform_t *platform = meui->platform_data;
    SDL_UpdateWindowSurface(platform->window);
}

void meui_end(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }

    Flex_freeRecursive(meui->render_context.root);
    plutovg_surface_destroy(meui->render_context.surface);
    plutovg_surface_destroy(meui->win_surface);

    struct meui_platform_t *platform = meui->platform_data;
    SDL_DestroyWindow(platform->window);
    SDL_Quit();
    free(platform);
    free(meui);
}

void meui_add_font_face(struct meui_t *meui, const char *file)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }

    if (!file)
    {
        LOGE("file == NULL");
        return;
    }

    meui->render_context.face = plutovg_font_face_load_from_file(file);
}

plutovg_font_t *meui_get_font(struct meui_t *meui, double size)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return NULL;
    }

    return plutovg_font_load_from_face(meui->render_context.face, size);
}

plutovg_surface_t *meui_get_surface(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return NULL;
    }
    return meui->render_context.surface;
}

FlexNodeRef meui_get_root_node(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return NULL;
    }
    return meui->render_context.root;
}