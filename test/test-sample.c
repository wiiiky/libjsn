#include <libjsn.h>
#include <stdio.h>
#include <stdlib.h>

const static char *JSON = "{\"name\":\"wiky\",\"male\":true,\"age\":123,"
    "\"title\":[\"developer\",\"freeman\"],\"height\":1.76,\"university\":"
    "{\"province\":\"Shanghai\",\"area\":\"Baoshan\",\"name\":\"Shanghai University\"}}";


static inline void tab_print(unsigned int tab)
{
    int i;
    for (i = 0; i < tab; i++) {
        printf("\t");
    }
}

static inline void json_print(JSONNode * node, unsigned int tab)
{
    const char *name = json_node_get_name(node);
    if (name == NULL) {
        name = "NULL";
    }
    tab_print(tab);
    if (json_node_is_string(node)) {
        printf("%s=%s\n", name, json_string_get(node));
    } else if (json_node_is_false(node)) {
        printf("%s=FALSE\n", name);
    } else if (json_node_is_true(node)) {
        printf("%s=TRUE\n", name);
    } else if (json_node_is_int(node)) {
        printf("%s=%ld\n", name, json_int_get(node));
    } else if (json_node_is_float(node)) {
        printf("%s=%g\n", name, json_float_get(node));
    } else if (json_node_is_object(node) || json_node_is_array(node)) {
        JList *children = json_node_get_children(node);
        if (json_node_is_array(node)) {
            printf("%s=[\n", name);
        } else {
            printf("%s={\n", name);
        }
        while (children) {
            JSONNode *child = (JSONNode *) j_list_data(children);
            json_print(child, tab + 1);
            children = j_list_next(children);
        }
        tab_print(tab);
        if (json_node_is_array(node)) {
            printf("]\n");
        } else {
            printf("}\n");
        }
    } else {
        printf("%s=NULL\n", name);
    }
}

int main(int argc, char *argv[])
{
    JSONNode *root = json_loads_from_data(JSON);
    if (root == NULL) {
        printf("invalid format!\n");
        return -1;
    }
    json_print(root, 0);
    json_node_free(root);
    return 0;
}
