//
//  FlexLayout.h
//  flex_layout
//
//  Created by Sleen on 16/1/25.
//  Copyright © 2016年 Sleen. All rights reserved.
//

#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include <math.h>
#include <stddef.h>
#ifndef __cplusplus
#   include <stdbool.h>
#endif

typedef enum {
    FlexHorizontal,
    FlexVertical,
    FlexHorizontalReverse,
    FlexVerticalReverse
} FlexDirection;

typedef enum {
    FlexNoWrap,
    FlexWrap,
    FlexWrapReverse,
} FlexWrapMode;

typedef enum {
    FlexInherit,
    FlexStretch,
    FlexStart,
    FlexCenter,
    FlexEnd,
    FlexSpaceBetween,
    FlexSpaceAround,
    FlexBaseline,
} FlexAlign;

typedef enum {
    FlexLengthTypeUndefined,
    FlexLengthTypePoint,
    FlexLengthTypePercent,
    FlexLengthTypeAuto,
    FlexLengthTypeContent,
} FlexLengthType;


typedef struct {
    float value;
    FlexLengthType type;
} FlexLength;

typedef enum {
    FLEX_LEFT = 0,
    FLEX_TOP,
    FLEX_RIGHT,
    FLEX_BOTTOM,
    FLEX_START,
    FLEX_END
} FlexPositionIndex;

#define FlexUndefined NAN
#define FlexIsUndefined(n) isnan(n)

#define FlexLengthZero      (FlexLength){ 0,   FlexLengthTypePoint     }
#define FlexLengthAuto      (FlexLength){ NAN, FlexLengthTypeAuto      }
#define FlexLengthContent   (FlexLength){ NAN, FlexLengthTypeContent   }
#define FlexLengthUndefined (FlexLength){ NAN, FlexLengthTypeUndefined }


typedef union {
    struct {
        float width;
        float height;
    };
    float size[2];
} FlexSize;

typedef enum {
    FlexPrintDefault = 0,
    FlexPrintStyle = 1 << 0,
    FlexPrintResult = 1 << 1,
    FlexPrintChildren = 1 << 2,
    FlexPrintHideUnspecified = 1 << 3,
} FlexPrintOptions;

typedef struct FlexNode * FlexNodeRef;

typedef FlexSize    (*FlexMeasureFunc )(void* context, FlexSize constrainedSize);
typedef float       (*FlexBaselineFunc)(void* context, FlexSize constrainedSize);
typedef FlexSize    (*CustomLayoutFunc)(FlexNodeRef node, float constrainedWidth, float constrainedHeight, float scale);

#define FLEX_PROPERTY(type, Name, field) \
    FLEX_GETTER(type, Name, field) \
    FLEX_SETTER(type, Name, field)
