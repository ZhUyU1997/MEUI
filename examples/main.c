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
            Flex_setBorderLeft(child1, 1);
            Flex_setBorderRight(child1, 1);
            Flex_setBorderTop(child1, 1);
            Flex_setBorderBottom(child1, 1);

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

void Flex_Test_App()
{
    srand((unsigned)time(NULL));
#define HEX(c) (plutovg_color_t){((c >> 24) & 0xff) / 255.0, ((c >> 16) & 0xff) / 255.0, ((c >> 8) & 0xff) / 255.0, ((c)&0xff) / 255.0}

    box_t root = meui_get_root_node(meui_get_instance());
    box_default_style_fill_color(root, HEX(0x0D0F12FF));
    Flex_setJustifyContent(root, FlexCenter);
    Flex_setAlignItems(root, FlexCenter);
    Flex_setAlignContent(root, FlexCenter);
    {
        box_t win = box_new();
        Flex_setWrap(win, FlexWrap);
        Flex_setDirection(win, FlexVertical);
        Flex_setJustifyContent(win, FlexCenter);
        Flex_setAlignItems(win, FlexCenter);
        Flex_setAlignContent(win, FlexCenter);
        Flex_setPaddingLeft(win, 25);
        Flex_setPaddingRight(win, 25);

        Flex_setWidth(win, 680);
        Flex_setHeight(win, 480);
        box_default_style_fill_color(win, HEX(0x31353DFF));
        box_default_style_border_radius(win, 15, 15, 15, 15);
        Flex_addChild(root, win);

        {
            box_t head = box_new();
            Flex_setWidthPercent(head, 100);
            Flex_setHeight(head, 60);
            Flex_setAlignItems(head, FlexCenter);
            Flex_setAlignContent(head, FlexCenter);
            Flex_addChild(win, head);

            {
                box_t circle = box_new();
                Flex_setWidth(circle, 20);
                Flex_setHeight(circle, 20);
                Flex_setMarginRight(circle, 20);
                Flex_addChild(head, circle);
                box_default_style_fill_color(circle, HEX(0xDF491AFF));
                box_default_style_border_radius(circle, 10, 10, 10, 10);
            }
            {
                box_t circle = box_new();
                Flex_setWidth(circle, 20);
                Flex_setHeight(circle, 20);
                Flex_setMarginRight(circle, 20);

                Flex_addChild(head, circle);
                box_default_style_fill_color(circle, HEX(0xFFCC0EFF));
                box_default_style_border_radius(circle, 10, 10, 10, 10);
            }
            {
                box_t circle = box_new();
                Flex_setWidth(circle, 20);
                Flex_setHeight(circle, 20);
                Flex_setMarginRight(circle, 20);

                Flex_addChild(head, circle);
                box_default_style_fill_color(circle, HEX(0x19DF74FF));
                box_default_style_border_radius(circle, 10, 10, 10, 10);
            }
        }
        {
            box_t bar = box_new();
            Flex_setWidthPercent(bar, 100);
            Flex_setAlignItems(bar, FlexCenter);

            Flex_setHeight(bar, 60);
            Flex_addChild(win, bar);
            {
                box_t icon = box_new();
                Flex_setWidth(icon, 50);
                Flex_setHeight(icon, 50);
                Flex_addChild(bar, icon);
                Flex_setPaddingRight(icon, 12.5);
                Flex_setPaddingTop(icon, 12.5);
                Flex_setPaddingLeft(icon, 12.5);
                Flex_setPaddingBottom(icon, 12.5);

                box_default_style_content_image(icon, "examples/left.svg");
                box_default_style_fill_color(icon, HEX(0x555A64FF));
                box_default_style_border_radius(icon, 7, 7, 7, 7);
            }
            {
                box_t icon = box_new();
                Flex_setWidth(icon, 50);
                Flex_setHeight(icon, 50);
                Flex_addChild(bar, icon);
                Flex_setPaddingRight(icon, 12.5);
                Flex_setPaddingTop(icon, 12.5);
                Flex_setPaddingLeft(icon, 12.5);
                Flex_setPaddingBottom(icon, 12.5);
                box_default_style_content_image(icon, "examples/right.svg");
            }
            {
                box_t icon = box_new();
                Flex_setWidth(icon, 50);
                Flex_setHeight(icon, 50);
                Flex_addChild(bar, icon);
                Flex_setMarginLeft(icon, 110);
                Flex_setPaddingRight(icon, 12.5);
                Flex_setPaddingTop(icon, 12.5);
                Flex_setPaddingLeft(icon, 12.5);
                Flex_setPaddingBottom(icon, 12.5);
                box_default_style_fill_color(icon, HEX(0x555A64FF));
                box_default_style_border_radius(icon, 7, 0, 0, 7);
                box_default_style_content_image(icon, "examples/small-icon-normal.svg");
            }
            {
                box_t icon = box_new();
                Flex_setWidth(icon, 50);
                Flex_setHeight(icon, 50);
                Flex_addChild(bar, icon);
                Flex_setPaddingRight(icon, 12.5);
                Flex_setPaddingTop(icon, 12.5);
                Flex_setPaddingLeft(icon, 12.5);
                Flex_setPaddingBottom(icon, 12.5);
                box_default_style_fill_color(icon, HEX(0xFFFFFFFF));
                box_default_style_content_image(icon, "examples/list-select.svg");
            }
            {
                box_t icon = box_new();
                Flex_setWidth(icon, 50);
                Flex_setHeight(icon, 50);
                Flex_addChild(bar, icon);
                Flex_setPaddingRight(icon, 12.5);
                Flex_setPaddingTop(icon, 12.5);
                Flex_setPaddingLeft(icon, 12.5);
                Flex_setPaddingBottom(icon, 12.5);
                box_default_style_fill_color(icon, HEX(0x555A64FF));
                box_default_style_border_radius(icon, 0, 7, 7, 0);
                box_default_style_content_image(icon, "examples/card-normal.svg");
            }
            {
                box_t icon = box_new();
                Flex_setWidth(icon, 225);
                Flex_setHeight(icon, 50);
                Flex_setMarginLeft(icon, 50);
                Flex_addChild(bar, icon);

                box_default_style_fill_color(icon, HEX(0x555A64FF));
                box_default_style_border_radius(icon, 25, 25, 25, 25);

                {
                    box_t search_icon = box_new();
                    Flex_setDirection(search_icon, FlexHorizontal);

                    Flex_setWidth(search_icon, 50);
                    Flex_setHeight(search_icon, 50);
                    Flex_addChild(icon, search_icon);
                    Flex_setPaddingRight(search_icon, 12.5);
                    Flex_setPaddingTop(search_icon, 12.5);
                    Flex_setPaddingLeft(search_icon, 12.5);
                    Flex_setPaddingBottom(search_icon, 12.5);
                    box_default_style_content_image(search_icon, "examples/search.svg");
                }
            }
        }

        {
            box_t body = box_new();
            Flex_setWidthPercent(body, 100);
            Flex_setHeight(body, 360);
            Flex_setPaddingTop(body, 30);
            Flex_setPaddingBottom(body, 30);

            Flex_setDirection(body, FlexHorizontal);

            Flex_addChild(win, body);
            {
                box_t menu = box_new();
                Flex_setWidth(menu, 212);
                Flex_setHeightPercent(menu, 100);
                Flex_setDirection(menu, FlexVertical);
                Flex_addChild(body, menu);

                const char *text[4] = {"Desktop", "Dribbble", "Images", "Downloads"};
                for (int i = 0; i < 4; i++)
                {
                    box_t item = box_new();
                    Flex_setWidth(item, 200);
                    Flex_setHeight(item, 55);
                    Flex_addChild(menu, item);
                    Flex_setAlignItems(item, FlexCenter);

                    {
                        box_style_t *style = box_style_new();
                        box_style_fill_color(style, HEX(0x41464EFF));
                        // box_style_border_radius(style, 7, 7, 7, 7);

                        box_set_style(item, style, BOX_STATE_HOVER);
                    }

                    {
                        box_t icon = box_new();
                        Flex_setWidth(icon, 38);
                        Flex_setHeight(icon, 38);
                        Flex_addChild(item, icon);
                        box_default_style_content_image(icon, "examples/file.svg");
                    }

                    {
                        box_t icon = box_new();
                        Flex_setWidth(icon, 145);
                        Flex_setHeight(icon, 38);
                        Flex_addChild(item, icon);
                        Flex_setMarginLeft(icon, 18);
                        box_default_style_text(icon, text[i]);
                        box_default_style_font_color(icon, HEX(0xABAFB7FF));
                        box_default_style_font_size(icon, 24);
                    }
                }
            }

            {
                box_t list = box_new();
                Flex_setWidth(list, 422);
                Flex_setHeightPercent(list, 100);
                Flex_setDirection(list, FlexVertical);

                Flex_addChild(body, list);

                const char *text[6][3] = {
                    {"ab.png", "3h", "1MB"},
                    {"cd.png", "4h", "4MB"},
                    {"ef.png", "9h", "1MB"},
                    {"gh.png", "1d", "3MB"},
                    {"ij.png", "1d", "5MB"},
                    {"kl.png", "2d", "2MB"},
                };
                for (int i = 0; i < 6; i++)
                {
                    box_t item = box_new();
                    Flex_setWidthPercent(item, 100);
                    Flex_setHeight(item, 55);
                    Flex_addChild(list, item);
                    Flex_setPaddingLeft(item, 7);
                    Flex_setPaddingTop(item, 7);
                    Flex_setPaddingBottom(item, 7);
                    Flex_setPaddingRight(item, 7);

                    {
                        box_style_t *style = box_style_new();
                        box_style_fill_color(style, HEX(0x41464EFF));
                        box_style_border_radius(style, 7, 7, 7, 7);

                        box_set_style(item, style, BOX_STATE_HOVER);
                    }
                    {
                        box_style_t *style = box_style_new();
                        box_style_fill_color(style, HEX(0x32C1FFFF));
                        box_style_border_radius(style, 7, 7, 7, 7);
                        box_set_style(item, style, BOX_STATE_ACTIVE);
                    }

                    {
                        box_t icon = box_new();
                        Flex_setWidth(icon, 38);
                        Flex_setHeight(icon, 38);
                        Flex_addChild(item, icon);
                        box_default_style_fill_color(icon, (plutovg_color_t){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
                    }

                    {
                        box_t icon = box_new();
                        Flex_setWidth(icon, 180);
                        Flex_setHeight(icon, 38);
                        Flex_addChild(item, icon);
                        Flex_setMarginLeft(icon, 16);
                        box_default_style_text(icon, text[i][0]);
                        box_default_style_font_color(icon, HEX(0xDDDDDFFF));
                        box_default_style_font_size(icon, 20);
                        {
                            box_style_t *style = box_style_new();
                            box_style_font_color(style, HEX(0xFFFFFFFF));
                            box_set_style(icon, style, BOX_STATE_ACTIVE);
                        }
                    }
                    {
                        box_t icon = box_new();
                        Flex_setWidth(icon, 50);
                        Flex_setHeight(icon, 38);
                        Flex_addChild(item, icon);
                        box_default_style_text(icon, text[i][1]);
                        box_default_style_font_color(icon, HEX(0xDDDDDFFF));
                        box_default_style_font_size(icon, 20);
                        box_default_style_text_align(icon, TEXT_ALIGN_CENTER_RIGHT);
                        {
                            box_style_t *style = box_style_new();
                            box_style_font_color(style, HEX(0xFFFFFFFF));
                            box_set_style(icon, style, BOX_STATE_ACTIVE);
                        }
                    }
                    {
                        box_t icon = box_new();
                        Flex_setWidth(icon, 110);
                        Flex_setHeight(icon, 38);
                        Flex_addChild(item, icon);
                        box_default_style_text(icon, text[i][2]);
                        box_default_style_font_color(icon, HEX(0xDDDDDFFF));
                        box_default_style_font_size(icon, 20);
                        box_default_style_text_align(icon, TEXT_ALIGN_CENTER_RIGHT);
                        {
                            box_style_t *style = box_style_new();
                            box_style_font_color(style, HEX(0xFFFFFFFF));
                            box_set_style(icon, style, BOX_STATE_ACTIVE);
                        }
                    }
                }
            }
        }
    }
}

static void meui_on_create(struct meui_t *meui)
{
    Flex_Test_App();
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