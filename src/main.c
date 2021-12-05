#include <FlexLayout.h>
#include <plutovg.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define WINDOW_W 400
#define WINDOW_H 400

#define KAPPA90 (0.5522847493f)

struct context_t
{
    plutovg_font_face_t *face;
    plutovg_surface_t *surface;
    plutovg_t *pluto;
} context;

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
};

FlexNodeRef Flex_newBox()
{
    FlexNodeRef node = Flex_newNode();
    struct Box *box = calloc(1, sizeof(struct Box));
    box->font_color.a = 1.0;
    box->text = "";
    box->font_size = 16;
    box->align = TEXT_ALIGN_LEFT;
    Flex_setContext(node, box);
    return node;
}

void Flex_freeBox(FlexNodeRef node)
{
    struct Box *box = Flex_getContext(node);
    free(box);
    Flex_freeNode(node);
}

static void round4_rect(plutovg_t *pluto, float r[4][2], int x, int y, int w, int h)
{
    const char a[4][3][2] = {
        {{0, -KAPPA90}, {1 - KAPPA90, -1}, {1, -1}},
        {{KAPPA90, 0}, {1, 1 - KAPPA90}, {1, 1}},
        {{0, KAPPA90}, {-(1 - KAPPA90), 1}, {-1, 1}},
        {{-KAPPA90, 0}, {-1, -(1 - KAPPA90)}, {-1, -1}},
    };

    int index = 0;
    plutovg_move_to(pluto, x, y + r[index][1]);
    plutovg_rel_cubic_to(pluto,
                         r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                         r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                         r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_rel_line_to(pluto, w - (r[index][0] + r[index + 1][0]), 0);
    index = 1;
    plutovg_rel_cubic_to(pluto,
                         r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                         r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                         r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_rel_line_to(pluto, 0, h - (r[index][1] + r[index + 1][1]));
    index = 2;
    plutovg_rel_cubic_to(pluto,
                         r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                         r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                         r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);
    plutovg_rel_line_to(pluto, -w + (r[index][0] + r[index + 1][0]), 0);
    index = 3;
    plutovg_rel_cubic_to(pluto,
                         r[index][0] * a[index][0][0], r[index][1] * a[index][0][1],
                         r[index][0] * a[index][1][0], r[index][1] * a[index][1][1],
                         r[index][0] * a[index][2][0], r[index][1] * a[index][2][1]);

    plutovg_close_path(pluto);
}

static void DrawBoxBackground(plutovg_t *pluto, float x, float y, float w, float h, float radius[4], float border[4], struct plutovg_color border_color, struct plutovg_color fill_color)
{
    float r[4][2] = {0};

    if (radius != NULL)
    {
        int max_index = 0;

        for (int i = 1; i < 4; i++)
            if (radius[i] > radius[max_index])
                max_index = i;

        float radius_max = radius[max_index];

        if (radius_max > 0.0f)
        {
            float radius_next = radius[(max_index + 1) % 4];
            float radius_other = radius[(max_index + 2) % 4];
            float radius_prev = radius[(max_index + 3) % 4];

            float r1 = radius_next + radius_max > w ? w * radius_max / (radius_next + radius_max) : radius_max;
            float r2 = radius_prev + radius_max > w ? w * radius_max / (radius_prev + radius_max) : radius_max;

            r[max_index][0] = r[max_index][1] = fmaxf(r1, r2);
            r[(max_index + 1) % 4][0] = r[(max_index + 1) % 4][1] = r[max_index][0] * radius_next / radius_max;
            r[(max_index + 2) % 4][0] = r[(max_index + 2) % 4][1] = r[max_index][0] * radius_other / radius_max;
            r[(max_index + 3) % 4][0] = r[(max_index + 3) % 4][1] = r[max_index][0] * radius_prev / radius_max;
        }
    }

    plutovg_save(pluto);

    round4_rect(pluto, r, x, y, w, h);
    plutovg_set_source_rgba(pluto, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
    plutovg_fill_preserve(pluto);

    if (border != NULL)
    {
        float border_top = border[0] < 0.0 ? 0.0 : border[0];
        float border_right = border[1] < 0.0 ? 0.0 : border[1];
        float border_bottom = border[2] < 0.0 ? 0.0 : border[2];
        float border_left = border[3] < 0.0 ? 0.0 : border[3];
        float border_h = border_right + border_left;
        float border_v = border_top + border_bottom;

        float r_inner[4][2] = {
            {
                (r[0][0] - border_left) > 0.0f ? ((w - r[0][0] > border_right) ? (r[0][0] - border_left) : h - border_h) : 0,
                (r[0][1] - border_top) > 0.0f ? ((h - r[0][1] > border_bottom) ? (r[0][1] - border_top) : h - border_v) : 0,
            },
            {
                (r[1][0] - border_right) > 0.0f ? ((w - r[1][0] > border_left) ? (r[1][0] - border_right) : h - border_h) : 0,
                (r[1][1] - border_top) > 0.0f ? ((h - r[1][1] > border_bottom) ? (r[1][1] - border_top) : h - border_v) : 0,
            },
            {
                (r[2][0] - border_right) > 0.0f ? ((w - r[2][0] > border_left) ? (r[2][0] - border_right) : h - border_h) : 0,
                (r[2][1] - border_bottom) > 0.0f ? ((h - r[2][1] > border_top) ? (r[2][1] - border_bottom) : h - border_v) : 0,
            },
            {
                (r[3][0] - border_left) > 0.0f ? ((w - r[3][0] > border_right) ? (r[3][0] - border_left) : h - border_h) : 0,
                (r[3][1] - border_bottom) > 0.0f ? ((h - r[3][1] > border_top) ? (r[3][1] - border_bottom) : h - border_v) : 0,
            },
        };

        round4_rect(pluto, r_inner,
                    x + border_left,
                    y + border_top,
                    w - border_right - border_left,
                    h - border_top - border_bottom);
        plutovg_set_fill_rule(pluto, plutovg_fill_rule_even_odd);
        plutovg_set_source_rgba(pluto, border_color.r, border_color.g, border_color.b, border_color.a);
        plutovg_fill(pluto);
    }

    plutovg_restore(pluto);
}

static inline int decode_utf8(const char **begin, const char *end, int *codepoint)
{
    static const int trailing[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5};

    static const uint32_t offsets[6] = {
        0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080};

    const char *ptr = *begin;

    int trailing_bytes = trailing[(uint8_t)(*ptr)];
    if (ptr + trailing_bytes >= end)
        return 0;

    uint32_t output = 0;
    switch (trailing_bytes)
    {
    case 5:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 4:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 3:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 2:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 1:
        output += (uint8_t)(*ptr++);
        output <<= 6;
    case 0:
        output += (uint8_t)(*ptr++);
    }

    output -= offsets[trailing_bytes];
    *begin = ptr;
    *codepoint = output;
    return 1;
}

static plutovg_path_t *draw_font_get_textn_oneline_path(const plutovg_font_t *font, const char **utf8, const char *end, double w, double *out_w)
{
    plutovg_path_t *result = plutovg_path_create();
    double advance = 0;
    double scale = plutovg_font_get_scale(font);
    plutovg_font_face_t *face = plutovg_font_get_face(font);

    while (*utf8 < end)
    {
        int ch = 0;
        const char *start = *utf8;
        if (!decode_utf8(utf8, end, &ch))
            break;

        if (ch == '\n')
            break;

        plutovg_matrix_t matrix;
        plutovg_matrix_init_translate(&matrix, advance, 0);
        plutovg_matrix_scale(&matrix, scale, -scale);

        double char_advance = plutovg_font_get_char_advance(font, ch);

        if (advance + char_advance > w)
        {
            *utf8 = start;
            break;
        }

        advance += char_advance;
        plutovg_path_t *path = plutovg_font_face_get_char_path(face, ch);
        plutovg_path_add_path(result, path, &matrix);
        plutovg_path_destroy(path);
    }

    *out_w = advance;

    return result;
}

static plutovg_path_t *draw_font_get_textn_path(const plutovg_font_t *font, TEXT_ALIGN align, const char *utf8, int size, double w, double h)
{
    plutovg_path_t *result = plutovg_path_create();
    double advance = 0;
    double scale = plutovg_font_get_scale(font);
    const char *end = utf8 + size;
    plutovg_font_face_t *face = plutovg_font_get_face(font);
    double ascent = plutovg_font_get_ascent(font);
    double descent = plutovg_font_get_descent(font);
    double line_gap = plutovg_font_get_line_gap(font);
    double leading = plutovg_font_get_leading(font);

    double y = 0.0;

    while (utf8 < end)
    {
        if (y + ascent > h)
            break;

        double line_width = 0;
        plutovg_path_t *line_path = draw_font_get_textn_oneline_path(font, &utf8, end, w, &line_width);

        plutovg_matrix_t matrix;

        if (align == TEXT_ALIGN_LEFT)
            plutovg_matrix_init_translate(&matrix, 0, y);
        else if (align == TEXT_ALIGN_RIGHT)
            plutovg_matrix_init_translate(&matrix, w - line_width, y);
        else if (align == TEXT_ALIGN_CENTER)
            plutovg_matrix_init_translate(&matrix, (w - line_width) / 2.0, y);

        plutovg_path_add_path(result, line_path, &matrix);
        plutovg_path_destroy(line_path);

        y += ascent;
    }

    return result;
}

static void DrawText(plutovg_t *pluto, double size, struct plutovg_color color, TEXT_ALIGN align, const char *utf8, double x, double y, double w, double h)
{
    plutovg_save(pluto);

    plutovg_font_t *font = plutovg_font_load_from_face(context.face, size);

    plutovg_set_font(pluto, font);
    double ascent = plutovg_font_get_ascent(font);

    plutovg_matrix_t matrix;
    plutovg_matrix_init_translate(&matrix, x, y + ascent);
    plutovg_path_t *path = draw_font_get_textn_path(plutovg_get_font(pluto), align, utf8, strlen(utf8), w, h);
    plutovg_path_add_path(plutovg_get_path(pluto), path, &matrix);
    plutovg_path_destroy(path);

    plutovg_set_source_rgba(pluto, color.r, color.g, color.b, color.a);
    plutovg_fill(pluto);
    plutovg_restore(pluto);
}

void Flex_setBorderRadius(FlexNodeRef node, float tl, float tr, float br, float bl)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->border_radius[0] = tl;
        box->border_radius[1] = tr;
        box->border_radius[2] = br;
        box->border_radius[3] = bl;
    }
}

void Flex_setBorderColor(FlexNodeRef node, struct plutovg_color c)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->border_color = c;
    }
}

