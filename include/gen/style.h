#pragma once
#include <stdint.h>
typedef struct box_style_t
{
    uint64_t flags; // 0: null, 1: assigned
    uint64_t dirty;

	FlexWrapMode wrap;
	FlexDirection direction;
	FlexAlign alignItems;
	FlexAlign alignSelf;
	FlexAlign alignContent;
	FlexAlign justifyContent;
	FlexLength flexBasis;
	float flexGrow;
	float flexShrink;
	FlexLength width;
	FlexLength height;
	FlexLength minWidth;
	FlexLength minHeight;
	FlexLength maxWidth;
	FlexLength maxHeight;
	FlexLength marginLeft;
	FlexLength marginTop;
	FlexLength marginBottom;
	FlexLength marginRight;
	FlexLength marginStart;
	FlexLength marginEnd;
	FlexLength paddingLeft;
	FlexLength paddingTop;
	FlexLength paddingBottom;
	FlexLength paddingRight;
	FlexLength paddingStart;
	FlexLength paddingEnd;
	float borderLeft;
	float borderTop;
	float borderBottom;
	float borderRight;
	float borderStart;
	float borderEnd;
	float borderTopLeftRadius;
	float borderTopRightRadius;
	float borderBottomRightRadius;
	float borderBottomLeftRadius;
	plutovg_color_t borderColor;
	plutovg_color_t backgroundColor;
	plutovg_color_t fontColor;
	TEXT_ALIGN textAlign;
	double fontSize;
	char * backgroundImage;
	char * contentImage;
	plutovg_matrix_t transform;
	struct transform_origin_t transformOrigin;
	char * fontFamily;
	CSS_OVERFLOW overflow;
	FlexLength left;
	FlexLength right;
	FlexLength top;
	FlexLength bottom;
	int zIndex;
} box_style_t;

typedef uint64_t box_style_flag_t;
#define BOX_STYLE_wrap (UINT64_C(1) << 0)
#define BOX_STYLE_direction (UINT64_C(1) << 1)
#define BOX_STYLE_alignItems (UINT64_C(1) << 2)
#define BOX_STYLE_alignSelf (UINT64_C(1) << 3)
#define BOX_STYLE_alignContent (UINT64_C(1) << 4)
#define BOX_STYLE_justifyContent (UINT64_C(1) << 5)
#define BOX_STYLE_flexBasis (UINT64_C(1) << 6)
#define BOX_STYLE_flexGrow (UINT64_C(1) << 7)
#define BOX_STYLE_flexShrink (UINT64_C(1) << 8)
#define BOX_STYLE_width (UINT64_C(1) << 9)
#define BOX_STYLE_height (UINT64_C(1) << 10)
#define BOX_STYLE_minWidth (UINT64_C(1) << 11)
#define BOX_STYLE_minHeight (UINT64_C(1) << 12)
#define BOX_STYLE_maxWidth (UINT64_C(1) << 13)
#define BOX_STYLE_maxHeight (UINT64_C(1) << 14)
#define BOX_STYLE_marginLeft (UINT64_C(1) << 15)
#define BOX_STYLE_marginTop (UINT64_C(1) << 16)
#define BOX_STYLE_marginBottom (UINT64_C(1) << 17)
#define BOX_STYLE_marginRight (UINT64_C(1) << 18)
#define BOX_STYLE_marginStart (UINT64_C(1) << 19)
#define BOX_STYLE_marginEnd (UINT64_C(1) << 20)
#define BOX_STYLE_paddingLeft (UINT64_C(1) << 21)
#define BOX_STYLE_paddingTop (UINT64_C(1) << 22)
#define BOX_STYLE_paddingBottom (UINT64_C(1) << 23)
#define BOX_STYLE_paddingRight (UINT64_C(1) << 24)
#define BOX_STYLE_paddingStart (UINT64_C(1) << 25)
#define BOX_STYLE_paddingEnd (UINT64_C(1) << 26)
#define BOX_STYLE_borderLeft (UINT64_C(1) << 27)
#define BOX_STYLE_borderTop (UINT64_C(1) << 28)
#define BOX_STYLE_borderBottom (UINT64_C(1) << 29)
#define BOX_STYLE_borderRight (UINT64_C(1) << 30)
#define BOX_STYLE_borderStart (UINT64_C(1) << 31)
#define BOX_STYLE_borderEnd (UINT64_C(1) << 32)
#define BOX_STYLE_borderTopLeftRadius (UINT64_C(1) << 33)
#define BOX_STYLE_borderTopRightRadius (UINT64_C(1) << 34)
#define BOX_STYLE_borderBottomRightRadius (UINT64_C(1) << 35)
#define BOX_STYLE_borderBottomLeftRadius (UINT64_C(1) << 36)
#define BOX_STYLE_borderColor (UINT64_C(1) << 37)
#define BOX_STYLE_backgroundColor (UINT64_C(1) << 38)
#define BOX_STYLE_fontColor (UINT64_C(1) << 39)
#define BOX_STYLE_textAlign (UINT64_C(1) << 40)
#define BOX_STYLE_fontSize (UINT64_C(1) << 41)
#define BOX_STYLE_backgroundImage (UINT64_C(1) << 42)
#define BOX_STYLE_contentImage (UINT64_C(1) << 43)
#define BOX_STYLE_transform (UINT64_C(1) << 44)
#define BOX_STYLE_transformOrigin (UINT64_C(1) << 45)
#define BOX_STYLE_fontFamily (UINT64_C(1) << 46)
#define BOX_STYLE_overflow (UINT64_C(1) << 47)
#define BOX_STYLE_left (UINT64_C(1) << 48)
#define BOX_STYLE_right (UINT64_C(1) << 49)
#define BOX_STYLE_top (UINT64_C(1) << 50)
#define BOX_STYLE_bottom (UINT64_C(1) << 51)
#define BOX_STYLE_zIndex (UINT64_C(1) << 52)

