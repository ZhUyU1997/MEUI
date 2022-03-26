#!/usr/bin/env python3

style_table = [
    # type,field,auto,content,default
    ["FlexWrapMode", "wrap", False, False],
    ["FlexDirection", "direction", False, False],
    ["FlexAlign", "alignItems", False, False],
    ["FlexAlign", "alignSelf", False, False],
    ["FlexAlign", "alignContent", False, False],
    ["FlexAlign", "justifyContent", False, False],
    ["FlexLength", "flexBasis", True, True],
    ["float", "flexGrow", False, False],
    ["float", "flexShrink", False, False],
    ["FlexLength", "width", True, False],
    ["FlexLength", "height", True, False],
    ["FlexLength", "minWidth", False, False],
    ["FlexLength", "minHeight", False, False],
    ["FlexLength", "maxWidth", False, False],
    ["FlexLength", "maxHeight", False, False],
    ["FlexLength", "marginLeft", True, False],
    ["FlexLength", "marginTop", True, False],
    ["FlexLength", "marginBottom", True, False],
    ["FlexLength", "marginRight", True, False],
    ["FlexLength", "marginStart", True, False],
    ["FlexLength", "marginEnd", True, False],
    ["FlexLength", "paddingLeft", False, False],
    ["FlexLength", "paddingTop", False, False],
    ["FlexLength", "paddingBottom", False, False],
    ["FlexLength", "paddingRight", False, False],
    ["FlexLength", "paddingStart", False, False],
    ["FlexLength", "paddingEnd", False, False],
    ["float", "borderLeft", False, False],
    ["float", "borderTop", False, False],
    ["float", "borderBottom", False, False],
    ["float", "borderRight", False, False],
    ["float", "borderStart", False, False],
    ["float", "borderEnd", False, False],
    ["float", "borderTopLeftRadius", False, False],
    ["float", "borderTopRightRadius", False, False],
    ["float", "borderBottomRightRadius", False, False],
    ["float", "borderBottomLeftRadius", False, False],
    ["plutovg_color_t", "borderColor", False, False],
    ["plutovg_color_t", "backgroundColor", False, False],
    ["plutovg_color_t", "fontColor", False, False],
    ["TEXT_ALIGN", "textAlign", False, False],
    ["double", "fontSize", False, False],
    ["char *", "backgroundImage", False, False],
    ["char *", "contentImage", False, False],
    ["plutovg_matrix_t", "transform", False, False],
    ["struct transform_origin_t", "transformOrigin", False, False],
    ["char *", "text", False, False],
    ["char *", "fontFamily", False, False],
    ["CSS_OVERFLOW", "overflow", False, False],
    ["FlexLength", "left", False, False],
    ["FlexLength", "right", False, False],
    ["FlexLength", "top", False, False],
    ["FlexLength", "bottom", False, False],
    ["int", "zIndex", False, False],
]

if len(style_table) > 64:
    print("style too many")
    exit(-1)