void Flex_setFillColor(FlexNodeRef node, struct plutovg_color c)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->fill_color = c;
    }
}

void Flex_setFontColor(FlexNodeRef node, struct plutovg_color c)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->font_color = c;
    }
}
void Flex_setText(FlexNodeRef node, const char *text)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->text = strdup(text);
    }
}
void Flex_setFontSize(FlexNodeRef node, double font_size)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->font_size = font_size;
    }
}

void Flex_setTextAlign(FlexNodeRef node, TEXT_ALIGN align)
{
    struct Box *box = Flex_getContext(node);
    if (box)
    {
        box->align = align;
    }
}

void Flex_drawNode(FlexNodeRef node, float x, float y)
{
    float left = Flex_getResultLeft(node);
    float top = Flex_getResultTop(node);
    float width = Flex_getResultWidth(node);
    float height = Flex_getResultHeight(node);
    printf("# %f %f %f %f ", left, top, width, height);
    printf("border %f %f %f %f\n", Flex_getBorderTop(node),
           Flex_getBorderRight(node),
           Flex_getBorderBottom(node),
           Flex_getBorderLeft(node));

    struct Box *box = Flex_getContext(node);

    if (box)
    {
        printf("%f %f %f %f\n", box->fill_color.a, box->fill_color.r, box->fill_color.b, box->fill_color.g);

        DrawBoxBackground(context.pluto, x + left, y + top, width, height, box->border_radius,
                          (float[]){
                              Flex_getBorderTop(node),
                              Flex_getBorderRight(node),
                              Flex_getBorderBottom(node),
                              Flex_getBorderLeft(node),
                          },
                          box->fill_color, box->border_color);
        DrawText(context.pluto, box->font_size, box->font_color, box->align, box->text,
                 x + left + Flex_getResultPaddingLeft(node),
                 y + top + Flex_getResultPaddingTop(node),
                 width - Flex_getResultPaddingLeft(node) - Flex_getResultPaddingRight(node),
                 height - Flex_getResultPaddingTop(node) - Flex_getResultPaddingBottom(node));
    }

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        Flex_drawNode(Flex_getChild(node, i), x + left, y + top);
    }
}

