#define _XOPEN_SOURCE 600

#include <meui.h>
#include <box.h>
#include <log.h>

#include <FlexLayout.h>
#include <plutovg.h>

#include <SDL2/SDL.h>
#include <time.h>

#define PROFILE

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
    ProfilerStart("test.prof");
    atexit(ProfilerStop);
#endif
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

    struct meui_platform_t *platform = calloc(1, sizeof(struct meui_platform_t));

    if (!platform)
        goto exit0;

    struct meui_t *meui = calloc(1, sizeof(struct meui_t));
    if (!meui)
        goto exit1;

    LOGI("SDL_CreateWindow");

    platform->window = SDL_CreateWindow("MEUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

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
    meui->render_context.root = box_new();

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

static box_t search_hit_node(struct meui_t *meui, box_t node, plutovg_point_t *point)
{
    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);

    struct Box *box = Flex_getContext(node);

    double content_width = width - Flex_getResultPaddingLeft(node) - Flex_getResultPaddingRight(node);
    double content_height = height - Flex_getResultPaddingTop(node) - Flex_getResultPaddingBottom(node);
    double content_left = Flex_getResultPaddingLeft(node);
    double content_top = Flex_getResultPaddingTop(node);

    box_t target = NULL;

    plutovg_matrix_t m = box->result.to_screen_matrix;
    plutovg_matrix_invert(&m);

    plutovg_point_t dst;
    plutovg_matrix_map_point(&m, point, &dst);

    if (dst.x >= 0 && dst.x < width && dst.y >= 0 && dst.y < height)
    {
        target = node;
    }

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_t ret = search_hit_node(meui, Flex_getChild(node, i), point);
        target = ret ? ret : target;
    }

    return target;
}

static void dispatch_event(struct meui_t *meui, struct meui_event_t *event)
{
    if (event->type == MEUI_EVENT_MOUSE_DOWN)
    {
        plutovg_point_t point = {event->MOUSE_DOWN.x, event->MOUSE_DOWN.y};

        box_t node = search_hit_node(meui, meui_get_root_node(meui), &point);

        if (node)
        {
            box_dispatch_event(node, event->type, event);
        }
    }
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

    meui_update(meui);

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
            case SDL_MOUSEMOTION:
                dispatch_event(meui, &(struct meui_event_t){.type = MEUI_EVENT_MOUSE_MOVE, .MOUSE_MOVE = {e.button.x, e.button.y}});
                break;
            case SDL_MOUSEBUTTONDOWN:
                dispatch_event(meui, &(struct meui_event_t){.type = MEUI_EVENT_MOUSE_DOWN, .MOUSE_DOWN = {e.button.x, e.button.y}});
                break;
            case SDL_MOUSEBUTTONUP:
                dispatch_event(meui, &(struct meui_event_t){.type = MEUI_EVENT_MOUSE_UP, .MOUSE_UP = {e.button.x, e.button.y}});
                break;
            }
        }
        meui_update(meui);
    }
}

void meui_flush(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }
    struct meui_platform_t *platform = meui->platform_data;
    SDL_UpdateWindowSurface(platform->window);
}

void meui_update(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }

    plutovg_t *pluto = plutovg_create(meui_get_surface(meui));
    plutovg_rect(pluto, 0, 0, meui->width, meui->height);
    plutovg_set_source_rgba(pluto, 0, 0, 0, 0);
    plutovg_fill(pluto);
    plutovg_destroy(pluto);

    Flex_layout(meui_get_root_node(meui), FlexUndefined, FlexUndefined, 1);
    box_draw(meui_get_root_node(meui));
    meui_flush(meui);
}

void meui_end(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }

    box_freeRecursive(meui->render_context.root);
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

box_t meui_get_root_node(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return NULL;
    }
    return meui->render_context.root;
}