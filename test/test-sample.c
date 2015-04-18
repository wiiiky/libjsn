#include <libjsn.h>
#include <stdio.h>
#include <stdlib.h>

const static char *JSON="{\"hello\": \"first line\\nsecond line\\nthird line\",\"yes\":{\"nice\":true, \"array\":[1.1,\"3\",-1.3,-23e2,0.3e+9,-2.1e2,\"\\tyes\\nasndfa\",\"\\u8bf7\\u628a\\u4f60\\u9700\\u8981\\u8f6c\\u6362\\u7684\\u5185\\u5bb9\\u7c98\\u8d34\\u5728\\u8fd9\\u91cc\\u3002\\u005b\\u0074\\u006f\\u006f\\u006c\\u002e\\u0063\\u0068\\u0069\\u006e\\u0061\\u007a\\u002e\\u0063\\u006f\\u006d\\u005d\"]}}";


int main(int argc, char *argv[])
{
    JSONNode *root =
        json_loads_from_data(JSON);
    if (root) {
        printf("success!\n");
        JSONNode *child = json_object_get(root, "hello");
        if (json_node_is_string(child)) {
            printf("hello: %s\n", json_string_get(child));
        } else if (json_node_is_false(child)) {
            printf("hello: FALSE\n");
        } else if (json_node_is_true(child)) {
            printf("hello: TRUE\n");
        }
        child = json_object_get(root, "yes");
        if (json_node_is_object(child)) {
            JSONNode *grandchild = json_object_get(child, "nice");
            if (json_node_is_string(grandchild)) {
                printf("nice: %s\n", json_string_get(grandchild));
            } else if (json_node_is_false(grandchild)) {
                printf("nice: FALSE\n");
            } else if (json_node_is_true(grandchild)) {
                printf("nice: TRUE\n");
            }
            JSONNode *array = json_object_get(child, "array");
            JList *children = json_node_get_children(array);
            while (children) {
                JSONNode *child = (JSONNode *) j_list_data(children);
                if (json_node_is_string(child)) {
                    printf("\t%s\n", json_string_get(child));
                } else if (json_node_is_int(child)) {
                    printf("\t%ld\n", json_int_get(child));
                } else if (json_node_is_float(child)) {
                    printf("\t%f\n", json_float_get(child));
                }
                children = j_list_next(children);
            }
        }
        char *string=json_node_to_string(root);
        printf("\n\n%s\n",string);
        free(string);
        json_node_free(root);
    } else {
        printf("fail!\n");
    }
    return 0;
}
