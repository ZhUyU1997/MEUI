#define _XOPEN_SOURCE 600

#include <meui.h>
#include <box.h>
#include <log.h>
#include <macro/function.h>

#include <FlexLayout.h>
#include <plutovg.h>

#include <platform/window.h>
#include <time.h>
#include <stdlib.h>

#define PROFILE

#ifdef PROFILE
#include <gperftools/profiler.h>
#endif

#include <stdbool.h>

struct meui_platform_t
{
    struct window_t *window;
    unsigned int frame;
    clock_t start_ticks;
};

static struct meui_t *meui_instance = NULL;

struct meui_t *meui_start(int width, int height)
{
#ifdef PROFILE
    ProfilerStart("test.prof");
    atexit(ProfilerStop);
#endif

    struct meui_platform_t *platform = calloc(1, sizeof(struct meui_platform_t));

    if (!platform)
        goto exit0;

    struct meui_t *meui = calloc(1, sizeof(struct meui_t));
    if (!meui)
        goto exit1;

    LOGI("Create Window");

    platform->window = window_create("MEUI", width, height);

    if (!platform->window)
        goto exit2;

    LOGI("Get Window Surface");

    meui->win_surface = plutovg_surface_create_for_data(window_get_image_data(platform->window), width, height, sizeof(int) * width);
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
    meui_instance = meui;

    box_default_style_init();
    return meui;
exit2:
    free(meui);
exit1:
    free(platform);
exit0:
    return NULL;
}

void meui_platform_fps(struct meui_t *meui)
{
    struct meui_platform_t *platform = meui->platform_data;
    platform->frame++;

    if (platform->start_ticks == 0)
    {
        platform->start_ticks = clock();
    }
    else
    {
        clock_t ticks = clock() - platform->start_ticks;

        if (ticks >= 1000)
        {
            char buf[128] = {0};
            snprintf(buf, 128, "FPS: %f", (double)platform->frame * CLOCKS_PER_SEC / ticks);
            window_set_name(platform->window, buf);
            platform->frame = 0;
            platform->start_ticks = clock();
        }
    }
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

box_t meui_search_node(struct meui_t *meui, box_t node, void *data, plutovg_point_t *point, void (*cb)(box_t node, bool hit, void *data))
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

    if (cb)
        cb(node, target == node, data);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        box_t ret = meui_search_node(meui, Flex_getChild(node, i), data, point, cb);
        target = ret ? ret : target;
    }

    return target;
}

void meui_render(struct meui_t *meui, box_t box)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }

    box_t root = meui_get_root_node(meui);
    Flex_addChild(root, box);
    meui_update(meui);
}

void meui_flush(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }
    struct meui_platform_t *platform = meui->platform_data;
    window_update_image(platform->window);
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
    plutovg_set_source_rgba(pluto, 0, 0, 0, 1);
    plutovg_fill(pluto);
    plutovg_destroy(pluto);

    box_updateStyleRecursive(meui_get_root_node(meui));
    Flex_layout(meui_get_root_node(meui), FlexUndefined, FlexUndefined, 1);
    box_draw(meui_get_root_node(meui));
    meui_flush(meui);

    meui_platform_fps(meui);
}

void meui_end(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }

    box_free_recursive(meui->render_context.root);
    plutovg_surface_destroy(meui->render_context.surface);
    plutovg_surface_destroy(meui->win_surface);

    struct meui_platform_t *platform = meui->platform_data;
    window_destory(platform->window);
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

int meui_get_connect_number(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return NULL;
    }

    struct meui_platform_t *platform = meui->platform_data;
    return window_connect_number(platform->window);
}

int meui_pending(struct meui_t *meui)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return -1;
    }

    struct meui_platform_t *platform = meui->platform_data;
    return window_pending(platform->window);
}

void meui_next_event(struct meui_t *meui, struct meui_event_t *event)
{
    if (!meui)
    {
        LOGE("meui == NULL");
        return;
    }

    if (!event)
    {
        LOGE("event == NULL");
        return;
    }

    struct meui_platform_t *platform = meui->platform_data;
    window_next_event(platform->window, event);
}