with open('include/gen/style.h', 'wt') as f:
    print('''#pragma once
typedef struct box_style_t
{
    unsigned long flags; // 0: null, 1: assigned
''', file=f)

    for item in style_table:
        print(f"\t{item[0]} {item[1]};", file=f)

    print('''
} box_style_t;
''', file=f)

    print('''
enum BOX_STYLE
{
''', file=f)

    for index, item in enumerate(style_table):
        print(f"\tBOX_STYLE_{item[1]} = 1UL << {index},", file=f)
    print('''
};
''', file=f)

    count = 0
    for index, item in enumerate(style_table):
        if item[0] in ["FlexWrapMode", "FlexDirection", "FlexAlign", "float", "plutovg_color_t", "TEXT_ALIGN", "double", "CSS_OVERFLOW", "int"]:
            count = count + 1
            print(
                f"void box_style_{item[1]}(box_style_t *style, {item[0]} {item[1]});", file=f)
        elif item[0] == "char *":
            count = count + 1
            print(
                f"void box_style_{item[1]}(box_style_t *style, const char *{item[1]});", file=f)
        elif item[0] == "plutovg_matrix_t":
            count = count + 1
            print(
                f"void box_style_{item[1]}(box_style_t *style, double m00, double m10, double m01, double m11, double m02, double m12);", file=f)
        elif item[0] == "struct transform_origin_t":
            count = count + 1
            print(f'''void box_style_{item[1]}Keyword(box_style_t *style, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y);
void box_style_{item[1]}Offset(box_style_t *style, double x, double y);''', file=f)
        elif item[0] == "FlexLength":
            count = count + 1
            supportAuto = item[2]
            supportContent = item[2]
            print(f'''void box_style_{item[1]}(box_style_t *style, float {item[1]});
void box_style_{item[1]}Percent(box_style_t *style, float {item[1]});''', file=f)
            if supportAuto:
                print(
                    f'''void box_style_{item[1]}Auto(box_style_t *style);''', file=f)
            if supportContent:
                print(
                    f'''void box_style_{item[1]}Content(box_style_t *style);''', file=f)

    print(f"// {len(style_table)} {count}", file=f)

    print(f'''box_style_t *box_style_new();
void box_style_clear(box_style_t *style);
void box_style_free(box_style_t *style);
int box_style_is_unset(box_style_t *style, enum BOX_STYLE prop);
void box_style_merge(box_style_t *dst, const box_style_t *src);
void box_style_to_flex(box_style_t *style, box_t box);''', file=f)

