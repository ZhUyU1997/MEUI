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
    ["FlexLength", "rowGap", false, false],
    ["FlexLength", "columnGap", false, false],
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

${style_table
    .map(([typeName, fieldName]) => `\t${typeName} ${fieldName};`)
    .join("\n")}
} box_style_t;
`

style_header += `
typedef uint64_t box_style_flag_t;
${style_table
    .map(
        ([, fieldName], index) =>
            `#define BOX_STYLE_${fieldName} (UINT64_C(1) << ${index})`
    )
    .join("\n")}

`

function GenCFunctionDeclaration(returnType, name, ...args) {
    const _argument = args
        .map((arg, index, arr) => arg + (arr.length - 1 === index ? "" : ", "))
        .join("")
    return `${returnType} ${name}(${_argument});\n`
}

function GenCFunctionDefine(returnType, name, ...args) {
    const _argument = args
        .slice(0, -1)
        .map((arg, index, arr) => arg + (arr.length - 1 === index ? "" : ", "))
        .join("")
    return `${returnType} ${name}(${_argument})
{${args[args.length - 1]}}
`
}

function GenBoxStyleDeclaration(name, ...args) {
    return GenCFunctionDeclaration(
        "void",
        "box_style_" + name,
        "box_style_t *style",
        ...args
    )
}

function GenBoxStyleDefine(name, ...args) {
    return GenCFunctionDefine(
        "void",
        "box_style_" + name,
        "box_style_t *style",
        ...args
    )
}

let count = 0
style_table.forEach(
    ([typeName, fieldName, supportAuto, supportContent], index) => {
        if (
            [
                "FlexWrapMode",
                "FlexDirection",
                "FlexAlign",
                "float",
                "plutovg_color_t",
                "TEXT_ALIGN",
                "double",
                "CSS_OVERFLOW",
                "int",
            ].includes(typeName)
        ) {
            count = count + 1
            style_header += GenBoxStyleDeclaration(
                fieldName,
                `${typeName} ${fieldName}`
            )
        } else if (typeName === "char *") {
            count = count + 1
            style_header += GenBoxStyleDeclaration(
                fieldName,
                `const char *${fieldName}`
            )
        } else if (typeName === "plutovg_matrix_t") {
            count = count + 1
            style_header += GenBoxStyleDeclaration(
                fieldName,
                "double m00",
                "double m10",
                "double m01",
                "double m11",
                "double m02",
                "double m12"
            )
        } else if (typeName === "struct transform_origin_t") {
            count = count + 1
            style_header += GenBoxStyleDeclaration(
                fieldName + "Keyword",
                "TRANSFORM_ORIGIN x",
                "TRANSFORM_ORIGIN y"
            )
            style_header += GenBoxStyleDeclaration(
                fieldName + "Offset",
                "double x",
                "double y"
            )
        } else if (typeName === "FlexLength") {
            count = count + 1
            style_header += GenBoxStyleDeclaration(
                fieldName,
                `float ${fieldName}`
            )
            style_header += GenBoxStyleDeclaration(
                fieldName + "Percent",
                `float ${fieldName}`
            )
            if (supportAuto) {
                style_header += GenBoxStyleDeclaration(fieldName + "Auto")
            }

            if (supportContent) {
                style_header += GenBoxStyleDeclaration(fieldName + "Content")
            }
        }
    }
)

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

writeFileSync("include/gen/style.h", style_header)

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
style_table.forEach(
    ([typeName, fieldName, supportAuto, supportContent], index) => {
        if (
            [
                "FlexWrapMode",
                "FlexDirection",
                "FlexAlign",
                "float",
                "plutovg_color_t",
                "TEXT_ALIGN",
                "double",
                "CSS_OVERFLOW",
                "int",
            ].includes(typeName)
        ) {
            count = count + 1
            style_source += GenBoxStyleDefine2(
                fieldName,
                index,
                `${typeName} ${fieldName}`,
                `style->${fieldName} = ${fieldName};`
            )
        } else if (typeName === "char *") {
            count = count + 1
            style_source += GenBoxStyleDefine2(
                fieldName,
                index,
                `const char *${fieldName}`,
                `assert(${fieldName});
    if (style->${fieldName}) free(style->${fieldName});
    style->${fieldName} = strdup(${fieldName});`
            )
        } else if (typeName === "plutovg_matrix_t") {
            count = count + 1
            style_source += GenBoxStyleDefine2(
                fieldName,
                index,
                "double m00",
                "double m10",
                "double m01",
                "double m11",
                "double m02",
                "double m12",
                `plutovg_matrix_init(&style->${fieldName}, m00, m10, m01, m11, m02, m12);`
            )
        } else if (typeName === "struct transform_origin_t") {
            count = count + 1
            style_source += GenBoxStyleDefine2(
                fieldName + "Keyword",
                index,
                "TRANSFORM_ORIGIN x",
                "TRANSFORM_ORIGIN y",
                `style->${fieldName}.type = TRANSFORM_ORIGIN_TYPE_KEYWORD;
    style->${fieldName}.x.keyword = x;
    style->${fieldName}.y.keyword = y;`
            )
            style_source += GenBoxStyleDefine2(
                fieldName + "Offset",
                index,
                "double x",
                "double y",
                `style->${fieldName}.type = TRANSFORM_ORIGIN_TYPE_OFFSET;
    style->${fieldName}.x.offset = x;
    style->${fieldName}.y.offset = y;`
            )
        } else if (typeName === "FlexLength") {
            count = count + 1
            style_source += GenBoxStyleDefine2(
                fieldName,
                index,
                `float ${fieldName}`,
                `style->${fieldName}.value = ${fieldName};
    style->${fieldName}.type = FlexLengthTypePoint;`
            )
            style_source += GenBoxStyleDefine2(
                fieldName + "Percent",
                index,
                `float ${fieldName}`,
                `style->${fieldName}.value = ${fieldName};
    style->${fieldName}.type = FlexLengthTypePercent;`
            )
            if (supportAuto) {
                style_source += GenBoxStyleDefine2(
                    fieldName + "Auto",
                    index,
                    `style->${fieldName} = FlexLengthAuto;`
                )
            }

            if (supportContent) {
                style_source += GenBoxStyleDefine2(
                    fieldName + "Content",
                    index,
                    `style->${fieldName} = FlexLengthContent;`
                )
            }
        }
    }
)

style_source += `// ${style_table.length} ${count}\n`

