/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

import { writeFileSync } from "fs"
const style_table = [
    // type,field,auto,content,default
    ["FlexWrapMode", "wrap", false, false],
    ["FlexDirection", "direction", false, false],
    ["FlexAlign", "alignItems", false, false],
    ["FlexAlign", "alignSelf", false, false],
    ["FlexAlign", "alignContent", false, false],
    ["FlexAlign", "justifyContent", false, false],
    ["FlexLength", "flexBasis", true, true],
    ["float", "flexGrow", false, false],
    ["float", "flexShrink", false, false],
    ["FlexLength", "width", true, false],
    ["FlexLength", "height", true, false],
    ["FlexLength", "minWidth", false, false],
    ["FlexLength", "minHeight", false, false],
    ["FlexLength", "maxWidth", false, false],
    ["FlexLength", "maxHeight", false, false],
    ["FlexLength", "marginLeft", true, false],
    ["FlexLength", "marginTop", true, false],
    ["FlexLength", "marginBottom", true, false],
    ["FlexLength", "marginRight", true, false],
    ["FlexLength", "marginStart", true, false],
    ["FlexLength", "marginEnd", true, false],
    ["FlexLength", "paddingLeft", false, false],
    ["FlexLength", "paddingTop", false, false],
    ["FlexLength", "paddingBottom", false, false],
    ["FlexLength", "paddingRight", false, false],
    ["FlexLength", "paddingStart", false, false],
    ["FlexLength", "paddingEnd", false, false],
    ["float", "borderLeft", false, false],
    ["float", "borderTop", false, false],
    ["float", "borderBottom", false, false],
    ["float", "borderRight", false, false],
    ["float", "borderStart", false, false],
    ["float", "borderEnd", false, false],
    ["float", "borderTopLeftRadius", false, false],
    ["float", "borderTopRightRadius", false, false],
    ["float", "borderBottomRightRadius", false, false],
    ["float", "borderBottomLeftRadius", false, false],
    ["plutovg_color_t", "borderColor", false, false],
    ["plutovg_color_t", "backgroundColor", false, false],
    ["plutovg_color_t", "fontColor", false, false],
    ["TEXT_ALIGN", "textAlign", false, false],
    ["double", "fontSize", false, false],
    ["char *", "backgroundImage", false, false],
    ["char *", "contentImage", false, false],
    ["plutovg_matrix_t", "transform", false, false],
    ["struct transform_origin_t", "transformOrigin", false, false],
    ["char *", "fontFamily", false, false],
    ["CSS_OVERFLOW", "overflow", false, false],
    ["FlexLength", "left", false, false],
    ["FlexLength", "right", false, false],
    ["FlexLength", "top", false, false],
    ["FlexLength", "bottom", false, false],
    ["int", "zIndex", false, false],
]

if (style_table.length > 64) {
    console.error("style too many")
    process.exit(-1)
}

let style_header = `#pragma once
#include <stdint.h>
typedef struct box_style_t
{
    uint64_t flags; // 0: null, 1: assigned
    uint64_t dirty;

${style_table.map(item => `\t${item[0]} ${item[1]};`).join("\n")}
} box_style_t;
`

style_header += `
typedef uint64_t box_style_flag_t;
${style_table.map((item, index) => `#define BOX_STYLE_${item[1]} (UINT64_C(1) << ${index})`).join("\n")}

