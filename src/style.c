/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "meui/box.h"
#include "gen/style.c"

uint64_t box_style_layout_flags = BOX_STYLE_wrap |
                                  BOX_STYLE_direction |
                                  BOX_STYLE_alignItems |
                                  BOX_STYLE_alignSelf |
                                  BOX_STYLE_alignContent |
                                  BOX_STYLE_justifyContent |
                                  BOX_STYLE_flexBasis |
                                  BOX_STYLE_flexGrow |
                                  BOX_STYLE_flexShrink |
                                  BOX_STYLE_width |
                                  BOX_STYLE_height |
                                  BOX_STYLE_minWidth |
                                  BOX_STYLE_minHeight |
                                  BOX_STYLE_maxWidth |
                                  BOX_STYLE_maxHeight |
                                  BOX_STYLE_marginLeft |
                                  BOX_STYLE_marginTop |
                                  BOX_STYLE_marginBottom |
                                  BOX_STYLE_marginRight |
                                  BOX_STYLE_marginStart |
                                  BOX_STYLE_marginEnd |
                                  BOX_STYLE_paddingLeft |
                                  BOX_STYLE_paddingTop |
                                  BOX_STYLE_paddingBottom |
                                  BOX_STYLE_paddingRight |
                                  BOX_STYLE_paddingStart |
                                  BOX_STYLE_paddingEnd |
                                  BOX_STYLE_borderLeft |
                                  BOX_STYLE_borderTop |
                                  BOX_STYLE_borderBottom |
                                  BOX_STYLE_borderRight |
                                  BOX_STYLE_borderStart |
                                  BOX_STYLE_borderEnd |
                                  BOX_STYLE_borderTopLeftRadius |
                                  BOX_STYLE_borderTopRightRadius |
                                  BOX_STYLE_borderBottomRightRadius |
                                  BOX_STYLE_borderBottomLeftRadius |
                                  //   BOX_STYLE_borderColor |
                                  //   BOX_STYLE_backgroundColor |
                                  //   BOX_STYLE_fontColor |
                                  BOX_STYLE_textAlign |
                                  BOX_STYLE_fontSize |
                                  //   BOX_STYLE_backgroundImage |
                                  //   BOX_STYLE_contentImage |
                                  //   BOX_STYLE_transform |
                                  //   BOX_STYLE_transformOrigin |
                                  //   BOX_STYLE_fontFamily |
                                  BOX_STYLE_overflow |
                                  BOX_STYLE_left |
                                  BOX_STYLE_right |
                                  BOX_STYLE_top |
                                  BOX_STYLE_bottom |
                                  0;

static box_style_t default_style = {
    .flags =
        BOX_STYLE_wrap |
        BOX_STYLE_direction |
        BOX_STYLE_alignItems |
        BOX_STYLE_alignSelf |
        BOX_STYLE_alignContent |
        BOX_STYLE_justifyContent |
        BOX_STYLE_flexBasis |
        BOX_STYLE_flexGrow |
        BOX_STYLE_flexShrink |
        BOX_STYLE_width |
        BOX_STYLE_height |
        BOX_STYLE_minWidth |
        BOX_STYLE_minHeight |
        BOX_STYLE_maxWidth |
        BOX_STYLE_maxHeight |
        BOX_STYLE_marginLeft |
        BOX_STYLE_marginTop |
        BOX_STYLE_marginBottom |
        BOX_STYLE_marginRight |
        BOX_STYLE_marginStart |
        BOX_STYLE_marginEnd |
        BOX_STYLE_paddingLeft |
        BOX_STYLE_paddingTop |
        BOX_STYLE_paddingBottom |
        BOX_STYLE_paddingRight |
        BOX_STYLE_paddingStart |
        BOX_STYLE_paddingEnd |
        BOX_STYLE_borderLeft |
        BOX_STYLE_borderTop |
        BOX_STYLE_borderBottom |
        BOX_STYLE_borderRight |
        BOX_STYLE_borderStart |
        BOX_STYLE_borderEnd |
        BOX_STYLE_borderTopLeftRadius |
        BOX_STYLE_borderTopRightRadius |
        BOX_STYLE_borderBottomRightRadius |
        BOX_STYLE_borderBottomLeftRadius |
        BOX_STYLE_borderColor |
        BOX_STYLE_backgroundColor |
        BOX_STYLE_fontColor |
        BOX_STYLE_textAlign |
        BOX_STYLE_fontSize |
        // BOX_STYLE_backgroundImage |
        // BOX_STYLE_contentImage |
        BOX_STYLE_transform |
        BOX_STYLE_transformOrigin |
        // BOX_STYLE_fontFamily |
        BOX_STYLE_overflow |
        BOX_STYLE_left |
        BOX_STYLE_right |
        BOX_STYLE_top |
        BOX_STYLE_bottom |
        // BOX_STYLE_zIndex | // unset is different from zero
        0,

    .wrap = FlexNoWrap,
    .direction = FlexHorizontal,
    .alignItems = FlexStretch,
    .alignSelf = FlexInherit,
    .alignContent = FlexStretch,
    .justifyContent = FlexStart,
    .flexBasis = FlexLengthAuto,
    .flexGrow = 0,
    .flexShrink = 1,

    .width = FlexLengthAuto,
    .height = FlexLengthAuto,
    .minWidth = FlexLengthZero,
    .minHeight = FlexLengthZero,
    .maxWidth = FlexLengthUndefined,
    .maxHeight = FlexLengthUndefined,

    .marginLeft = FlexLengthZero,
    .marginTop = FlexLengthZero,
    .marginBottom = FlexLengthZero,
    .marginRight = FlexLengthZero,
    .marginStart = FlexLengthUndefined,
    .marginEnd = FlexLengthUndefined,
    .paddingLeft = FlexLengthZero,
    .paddingTop = FlexLengthZero,
    .paddingBottom = FlexLengthZero,
    .paddingRight = FlexLengthZero,
    .paddingStart = FlexLengthUndefined,
    .paddingEnd = FlexLengthUndefined,

    .borderLeft = 0,
    .borderTop = 0,
    .borderBottom = 0,
    .borderRight = 0,
    .borderStart = FlexUndefined,
    .borderEnd = FlexUndefined,
    .borderTopLeftRadius = 0,
    .borderTopRightRadius = 0,
    .borderBottomRightRadius = 0,
    .borderBottomLeftRadius = 0,

    .borderColor = {0, 0, 0, 0},
    .backgroundColor = {0, 0, 0, 0},
    .fontColor = {0, 0, 0, 1},
    .textAlign = TEXT_ALIGN_CENTER_LEFT,
    .fontSize = 16,
    .backgroundImage = NULL,
    .contentImage = NULL,
    .transform = {
        .m00 = 1.0,
        .m10 = 0.0,
        .m01 = 0.0,
        .m11 = 1.0,
        .m02 = 0.0,
        .m12 = 0.0,
    },
    .transformOrigin.type = TRANSFORM_ORIGIN_TYPE_KEYWORD,
    .transformOrigin.x.keyword = TRANSFORM_ORIGIN_CENTER,
    .transformOrigin.y.keyword = TRANSFORM_ORIGIN_CENTER,
    .fontFamily = NULL,
    .overflow = CSS_OVERFLOW_VISIBLE,
    .left = FlexLengthUndefined,
    .right = FlexLengthUndefined,
    .top = FlexLengthUndefined,
    .bottom = FlexLengthUndefined,
};

void box_default_style_init()
{
}

const box_style_t *box_default_style()
{
    return &default_style;
}