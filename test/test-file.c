#include <libjsn.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    JSONNode *root =json_loads_from_file("./1.json");
    if(root==NULL){
        printf("fail to load 1.json\n");
        return -1;
    }
    JSONNode *hello=json_object_get(root,"hello");
    if(hello==NULL){
        printf("fail to get hello from root\n");
        goto OUT;
    }
    if(json_node_is_int(hello)){
        printf("hello<int>: %ld\n",json_int_get(hello));
    }
    JSONNode *world=json_object_get(root,"world");
    if(world==NULL){
        printf("fail to get world from root\n");
        goto OUT;
    }
    if(json_node_is_array(world)){
        JList *children=json_node_get_children(world);
        printf("world<array>:[");
        while(children){
            JSONNode *child=(JSONNode*)j_list_data(children);
            if(json_node_is_string(child)){
                printf("%s,",json_string_get(child));
            }
            children=j_list_next(children);
        }
        printf("]\n");
    }
    JSONNode *unx=json_object_get(root,"unix");
    if(unx==NULL){
        printf("fail to get unxi from root\n");
        goto OUT;
    }
OUT:
    json_node_free(root);
    return 0;
}
