#pragma once

#include <time.h>
#include <plutovg.h>
#include <FlexLayout.h>
#include <meui/event.h>
#include <box.h>

struct meui_t;

typedef void (*meui_callback_t)(struct meui_t *);

enum MEUI_CALLBACK
{
    MEUI_CB_ON_CREATE,
    MEUI_CB_ON_EXIT,
    MEUI_CB_MAX,
};

struct meui_render_context_t
{
    plutovg_font_face_t *face;
    plutovg_surface_t *surface;
    box_t root;
};

typedef struct meui_render_context_t meui_render_context_t;

struct meui_t
{
    int width;
    int height;
    plutovg_surface_t *win_surface;
    void *platform_data;
    struct meui_render_context_t render_context;
    meui_callback_t callback[MEUI_CB_MAX];
};

struct meui_t *meui_start(int width, int height);
struct meui_t *meui_get_instance();
void meui_register_callback(struct meui_t *, enum MEUI_CALLBACK, meui_callback_t);
void meui_render(struct meui_t *meui, box_t box);
void meui_main_loop(struct meui_t *);
void meui_flush(struct meui_t *);
void meui_update(struct meui_t *);
void meui_end(struct meui_t *);

void meui_add_font_face(struct meui_t *, const char *);
plutovg_font_t *meui_get_font(struct meui_t *, double);
plutovg_surface_t *meui_get_surface(struct meui_t *);
box_t meui_get_root_node(struct meui_t *);