void Flex_draw(FlexNodeRef root)
{
    float left = Flex_getResultLeft(root);
    float top = Flex_getResultTop(root);
    Flex_drawNode(root, left, top);
}

void Flex_Test()
{
    FlexNodeRef root = Flex_newNode();
    Flex_setWidth(root, WINDOW_W);
    Flex_setHeight(root, WINDOW_H);

    {
        FlexNodeRef child = Flex_newNode();
        Flex_setWrap(child, FlexWrap);
        Flex_setJustifyContent(child, FlexCenter);
        Flex_setAlignItems(child, FlexCenter);
        Flex_setAlignContent(child, FlexCenter);
        Flex_setWidthPercent(child, 100);
        Flex_setHeightPercent(child, 100);
        Flex_addChild(root, child);

        for (int i = 0; i < 1; i++)
        {
            FlexNodeRef child1 = Flex_newBox();

            Flex_setBorderTop(child1, rand() % 20);
            Flex_setBorderBottom(child1, rand() % 20);
            Flex_setBorderLeft(child1, rand() % 20);
            Flex_setBorderRight(child1, rand() % 20);
            Flex_setWidth(child1, 200);
            Flex_setHeight(child1, 100);
            Flex_addChild(child, child1);

            Flex_setBorderRadius(child1, 20, 20, 20, 20);
            Flex_setBorderColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setFillColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setText(child1, "MEUI\nModern embedded UI\n现代化嵌入式用户界面");
            Flex_setTextAlign(child1, TEXT_ALIGN_CENTER);
        }

        Flex_layout(root, FlexUndefined, FlexUndefined, 1);
        Flex_print(root, FlexPrintDefault);
        Flex_draw(root);

        Flex_freeNodeRecursive(root);
    }
}
int main(int argc, char **argv)
{
    srand((unsigned)time(NULL));

    context.face = plutovg_font_face_load_from_file("res/font/Droid-Sans-Fallback.ttf");
    context.surface = plutovg_surface_create(WINDOW_W, WINDOW_H);
    context.pluto = plutovg_create(context.surface);

    Flex_Test();

    plutovg_surface_write_to_png(context.surface, "output.png");
    plutovg_destroy(context.pluto);
    return 0;
}