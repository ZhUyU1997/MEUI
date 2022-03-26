#include "box.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gen/style.c"

static box_style_t default_style = {
    .fontColor.a = 1.0,
    .fontSize = 16,
    .textAlign = TEXT_ALIGN_CENTER_LEFT,
    .transformOrigin.type = TRANSFORM_ORIGIN_TYPE_KEYWORD,
    .transformOrigin.x.keyword = TRANSFORM_ORIGIN_CENTER,
    .transformOrigin.y.keyword = TRANSFORM_ORIGIN_CENTER,
    .left = FlexLengthUndefined,
    .right = FlexLengthUndefined,
    .top = FlexLengthUndefined,
    .bottom = FlexLengthUndefined,
};

void box_default_style_init()
{
    plutovg_matrix_init_identity(&default_style.transform);

    default_style.flags = BOX_STYLE_fontColor | BOX_STYLE_fontSize | BOX_STYLE_textAlign | BOX_STYLE_transformOrigin | BOX_STYLE_transform;
}

const box_style_t *box_default_style()
{
    return &default_style;
}