`

function GenCFunctionDeclaration(returnType, name, ...args) {
    const _argument = args.map((arg, index, arr) => arg + (arr.length - 1 === index ? "" : ", ")).join("")
    return `${returnType} ${name}(${_argument});\n`
}

function GenCFunctionDefine(returnType, name, ...args) {
    const _argument = args.slice(0, -1).map((arg, index, arr) => arg + (arr.length - 1 === index ? "" : ", ")).join("")
    return `${returnType} ${name}(${_argument})
{${args[args.length - 1]}}
`
}

function GenBoxStyleDeclaration(name, ...args) {
    return GenCFunctionDeclaration("void", "box_style_" + name, "box_style_t *style", ...args)
}

function GenBoxStyleDefine(name, ...args) {
    return GenCFunctionDefine("void", "box_style_" + name, "box_style_t *style", ...args)
}

let count = 0
style_table.forEach((item, index) => {
    if (["FlexWrapMode", "FlexDirection", "FlexAlign", "float", "plutovg_color_t", "TEXT_ALIGN", "double", "CSS_OVERFLOW", "int"]
        .includes(item[0])) {
        count = count + 1
        style_header += GenBoxStyleDeclaration(item[1], `${item[0]} ${item[1]}`)
    }
    else if (item[0] === "char *") {
        count = count + 1
        style_header += GenBoxStyleDeclaration(item[1], `const char *${item[1]}`)
    }
    else if (item[0] === "plutovg_matrix_t") {
        count = count + 1
        style_header += GenBoxStyleDeclaration(item[1], "double m00", "double m10", "double m01", "double m11", "double m02", "double m12")
    }
    else if (item[0] === "struct transform_origin_t") {
        count = count + 1
        style_header += GenBoxStyleDeclaration(item[1] + "Keyword", "TRANSFORM_ORIGIN x", "TRANSFORM_ORIGIN y")
        style_header += GenBoxStyleDeclaration(item[1] + "Offset", "double x", "double y")
    }
    else if (item[0] === "FlexLength") {
        count = count + 1
        const supportAuto = item[2]
        const supportContent = item[2]
        style_header += GenBoxStyleDeclaration(item[1], `float ${item[1]}`)
        style_header += GenBoxStyleDeclaration(item[1] + "Percent", `float ${item[1]}`)
        if (supportAuto) {
            style_header += GenBoxStyleDeclaration(item[1] + "Auto")
        }

        if (supportContent) {
            style_header += GenBoxStyleDeclaration(item[1] + "Content")
        }
    }
})


style_header += `// ${style_table.length} ${count}\n`

style_header += `box_style_t *box_style_new();
void box_style_clear(box_style_t *style);
void box_style_free(box_style_t *style);
uint64_t box_style_is_dirty(box_style_t *style);
void box_style_clear_dirty(box_style_t *style);
int box_style_is_unset(box_style_t *style, box_style_flag_t prop);
void box_style_unset(box_style_t *dst, box_style_flag_t prop);
void box_style_merge(box_style_t *dst, const box_style_t *src);
void box_style_to_flex(box_style_t *style, box_t box);\n`

writeFileSync('include/gen/style.h', style_header)

let style_source = ""

function GenBoxStyleDefine2(name, index, ...args) {
    args[args.length - 1] = `
    assert(style);
    ${args[args.length - 1]}
    style->flags |= UINT64_C(1) << ${index};
    style->dirty |= UINT64_C(1) << ${index};
`
    return GenBoxStyleDefine(name, ...args)
}

count = 0
style_table.forEach((item, index) => {
    if (["FlexWrapMode", "FlexDirection", "FlexAlign", "float", "plutovg_color_t", "TEXT_ALIGN", "double", "CSS_OVERFLOW", "int"]
        .includes(item[0])) {
        count = count + 1
        style_source += GenBoxStyleDefine2(item[1], index, `${item[0]} ${item[1]}`, `style->${item[1]} = ${item[1]};`)
    }
    else if (item[0] === "char *") {
        count = count + 1
        style_source += GenBoxStyleDefine2(item[1], index, `const char *${item[1]}`, `assert(${item[1]});
    if (style->${item[1]}) free(style->${item[1]});
    style->${item[1]} = strdup(${item[1]});`)
    }
    else if (item[0] === "plutovg_matrix_t") {
        count = count + 1
        style_source += GenBoxStyleDefine2(item[1], index, "double m00", "double m10", "double m01", "double m11", "double m02", "double m12",
            `plutovg_matrix_init(&style->${item[1]}, m00, m10, m01, m11, m02, m12);`)
    }
    else if (item[0] === "struct transform_origin_t") {
        count = count + 1
        style_source += GenBoxStyleDefine2(item[1] + "Keyword", index, "TRANSFORM_ORIGIN x", "TRANSFORM_ORIGIN y", `style->${item[1]}.type = TRANSFORM_ORIGIN_TYPE_KEYWORD;
    style->${item[1]}.x.keyword = x;
    style->${item[1]}.y.keyword = y;`)
        style_source += GenBoxStyleDefine2(item[1] + "Offset", index, "double x", "double y", `style->${item[1]}.type = TRANSFORM_ORIGIN_TYPE_OFFSET;
    style->${item[1]}.x.offset = x;
    style->${item[1]}.y.offset = y;`)
    }
    else if (item[0] === "FlexLength") {
        count = count + 1
        const supportAuto = item[2]
        const supportContent = item[2]
        style_source += GenBoxStyleDefine2(item[1], index, `float ${item[1]}`, `style->${item[1]}.value = ${item[1]};
    style->${item[1]}.type = FlexLengthTypePoint;`)
    style_source += GenBoxStyleDefine2(item[1] + "Percent", index, `float ${item[1]}`, `style->${item[1]}.value = ${item[1]};
    style->${item[1]}.type = FlexLengthTypePercent;`)
        if (supportAuto) {
            style_source += GenBoxStyleDefine2(item[1] + "Auto", index, `style->${item[1]} = FlexLengthAuto;`)
        }

        if (supportContent) {
            style_source += GenBoxStyleDefine2(item[1] + "Content", index, `style->${item[1]} = FlexLengthContent;`)
        }
    }
})

