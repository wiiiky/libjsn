/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lib.c
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

#include "libjsn.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>


#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)


static inline JSONNode *json_node_alloc(JSONType type)
{
    JSONNode *node = (JSONNode *) malloc(sizeof(JSONNode));
    node->type = type;
    node->name = NULL;
    return node;
}

/*
 * Creates JSONNode of different types
 */
JSONNode *json_create_object(void)
{
    return json_node_alloc(JSON_TYPE_OBJECT);
}

JSONNode *json_create_array(void)
{
    return json_node_alloc(JSON_TYPE_ARRAY);
}

JSONNode *json_create_int(int64_t integer)
{
    JSONNode *node = json_node_alloc(JSON_TYPE_INT);
    node->data.integer = integer;
    return node;
}

JSONNode *json_create_float(double floating)
{
    JSONNode *node = json_node_alloc(JSON_TYPE_FLOAT);
    node->data.floating = floating;
    return node;
}

JSONNode *json_create_string(const char *data)
{
    return json_create_string_take(strdup(data));
}

JSONNode *json_create_string_take(char *data)
{
    JSONNode *node = json_node_alloc(JSON_TYPE_STRING);
    node->data.string = data;
    return node;
}

JSONNode *json_create_string_with_len(const char *data, unsigned int len)
{
    JSONNode *node = json_node_alloc(JSON_TYPE_STRING);
    node->data.string = strndup(data, len);
    return node;
}

JSONNode *json_create_true(void)
{
    return json_node_alloc(JSON_TYPE_TRUE);
}

JSONNode *json_create_false(void)
{
    return json_node_alloc(JSON_TYPE_FALSE);
}

JSONNode *json_create_null(void)
{
    return json_node_alloc(JSON_TYPE_NULL);
}

void json_node_set_name(JSONNode * node, const char *name)
{
    free(node->name);
    node->name = strdup(name);
}

void json_node_set_name_take(JSONNode * node, char *name)
{
    free(node->name);
    node->name = name;
}

void json_node_free(JSONNode * node)
{
    if (json_node_is_object(node) || json_node_is_array(node)) {
        j_list_free_full(node->data.children,
                         (JListDestroy) json_node_free);
    } else if (json_node_is_string(node)) {
        free(node->data.string);
    }
    free(node->name);
    free(node);
}


/*
 * 解析一个JSON对象，如果成功返回后续的data指针，否则返回NULL
 * Parses a JSON object from data, returns the pointer after the end of json object on success.
 * otherwise returns NULL
 */
static inline const char *json_object_parse(JSONNode * node,
                                            const char *data);

/*
 * Builds an array from data
 */
static inline const char *json_array_parse(JSONNode * node,
                                           const char *data);

/*
 * json value
 */
static inline const char *json_value_parse(JSONNode ** node,
                                           const char *data);

/*
 * json string
 */
static inline const char *json_string_parse(char **ret, const char *data);

/*
 * json number
 */
static inline const char *json_number_parse(JSONNode ** node,
                                            const char *data);


/*
 * Skips whitespace and cr/lf
 */
static inline const char *json_skip(const char *data)
{
    if (unlikely(data == NULL)) {
        return NULL;
    }
    while (*data && (unsigned char) *data <= 32) {
        data++;
    }
    return data;
}

/*
 * Parses JSON from a string, and returns a JSONNode with type JSON_TYPE_OBJECT you can interrogate
 */
JSONNode *json_loads_from_data(const char *data)
{
    JSONNode *obj = json_create_object();
    data = json_skip(data);
    if ((data = json_object_parse(obj, data)) == NULL) {
        json_node_free(obj);
        return NULL;
    }
    return obj;
}

JSONNode *json_loads_from_file(const char *path)
{
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        return NULL;
    }
    struct stat sbuf;
    if (fstat(fd, &sbuf) < 0) {
        close(fd);
        return NULL;
    }
    unsigned int size = sbuf.st_size;
    char *data =
        (char *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd,
                      0);
    data[size] = '\0';
    close(fd);
    JSONNode *node = json_loads_from_data(data);
    munmap(data, size);
    return node;
}

static inline const char *json_object_parse(JSONNode * node,
                                            const char *data)
{
    if (data == NULL || *data != '{') {
        return NULL;
    }
    data = json_skip(data + 1);
    char c = *data;

    while (c) {
        char *name = NULL;
        if (c == '}') {
            return data + 1;
        } else if (c != '\"') {
            return NULL;
        }
        if ((data = json_string_parse(&name, data)) == NULL) {
            return NULL;
        }
        data = json_skip(data);
        if (*data != ':') {
            free(name);
            return NULL;
        }
        data = json_skip(data + 1);
        JSONNode *child = NULL;
        if ((data = json_value_parse(&child, data)) == NULL) {
            free(name);
            return NULL;
        }
        json_node_set_name_take(child, name);
        json_node_append_child(node, child);
        data = json_skip(data);
        if (*data == ',') {
            data = json_skip(data + 1);
        } else if (*data == '}') {
            return data + 1;
        } else {
            return NULL;
        }
        c = *data;
    }
    return NULL;
}