#define FLEX_FLOAT_PROPERTY(Name, field) FLEX_PROPERTY(float, Name, field)
#define FLEX_LENGTH_PROPERTY(Name, field) \
    FLEX_GETTER(FlexLength, Name, field) \
    FLEX_SETTER_LENGTH(Name##_Length, field) \
    FLEX_SETTER_LENGTH_VALUE(Name, field, Point) \
    FLEX_SETTER_LENGTH_VALUE(Name##Percent, field, Percent)
#define FLEX_LENGTH_PROPERTY_AUTO(Name, field) \
    FLEX_LENGTH_PROPERTY(Name, field) \
    FLEX_SETTER_LENGTH_TYPE(Name, field, Auto)
#define FLEX_LENGTH_PROPERTY_AUTO_CONTENT(Name, field) \
    FLEX_LENGTH_PROPERTY_AUTO(Name, field) \
    FLEX_SETTER_LENGTH_TYPE(Name, field, Content)
#define FLEX_RESULT_PROPERTY(Name, field) \
    FLEX_GETTER(float, Result##Name, field) \
    FLEX_SETTER(float, Result##Name, field)

#define FLEX_PROPERTYES() \
    FLEX_PROPERTY                       ( FlexWrapMode,     Wrap,           wrap                        ); \
    FLEX_PROPERTY                       ( FlexDirection,    Direction,      direction                   ); \
    FLEX_PROPERTY                       ( FlexAlign,        AlignItems,     alignItems                  ); \
    FLEX_PROPERTY                       ( FlexAlign,        AlignSelf,      alignSelf                   ); \
    FLEX_PROPERTY                       ( FlexAlign,        AlignContent,   alignContent                ); \
    FLEX_PROPERTY                       ( FlexAlign,        JustifyContent, justifyContent              ); \
    FLEX_LENGTH_PROPERTY_AUTO_CONTENT   (                   FlexBasis,      flexBasis                   ); \
    FLEX_FLOAT_PROPERTY                 (                   FlexGrow,       flexGrow                    ); \
    FLEX_FLOAT_PROPERTY                 (                   FlexShrink,     flexShrink                  ); \
    FLEX_LENGTH_PROPERTY_AUTO           (                   Width,          size[FLEX_WIDTH]            ); \
    FLEX_LENGTH_PROPERTY_AUTO           (                   Height,         size[FLEX_HEIGHT]           ); \
    FLEX_LENGTH_PROPERTY                (                   MinWidth,       minSize[FLEX_WIDTH]         ); \
    FLEX_LENGTH_PROPERTY                (                   MinHeight,      minSize[FLEX_HEIGHT]        ); \
    FLEX_LENGTH_PROPERTY                (                   MaxWidth,       maxSize[FLEX_WIDTH]         ); \
    FLEX_LENGTH_PROPERTY                (                   MaxHeight,      maxSize[FLEX_HEIGHT]        ); \
    FLEX_LENGTH_PROPERTY_AUTO           (                   MarginLeft,     margin[FLEX_LEFT]           ); \
    FLEX_LENGTH_PROPERTY_AUTO           (                   MarginTop,      margin[FLEX_TOP]            ); \
    FLEX_LENGTH_PROPERTY_AUTO           (                   MarginBottom,   margin[FLEX_BOTTOM]         ); \
    FLEX_LENGTH_PROPERTY_AUTO           (                   MarginRight,    margin[FLEX_RIGHT]          ); \
    FLEX_LENGTH_PROPERTY_AUTO           (                   MarginStart,    margin[FLEX_START]          ); \
    FLEX_LENGTH_PROPERTY_AUTO           (                   MarginEnd,      margin[FLEX_END]            ); \
    FLEX_LENGTH_PROPERTY                (                   PaddingLeft,    padding[FLEX_LEFT]          ); \
    FLEX_LENGTH_PROPERTY                (                   PaddingTop,     padding[FLEX_TOP]           ); \
    FLEX_LENGTH_PROPERTY                (                   PaddingBottom,  padding[FLEX_BOTTOM]        ); \
    FLEX_LENGTH_PROPERTY                (                   PaddingRight,   padding[FLEX_RIGHT]         ); \
    FLEX_LENGTH_PROPERTY                (                   PaddingStart,   padding[FLEX_START]         ); \
    FLEX_LENGTH_PROPERTY                (                   PaddingEnd,     padding[FLEX_END]           ); \
    FLEX_FLOAT_PROPERTY                 (                   BorderLeft,     border[FLEX_LEFT]           ); \
    FLEX_FLOAT_PROPERTY                 (                   BorderTop,      border[FLEX_TOP]            ); \
    FLEX_FLOAT_PROPERTY                 (                   BorderBottom,   border[FLEX_BOTTOM]         ); \
    FLEX_FLOAT_PROPERTY                 (                   BorderRight,    border[FLEX_RIGHT]          ); \
    FLEX_FLOAT_PROPERTY                 (                   BorderStart,    border[FLEX_START]          ); \
    FLEX_FLOAT_PROPERTY                 (                   BorderEnd,      border[FLEX_END]            ); \
    FLEX_PROPERTY                       ( void*,            Context,        context                     ); \

#define FLEX_CALLBACK_PROPERTIES() \
    FLEX_PROPERTY                       ( FlexMeasureFunc,  MeasureFunc,    measure                     ); \
    FLEX_PROPERTY                       ( FlexBaselineFunc, BaselineFunc,   baseline                    ); \
    FLEX_PROPERTY                       ( CustomLayoutFunc, CustomLayout,   customLayout                ); \

#define FLEX_EXT_PROPERTYES() \
    FLEX_PROPERTY                       ( bool,             Fixed,          fixed                       ); \
    FLEX_LENGTH_PROPERTY                (                   Spacing,        spacing                     ); \
    FLEX_LENGTH_PROPERTY                (                   LineSpacing,    lineSpacing                 ); \
    FLEX_PROPERTY                       ( unsigned int,     Lines,          lines                       ); \
    FLEX_PROPERTY                       ( unsigned int,     ItemsPerLine,   itemsPerLine                ); \

#define FLEX_RESULT_PROPERTYES() \
    FLEX_RESULT_PROPERTY                (                   Width,          result.size[FLEX_WIDTH]     ); \
    FLEX_RESULT_PROPERTY                (                   Height,         result.size[FLEX_HEIGHT]    ); \
    FLEX_RESULT_PROPERTY                (                   ScrollWidth,    result.scroll[FLEX_WIDTH]   ); \
    FLEX_RESULT_PROPERTY                (                   ScrollHeight,   result.scroll[FLEX_HEIGHT]  ); \
    FLEX_RESULT_PROPERTY                (                   Left,           result.position[FLEX_LEFT]  ); \
    FLEX_RESULT_PROPERTY                (                   Top,            result.position[FLEX_TOP]   ); \
    FLEX_RESULT_PROPERTY                (                   MarginLeft,     result.margin[FLEX_LEFT]    ); \
    FLEX_RESULT_PROPERTY                (                   MarginRight,    result.margin[FLEX_RIGHT]   ); \
    FLEX_RESULT_PROPERTY                (                   MarginTop,      result.margin[FLEX_TOP]     ); \
    FLEX_RESULT_PROPERTY                (                   MarginBottom,   result.margin[FLEX_BOTTOM]  ); \
    FLEX_RESULT_PROPERTY                (                   PaddingLeft,    result.padding[FLEX_LEFT]   ); \
    FLEX_RESULT_PROPERTY                (                   PaddingRight,   result.padding[FLEX_RIGHT]  ); \
    FLEX_RESULT_PROPERTY                (                   PaddingTop,     result.padding[FLEX_TOP]    ); \
    FLEX_RESULT_PROPERTY                (                   PaddingBottom,  result.padding[FLEX_BOTTOM] ); \
    FLEX_RESULT_PROPERTY                (                   BorderLeft,     result.border[FLEX_LEFT]    ); \
    FLEX_RESULT_PROPERTY                (                   BorderRight,    result.border[FLEX_RIGHT]   ); \
    FLEX_RESULT_PROPERTY                (                   BorderTop,      result.border[FLEX_TOP]     ); \
    FLEX_RESULT_PROPERTY                (                   BorderBottom,   result.border[FLEX_BOTTOM]  ); \

// declaration of getters and setters
#define FLEX_GETTER(type, Name, field)                  type Flex_get##Name(FlexNodeRef node);
#define FLEX_SETTER(type, Name, field)                  void Flex_set##Name(FlexNodeRef node, type Name);
#define FLEX_SETTER_LENGTH(Name, field)                 FLEX_SETTER(FlexLength, Name, field)
#define FLEX_SETTER_LENGTH_VALUE(Name, field, Type)     void Flex_set##Name(FlexNodeRef node, float Name);
#define FLEX_SETTER_LENGTH_TYPE(Name, field, Type)      void Flex_set##Name##Type(FlexNodeRef node);

FLEX_PROPERTYES()
FLEX_CALLBACK_PROPERTIES()
FLEX_EXT_PROPERTYES()
FLEX_RESULT_PROPERTYES()

#undef FLEX_GETTER
#undef FLEX_SETTER
#undef FLEX_SETTER_LENGTH
#undef FLEX_SETTER_LENGTH_VALUE
#undef FLEX_SETTER_LENGTH_TYPE


FlexNodeRef Flex_newNode();
void        Flex_freeNode          (FlexNodeRef node);
void        Flex_insertChild       (FlexNodeRef node, FlexNodeRef child, size_t index);
void        Flex_addChild          (FlexNodeRef node, FlexNodeRef child);
void        Flex_removeChild       (FlexNodeRef node, FlexNodeRef child);
FlexNodeRef Flex_getChild          (FlexNodeRef node, size_t index);
size_t      Flex_getChildrenCount  (FlexNodeRef node);
FlexNodeRef Flex_getParent         (FlexNodeRef node);
void        Flex_layout            (FlexNodeRef node, float constrainedWidth, float constrainedHeight, float scale);
void        Flex_print             (FlexNodeRef node, FlexPrintOptions options);

#if DEBUG
#   define flex_assert(e) assert(e)
#else
#   define flex_assert(e) ((void)0)
#endif

#define FlexTreatNanAsInf(n) (isnan(n) ? INFINITY : n)
#define FlexFloatEquals(a, b) ((isnan(a) && isnan(b)) || a == b)
#define FlexLengthEquals(a, b) (FlexFloatEquals(a.value, b.value) && a.type == b.type)
#define FlexPixelRound(value, scale) (roundf((value) * (scale)) / (scale))
#define FlexIsResolved(n) !FlexIsUndefined(n)
#define FlexCacheSizeUndefined (FlexSize){ -1000, -1000 }

#define FlexVector(type) FlexVector_##type
#define FlexVectorRef(type) FlexVector(type)*
#define FlexVector_new(type, initialCapacity) _FlexVector_new_##type(initialCapacity)
#define FlexVector_free(type, vector) _FlexVector_free_##type(vector)
#define FlexVector_clear(type, vector) _FlexVector_clear_##type(vector)
#define FlexVector_insert(type, vector, value, index) _FlexVector_insert_##type(vector, value, index)
#define FlexVector_add(type, vector, value) _FlexVector_add_##type(vector, value)
#define FlexVector_removeAt(type, vector, index) _FlexVector_removeAt_##type(vector, index)
#define FlexVector_remove(type, vector, value) _FlexVector_remove_##type(vector, value)
#define FlexVector_size(type, vector) _FlexVector_size_##type(vector)

typedef struct {
    float scale;
} FlexLayoutContext;

static inline bool flex_isAbsolute(FlexLength length) {
    return length.type == FlexLengthTypePoint;
}

static inline float flex_absoluteValue(FlexLength length, FlexLayoutContext *context) {
    flex_assert(flex_isAbsolute(length));    // absolute value is requested
    return length.value;
}

static inline float flex_resolve(FlexLength length, FlexLayoutContext *context, float relativeTo) {
    if (flex_isAbsolute(length)) {
        return flex_absoluteValue(length, context);
    }
    else if (length.type == FlexLengthTypePercent && FlexIsResolved(relativeTo)) {
        return length.value / 100 * relativeTo;
    }
    
    return NAN;
}

static inline float flex_auto_to_0(float value) {
    return FlexIsResolved(value) ? value : 0;
}

static inline float flex_clamp(float value, float minValue, float maxValue) {
    if (FlexIsUndefined(value)) {
        return value;
    }
    
    if (FlexIsUndefined(maxValue)) {
        return fmaxf(value, minValue);
    } else {
        return fmaxf(fminf(value, maxValue), minValue);
    }
}

static inline float flex_clampMax(float value, float maxValue) {
    if (FlexIsUndefined(value)) {
        return value;
    }
    
    if (FlexIsUndefined(maxValue)) {
        return value;
    } else {
        return fminf(value, maxValue);
    }
}

static inline float flex_clampMin(float value, float minValue) {
    if (FlexIsUndefined(value)) {
        return value;
    }
    
    if (FlexIsUndefined(minValue)) {
        return value;
    } else {
        return fmaxf(value, minValue);
    }
}

void flex_markDirty(FlexNodeRef node);

#ifdef __cplusplus
}
#endif
