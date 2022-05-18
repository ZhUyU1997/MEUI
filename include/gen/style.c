void box_style_wrap(box_style_t *style, FlexWrapMode wrap)
{
    assert(style);
    style->wrap = wrap;
    style->flags |= UINT64_C(1) << 0;
    style->dirty |= UINT64_C(1) << 0;
}
void box_style_direction(box_style_t *style, FlexDirection direction)
{
    assert(style);
    style->direction = direction;
    style->flags |= UINT64_C(1) << 1;
    style->dirty |= UINT64_C(1) << 1;
}
void box_style_alignItems(box_style_t *style, FlexAlign alignItems)
{
    assert(style);
    style->alignItems = alignItems;
    style->flags |= UINT64_C(1) << 2;
    style->dirty |= UINT64_C(1) << 2;
}
void box_style_alignSelf(box_style_t *style, FlexAlign alignSelf)
{
    assert(style);
    style->alignSelf = alignSelf;
    style->flags |= UINT64_C(1) << 3;
    style->dirty |= UINT64_C(1) << 3;
}
void box_style_alignContent(box_style_t *style, FlexAlign alignContent)
{
    assert(style);
    style->alignContent = alignContent;
    style->flags |= UINT64_C(1) << 4;
    style->dirty |= UINT64_C(1) << 4;
}
void box_style_justifyContent(box_style_t *style, FlexAlign justifyContent)
{
    assert(style);
    style->justifyContent = justifyContent;
    style->flags |= UINT64_C(1) << 5;
    style->dirty |= UINT64_C(1) << 5;
}
void box_style_flexBasis(box_style_t *style, float flexBasis)
{
    assert(style);
    style->flexBasis.value = flexBasis;
    style->flexBasis.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 6;
    style->dirty |= UINT64_C(1) << 6;
}
void box_style_flexBasisPercent(box_style_t *style, float flexBasis)
{
    assert(style);
    style->flexBasis.value = flexBasis;
    style->flexBasis.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 6;
    style->dirty |= UINT64_C(1) << 6;
}
void box_style_flexBasisAuto(box_style_t *style)
{
    assert(style);
    style->flexBasis.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 6;
    style->dirty |= UINT64_C(1) << 6;
}
void box_style_flexBasisContent(box_style_t *style)
{
    assert(style);
    style->flexBasis.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 6;
    style->dirty |= UINT64_C(1) << 6;
}
void box_style_flexGrow(box_style_t *style, float flexGrow)
{
    assert(style);
    style->flexGrow = flexGrow;
    style->flags |= UINT64_C(1) << 7;
    style->dirty |= UINT64_C(1) << 7;
}
void box_style_flexShrink(box_style_t *style, float flexShrink)
{
    assert(style);
    style->flexShrink = flexShrink;
    style->flags |= UINT64_C(1) << 8;
    style->dirty |= UINT64_C(1) << 8;
}
void box_style_width(box_style_t *style, float width)
{
    assert(style);
    style->width.value = width;
    style->width.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 9;
    style->dirty |= UINT64_C(1) << 9;
}
void box_style_widthPercent(box_style_t *style, float width)
{
    assert(style);
    style->width.value = width;
    style->width.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 9;
    style->dirty |= UINT64_C(1) << 9;
}
void box_style_widthAuto(box_style_t *style)
{
    assert(style);
    style->width.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 9;
    style->dirty |= UINT64_C(1) << 9;
}
void box_style_widthContent(box_style_t *style)
{
    assert(style);
    style->width.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 9;
    style->dirty |= UINT64_C(1) << 9;
}
void box_style_height(box_style_t *style, float height)
{
    assert(style);
    style->height.value = height;
    style->height.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 10;
    style->dirty |= UINT64_C(1) << 10;
}
void box_style_heightPercent(box_style_t *style, float height)
{
    assert(style);
    style->height.value = height;
    style->height.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 10;
    style->dirty |= UINT64_C(1) << 10;
}
void box_style_heightAuto(box_style_t *style)
{
    assert(style);
    style->height.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 10;
    style->dirty |= UINT64_C(1) << 10;
}
void box_style_heightContent(box_style_t *style)
{
    assert(style);
    style->height.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 10;
    style->dirty |= UINT64_C(1) << 10;
}
void box_style_minWidth(box_style_t *style, float minWidth)
{
    assert(style);
    style->minWidth.value = minWidth;
    style->minWidth.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 11;
    style->dirty |= UINT64_C(1) << 11;
}
void box_style_minWidthPercent(box_style_t *style, float minWidth)
{
    assert(style);
    style->minWidth.value = minWidth;
    style->minWidth.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 11;
    style->dirty |= UINT64_C(1) << 11;
}
void box_style_minHeight(box_style_t *style, float minHeight)
{
    assert(style);
    style->minHeight.value = minHeight;
    style->minHeight.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 12;
    style->dirty |= UINT64_C(1) << 12;
}
void box_style_minHeightPercent(box_style_t *style, float minHeight)
{
    assert(style);
    style->minHeight.value = minHeight;
    style->minHeight.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 12;
    style->dirty |= UINT64_C(1) << 12;
}
void box_style_maxWidth(box_style_t *style, float maxWidth)
{
    assert(style);
    style->maxWidth.value = maxWidth;
    style->maxWidth.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 13;
    style->dirty |= UINT64_C(1) << 13;
}
void box_style_maxWidthPercent(box_style_t *style, float maxWidth)
{
    assert(style);
    style->maxWidth.value = maxWidth;
    style->maxWidth.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 13;
    style->dirty |= UINT64_C(1) << 13;
}
void box_style_maxHeight(box_style_t *style, float maxHeight)
{
    assert(style);
    style->maxHeight.value = maxHeight;
    style->maxHeight.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 14;
    style->dirty |= UINT64_C(1) << 14;
}
void box_style_maxHeightPercent(box_style_t *style, float maxHeight)
{
    assert(style);
    style->maxHeight.value = maxHeight;
    style->maxHeight.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 14;
    style->dirty |= UINT64_C(1) << 14;
}
void box_style_marginLeft(box_style_t *style, float marginLeft)
{
    assert(style);
    style->marginLeft.value = marginLeft;
    style->marginLeft.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 15;
    style->dirty |= UINT64_C(1) << 15;
}
void box_style_marginLeftPercent(box_style_t *style, float marginLeft)
{
    assert(style);
    style->marginLeft.value = marginLeft;
    style->marginLeft.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 15;
    style->dirty |= UINT64_C(1) << 15;
}
void box_style_marginLeftAuto(box_style_t *style)
{
    assert(style);
    style->marginLeft.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 15;
    style->dirty |= UINT64_C(1) << 15;
}
void box_style_marginLeftContent(box_style_t *style)
{
    assert(style);
    style->marginLeft.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 15;
    style->dirty |= UINT64_C(1) << 15;
}
void box_style_marginTop(box_style_t *style, float marginTop)
{
    assert(style);
    style->marginTop.value = marginTop;
    style->marginTop.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 16;
    style->dirty |= UINT64_C(1) << 16;
}
void box_style_marginTopPercent(box_style_t *style, float marginTop)
{
    assert(style);
    style->marginTop.value = marginTop;
    style->marginTop.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 16;
    style->dirty |= UINT64_C(1) << 16;
}
void box_style_marginTopAuto(box_style_t *style)
{
    assert(style);
    style->marginTop.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 16;
    style->dirty |= UINT64_C(1) << 16;
}
void box_style_marginTopContent(box_style_t *style)
{
    assert(style);
    style->marginTop.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 16;
    style->dirty |= UINT64_C(1) << 16;
}
void box_style_marginBottom(box_style_t *style, float marginBottom)
{
    assert(style);
    style->marginBottom.value = marginBottom;
    style->marginBottom.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 17;
    style->dirty |= UINT64_C(1) << 17;
}
void box_style_marginBottomPercent(box_style_t *style, float marginBottom)
{
    assert(style);
    style->marginBottom.value = marginBottom;
    style->marginBottom.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 17;
    style->dirty |= UINT64_C(1) << 17;
}
void box_style_marginBottomAuto(box_style_t *style)
{
    assert(style);
    style->marginBottom.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 17;
    style->dirty |= UINT64_C(1) << 17;
}
void box_style_marginBottomContent(box_style_t *style)
{
    assert(style);
    style->marginBottom.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 17;
    style->dirty |= UINT64_C(1) << 17;
}
void box_style_marginRight(box_style_t *style, float marginRight)
{
    assert(style);
    style->marginRight.value = marginRight;
    style->marginRight.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 18;
    style->dirty |= UINT64_C(1) << 18;
}
void box_style_marginRightPercent(box_style_t *style, float marginRight)
{
    assert(style);
    style->marginRight.value = marginRight;
    style->marginRight.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 18;
    style->dirty |= UINT64_C(1) << 18;
}
void box_style_marginRightAuto(box_style_t *style)
{
    assert(style);
    style->marginRight.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 18;
    style->dirty |= UINT64_C(1) << 18;
}
void box_style_marginRightContent(box_style_t *style)
{
    assert(style);
    style->marginRight.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 18;
    style->dirty |= UINT64_C(1) << 18;
}
void box_style_marginStart(box_style_t *style, float marginStart)
{
    assert(style);
    style->marginStart.value = marginStart;
    style->marginStart.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 19;
    style->dirty |= UINT64_C(1) << 19;
}
void box_style_marginStartPercent(box_style_t *style, float marginStart)
{
    assert(style);
    style->marginStart.value = marginStart;
    style->marginStart.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 19;
    style->dirty |= UINT64_C(1) << 19;
}
void box_style_marginStartAuto(box_style_t *style)
{
    assert(style);
    style->marginStart.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 19;
    style->dirty |= UINT64_C(1) << 19;
}
void box_style_marginStartContent(box_style_t *style)
{
    assert(style);
    style->marginStart.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 19;
    style->dirty |= UINT64_C(1) << 19;
}
void box_style_marginEnd(box_style_t *style, float marginEnd)
{
    assert(style);
    style->marginEnd.value = marginEnd;
    style->marginEnd.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 20;
    style->dirty |= UINT64_C(1) << 20;
}
void box_style_marginEndPercent(box_style_t *style, float marginEnd)
{
    assert(style);
    style->marginEnd.value = marginEnd;
    style->marginEnd.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 20;
    style->dirty |= UINT64_C(1) << 20;
}
void box_style_marginEndAuto(box_style_t *style)
{
    assert(style);
    style->marginEnd.type = FlexLengthTypeAuto;
    style->flags |= UINT64_C(1) << 20;
    style->dirty |= UINT64_C(1) << 20;
}
void box_style_marginEndContent(box_style_t *style)
{
    assert(style);
    style->marginEnd.type = FlexLengthTypeContent;
    style->flags |= UINT64_C(1) << 20;
    style->dirty |= UINT64_C(1) << 20;
}
void box_style_paddingLeft(box_style_t *style, float paddingLeft)
{
    assert(style);
    style->paddingLeft.value = paddingLeft;
    style->paddingLeft.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 21;
    style->dirty |= UINT64_C(1) << 21;
}
void box_style_paddingLeftPercent(box_style_t *style, float paddingLeft)
{
    assert(style);
    style->paddingLeft.value = paddingLeft;
    style->paddingLeft.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 21;
    style->dirty |= UINT64_C(1) << 21;
}
void box_style_paddingTop(box_style_t *style, float paddingTop)
{
    assert(style);
    style->paddingTop.value = paddingTop;
    style->paddingTop.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 22;
    style->dirty |= UINT64_C(1) << 22;
}
void box_style_paddingTopPercent(box_style_t *style, float paddingTop)
{
    assert(style);
    style->paddingTop.value = paddingTop;
    style->paddingTop.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 22;
    style->dirty |= UINT64_C(1) << 22;
}
void box_style_paddingBottom(box_style_t *style, float paddingBottom)
{
    assert(style);
    style->paddingBottom.value = paddingBottom;
    style->paddingBottom.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 23;
    style->dirty |= UINT64_C(1) << 23;
}
void box_style_paddingBottomPercent(box_style_t *style, float paddingBottom)
{
    assert(style);
    style->paddingBottom.value = paddingBottom;
    style->paddingBottom.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 23;
    style->dirty |= UINT64_C(1) << 23;
}
void box_style_paddingRight(box_style_t *style, float paddingRight)
{
    assert(style);
    style->paddingRight.value = paddingRight;
    style->paddingRight.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 24;
    style->dirty |= UINT64_C(1) << 24;
}
void box_style_paddingRightPercent(box_style_t *style, float paddingRight)
{
    assert(style);
    style->paddingRight.value = paddingRight;
    style->paddingRight.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 24;
    style->dirty |= UINT64_C(1) << 24;
}
void box_style_paddingStart(box_style_t *style, float paddingStart)
{
    assert(style);
    style->paddingStart.value = paddingStart;
    style->paddingStart.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 25;
    style->dirty |= UINT64_C(1) << 25;
}
void box_style_paddingStartPercent(box_style_t *style, float paddingStart)
{
    assert(style);
    style->paddingStart.value = paddingStart;
    style->paddingStart.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 25;
    style->dirty |= UINT64_C(1) << 25;
}
void box_style_paddingEnd(box_style_t *style, float paddingEnd)
{
    assert(style);
    style->paddingEnd.value = paddingEnd;
    style->paddingEnd.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 26;
    style->dirty |= UINT64_C(1) << 26;
}
void box_style_paddingEndPercent(box_style_t *style, float paddingEnd)
{
    assert(style);
    style->paddingEnd.value = paddingEnd;
    style->paddingEnd.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 26;
    style->dirty |= UINT64_C(1) << 26;
}
void box_style_borderLeft(box_style_t *style, float borderLeft)
{
    assert(style);
    style->borderLeft = borderLeft;
    style->flags |= UINT64_C(1) << 27;
    style->dirty |= UINT64_C(1) << 27;
}
void box_style_borderTop(box_style_t *style, float borderTop)
{
    assert(style);
    style->borderTop = borderTop;
    style->flags |= UINT64_C(1) << 28;
    style->dirty |= UINT64_C(1) << 28;
}
void box_style_borderBottom(box_style_t *style, float borderBottom)
{
    assert(style);
    style->borderBottom = borderBottom;
    style->flags |= UINT64_C(1) << 29;
    style->dirty |= UINT64_C(1) << 29;
}
void box_style_borderRight(box_style_t *style, float borderRight)
{
    assert(style);
    style->borderRight = borderRight;
    style->flags |= UINT64_C(1) << 30;
    style->dirty |= UINT64_C(1) << 30;
}
void box_style_borderStart(box_style_t *style, float borderStart)
{
    assert(style);
    style->borderStart = borderStart;
    style->flags |= UINT64_C(1) << 31;
    style->dirty |= UINT64_C(1) << 31;
}
void box_style_borderEnd(box_style_t *style, float borderEnd)
{
    assert(style);
    style->borderEnd = borderEnd;
    style->flags |= UINT64_C(1) << 32;
    style->dirty |= UINT64_C(1) << 32;
}
void box_style_borderTopLeftRadius(box_style_t *style, float borderTopLeftRadius)
{
    assert(style);
    style->borderTopLeftRadius = borderTopLeftRadius;
    style->flags |= UINT64_C(1) << 33;
    style->dirty |= UINT64_C(1) << 33;
}
void box_style_borderTopRightRadius(box_style_t *style, float borderTopRightRadius)
{
    assert(style);
    style->borderTopRightRadius = borderTopRightRadius;
    style->flags |= UINT64_C(1) << 34;
    style->dirty |= UINT64_C(1) << 34;
}
void box_style_borderBottomRightRadius(box_style_t *style, float borderBottomRightRadius)
{
    assert(style);
    style->borderBottomRightRadius = borderBottomRightRadius;
    style->flags |= UINT64_C(1) << 35;
    style->dirty |= UINT64_C(1) << 35;
}
void box_style_borderBottomLeftRadius(box_style_t *style, float borderBottomLeftRadius)
{
    assert(style);
    style->borderBottomLeftRadius = borderBottomLeftRadius;
    style->flags |= UINT64_C(1) << 36;
    style->dirty |= UINT64_C(1) << 36;
}
void box_style_borderColor(box_style_t *style, plutovg_color_t borderColor)
{
    assert(style);
    style->borderColor = borderColor;
    style->flags |= UINT64_C(1) << 37;
    style->dirty |= UINT64_C(1) << 37;
}
void box_style_backgroundColor(box_style_t *style, plutovg_color_t backgroundColor)
{
    assert(style);
    style->backgroundColor = backgroundColor;
    style->flags |= UINT64_C(1) << 38;
    style->dirty |= UINT64_C(1) << 38;
}
void box_style_fontColor(box_style_t *style, plutovg_color_t fontColor)
{
    assert(style);
    style->fontColor = fontColor;
    style->flags |= UINT64_C(1) << 39;
    style->dirty |= UINT64_C(1) << 39;
}
void box_style_textAlign(box_style_t *style, TEXT_ALIGN textAlign)
{
    assert(style);
    style->textAlign = textAlign;
    style->flags |= UINT64_C(1) << 40;
    style->dirty |= UINT64_C(1) << 40;
}
void box_style_fontSize(box_style_t *style, double fontSize)
{
    assert(style);
    style->fontSize = fontSize;
    style->flags |= UINT64_C(1) << 41;
    style->dirty |= UINT64_C(1) << 41;
}
void box_style_backgroundImage(box_style_t *style, const char *backgroundImage)
{
    assert(style);
    assert(backgroundImage);
    if (style->backgroundImage) free(style->backgroundImage);
    style->backgroundImage = strdup(backgroundImage);
    style->flags |= UINT64_C(1) << 42;
    style->dirty |= UINT64_C(1) << 42;
}
void box_style_contentImage(box_style_t *style, const char *contentImage)
{
    assert(style);
    assert(contentImage);
    if (style->contentImage) free(style->contentImage);
    style->contentImage = strdup(contentImage);
    style->flags |= UINT64_C(1) << 43;
    style->dirty |= UINT64_C(1) << 43;
}
void box_style_transform(box_style_t *style, double m00, double m10, double m01, double m11, double m02, double m12)
{
    assert(style);
    plutovg_matrix_init(&style->transform, m00, m10, m01, m11, m02, m12);
    style->flags |= UINT64_C(1) << 44;
    style->dirty |= UINT64_C(1) << 44;
}
void box_style_transformOriginKeyword(box_style_t *style, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y)
{
    assert(style);
    style->transformOrigin.type = TRANSFORM_ORIGIN_TYPE_KEYWORD;
    style->transformOrigin.x.keyword = x;
    style->transformOrigin.y.keyword = y;
    style->flags |= UINT64_C(1) << 45;
    style->dirty |= UINT64_C(1) << 45;
}
void box_style_transformOriginOffset(box_style_t *style, double x, double y)
{
    assert(style);
    style->transformOrigin.type = TRANSFORM_ORIGIN_TYPE_OFFSET;
    style->transformOrigin.x.offset = x;
    style->transformOrigin.y.offset = y;
    style->flags |= UINT64_C(1) << 45;
    style->dirty |= UINT64_C(1) << 45;
}
void box_style_text(box_style_t *style, const char *text)
{
    assert(style);
    assert(text);
    if (style->text) free(style->text);
    style->text = strdup(text);
    style->flags |= UINT64_C(1) << 46;
    style->dirty |= UINT64_C(1) << 46;
}
void box_style_fontFamily(box_style_t *style, const char *fontFamily)
{
    assert(style);
    assert(fontFamily);
    if (style->fontFamily) free(style->fontFamily);
    style->fontFamily = strdup(fontFamily);
    style->flags |= UINT64_C(1) << 47;
    style->dirty |= UINT64_C(1) << 47;
}
void box_style_overflow(box_style_t *style, CSS_OVERFLOW overflow)
{
    assert(style);
    style->overflow = overflow;
    style->flags |= UINT64_C(1) << 48;
    style->dirty |= UINT64_C(1) << 48;
}
void box_style_left(box_style_t *style, float left)
{
    assert(style);
    style->left.value = left;
    style->left.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 49;
    style->dirty |= UINT64_C(1) << 49;
}
void box_style_leftPercent(box_style_t *style, float left)
{
    assert(style);
    style->left.value = left;
    style->left.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 49;
    style->dirty |= UINT64_C(1) << 49;
}
void box_style_right(box_style_t *style, float right)
{
    assert(style);
    style->right.value = right;
    style->right.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 50;
    style->dirty |= UINT64_C(1) << 50;
}
void box_style_rightPercent(box_style_t *style, float right)
{
    assert(style);
    style->right.value = right;
    style->right.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 50;
    style->dirty |= UINT64_C(1) << 50;
}
void box_style_top(box_style_t *style, float top)
{
    assert(style);
    style->top.value = top;
    style->top.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 51;
    style->dirty |= UINT64_C(1) << 51;
}
void box_style_topPercent(box_style_t *style, float top)
{
    assert(style);
    style->top.value = top;
    style->top.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 51;
    style->dirty |= UINT64_C(1) << 51;
}
void box_style_bottom(box_style_t *style, float bottom)
{
    assert(style);
    style->bottom.value = bottom;
    style->bottom.type = FlexLengthTypePoint;
    style->flags |= UINT64_C(1) << 52;
    style->dirty |= UINT64_C(1) << 52;
}
void box_style_bottomPercent(box_style_t *style, float bottom)
{
    assert(style);
    style->bottom.value = bottom;
    style->bottom.type = FlexLengthTypePercent;
    style->flags |= UINT64_C(1) << 52;
    style->dirty |= UINT64_C(1) << 52;
}
void box_style_zIndex(box_style_t *style, int zIndex)
{
    assert(style);
    style->zIndex = zIndex;
    style->flags |= UINT64_C(1) << 53;
    style->dirty |= UINT64_C(1) << 53;
}
// 54 54

