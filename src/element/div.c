/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "meui/element.h"

class_impl(DivEle, Box){};

constructor(DivEle)
{
    Flex_setMeasureFunc(dynamic_cast(Box)(this)->node, box_measure_text);
}

destructor(DivEle) {}