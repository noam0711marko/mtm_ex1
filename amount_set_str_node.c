#include "amount_set_str_node.h"


struct node_t {
    char *name;
    double quantity;
    struct node_t *next;
};

Node createNode(const char *new_name) {
    if(new_name==NULL) {
        return NULL;
    }
    Node new_node = (Node) malloc(sizeof(struct node_t));
    if (new_node == NULL) {
        return NULL;
    }
    int string_len = (int)strlen(new_name);
    char *product_name = (char *)malloc(sizeof(char) * (string_len + 1));
    if (product_name == NULL) {
        free(new_node);
        return NULL;
    }
    strcpy(product_name,new_name);
    new_node->name=product_name;
    new_node->quantity=0;
    new_node->next=NULL;
    return new_node;
}

void destroyNode(Node to_destroy)
{
    if(to_destroy==NULL) {
        return;
    }
    free(to_destroy->name);
    free(to_destroy);
}

Node copyNode(Node old_node)
{
    if(old_node==NULL){
        return NULL;
    }
    Node copy_of_node = createNode(old_node->name);
    if (copy_of_node != NULL){
        assert(old_node->quantity>=0);
        copy_of_node->quantity= old_node->quantity;
        copy_of_node->next=old_node->next;
    }
    return copy_of_node;
}

void updateNextNode(Node node_to_update,Node next)
{
    assert(node_to_update!=NULL);
    node_to_update->next=next;
}

char* getNameNode(Node node){
    if(node==NULL){
        return NULL;
    }
    return node->name;
}

Node getNextNode(Node node){
    if(node==NULL){
        return NULL;
    }
    return node->next;
}

double getQuantityNode(Node node){
    if(node==NULL){
        return -1;
    }
    return node->quantity;
}

NodeResult updateQuantityNode(Node node, const double num){
    if(node==NULL){
        return NODE_NULL_ARGUMENT;
    }
    NodeResult res=NODE_SUCCESS;
    if (((node->quantity)+num)<0){
        return NODE_INSUFFICIENT_AMOUNT;
    }
    else{
        node->quantity+=num;
    }
    return res;
}

