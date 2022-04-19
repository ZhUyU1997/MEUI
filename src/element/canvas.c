#include "element.h"
#include "meui.h"
#include <string.h>
#include <malloc.h>

class_impl(CanvasEle, Box){
    .width = 300,
    .height = 150,
    .surface = false,
    .direction = CANVAS_TEXT_DIRECTION_INHERIT,
    .text_align = CANVAS_TEXT_ALIGN_START,
    .text_baseline = CANVAS_TEXT_BASELINE_ALPHABETIC,
    .font_size = 10,
    .font_weight = 400,
    .font_family = NULL,
};

static void draw(Box *this, plutovg_t *pluto)
{
    CanvasEle *e = dynamic_cast(CanvasEle)(this);

    if (e->surface)
    {
        plutovg_rect(pluto, 0, 0, e->width, e->height);
        plutovg_set_source_surface(pluto, e->surface, 0, 0);
        plutovg_fill(pluto);
    }
}

int canvas_get_width(CanvasEle *e)
{
    return e->width;
}

int canvas_get_height(CanvasEle *e)
{
    return e->height;
}

void canvas_set_width(CanvasEle *e, int width)
{
    plutovg_surface_t *old_surface = e->surface;
    plutovg_surface_t *new_surface = plutovg_surface_create(width, e->height);

    plutovg_t *pluto = plutovg_create(new_surface);
    plutovg_rect(pluto, 0, 0, e->width, e->height);
    plutovg_set_source_surface(pluto, e->surface, 0, 0);
    plutovg_fill(pluto);
    plutovg_surface_destroy(old_surface);

    plutovg_destroy(e->pluto);
    e->pluto = pluto;

    e->surface = new_surface;
    e->width = width;
}

void canvas_set_height(CanvasEle *e, int height)
{
    plutovg_surface_t *old_surface = e->surface;
    plutovg_surface_t *new_surface = plutovg_surface_create(e->width, height);

    plutovg_t *pluto = plutovg_create(new_surface);
    plutovg_rect(pluto, 0, 0, e->width, e->height);
    plutovg_set_source_surface(pluto, e->surface, 0, 0);
    plutovg_fill(pluto);
    plutovg_surface_destroy(old_surface);

    plutovg_destroy(e->pluto);
    e->pluto = pluto;

    e->surface = new_surface;
    e->height = height;
}

constructor(CanvasEle)
{
    Box *box = dynamic_cast(Box)(this);
    Flex_setMeasureFunc(box->node, box_measure_text);

    plutovg_color_init_rgb8(&this->fillColor, 0, 0, 0);
    plutovg_color_init_rgb8(&this->strokeColor, 0, 0, 0);

    box->draw = draw;
    this->surface = plutovg_surface_create(this->width, this->height);
    this->pluto = plutovg_create(this->surface);
    this->path = plutovg_path_create();
    this->font_family = strdup(meui_get_default_font_family(meui_get_instance()));
}

destructor(CanvasEle)
{
    if (this->font_family)
        free(this->font_family);
    plutovg_path_destroy(this->path);
    plutovg_destroy(this->pluto);
    plutovg_surface_destroy(this->surface);
}