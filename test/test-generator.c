#include <libjsn.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    JSONNode *root = json_create_object();
    json_object_put_int(root, "id", 123);
    json_object_put_string(root, "name", "wiky");
    json_object_put_string(root, "email", "wiiiky@outlook.com");
    json_object_put_true(root, "male");
    JSONNode *meta = json_create_object();
    json_object_put_null(meta, "meta");
    json_object_put_object(root, "meta", meta);
    JSONNode *array = json_create_array();
    json_object_put_array(meta, "array", array);
    json_array_add_string(array, "hello world");
    json_array_add_string(array, "nice try");
    char *string = json_node_to_string(root);
    printf("%s\n", string);
    free(string);
    json_node_free(root);
    return 0;
}