static inline const char *json_value_parse(JSONNode ** node,
                                           const char *data)
{
    char c = *data;
    if (c == '\"') {            /* string */
        char *value = NULL;
        if ((data = json_string_parse(&value, data)) == NULL) {
            return NULL;
        }
        *node = json_create_string_take(value);
    } else if (c == '[') {      /* array */
        *node = json_create_array();
        if ((data = json_array_parse(*node, data)) == NULL) {
            json_node_free(*node);
            return NULL;
        }
    } else if (c == '{') {      /* object */
        *node = json_create_object();
        if ((data = json_object_parse(*node, data)) == NULL) {
            json_node_free(*node);
            return NULL;
        }
    } else if (c == '-' || isdigit(c)) {    /* number */
        if ((data = json_number_parse(node, data)) == NULL) {
            return NULL;
        }
    } else if (strncmp(data, "true", 4) == 0) {
        *node = json_create_true();
        data += 4;
    } else if (strncmp(data, "false", 5) == 0) {
        *node = json_create_false();
        data += 5;
    } else {
        return NULL;
    }
    return data;
}

static inline const char *json_array_parse(JSONNode * node,
                                           const char *data)
{
    if (unlikely(*data != '[')) {
        return NULL;
    }
    data = json_skip(data + 1);
    while (*data) {
        if (*data == ']') {
            return data + 1;
        }
        JSONNode *child = NULL;
        if ((data = json_value_parse(&child, data)) == NULL) {
            return NULL;
        }
        json_node_append_child(node, child);

        data = json_skip(data);
        if (*data == ',') {
            data = json_skip(data + 1);
        } else if (*data == ']') {
            return data + 1;
        }
    }
    return NULL;
}

/*
 * 解析四个字节表示的16进制数
 */
static inline uint32_t parse_hex4(const char *str)
{
    unsigned int h = 0;
    if (*str >= '0' && *str <= '9')
        h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F')
        h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f')
        h += 10 + (*str) - 'a';
    else
        return 0;
    h = h << 4;
    str++;
    if (*str >= '0' && *str <= '9')
        h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F')
        h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f')
        h += 10 + (*str) - 'a';
    else
        return 0;
    h = h << 4;
    str++;
    if (*str >= '0' && *str <= '9')
        h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F')
        h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f')
        h += 10 + (*str) - 'a';
    else
        return 0;
    h = h << 4;
    str++;
    if (*str >= '0' && *str <= '9')
        h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F')
        h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f')
        h += 10 + (*str) - 'a';
    else
        return 0;
    return h;
}

