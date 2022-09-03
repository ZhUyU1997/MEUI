/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "common/log.h"
#include "common/class.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define LOG(fmt, args...) printf(fmt, ##args)

#define get_head_info_from_obj(obj) ((class_object_head_t *)obj - 1)
#define get_class_info_from_obj(info) (get_head_info_from_obj(obj)->type)
#define get_class_type_from_obj(info) get_class_info_from_obj(obj)
#define get_parent_obj_from_head_info(info) ((void *)((char *)info - info->type->parent_type->size))

typedef struct class_object_head
{
	type_index type;
	type_index child;
} class_object_head_t;

class_impl(object_t){};

struct list_head *get_class_child(type_index type)
{
	return &type->child;
}

char *get_class_name(void *obj)
{
	if (!obj)
		return "unknown";
	return get_class_info_from_obj(obj)->name;
}

type_index get_class_type(void *obj)
{
	if (!obj)
		LOGE("obj == NULL");
	return get_class_type_from_obj(obj);
}

char *__class_name(type_index type)
{
	return type->name;
}

void show_class_structure(void *obj)
{
	if (!obj)
	{
		LOGE("unknown");
		return;
	}

	class_object_head_t *head = get_head_info_from_obj(obj);
	LOG("%s\n", head->type->name);

	if (head->type == class_type(object_t))
		return;
	show_class_structure(get_parent_obj_from_head_info(head));
}

static void object_init(type_index type, type_index child, void *obj)
{
	class_object_head_t *head = get_head_info_from_obj(obj);
	head->type = type;
	head->child = child;

	if (type == class_type(object_t))
		return;

	object_init(type->parent_type, type, get_parent_obj_from_head_info(head));

	memcpy(obj, type->init_obj, type->size);

	if (type->pointer_constructor)
		type->pointer_constructor(obj);
}

char *new_class_object(type_index type)
{
	class_table_info_t *info = type;

	char *base = calloc(1, info->full_size);
	if (!base)
		return NULL;

	char *obj = base + info->full_size - info->size;
	object_init(type, type, obj);
	return obj;
}

static void object_exit(void *obj)
{
	class_object_head_t *head = get_head_info_from_obj(obj);

	class_table_info_t *info = head->type;

	if (info->pointer_destructor)
		info->pointer_destructor(obj);

	if (info == class_type(object_t))
		return;

	object_exit(get_parent_obj_from_head_info(head));
}

void delete_class_object(void *obj)
{
	assert(obj);

	for (;;)
	{
		class_object_head_t *head = get_head_info_from_obj(obj);
		if (head->type == head->child)
			break;
		obj = (char *)obj + head->type->size + sizeof(class_object_head_t);
	}

	object_exit(obj);

	type_index type = get_class_type(obj);

	char *base = (char *)obj - type->full_size + type->size;
	free(base);
}

void *class_cast(type_index type, void *obj)
{
	// printf("type = %d\n",type);

	if (!obj)
		return NULL;

	for (;;)
	{
		class_object_head_t *head = get_head_info_from_obj(obj);
		if (head->type == type)
			return obj;
		else if (head->type == head->child)
			break;
		obj = (char *)obj + head->type->size + sizeof(class_object_head_t);
	}

	for (;;)
	{
		class_object_head_t *head = get_head_info_from_obj(obj);
		if (head->type == type)
			return obj;
		else if (head->type == class_type(object_t))
			break;
		obj = get_parent_obj_from_head_info(head);
	}
	return NULL;
}

unsigned int set_class_table_info(class_table_info_t *info)
{
	if (info->list.next != NULL)
		return info->full_size;

	unsigned int size = 0;

	init_list_head(&info->list);
	init_list_head(&info->child);

	if (info->type != class_type(object_t))
	{
		if (info == info->parent_type)
		{
			PANIC("Can't inhert itself: " $(info->name));
		}
		size = set_class_table_info(info->parent_type);
		list_add_tail(&info->list, &info->parent_type->child);
	}

	info->full_size = size + sizeof(class_object_head_t) + info->size;
	return info->full_size;
}