with open('include/gen/style.c', 'wt') as f:
    count = 0
    for index, item in enumerate(style_table):
        if item[0] in ["FlexWrapMode", "FlexDirection", "FlexAlign", "float", "plutovg_color_t", "TEXT_ALIGN", "double", "CSS_OVERFLOW", "int"]:
            count = count + 1
            print(f'''
void box_style_{item[1]}(box_style_t *style, {item[0]} {item[1]})
{{
    assert(style);
    style->{item[1]} = {item[1]};
    style->flags |= 1UL << {index};
}}
''', file=f)
        elif item[0] == "char *":
            count = count + 1
            print(f'''
void box_style_{item[1]}(box_style_t *style, const char *{item[1]})
{{
    assert(style && {item[1]});
    if (style->{item[1]}) free(style->{item[1]});
    style->{item[1]} = strdup({item[1]});
    style->flags |= 1UL << {index};
}}
''', file=f)
        elif item[0] == "plutovg_matrix_t":
            count = count + 1
            print(f'''
void box_style_{item[1]}(box_style_t *style, double m00, double m10, double m01, double m11, double m02, double m12)
{{
    assert(style);
    plutovg_matrix_init(&style->{item[1]}, m00, m10, m01, m11, m02, m12);
    style->flags |= 1UL << {index};
}}
''', file=f)
        elif item[0] == "struct transform_origin_t":
            count = count + 1
            print(f'''
void box_style_{item[1]}Keyword(box_style_t *style, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y)
{{
    assert(style);
    style->{item[1]}.type = TRANSFORM_ORIGIN_TYPE_KEYWORD;
    style->{item[1]}.x.keyword = x;
    style->{item[1]}.y.keyword = y;
    style->flags |= 1UL << {index};
}}

void box_style_{item[1]}Offset(box_style_t *style, double x, double y)
{{
    assert(style);
    style->{item[1]}.type = TRANSFORM_ORIGIN_TYPE_OFFSET;
    style->{item[1]}.x.offset = x;
    style->{item[1]}.y.offset = y;
    style->flags |= 1UL << {index};
}}
''', file=f)
        elif item[0] == "FlexLength":
            count = count + 1
            supportAuto = item[2]
            supportContent = item[2]
            print(f'''
void box_style_{item[1]}(box_style_t *style, float {item[1]})
{{
    assert(style);
    style->{item[1]}.value = {item[1]};
    style->{item[1]}.type = FlexLengthTypePoint;
    style->flags |= 1UL << {index};
}}

void box_style_{item[1]}Percent(box_style_t *style, float {item[1]})
{{
    assert(style);
    style->{item[1]}.value = {item[1]};
    style->{item[1]}.type = FlexLengthTypePercent;
    style->flags |= 1UL << {index};
}}
''', file=f)
            if supportAuto:
                print(f'''
void box_style_{item[1]}Auto(box_style_t *style)
{{
    assert(style);
    style->{item[1]}.type = FlexLengthTypeAuto;
    style->flags |= 1UL << {index};
}}
''', file=f)
            if supportContent:
                print(f'''
void box_style_{item[1]}Content(box_style_t *style)
{{
    assert(style);
    style->{item[1]}.type = FlexLengthTypeAuto;
    style->flags |= 1UL << {index};
}}
''', file=f)

    print(f"// {len(style_table)} {count}", file=f)

    print(f'''
box_style_t *box_style_new()
{{
    box_style_t *style = calloc(1, sizeof(box_style_t));
    style->flags = 0UL;
    return style;
}}

void box_style_clear(box_style_t *style)
{{
    assert(style);
''', file=f)

    for index, item in enumerate(style_table):
        if item[0] in ["char *"]:
            print(f'''
    if (style->{item[1]})
        free(style->{item[1]});
    style->{item[1]} = NULL;
''', file=f)

    print(f'''
    *style = (box_style_t){{}};
    style->flags = 0UL;
}}

void box_style_free(box_style_t *style)
{{
    assert(style);
    box_style_clear(style);
    free(style);
}}

int box_style_is_unset(box_style_t *style, enum BOX_STYLE prop)
{{
    return !(style->flags & prop);
}}
''', file=f)

    print(f'''
void box_style_merge(box_style_t *dst, const box_style_t *src)
{{
    assert(dst && src);

    for(int i = 0; i < 64; i++)
    {{
        if(!(src->flags & (1UL << i)))
            continue;
        switch(i)
        {{''', file=f)

    for index, item in enumerate(style_table):
        if item[0] in ["char *"]:
            print(f'''
            case {index}:
                if(dst->{item[1]}) free(dst->{item[1]});
                dst->{item[1]} = strdup(src->{item[1]});
                break;''', file=f)
        else:
            print(f'''
            case {index}:
                dst->{item[1]} = src->{item[1]};
                break;''', file=f)
    print(f'''
        }}
        dst->flags |= 1UL << i;
    }}
}}
''', file=f)

    print(f'''
void box_style_to_flex(box_style_t *style, box_t box)
{{
    assert(box && style);

    for(int i = 0; i < 64; i++)
    {{
        if(!(style->flags & (1UL << i)))
            continue;
        switch(i)
        {{''', file=f)

    for index, item in enumerate(style_table):
        if item[0] in ["FlexWrapMode", "FlexDirection", "FlexAlign"]:
            print(f'''
            case {index}:
                Flex_set{item[1][:1].upper()}{item[1][1:]}(box, style->{item[1]});
                break;''', file=f)
        elif item[0] in ["FlexLength"] and not item[1] in ["left", "right", "top", "bottom"]:
            print(f'''
            case {index}:
                Flex_set{item[1][:1].upper()}{item[1][1:]}_Length(box, style->{item[1]});
                break;''', file=f)
        elif item[1] in ["borderLeft", "borderTop", "borderBottom", "borderRight", "borderStart", "borderEnd", "flexGrow", "flexShrink"]:
            print(f'''
            case {index}:
                Flex_set{item[1][:1].upper()}{item[1][1:]}(box, style->{item[1]});
                break;''', file=f)
    print(f'''
            default:
                break;
        }}
    }}
}}
''', file=f)
