#pragma once

#include <FlexLayout.h>
#include <plutovg.h>

typedef enum TEXT_ALIGN
{
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_CENTER,
} TEXT_ALIGN;

typedef enum TRANSFORM_ORIGIN_TYPE
{
    TRANSFORM_ORIGIN_TYPE_KEYWORD,
    TRANSFORM_ORIGIN_TYPE_OFFSET,

} TRANSFORM_ORIGIN_TYPE;

typedef enum TRANSFORM_ORIGIN
{
    TRANSFORM_ORIGIN_LEFT = 0,
    TRANSFORM_ORIGIN_TOP = 0,
    TRANSFORM_ORIGIN_CENTER = 1,
    TRANSFORM_ORIGIN_RIGHT = 2,
    TRANSFORM_ORIGIN_BOTTOM = 2,
} TRANSFORM_ORIGIN;

struct transform_origin_t
{
    TRANSFORM_ORIGIN_TYPE type;

    union
    {
        TRANSFORM_ORIGIN keyword;
        double offset;
    } x, y;
};

struct Box
{
    float border_radius[4];
    struct plutovg_color border_color;
    struct plutovg_color fill_color;
    struct plutovg_color font_color;
    TEXT_ALIGN align;
    char *text;
    double font_size;
    char *background_image;
    char *content_image;
    plutovg_matrix_t transform[1];
    struct transform_origin_t transform_origin;
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
void Flex_transform_matrix(FlexNodeRef node, double m00, double m10, double m01, double m11, double m02, double m12);
void Flex_transform_translate(FlexNodeRef node, double x, double y);
void Flex_transform_rotate(FlexNodeRef node, double radians);
void Flex_transform_skew(FlexNodeRef node, double x, double y);
void Flex_transform_origin_keyword(FlexNodeRef node, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y);
void Flex_transform_origin_offset(FlexNodeRef node, double x, double y);

void Flex_drawNode(FlexNodeRef node, float x, float y);