box_style_t *box_style_new()
{
    box_style_t *style = calloc(1, sizeof(box_style_t));
    style->flags = UINT64_C(0);
    style->dirty = ~UINT64_C(0);
    return style;
}

void box_style_clear(box_style_t *style)
{
    assert(style);


    if (style->backgroundImage)
        free(style->backgroundImage);
    style->backgroundImage = NULL;


    if (style->contentImage)
        free(style->contentImage);
    style->contentImage = NULL;


    if (style->text)
        free(style->text);
    style->text = NULL;


    if (style->fontFamily)
        free(style->fontFamily);
    style->fontFamily = NULL;


    *style = (box_style_t){};
    style->flags = UINT64_C(0);
    style->dirty = ~UINT64_C(0);
}

void box_style_free(box_style_t *style)
{
    assert(style);
    box_style_clear(style);
    free(style);
}

uint64_t box_style_is_dirty(box_style_t *style)
{
    return style->dirty;
}

void box_style_clear_dirty(box_style_t *style)
{
    style->dirty = UINT64_C(0);
}

int box_style_is_unset(box_style_t *style, enum BOX_STYLE prop)
{
    return !(style->flags & prop);
}


void box_style_unset(box_style_t *dst, enum BOX_STYLE prop)
{
    assert(dst);
    dst->flags &= ~prop;
}


