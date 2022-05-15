#pragma once
typedef struct box_style_t
{
    unsigned long flags; // 0: null, 1: assigned

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
	char * text;
	char * fontFamily;
	CSS_OVERFLOW overflow;
	FlexLength left;
	FlexLength right;
	FlexLength top;
	FlexLength bottom;
	int zIndex;

} box_style_t;


enum BOX_STYLE
{

	BOX_STYLE_wrap = 1UL << 0,
	BOX_STYLE_direction = 1UL << 1,
	BOX_STYLE_alignItems = 1UL << 2,
	BOX_STYLE_alignSelf = 1UL << 3,
	BOX_STYLE_alignContent = 1UL << 4,
	BOX_STYLE_justifyContent = 1UL << 5,
	BOX_STYLE_flexBasis = 1UL << 6,
	BOX_STYLE_flexGrow = 1UL << 7,
	BOX_STYLE_flexShrink = 1UL << 8,
	BOX_STYLE_width = 1UL << 9,
	BOX_STYLE_height = 1UL << 10,
	BOX_STYLE_minWidth = 1UL << 11,
	BOX_STYLE_minHeight = 1UL << 12,
	BOX_STYLE_maxWidth = 1UL << 13,
	BOX_STYLE_maxHeight = 1UL << 14,
	BOX_STYLE_marginLeft = 1UL << 15,
	BOX_STYLE_marginTop = 1UL << 16,
	BOX_STYLE_marginBottom = 1UL << 17,
	BOX_STYLE_marginRight = 1UL << 18,
	BOX_STYLE_marginStart = 1UL << 19,
	BOX_STYLE_marginEnd = 1UL << 20,
	BOX_STYLE_paddingLeft = 1UL << 21,
	BOX_STYLE_paddingTop = 1UL << 22,
	BOX_STYLE_paddingBottom = 1UL << 23,
	BOX_STYLE_paddingRight = 1UL << 24,
	BOX_STYLE_paddingStart = 1UL << 25,
	BOX_STYLE_paddingEnd = 1UL << 26,
	BOX_STYLE_borderLeft = 1UL << 27,
	BOX_STYLE_borderTop = 1UL << 28,
	BOX_STYLE_borderBottom = 1UL << 29,
	BOX_STYLE_borderRight = 1UL << 30,
	BOX_STYLE_borderStart = 1UL << 31,
	BOX_STYLE_borderEnd = 1UL << 32,
	BOX_STYLE_borderTopLeftRadius = 1UL << 33,
	BOX_STYLE_borderTopRightRadius = 1UL << 34,
	BOX_STYLE_borderBottomRightRadius = 1UL << 35,
	BOX_STYLE_borderBottomLeftRadius = 1UL << 36,
	BOX_STYLE_borderColor = 1UL << 37,
	BOX_STYLE_backgroundColor = 1UL << 38,
	BOX_STYLE_fontColor = 1UL << 39,
	BOX_STYLE_textAlign = 1UL << 40,
	BOX_STYLE_fontSize = 1UL << 41,
	BOX_STYLE_backgroundImage = 1UL << 42,
	BOX_STYLE_contentImage = 1UL << 43,
	BOX_STYLE_transform = 1UL << 44,
	BOX_STYLE_transformOrigin = 1UL << 45,
	BOX_STYLE_text = 1UL << 46,
	BOX_STYLE_fontFamily = 1UL << 47,
	BOX_STYLE_overflow = 1UL << 48,
	BOX_STYLE_left = 1UL << 49,
	BOX_STYLE_right = 1UL << 50,
	BOX_STYLE_top = 1UL << 51,
	BOX_STYLE_bottom = 1UL << 52,
	BOX_STYLE_zIndex = 1UL << 53,

};

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
void box_style_text(box_style_t *style, const char *text);
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
// 54 54
box_style_t *box_style_new();
void box_style_clear(box_style_t *style);
void box_style_free(box_style_t *style);
int box_style_is_unset(box_style_t *style, enum BOX_STYLE prop);
void box_style_unset(box_style_t *dst, enum BOX_STYLE prop);
void box_style_merge(box_style_t *dst, const box_style_t *src);
void box_style_to_flex(box_style_t *style, box_t box);