void box_style_wrap(box_style_t *style, FlexWrapMode wrap);
void box_style_direction(box_style_t *style, FlexDirection direction);
void box_style_alignItems(box_style_t *style, FlexAlign alignItems);
void box_style_alignSelf(box_style_t *style, FlexAlign alignSelf);
void box_style_alignContent(box_style_t *style, FlexAlign alignContent);
void box_style_justifyContent(box_style_t *style, FlexAlign justifyContent);
void box_style_flexBasis(box_style_t *style, float flexBasis);
void box_style_flexBasisPercent(box_style_t *style, float flexBasis);
void box_style_flexBasisAuto(box_style_t *style);
void box_style_flexBasisContent(box_style_t *style);
void box_style_flexGrow(box_style_t *style, float flexGrow);
void box_style_flexShrink(box_style_t *style, float flexShrink);
void box_style_width(box_style_t *style, float width);
void box_style_widthPercent(box_style_t *style, float width);
void box_style_widthAuto(box_style_t *style);
void box_style_widthContent(box_style_t *style);
void box_style_height(box_style_t *style, float height);
void box_style_heightPercent(box_style_t *style, float height);
void box_style_heightAuto(box_style_t *style);
void box_style_heightContent(box_style_t *style);
void box_style_minWidth(box_style_t *style, float minWidth);
void box_style_minWidthPercent(box_style_t *style, float minWidth);
void box_style_minHeight(box_style_t *style, float minHeight);
void box_style_minHeightPercent(box_style_t *style, float minHeight);
void box_style_maxWidth(box_style_t *style, float maxWidth);
void box_style_maxWidthPercent(box_style_t *style, float maxWidth);
void box_style_maxHeight(box_style_t *style, float maxHeight);
void box_style_maxHeightPercent(box_style_t *style, float maxHeight);
void box_style_marginLeft(box_style_t *style, float marginLeft);
void box_style_marginLeftPercent(box_style_t *style, float marginLeft);
void box_style_marginLeftAuto(box_style_t *style);
void box_style_marginLeftContent(box_style_t *style);
void box_style_marginTop(box_style_t *style, float marginTop);
void box_style_marginTopPercent(box_style_t *style, float marginTop);
void box_style_marginTopAuto(box_style_t *style);
void box_style_marginTopContent(box_style_t *style);
void box_style_marginBottom(box_style_t *style, float marginBottom);
void box_style_marginBottomPercent(box_style_t *style, float marginBottom);
void box_style_marginBottomAuto(box_style_t *style);
void box_style_marginBottomContent(box_style_t *style);
void box_style_marginRight(box_style_t *style, float marginRight);
void box_style_marginRightPercent(box_style_t *style, float marginRight);
void box_style_marginRightAuto(box_style_t *style);
void box_style_marginRightContent(box_style_t *style);
void box_style_marginStart(box_style_t *style, float marginStart);
void box_style_marginStartPercent(box_style_t *style, float marginStart);
void box_style_marginStartAuto(box_style_t *style);
void box_style_marginStartContent(box_style_t *style);
void box_style_marginEnd(box_style_t *style, float marginEnd);
void box_style_marginEndPercent(box_style_t *style, float marginEnd);
void box_style_marginEndAuto(box_style_t *style);
void box_style_marginEndContent(box_style_t *style);
void box_style_paddingLeft(box_style_t *style, float paddingLeft);
void box_style_paddingLeftPercent(box_style_t *style, float paddingLeft);
void box_style_paddingTop(box_style_t *style, float paddingTop);
void box_style_paddingTopPercent(box_style_t *style, float paddingTop);
void box_style_paddingBottom(box_style_t *style, float paddingBottom);
void box_style_paddingBottomPercent(box_style_t *style, float paddingBottom);
void box_style_paddingRight(box_style_t *style, float paddingRight);
void box_style_paddingRightPercent(box_style_t *style, float paddingRight);
void box_style_paddingStart(box_style_t *style, float paddingStart);
void box_style_paddingStartPercent(box_style_t *style, float paddingStart);
void box_style_paddingEnd(box_style_t *style, float paddingEnd);
void box_style_paddingEndPercent(box_style_t *style, float paddingEnd);
void box_style_borderLeft(box_style_t *style, float borderLeft);
void box_style_borderTop(box_style_t *style, float borderTop);
void box_style_borderBottom(box_style_t *style, float borderBottom);
void box_style_borderRight(box_style_t *style, float borderRight);
void box_style_borderStart(box_style_t *style, float borderStart);
void box_style_borderEnd(box_style_t *style, float borderEnd);
void box_style_borderTopLeftRadius(box_style_t *style, float borderTopLeftRadius);
void box_style_borderTopRightRadius(box_style_t *style, float borderTopRightRadius);
void box_style_borderBottomRightRadius(box_style_t *style, float borderBottomRightRadius);
void box_style_borderBottomLeftRadius(box_style_t *style, float borderBottomLeftRadius);
void box_style_borderColor(box_style_t *style, plutovg_color_t borderColor);
void box_style_backgroundColor(box_style_t *style, plutovg_color_t backgroundColor);
void box_style_fontColor(box_style_t *style, plutovg_color_t fontColor);
void box_style_textAlign(box_style_t *style, TEXT_ALIGN textAlign);
void box_style_fontSize(box_style_t *style, double fontSize);
void box_style_backgroundImage(box_style_t *style, const char *backgroundImage);
void box_style_contentImage(box_style_t *style, const char *contentImage);
void box_style_transform(box_style_t *style, double m00, double m10, double m01, double m11, double m02, double m12);
void box_style_transformOriginKeyword(box_style_t *style, TRANSFORM_ORIGIN x, TRANSFORM_ORIGIN y);
void box_style_transformOriginOffset(box_style_t *style, double x, double y);
void box_style_fontFamily(box_style_t *style, const char *fontFamily);
void box_style_overflow(box_style_t *style, CSS_OVERFLOW overflow);
void box_style_left(box_style_t *style, float left);
void box_style_leftPercent(box_style_t *style, float left);
void box_style_right(box_style_t *style, float right);
void box_style_rightPercent(box_style_t *style, float right);
void box_style_top(box_style_t *style, float top);
void box_style_topPercent(box_style_t *style, float top);
void box_style_bottom(box_style_t *style, float bottom);
void box_style_bottomPercent(box_style_t *style, float bottom);
void box_style_zIndex(box_style_t *style, int zIndex);
// 53 53
box_style_t *box_style_new();
void box_style_clear(box_style_t *style);
void box_style_free(box_style_t *style);
uint64_t box_style_is_dirty(box_style_t *style);
void box_style_clear_dirty(box_style_t *style);
int box_style_is_unset(box_style_t *style, box_style_flag_t prop);
void box_style_unset(box_style_t *dst, box_style_flag_t prop);
void box_style_merge(box_style_t *dst, const box_style_t *src);
void box_style_to_flex(box_style_t *style, box_t box);