void box_style_merge(box_style_t *dst, const box_style_t *src)
{
    assert(dst && src);

    uint64_t flags = src->flags;

    if(!flags)
        return;

    for(int i = 0; i < 54; i++)
    {
        if(!(flags & (UINT64_C(1) << i)))
            continue;
        switch(i)
        {

            case 0:
                dst->wrap = src->wrap;
                break;

            case 1:
                dst->direction = src->direction;
                break;

            case 2:
                dst->alignItems = src->alignItems;
                break;

            case 3:
                dst->alignSelf = src->alignSelf;
                break;

            case 4:
                dst->alignContent = src->alignContent;
                break;

            case 5:
                dst->justifyContent = src->justifyContent;
                break;

            case 6:
                dst->flexBasis = src->flexBasis;
                break;

            case 7:
                dst->flexGrow = src->flexGrow;
                break;

            case 8:
                dst->flexShrink = src->flexShrink;
                break;

            case 9:
                dst->width = src->width;
                break;

            case 10:
                dst->height = src->height;
                break;

            case 11:
                dst->minWidth = src->minWidth;
                break;

            case 12:
                dst->minHeight = src->minHeight;
                break;

            case 13:
                dst->maxWidth = src->maxWidth;
                break;

            case 14:
                dst->maxHeight = src->maxHeight;
                break;

            case 15:
                dst->marginLeft = src->marginLeft;
                break;

            case 16:
                dst->marginTop = src->marginTop;
                break;

            case 17:
                dst->marginBottom = src->marginBottom;
                break;

            case 18:
                dst->marginRight = src->marginRight;
                break;

            case 19:
                dst->marginStart = src->marginStart;
                break;

            case 20:
                dst->marginEnd = src->marginEnd;
                break;

            case 21:
                dst->paddingLeft = src->paddingLeft;
                break;

            case 22:
                dst->paddingTop = src->paddingTop;
                break;

            case 23:
                dst->paddingBottom = src->paddingBottom;
                break;

            case 24:
                dst->paddingRight = src->paddingRight;
                break;

            case 25:
                dst->paddingStart = src->paddingStart;
                break;

            case 26:
                dst->paddingEnd = src->paddingEnd;
                break;

            case 27:
                dst->borderLeft = src->borderLeft;
                break;

            case 28:
                dst->borderTop = src->borderTop;
                break;

            case 29:
                dst->borderBottom = src->borderBottom;
                break;

            case 30:
                dst->borderRight = src->borderRight;
                break;

            case 31:
                dst->borderStart = src->borderStart;
                break;

            case 32:
                dst->borderEnd = src->borderEnd;
                break;

            case 33:
                dst->borderTopLeftRadius = src->borderTopLeftRadius;
                break;

            case 34:
                dst->borderTopRightRadius = src->borderTopRightRadius;
                break;

            case 35:
                dst->borderBottomRightRadius = src->borderBottomRightRadius;
                break;

            case 36:
                dst->borderBottomLeftRadius = src->borderBottomLeftRadius;
                break;

            case 37:
                dst->borderColor = src->borderColor;
                break;

            case 38:
                dst->backgroundColor = src->backgroundColor;
                break;

            case 39:
                dst->fontColor = src->fontColor;
                break;

            case 40:
                dst->textAlign = src->textAlign;
                break;

            case 41:
                dst->fontSize = src->fontSize;
                break;

            case 42:
                if(dst->backgroundImage) free(dst->backgroundImage);
                dst->backgroundImage = strdup(src->backgroundImage);
                break;

            case 43:
                if(dst->contentImage) free(dst->contentImage);
                dst->contentImage = strdup(src->contentImage);
                break;

            case 44:
                dst->transform = src->transform;
                break;

            case 45:
                dst->transformOrigin = src->transformOrigin;
                break;

            case 46:
                if(dst->text) free(dst->text);
                dst->text = strdup(src->text);
                break;

            case 47:
                if(dst->fontFamily) free(dst->fontFamily);
                dst->fontFamily = strdup(src->fontFamily);
                break;

            case 48:
                dst->overflow = src->overflow;
                break;

            case 49:
                dst->left = src->left;
                break;

            case 50:
                dst->right = src->right;
                break;

            case 51:
                dst->top = src->top;
                break;

            case 52:
                dst->bottom = src->bottom;
                break;

            case 53:
                dst->zIndex = src->zIndex;
                break;

        }
        dst->flags |= UINT64_C(1) << i;
        dst->dirty |= UINT64_C(1) << i;
    }
}


