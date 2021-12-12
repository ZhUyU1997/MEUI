#include "box.h"

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
    FlexNodeRef root = Flex_newBox();
    Flex_setWidth(root, WINDOW_W);
    Flex_setHeight(root, WINDOW_H);

    {
        FlexNodeRef child = Flex_newBox();
        Flex_setWrap(child, FlexWrap);
        Flex_setJustifyContent(child, FlexCenter);
        Flex_setAlignItems(child, FlexCenter);
        Flex_setAlignContent(child, FlexCenter);
        Flex_setWidthPercent(child, 100);
        Flex_setHeightPercent(child, 100);
        Flex_addChild(root, child);

        for (int i = 0; i < 10; i++)
        {
            FlexNodeRef child1 = Flex_newBox();

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

            Flex_setBorderRadius(child1, 20, 20, 20, 20);
            Flex_setBorderColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setFillColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setFontColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setText(child1, "MEUI\nModern embedded UI\n现代化嵌入式用户界面");
            Flex_setTextAlign(child1, TEXT_ALIGN_CENTER);
            Flex_setContentImage(child1, "res/img/picture.png");
            Flex_setBackgroundImage(child1, "res/img/Gradient2.svg");
        }
    }

    Flex_layout(root, FlexUndefined, FlexUndefined, 1);
    Flex_print(root, FlexPrintDefault);
    Flex_draw(root);
    Flex_freeRecursive(root);
}

void Flex_Test()
{
    srand((unsigned)time(NULL));

    FlexNodeRef root = meui_get_root_node(meui_get_instance());
    {
        FlexNodeRef child = Flex_newBox();
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

            Flex_setPaddingTop(child1, rand() % 20);
            Flex_setPaddingBottom(child1, rand() % 20);
            Flex_setPaddingLeft(child1, rand() % 20);
            Flex_setPaddingRight(child1, rand() % 20);
            Flex_setWidth(child1, 340);
            Flex_setHeight(child1, 240);
            Flex_addChild(child, child1);

            Flex_setBorderRadius(child1, 20, 20, 20, 20);
            Flex_setBorderColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setFillColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setFontColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setText(child1, "MEUI\nModern embedded UI\n现代化嵌入式用户界面");
            Flex_setTextAlign(child1, TEXT_ALIGN_CENTER);
            Flex_setContentImage(child1, "res/img/picture.png");
            Flex_setBackgroundImage(child1, "res/img/Gradient2.svg");
            Flex_transform_rotate(child1, 0.25 * 3.1415926);
            Flex_transform_origin_keyword(child1, TRANSFORM_ORIGIN_CENTER, TRANSFORM_ORIGIN_CENTER);
        }

        {
            FlexNodeRef child2 = Flex_newBox();
            Flex_setFillColor(child2, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_addChild(child, child2);
            Flex_setWidth(child2, 340);
            Flex_setHeight(child2, 240);
            Flex_setFillColor(child2, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_transform_rotate(child2, 0.25 * 3.1415926);
            {
                FlexNodeRef child11 = Flex_newBox();
                Flex_setFillColor(child11, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
                Flex_addChild(child2, child11);
                Flex_setWidth(child11, 100);
                Flex_setHeight(child11, 100);
                Flex_transform_rotate(child11, 0.25 * 3.1415926);
            }
        }
    }
}

static void meui_on_create(struct meui_t *meui)
{
    Flex_Test();
    Flex_layout(meui_get_root_node(meui), FlexUndefined, FlexUndefined, 1);
    // Flex_print(meui_get_root_node(meui), FlexPrintDefault);
    Flex_draw(meui_get_root_node(meui));

    plutovg_surface_write_to_png(meui_get_surface(meui), "output.png");
    meui_update_surface(meui);
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