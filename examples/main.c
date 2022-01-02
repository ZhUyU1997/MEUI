#include "box.h"
#include <macro/function.h>

#include <FlexLayout.h>
#include <plutovg.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <meui.h>

#define WINDOW_W 1920
#define WINDOW_H 1080

void Flex_Test2()
{
    box_t root = box_new();
    Flex_setWidth(root, WINDOW_W);
    Flex_setHeight(root, WINDOW_H);

    {
        box_t child = box_new();
        Flex_setWrap(child, FlexWrap);
        Flex_setJustifyContent(child, FlexCenter);
        Flex_setAlignItems(child, FlexCenter);
        Flex_setAlignContent(child, FlexCenter);
        Flex_setWidthPercent(child, 100);
        Flex_setHeightPercent(child, 100);
        Flex_addChild(root, child);

        for (int i = 0; i < 10; i++)
        {
            box_t child1 = box_new();

            Flex_setBorderTop(child1, rand() % 20);
            Flex_setBorderBottom(child1, rand() % 20);
            Flex_setBorderLeft(child1, rand() % 20);
            Flex_setBorderRight(child1, rand() % 20);

            Flex_setPaddingTop(child1, rand() % 20);
            Flex_setPaddingBottom(child1, rand() % 20);
            Flex_setPaddingLeft(child1, rand() % 20);
            Flex_setPaddingRight(child1, rand() % 20);
            Flex_setWidth(child1, 340);
            Flex_setHeight(child1, 240);
            Flex_addChild(child, child1);

            box_default_style_border_radius(child1, 20, 20, 20, 20);
            box_default_style_border_color(child1, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            box_default_style_fill_color(child1, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            box_default_style_font_color(child1, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            box_default_style_text(child1, "MEUI\nModern embedded UI\n现代化嵌入式用户界面");
            box_default_style_text_align(child1, TEXT_ALIGN_CENTER_CENTER);
            box_default_style_content_image(child1, "res/img/picture.png");
            box_default_style_background_image(child1, "res/img/Gradient2.svg");
        }
    }

    Flex_layout(root, FlexUndefined, FlexUndefined, 1);
    Flex_print(root, FlexPrintDefault);
    box_draw(root);
    box_free_recursive(root);
}

void Flex_Test()
{
    srand((unsigned)time(NULL));

    box_t root = meui_get_root_node(meui_get_instance());
    {
        box_t child = box_new();
        Flex_setWrap(child, FlexWrap);
        Flex_setJustifyContent(child, FlexCenter);
        Flex_setAlignItems(child, FlexCenter);
        Flex_setAlignContent(child, FlexCenter);
        Flex_setWidthPercent(child, 100);
        Flex_setHeightPercent(child, 100);
        Flex_addChild(root, child);

        for (int i = 0; i < 10; i++)
        {
            box_t child1 = box_new();

            Flex_setBorderTop(child1, rand() % 20);
            Flex_setBorderBottom(child1, rand() % 20);
            Flex_setBorderLeft(child1, rand() % 20);
            Flex_setBorderRight(child1, rand() % 20);

            Flex_setPaddingTop(child1, rand() % 20);
            Flex_setPaddingBottom(child1, rand() % 20);
            Flex_setPaddingLeft(child1, rand() % 20);
            Flex_setPaddingRight(child1, rand() % 20);
            Flex_setWidth(child1, 340);
            Flex_setHeight(child1, 240);
            Flex_addChild(child, child1);

            box_default_style_border_radius(child1, 20, 20, 20, 20);
            box_default_style_border_color(child1, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            box_default_style_fill_color(child1, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            box_default_style_font_color(child1, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            box_default_style_text(child1, "MEUI\nModern embedded UI\n现代化嵌入式用户界面");
            box_default_style_text_align(child1, TEXT_ALIGN_CENTER_CENTER);
            box_default_style_content_image(child1, "res/img/picture.png");
            box_default_style_background_image(child1, "res/img/Gradient2.svg");
            box_default_style_transform_rotate(child1, 0.25 * 3.1415926);
            box_default_style_transform_origin_keyword(child1, TRANSFORM_ORIGIN_CENTER, TRANSFORM_ORIGIN_CENTER);
        }

        {
            box_t child2 = box_new();
            box_default_style_fill_color(child2, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_addChild(child, child2);
            Flex_setWidth(child2, 340);
            Flex_setHeight(child2, 240);
            box_default_style_fill_color(child2, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            box_default_style_transform_rotate(child2, 0.25 * 3.1415926);
            {
                box_t child11 = box_new();
                box_default_style_fill_color(child11, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
                Flex_addChild(child2, child11);
                Flex_setWidth(child11, 100);
                Flex_setHeight(child11, 100);
                box_default_style_transform_rotate(child11, 0.25 * 3.1415926);
            }
            {
                box_t child11 = box_new();
                box_default_style_fill_color(child11, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
                Flex_addChild(child2, child11);
                Flex_setWidth(child11, 100);
                Flex_setHeight(child11, 100);
                box_default_style_transform_rotate(child11, 0.25 * 3.1415926);

                box_add_event_listener(child11, MEUI_EVENT_MOUSE_DOWN, function(void, (box_event_t * e)(printf("MOUSE_DOWN\n"); box_default_style_transform_scale(e->target, 1.2, 1.2);)));
            }
        }
    }
}

void Flex_Test_Text_Align()
{
    srand((unsigned)time(NULL));

    box_t root = meui_get_root_node(meui_get_instance());
    box_default_style_fill_color(root, (plutovg_color_t){1, 1, 1, 1});

    {
        box_t child = box_new();
        Flex_setWrap(child, FlexWrap);
        Flex_setDirection(child, FlexVertical);
        Flex_setJustifyContent(child, FlexCenter);
        Flex_setAlignItems(child, FlexCenter);
        Flex_setAlignContent(child, FlexCenter);
        Flex_setWidthPercent(child, 100);
        Flex_setHeightPercent(child, 100);
        Flex_addChild(root, child);

        static int text_align[] = {
            TEXT_ALIGN_CENTER_LEFT,
            TEXT_ALIGN_CENTER_RIGHT,
            TEXT_ALIGN_CENTER_CENTER,
            TEXT_ALIGN_TOP_LEFT,
            TEXT_ALIGN_TOP_RIGHT,
            TEXT_ALIGN_TOP_CENTER,
            TEXT_ALIGN_BOTTOM_LEFT,
            TEXT_ALIGN_BOTTOM_RIGHT,
            TEXT_ALIGN_BOTTOM_CENTER,
        };

        for (int i = 0; i < 9; i++)
        {
            box_t child1 = box_new();

            Flex_setWidth(child1, 340);
            Flex_setHeight(child1, 80);
            Flex_addChild(child, child1);
            Flex_setBorderLeft(child1, 1);
            Flex_setBorderRight(child1, 1);
            Flex_setBorderTop(child1, 1);
            Flex_setBorderBottom(child1, 1);
            box_default_style_border_color(child1, (plutovg_color_t){0, 0, 0, 1.0});
            box_default_style_fill_color(child1, (plutovg_color_t){0x18 / 255.0, 0x80 / 255.0, 1, 1.0});
            box_default_style_font_color(child1, (plutovg_color_t){0, 0, 0, 1});
            box_default_style_text(child1, "MEUI\nModern embedded UI\n现代化嵌入式用户界面");
            box_default_style_text_align(child1, text_align[i]);
        }
    }
}

void Flex_Test_State()
{
    srand((unsigned)time(NULL));

    box_t root = meui_get_root_node(meui_get_instance());
    box_default_style_fill_color(root, (plutovg_color_t){1, 1, 1, 1});

    {
        box_t child = box_new();
        Flex_setWrap(child, FlexWrap);
        Flex_setDirection(child, FlexVertical);
        Flex_setJustifyContent(child, FlexCenter);
        Flex_setAlignItems(child, FlexCenter);
        Flex_setAlignContent(child, FlexCenter);
        Flex_setWidthPercent(child, 100);
        Flex_setHeightPercent(child, 100);
        Flex_addChild(root, child);
        static const char *style_name[8] = {"primary", "secondary", "success", "info", "warning", "danger", "light", "dark"};
        static unsigned int style_color[8][3][3] = {{{0x536de6ff, 0xffffffff, 0x00000000},
                                                 {0x3a57e2ff, 0xffffffff, 0x00000000},
                                                 {0x2647e0ff, 0xffffffff, 0x536de660}},
                                                {{0x6c757dff, 0xffffffff, 0x00000000},
                                                 {0x5a6268ff, 0xffffffff, 0x00000000},
                                                 {0x545b62ff, 0xffffffff, 0x6c757d60}},
                                                {{0x10c469ff, 0xffffffff, 0x00000000},
                                                 {0x0da156ff, 0xffffffff, 0x00000000},
                                                 {0x0c9550ff, 0xffffffff, 0x10c46960}},
                                                {{0x35b8e0ff, 0xffffffff, 0x00000000},
                                                 {0x20a6cfff, 0xffffffff, 0x00000000},
                                                 {0x1e9dc4ff, 0xffffffff, 0x35b8e060}},
                                                {{0xf9c851ff, 0xffffffff, 0x00000000},
                                                 {0xf8bc2cff, 0xffffffff, 0x00000000},
                                                 {0xf7b820ff, 0xffffffff, 0xf9c85160}},
                                                {{0xff5b5bff, 0xffffffff, 0x00000000},
                                                 {0xff3535ff, 0xffffffff, 0x00000000},
                                                 {0xff2828ff, 0xffffffff, 0xff5b5b60}},
                                                {{0xeef2f7ff, 0x323a46ff, 0x00000000},
                                                 {0xd4deebff, 0x323a46ff, 0x00000000},
                                                 {0xcbd7e7ff, 0x323a46ff, 0xeef2f760}},
                                                {{0x323a46ff, 0xffffffff, 0x00000000},
                                                 {0x222830ff, 0xffffffff, 0x00000000},
                                                 {0x1d2128ff, 0xffffffff, 0x323a4660}}};
        for (int i = 0; i < 8; i++)
        {
            box_t child1 = box_new();

            Flex_setWidth(child1, 340);
            Flex_setHeight(child1, 80);
            Flex_addChild(child, child1);
            Flex_setMarginBottom(child1, 10);
            Flex_setBorderLeft(child1, 2);
            Flex_setBorderRight(child1, 2);
            Flex_setBorderTop(child1, 2);
            Flex_setBorderBottom(child1, 2);

#define HEX(c) (plutovg_color_t){((c >> 24) & 0xff) / 255.0, ((c >> 16) & 0xff) / 255.0, ((c >> 8) & 0xff) / 255.0, ((c)&0xff) / 255.0}

            box_default_style_border_radius(child1, 20, 20, 20, 20);
            box_default_style_fill_color(child1, HEX(style_color[i][0][0]));
            box_default_style_font_color(child1, HEX(style_color[i][0][1]));
            box_default_style_border_color(child1, HEX(style_color[i][0][2]));

            box_default_style_text(child1, style_name[i]);
            box_default_style_text_align(child1, TEXT_ALIGN_CENTER_CENTER);
            {
                box_style_t *style = box_style_new();
                box_style_fill_color(style, HEX(style_color[i][1][0]));
                box_style_font_color(style, HEX(style_color[i][1][1]));
                box_style_border_color(style, HEX(style_color[i][1][2]));
                box_style_text(style, "Box State: Hover");
                box_set_style(child1, style, BOX_STATE_HOVER);
            }

            {
                box_style_t *style = box_style_new();
                box_style_fill_color(style, HEX(style_color[i][2][0]));
                box_style_font_color(style, HEX(style_color[i][2][1]));
                box_style_border_color(style, HEX(style_color[i][2][2]));
                box_style_text(style, "Box State: Active");
                box_set_style(child1, style, BOX_STATE_ACTIVE);
            }
        }
    }
}

static void meui_on_create(struct meui_t *meui)
{
    Flex_Test_State();
    Flex_layout(meui_get_root_node(meui), FlexUndefined, FlexUndefined, 1);
    // Flex_print(meui_get_root_node(meui), FlexPrintDefault);
    box_draw(meui_get_root_node(meui));

    plutovg_surface_write_to_png(meui_get_surface(meui), "output.png");
    meui_flush(meui);
}

int main(int argc, char **argv)
{
    struct meui_t *meui = meui_start(WINDOW_W, WINDOW_H);
    meui_add_font_face(meui, "res/font/Droid-Sans-Fallback.ttf");
    meui_register_callback(meui, MEUI_CB_ON_CREATE, meui_on_create);
    meui_main_loop(meui);
    meui_end(meui);
    return 0;
}