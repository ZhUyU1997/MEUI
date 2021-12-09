#pragma once

#include <FlexLayout.h>
#include <plutovg.h>

typedef enum TEXT_ALIGN
{
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_CENTER,
} TEXT_ALIGN;

struct Box
{
    float border_radius[4];
    struct plutovg_color border_color;
    struct plutovg_color fill_color;
    struct plutovg_color font_color;
    TEXT_ALIGN align;
    const char *text;
    double font_size;
    const char *background_image;
    const char *content_image;
};

FlexNodeRef Flex_newBox();
void Flex_freeBox(FlexNodeRef node);
void Flex_freeRecursive(FlexNodeRef node);

void Flex_setBorderRadius(FlexNodeRef node, float tl, float tr, float br, float bl);
void Flex_setBorderColor(FlexNodeRef node, struct plutovg_color c);
void Flex_setFillColor(FlexNodeRef node, struct plutovg_color c);
void Flex_setFontColor(FlexNodeRef node, struct plutovg_color c);
void Flex_setText(FlexNodeRef node, const char *text);
void Flex_setFontSize(FlexNodeRef node, double font_size);
void Flex_setTextAlign(FlexNodeRef node, TEXT_ALIGN align);
void Flex_setBackgroundImage(FlexNodeRef node, const char *background_image);
void Flex_setContentImage(FlexNodeRef node, const char *content_image);

void Flex_drawNode(FlexNodeRef node, float x, float y);