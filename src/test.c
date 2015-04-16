#include "libjsn.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
    JSONNode *root =
        json_loads_from_data
        ("{\"hello\": false,\"yes\":{\"nice\":true, \"array\":[1.1,\"3\",-1.3,-23]}}");
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
        json_node_free(root);
    } else {
        printf("fail!\n");
    }
    return 0;
}
