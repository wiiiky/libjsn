#include <libjsn.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    const char *path=PACKAGE_TEST_DIR "/1.json";
    JSONNode *root = json_loads_from_file(path);
    if(root==NULL){
        printf("fail to load %s\n",path);
        return -1;
    }
    char *string=json_node_to_string(root);
    printf("%s\n",string);
    free(string);
    json_node_free(root);
    return 0;
}
