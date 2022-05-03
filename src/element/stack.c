#include "element.h"
#include <FlexLayout.h>

static FlexSize stack_layout(FlexNodeRef node, float constrainedWidth, float constrainedHeight, float scale)
{
    Box *box = Flex_getContext(node);

    for (size_t i = 0; i < Flex_getChildrenCount(node); i++)
    {
        FlexNodeRef item = Flex_getChild(node, i);
        Box *childBox = Flex_getContext(item);

        Flex_layout(item,
                    flex_resolve(childBox->style.width, NULL, constrainedWidth),
                    flex_resolve(childBox->style.height, NULL, constrainedHeight),
                    scale);
        float left = flex_resolve(childBox->style.left, NULL, constrainedWidth);
        float right = flex_resolve(childBox->style.right, NULL, constrainedWidth);
        float top = flex_resolve(childBox->style.top, NULL, constrainedHeight);
        float bottom = flex_resolve(childBox->style.bottom, NULL, constrainedHeight);

        if (FlexIsResolved(right))
            Flex_setResultLeft(item, constrainedWidth - Flex_getResultWidth(item) - right);

        if (FlexIsResolved(bottom))
            Flex_setResultTop(item, constrainedHeight - Flex_getResultHeight(item) - bottom);

        // below code with high priority
        if (FlexIsResolved(left))
            Flex_setResultLeft(item, left);

        if (FlexIsResolved(top))
            Flex_setResultTop(item, top);
    }
    return (FlexSize){.width = constrainedWidth, .height = constrainedHeight};
}

class_impl(StackEle, Box){};

constructor(StackEle)
{
    box_t node = dynamic_cast(Box)(this)->node;
    Flex_setMeasureFunc(node, box_measure_text);
    Flex_setCustomLayout(node, stack_layout);
}

destructor(StackEle) {}