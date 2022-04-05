#include "element.h"

class_impl(StackEle, Box){};

constructor(StackEle)
{
    box_t node = dynamic_cast(Box)(this)->node;
    Flex_setMeasureFunc(node, box_measure_text);
    Flex_setCustomLayout(node, box_stack_layout);
}

destructor(StackEle) {}