void box_style_to_flex(box_style_t *style, box_t box)
{
    assert(box && style);

    for(int i = 0; i < 64; i++)
    {
        if(!(style->flags & (UINT64_C(1) << i)))
            continue;
        switch(i)
        {

            case 0:
                Flex_setWrap(box, style->wrap);
                break;

            case 1:
                Flex_setDirection(box, style->direction);
                break;

            case 2:
                Flex_setAlignItems(box, style->alignItems);
                break;

            case 3:
                Flex_setAlignSelf(box, style->alignSelf);
                break;

            case 4:
                Flex_setAlignContent(box, style->alignContent);
                break;

            case 5:
                Flex_setJustifyContent(box, style->justifyContent);
                break;

            case 6:
                Flex_setFlexBasis_Length(box, style->flexBasis);
                break;

            case 7:
                Flex_setFlexGrow(box, style->flexGrow);
                break;

            case 8:
                Flex_setFlexShrink(box, style->flexShrink);
                break;

            case 9:
                Flex_setWidth_Length(box, style->width);
                break;

            case 10:
                Flex_setHeight_Length(box, style->height);
                break;

            case 11:
                Flex_setMinWidth_Length(box, style->minWidth);
                break;

            case 12:
                Flex_setMinHeight_Length(box, style->minHeight);
                break;

            case 13:
                Flex_setMaxWidth_Length(box, style->maxWidth);
                break;

            case 14:
                Flex_setMaxHeight_Length(box, style->maxHeight);
                break;

            case 15:
                Flex_setMarginLeft_Length(box, style->marginLeft);
                break;

            case 16:
                Flex_setMarginTop_Length(box, style->marginTop);
                break;

            case 17:
                Flex_setMarginBottom_Length(box, style->marginBottom);
                break;

            case 18:
                Flex_setMarginRight_Length(box, style->marginRight);
                break;

            case 19:
                Flex_setMarginStart_Length(box, style->marginStart);
                break;

            case 20:
                Flex_setMarginEnd_Length(box, style->marginEnd);
                break;

            case 21:
                Flex_setPaddingLeft_Length(box, style->paddingLeft);
                break;

            case 22:
                Flex_setPaddingTop_Length(box, style->paddingTop);
                break;

            case 23:
                Flex_setPaddingBottom_Length(box, style->paddingBottom);
                break;

            case 24:
                Flex_setPaddingRight_Length(box, style->paddingRight);
                break;

            case 25:
                Flex_setPaddingStart_Length(box, style->paddingStart);
                break;

            case 26:
                Flex_setPaddingEnd_Length(box, style->paddingEnd);
                break;

            case 27:
                Flex_setBorderLeft(box, style->borderLeft);
                break;

            case 28:
                Flex_setBorderTop(box, style->borderTop);
                break;

            case 29:
                Flex_setBorderBottom(box, style->borderBottom);
                break;

            case 30:
                Flex_setBorderRight(box, style->borderRight);
                break;

            case 31:
                Flex_setBorderStart(box, style->borderStart);
                break;

            case 32:
                Flex_setBorderEnd(box, style->borderEnd);
                break;

            default:
                break;
        }
    }
}