style_source += `// ${style_table.length} ${count}\n`

style_source +=
    `
box_style_t *box_style_new()
{
    box_style_t *style = calloc(1, sizeof(box_style_t));
    style->flags = UINT64_C(0);
    style->dirty = ~UINT64_C(0);
    return style;
}

void box_style_clear(box_style_t *style)
{
    assert(style);

${style_table
        .filter(item => ["char *"].includes(item[0]))
        .map(item =>
            `
    if (style->${item[1]})
        free(style->${item[1]});
    style->${item[1]} = NULL;

`).join("")
    }
    *style = (box_style_t){};
    style->flags = UINT64_C(0);
    style->dirty = ~UINT64_C(0);
}

void box_style_free(box_style_t *style)
{
    assert(style);
    box_style_clear(style);
    free(style);
}

uint64_t box_style_is_dirty(box_style_t *style)
{
    return style->dirty;
}

void box_style_clear_dirty(box_style_t *style)
{
    style->dirty = UINT64_C(0);
}

int box_style_is_unset(box_style_t *style, box_style_flag_t prop)
{
    return !(style->flags & prop);
}


void box_style_unset(box_style_t *dst, box_style_flag_t prop)
{
    assert(dst);
    dst->flags &= ~prop;
    dst->dirty |= prop;
}


void box_style_merge(box_style_t *dst, const box_style_t *src)
{
    assert(dst && src);

    uint64_t flags = src->flags;

    if(!flags)
        return;

    for(int i = 0; i < ${style_table.length}; i++)
    {
        if(!(flags & (UINT64_C(1) << i)))
            continue;
        switch(i)
        {
${style_table.map((item, index) => {
        if (["char *"].includes(item[0])) {

            return `
            case ${index}:
                if(dst->${item[1]}) free(dst->${item[1]});
                dst->${item[1]} = strdup(src->${item[1]});
                break;
`
        }
        else {
            return `
            case ${index}:
                dst->${item[1]} = src->${item[1]};
                break;
`
        }
    }).join("")
    }
        }
        dst->flags |= UINT64_C(1) << i;
        dst->dirty |= UINT64_C(1) << i;
    }
}

`

style_source += `
void box_style_to_flex(box_style_t *style, box_t box)
{
    assert(box && style);

    for(int i = 0; i < 64; i++)
    {
        if(!(style->flags & (UINT64_C(1) << i)))
            continue;
        switch(i)
        {
${style_table.map((item, index) => {
    const name = item[1].slice(0, 1).toUpperCase() + item[1].slice(1)

    if (["FlexWrapMode", "FlexDirection", "FlexAlign"].includes(item[0])) {
        return `
            case ${index}:
                Flex_set${name}(box, style->${item[1]});
                break;
`
    }
    else if (["FlexLength"].includes(item[0]) && !["left", "right", "top", "bottom"].includes(item[1])) {
        return `
            case ${index}:
                Flex_set${name}_Length(box, style->${item[1]});
                break;
`
    }
    else if (["borderLeft", "borderTop", "borderBottom", "borderRight", "borderStart", "borderEnd", "flexGrow", "flexShrink"]
        .includes(item[1])) {
        return `
            case ${index}:
                Flex_set${name}(box, style->${item[1]});
                break;
`
    }

}).join("")

    }
            default:
                break;
        }
    }
}
`
writeFileSync('include/gen/style.c', style_source)