style_source += `
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
    .filter(([typeName]) => ["char *"].includes(typeName))
    .map(
        ([typeName, fieldName]) =>
            `
    if (style->${fieldName})
        free(style->${fieldName});
    style->${fieldName} = NULL;

`
    )
    .join("")}
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
${style_table
    .map(([typeName, fieldName], index) => {
        if (["char *"].includes(typeName)) {
            return `
            case ${index}:
                if(dst->${fieldName}) free(dst->${fieldName});
                dst->${fieldName} = strdup(src->${fieldName});
                break;
`
        } else {
            return `
            case ${index}:
                dst->${fieldName} = src->${fieldName};
                break;
`
        }
    })
    .join("")}
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
${style_table
    .map(([typeName, fieldName], index) => {
        const name = fieldName.slice(0, 1).toUpperCase() + fieldName.slice(1)

        if (["FlexWrapMode", "FlexDirection", "FlexAlign"].includes(typeName)) {
            return `
            case ${index}:
                Flex_set${name}(box, style->${fieldName});
                break;
`
        } else if (
            ["FlexLength"].includes(typeName) &&
            !["left", "right", "top", "bottom"].includes(fieldName)
        ) {
            return `
            case ${index}:
                Flex_set${name}_Length(box, style->${fieldName});
                break;
`
        } else if (
            [
                "borderLeft",
                "borderTop",
                "borderBottom",
                "borderRight",
                "borderStart",
                "borderEnd",
                "flexGrow",
                "flexShrink",
            ].includes(fieldName)
        ) {
            return `
            case ${index}:
                Flex_set${name}(box, style->${fieldName});
                break;
`
        }
    })
    .join("")}
            default:
                break;
        }
    }
}
`
writeFileSync("include/gen/style.c", style_source)