static const unsigned char firstByteMark[7] =
    { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static inline const char *json_string_parse(char **ret, const char *data)
{
    if (unlikely(*data != '\"')) {
        return NULL;
    }
    char *ptr = (char *) ++data;
    int len = 0;
    while (*ptr != '\"') {
        if (*ptr == '\0') {
            return NULL;
        } else if (*ptr++ == '\\') {
            ptr++;              /* Skip escaped quotes. */
        }
        len++;
    }
    char *out = (char *) malloc(sizeof(char) * len);    /* allocates enough memory for string */
    ptr = out;
    uint32_t uc, uc2;
    while (*data != '\"') {
        if (*data != '\\') {
            *ptr++ = *data++;
        } else {
            data++;
            switch (*data) {
            case 'b':
                *ptr++ = '\b';
                break;
            case 'f':
                *ptr++ = '\f';
                break;
            case 'n':
                *ptr++ = '\n';
                break;
            case 'r':
                *ptr++ = '\r';
                break;
            case 't':
                *ptr++ = '\t';
                break;
            case 'u':          /* UNICODE */
                uc = parse_hex4(data + 1);
                data += 4;      /* gets the unicode char */
                if (uc == 0 || (uc >= 0xDC00 && uc <= 0xDFFF)) {
                    goto ERROR;
                } else if (uc >= 0xD800 && uc <= 0xDBFF) {
                    if (data[1] != '\\' || data[2] != 'u') {
                        goto ERROR; /* second-half is missing */
                    }
                    uc2 = parse_hex4(data + 3);
                    data += 6;
                    if (uc2 < 0xDC00 || uc2 > 0xDFFF) {
                        goto ERROR; /* invalid second-half */
                    }
                    uc = 0x10000 + (((uc & 0x3FF) << 10) | (uc2 & 0x3FF));
                }
                len = 4;
                if (uc < 0x80) {
                    len = 1;
                } else if (uc < 0x800) {
                    len = 2;
                } else if (uc < 0x10000) {
                    len = 3;
                }
                ptr += len;
                switch (len) {
                case 4:
                    *--ptr = ((uc | 0x80) & 0xBF);
                    uc >>= 6;
                case 3:
                    *--ptr = ((uc | 0x80) & 0xBF);
                    uc >>= 6;
                case 2:
                    *--ptr = ((uc | 0x80) & 0xBF);
                    uc >>= 6;
                case 1:
                    *--ptr = (uc | firstByteMark[len]);
                }
                ptr += len;
                data++;
                break;
            default:
                *ptr++ = *data++;
            }
        }
    }
    *ptr = '\0';
    *ret = out;
    return data + 1;
  ERROR:
    free(out);
    return NULL;
}

/*
 * 实现pow10，避免需要加入math.h
 */
static inline double pow_10(int p)
{
    double sum = 1;
    while (p > 0) {
        sum *= 10;
        p--;
    }
    while (p < 0) {
        sum /= 10;
        p++;
    }
    return sum;
}


/*
 * 解析一个整数
 */
static inline const char *json_int_parse(int64_t * integer,
                                         const char *data)
{
    int sign = 1;
    if (*data == '-') {
        sign = -1;
        data++;
    } else if (*data == '+') {
        data++;
    }
    const char *ptr = data;
    int len = 0;
    while (1) {
        if (unlikely(*ptr == '\0')) {
            return NULL;
        } else if (isdigit(*ptr)) {
            ptr++;
            len++;
        } else {
            break;
        }
    }
    int64_t sum = 0;
    while (data < ptr) {
        sum += (*data++ - '0') * pow_10(--len);
    }
    *integer = sign * sum;
    return ptr;
}

static inline const char *json_number_parse(JSONNode ** node,
                                            const char *data)
{
    int sign = 1;
    if (*data == '-') {
        sign = -1;
        data++;
    }
    const char *start = data;
    const char *dot = NULL;
    const char *e = NULL;
    while (*data) {
        if (*data == '.') {
            if (dot != NULL) {  /* 不能出现两个小数点  */
                return NULL;
            }
            dot = data;
        } else if (*data == 'e' || *data == 'E') {
            e = data + 1;
            break;
        } else if (!isdigit(*data)) {
            break;
        }
        data++;
    }
    if (*data == '\0' || data <= start) {
        return NULL;
    }
    if (dot == NULL) {
        dot = data;
    }
    int len = dot - start - 1;
    int64_t integer = 0;
    while (start < dot) {
        integer += (*start++ - '0') * pow_10(len--);
    }
    int64_t floating = 0;
    if (dot < data - 1) {       /* float */
        start = dot + 1;
        len = data - start;
        int j = len - 1;
        while (start < data) {
            floating += (*start - '0') * pow_10(j);
            j--;
            start++;
        }
    }
    double ret = sign * ((double) integer +
                         (double) floating / pow_10(len));
    if (e) {
        int64_t inte;
        if ((data = json_int_parse(&inte, e)) == NULL) {
            return NULL;
        }
        double doublee = pow_10(inte);
        ret *= doublee;
    }
    if ((int64_t) ret != ret) {
        *node = json_create_float(ret);
    } else {
        *node = json_create_int((int64_t) ret);
    }
    return data;
}

void json_node_append_child(JSONNode * node, JSONNode * new)
{
    if (unlikely(!json_node_is_object(node) && !json_node_is_array(node))) {
        return;
    }
    JList *children = node->data.children;
    if (json_node_is_object(node)) {
        while (children) {
            JSONNode *child = j_list_data(children);
            if (strcmp(json_node_get_name(child), json_node_get_name(new))
                == 0) {
                children->data = new;
                json_node_free(child);
                return;
            }
            children = j_list_next(children);
        }
    }
    node->data.children = j_list_append(node->data.children, new);
}

JList *json_node_get_children(JSONNode * node)
{
    if (unlikely(!json_node_is_object(node) && !json_node_is_array(node))) {
        return NULL;
    }
    return node->data.children;
}

/*
 * Gets an element named as name from a json object
 */
JSONNode *json_object_get(JSONNode * node, const char *name)
{
    if (unlikely(!json_node_is_object(node))) {
        return NULL;
    }
    JList *children = node->data.children;
    while (children) {
        JSONNode *child = (JSONNode *) j_list_data(children);
        if (strcmp(name, json_node_get_name(child)) == 0) {
            return child;
        }
        children = j_list_next(children);
    }
    return NULL;
}

/*
 * Gets the string
 */
const char *json_string_get(JSONNode * node)
{
    if (unlikely(!json_node_is_string(node))) {
        return NULL;
    }
    return node->data.string;
}

int64_t json_int_get(JSONNode * node)
{
    if (unlikely(!json_node_is_int(node))) {
        return 0;
    }
    return node->data.integer;
}

double json_float_get(JSONNode * node)
{
    if (unlikely(!json_node_is_float(node))) {
        return 0;
    }
    return node->data.floating;
}
