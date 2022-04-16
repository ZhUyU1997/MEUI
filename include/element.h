#pragma once

#include <box.h>
#include <class.h>
#include <plutovg.h>

class(DivEle, Box){};
class(StackEle, Box){};
class(CanvasEle, Box)
{
    int width;
    int height;
    plutovg_surface_t *surface;
    plutovg_t *pluto;
    plutovg_path_t *path;
};

int canvas_get_width(CanvasEle *e);
int canvas_get_height(CanvasEle *e);
void canvas_set_width(CanvasEle *e, int width);
void canvas_set_height(CanvasEle *e, int height);