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

        for (int i = 0; i < 10; i++)
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

            box_default_style_border_radius(child1, 20, 20, 20, 20);
            box_default_style_fill_color(child1, (plutovg_color_t){0x18 / 255.0, 0x80 / 255.0, 1, 1.0});
            box_default_style_font_color(child1, (plutovg_color_t){0, 0, 0, 1});
            box_default_style_text(child1, "MEUI\nModern embedded UI\n现代化嵌入式用户界面");
            box_default_style_text_align(child1, TEXT_ALIGN_CENTER_CENTER);

            box_style_t *style = box_style_new();
            box_style_fill_color(style, (plutovg_color_t){0x40 / 255.0, 0xA9 / 255.0, 1, 1.0});
            box_style_border_color(style, (plutovg_color_t){1, 0, 0, 1});
            box_style_text(style, "Box State: Hover");
            box_set_style(child1, style, BOX_STATE_HOVER);
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