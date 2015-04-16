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

#ifdef __cplusplus
extern "C" {
#endif

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
            JList *children;    /* Children of OBJECT or ARRAY */
            int64_t integer;
            double floating;
            char *string;
        } data;

        char *name;             /* the name of item */
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
    JSONNode *json_create_string_with_len(const char *data,
                                          unsigned int len);
    JSONNode *json_create_true(void);
    JSONNode *json_create_false(void);
    JSONNode *json_create_null(void);

    void json_node_free(JSONNode * node);

    void json_node_set_name(JSONNode * node, const char *name);
    void json_node_set_name_take(JSONNode * node, char *name);
    void json_node_set_name_with_len(JSONNode * node, const char *name,
                                     unsigned int len);

    void json_node_append_child(JSONNode * node, JSONNode * child);
    JList *json_node_get_children(JSONNode * node);



/*
 * Parses JSON from a string, and returns a JSONNode with type JSON_TYPE_OBJECT you can interrogate
 */
    JSONNode *json_loads_from_data(const char *data);

/*
 * Gets an element named as name from a json object
 */
    JSONNode *json_object_get(JSONNode * node, const char *name);
/*
 * Gets the string
 */
    const char *json_string_get(JSONNode * node);
    int64_t json_int_get(JSONNode * node);
    double json_float_get(JSONNode * node);

    const char *json_get_error(void);

#ifdef __cplusplus
}
#endif
#endif
