#include "box.h"

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

            Flex_setPaddingTop(child1, rand() % 20);
            Flex_setPaddingBottom(child1, rand() % 20);
            Flex_setPaddingLeft(child1, rand() % 20);
            Flex_setPaddingRight(child1, rand() % 20);
            Flex_setWidth(child1, 240);
            Flex_setHeight(child1, 140);
            Flex_addChild(child, child1);

            Flex_setBorderRadius(child1, 20, 20, 20, 20);
            Flex_setBorderColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setFillColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setFontColor(child1, (struct plutovg_color){rand() % 256 / 255.0, rand() % 256 / 255.0, rand() % 256 / 255.0, 1.0});
            Flex_setText(child1, "MEUI\nModern embedded UI\n现代化嵌入式用户界面");
            Flex_setTextAlign(child1, TEXT_ALIGN_CENTER);
            Flex_setContentImage(child1, "res/img/Gradient.svg");
            Flex_setBackgroundImage(child1, "res/img/Gradient2.svg");
        }
    }

    Flex_layout(root, FlexUndefined, FlexUndefined, 1);
    Flex_print(root, FlexPrintDefault);
    Flex_draw(root);
    Flex_freeRecursive(root);
}
int main(int argc, char **argv)
{
    srand((unsigned)time(NULL));

    Flex_getRenderContext()->face = plutovg_font_face_load_from_file("res/font/Droid-Sans-Fallback.ttf");
    Flex_getRenderContext()->surface = plutovg_surface_create(WINDOW_W, WINDOW_H);
    Flex_getRenderContext()->pluto = plutovg_create(Flex_getRenderContext()->surface);

    Flex_Test();

    plutovg_surface_write_to_png(Flex_getRenderContext()->surface, "output.png");
    plutovg_destroy(Flex_getRenderContext()->pluto);
    return 0;
}