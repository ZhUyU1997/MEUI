#include "element.h"

class_impl(DivEle, Box){};

constructor(DivEle)
{
    Flex_setMeasureFunc(dynamic_cast(Box)(this)->node, box_measure_text);
}

destructor(DivEle) {}