/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lib.h
 * Copyright (C) 2015 Wiky L <wiiiky@outlook.com>
 * 
 * libjsn is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libjsn is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */
#ifndef __LIB_JSN_H__
#define __LIB_JSN_H__


#include "jlist.h"
#include <inttypes.h>


typedef struct _JSONNode JSONNode;

typedef enum {
    JSON_TYPE_OBJECT,
    JSON_TYPE_ARRAY,
    JSON_TYPE_STRING,
    JSON_TYPE_INT,
    JSON_TYPE_FLOAT,
    JSON_TYPE_TRUE,
    JSON_TYPE_FALSE,
    JSON_TYPE_NULL,
} JSONType;

struct _JSONNode {
    JSONType type;
    union {
        JList *children;        /* Children of OBJECT or ARRAY */
        int64_t integer;
        double floating;
        char *string;
    } data;

    char *name;                 /* the name of item */
};
#define json_node_get_type(n)   ((n)->type)
#define json_node_is_object(n)  (json_node_get_type(n)==JSON_TYPE_OBJECT)
#define json_node_is_array(n)   (json_node_get_type(n)==JSON_TYPE_ARRAY)
#define json_node_is_string(n)  (json_node_get_type(n)==JSON_TYPE_STRING)
#define json_node_is_int(n)     (json_node_get_type(n)==JSON_TYPE_INT)
#define json_node_is_float(n)   (json_node_get_type(n)==JSON_TYPE_FLOAT)
#define json_node_is_true(n)    (json_node_get_type(n)==JSON_TYPE_TRUE)
#define json_node_is_false(n)   (json_node_get_type(n)==JSON_TYPE_FALSE)
#define json_node_is_null(n)    (json_node_get_type(n)==JSON_TYPE_NULL)
#define json_node_is_number(n)  (json_node_is_int(n)||json_node_is_float(n))

#define json_node_get_name(n)   ((n)->name)

/*
 * Creates JSONNode of different types
 */
JSONNode *json_create_object(void);
JSONNode *json_create_array(void);
JSONNode *json_create_int(int64_t integer);
JSONNode *json_create_float(double floating);
JSONNode *json_create_string(const char *data);
JSONNode *json_create_string_take(char *data);
JSONNode *json_create_string_with_len(const char *data, unsigned int len);
JSONNode *json_create_true(void);
JSONNode *json_create_false(void);
JSONNode *json_create_null(void);

void json_node_free(JSONNode * node);

void json_node_set_name(JSONNode * node, const char *name);
void json_node_set_name_take(JSONNode * node, char *name);

/*
 * 添加子节点,node必须是object或者array
 */
void json_object_put_string(JSONNode * node, const char *name,
                            const char *string);
void json_object_put_int(JSONNode * node, const char *name,
                         int64_t integer);
void json_object_put_float(JSONNode * node, const char *name,
                           double floating);
void json_object_put_true(JSONNode * node, const char *name);
void json_object_put_false(JSONNode * node, const char *name);
void json_object_put_null(JSONNode * node, const char *name);
void json_object_put_array(JSONNode * node, const char *name,
                           JSONNode * array);
void json_object_put_object(JSONNode * node, const char *name,
                            JSONNode * object);
void json_array_add_string(JSONNode * node, const char *string);
void json_array_add_int(JSONNode * node, int64_t integer);
void json_array_add_float(JSONNode * node, double floating);
void json_array_add_true(JSONNode * node);
void json_array_add_false(JSONNode * node);
void json_array_add_null(JSONNode * node);
void json_array_add_object(JSONNode * node, JSONNode * object);
void json_array_add_array(JSONNode * node, JSONNode * array);

/* 获取子节点列表 */
JList *json_node_get_children(JSONNode * node);
#define json_node_get_children_length(n) \
                    (j_list_length(json_node_get_children(n)))



/*
* Parses JSON, and returns a JSONNode with type JSON_TYPE_OBJECT you can interrogate
*/
JSONNode *json_loads_from_data(const char *data);
JSONNode *json_loads_from_file(const char *path);

/*
* 获取object的子内容
*/
JSONNode *json_object_get(JSONNode * node, const char *name);
/*
* 获取字符串内容
*/
const char *json_string_get(JSONNode * node);
/*
 * 获取整数内容
 */
int64_t json_int_get(JSONNode * node);
/*
 * 获取浮点数内容
 */
double json_float_get(JSONNode * node);

/*
 * serializes JSONNode to a JSON formatted string
 * returns a new allocated string
 */
char *json_node_dumps(JSONNode * node);

#endif
