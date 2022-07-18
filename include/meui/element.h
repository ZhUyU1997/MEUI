#pragma once

#include "common/class.h"
#include "meui/box.h"
#include <plutovg.h>

class(DivEle, Box){};
class(StackEle, Box){};

enum CANVAS_TEXT_DIRECTION
{
    CANVAS_TEXT_DIRECTION_INHERIT,
    CANVAS_TEXT_DIRECTION_LTR,
    CANVAS_TEXT_DIRECTION_RTL,
};

enum CANVAS_TEXT_ALIGN
{
    CANVAS_TEXT_ALIGN_CENTER,
    CANVAS_TEXT_ALIGN_END,
    CANVAS_TEXT_ALIGN_LEFT,
    CANVAS_TEXT_ALIGN_RIGHT,
    CANVAS_TEXT_ALIGN_START,
};

enum CANVAS_TEXT_BASELINE
{
    CANVAS_TEXT_BASELINE_ALPHABETIC,
    CANVAS_TEXT_BASELINE_BOTTOM,
    CANVAS_TEXT_BASELINE_HANGING,
    CANVAS_TEXT_BASELINE_IDEOGRAPHIC,
    CANVAS_TEXT_BASELINE_MIDDLE,
    CANVAS_TEXT_BASELINE_TOP,
};

class(CanvasEle, Box)
{
    int width;
    int height;
    int fit;
    plutovg_surface_t *surface;
    plutovg_t *pluto;
    plutovg_path_t *path;

    plutovg_paint_t *fillPaint;
    plutovg_paint_t *strokePaint;

    enum CANVAS_TEXT_DIRECTION direction;
    enum CANVAS_TEXT_ALIGN text_align;
    enum CANVAS_TEXT_BASELINE text_baseline;
    double font_size;
    double font_weight;
    const char *font_family;
};

int canvas_get_width(CanvasEle *e);
int canvas_get_height(CanvasEle *e);
void canvas_set_width(CanvasEle *e, int width);
void canvas_set_height(CanvasEle *e, int height);
int canvas_get_fit(CanvasEle *e);
void canvas_set_fit(CanvasEle *e, int fit);

void canvas_set_fill_color(CanvasEle *e, double r, double g, double b, double a);
void canvas_set_fill_gradient(CanvasEle *e, plutovg_gradient_t *gradient);
void canvas_set_stroke_color(CanvasEle *e, double r, double g, double b, double a);
void canvas_set_stroke_gradient(CanvasEle *e, plutovg_gradient_t *gradient);
void canvas_fill(CanvasEle *e);
void canvas_stroke(CanvasEle *e);