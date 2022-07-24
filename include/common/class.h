/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __CLASS_H__
#define __CLASS_H__

#pragma once

#include "macro/base.h"
#include "common/list.h"

#ifdef __cplusplus
#error Not support C++
#endif

typedef unsigned int type_index;

struct class_table_info
{
	struct list_head list;
	struct list_head child;
	unsigned int type;
	unsigned int size;
	unsigned int full_size;
	unsigned int parent_type;
	char *name;
	void *init_obj;
	unsigned int *pointer_type;
	unsigned int *pointer_parent_type;
	void (*pointer_constructor)(void *);
	void (*pointer_destructor)(void *);
};

struct list_head *get_class_child(type_index type);
char *get_class_name(void *obj);
type_index get_class_type(void *obj);
char *__class_name(type_index type);
#define class_name(type) __class_name(type##_class_type)
#define class_type(type) type##_class_type

void show_class_structure(void *obj);
void object_init(type_index type, unsigned int child, void *data);
char *new_class_object(type_index type);
void delete_class_object(void *obj);
void *class_cast(type_index type, void *obj);

#define constructor_real_name(type_name, ...) CAT(type_name##_constructor, VARIADIC_SIZE(__VA_ARGS__))
#define constructor(type_name, args...) void constructor_real_name(type_name, ##args)(type_name * this, ##args)
#define destructor(type_name) void type_name##_destructor(type_name * this)

#define __CLASS(type_name, parent_type_name)                                                                                                        \
	type_index type_name##_class_type __attribute((aligned(8))) __attribute__((__used__, __section__(".class.type"))) = ~0;                         \
	extern type_index parent_type_name##_class_type;                                                                                                \
	static type_name type_name##_class_init;                                                                                                        \
	constructor(type_name) __attribute__((weak));                                                                                                   \
	destructor(type_name) __attribute__((weak));                                                                                                    \
	static struct class_table_info type_name##class_table_info __attribute((aligned(8))) __attribute__((__used__, __section__(".class.table"))) = { \
		.name = #type_name,                                                                                                                         \
		.pointer_type = &type_name##_class_type,                                                                                                    \
		.pointer_parent_type = &parent_type_name##_class_type,                                                                                      \
		.size = sizeof(type_name),                                                                                                                  \
		.init_obj = &type_name##_class_init,                                                                                                        \
		.pointer_constructor = (void (*)(void *))constructor_real_name(type_name),                                                                  \
		.pointer_destructor = (void (*)(void *))type_name##_destructor,                                                                             \
	};                                                                                                                                              \
	static type_name type_name##_class_init =

#define __CLASS0(type_name) __CLASS(type_name, object_t)
#define __CLASS1(type_name, parent_type_name) __CLASS(type_name, parent_type_name)
#define class_impl(type_name, args...)      \
	CAT(__CLASS, VARIADIC_SIZE(args))       \
	(type_name, ##args)

#define class(type_name, args...)             \
	struct type_name;                         \
	typedef struct type_name type_name;       \
	extern type_index type_name##_class_type; \
	struct type_name

#define __NEW0(type, args...) constructor_real_name(type, ##args)(obj, ##args)
#define __NEW1(type)

#define __NEW(type, ...)                        \
	if (obj)                                    \
	{                                           \
		CAT(__NEW, IS_EMPTY(__VA_ARGS__))       \
		(type, ##__VA_ARGS__);                  \
	}

#define new(type, args...) ({type * obj = (type *)new_class_object(type##_class_type); __NEW(type, ##args);obj; })
#define delete(obj) delete_class_object(obj)
#define __dynamic_cast(obj) (char *)(obj)))
#define dynamic_cast(type) ((type *) class_cast(type##_class_type, __dynamic_cast

class(object_t){};

void do_class_init();

#endif /* __CHARSET_H__ */
