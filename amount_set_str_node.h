#ifndef AMOUNT_SET_STR_NODE_H_
#define AMOUNT_SET_STR_NODE_H_

#include <string.h>
#include <assert.h>
#include <stdlib.h>

/**
 * Node
 *
 * The following functions are available:
 *   createNode         - Allocates a new empty Node
 *   destroyNode        - Deallocates an existing Node. Clears all elements
 *   copyNode           - Creates a copy of existing Node.
 *   updateNextNode     - Updates Target Node's next node pointing.
 *   getNameNode        - Returns the size of the node.
 *   getNextNode        - Returns the next Node which sent Node is pointing to.
 *   getQuantityNode    - Returns the quantity of the node.
 *   updateQuantityNode - Updates Target Node's quantity.
 */

/** Type for defining the node */
typedef struct node_t* Node;

/** Type used for returning error codes from Node functions */
typedef enum{
    NODE_SUCCESS=0,
    NODE_NULL_ARGUMENT,
    NODE_INSUFFICIENT_AMOUNT
}NodeResult;

/**
 * createNode: Allocates a new empty Node.
 *
 * @param new_name - Target name of the new Node.
 *
 * @return
 *     NULL - if one of the parameters is NULL or allocations failed.
 *     A new Node in case of success.
 */
Node createNode(const char *new_name);

/**
 * destroyNode: Deallocates an existing Node. Clears all elements.
 *
 * @param to_destroy - Target Node to be deallocated. If set is NULL nothing will be done.
 */
void destroyNode(Node to_destroy);

/**
 * copyNode: Creates a copy of existing Node.
 *
 * @param old_node - Target Node.
 *
 * @return
 *     NULL if a NULL was sent or a memory allocation failed.
 *     A new Node containing the same quantity and next as Target Node, otherwise.
 */
Node copyNode(Node old_node);

/**
 * updateNextNode: Updates Target Node's next node pointing.
 *
 * @param node_to_update - Target non-NULL Node that need the update.
 * @param next - the node that need to be point on by node_to_update
 *
 */
void updateNextNode(Node node_to_update,Node next);

/**
 * getNameNode: Returns the name of the node.
 *
 * @param node - The node which it's name is requested.
 *
 * @return
 *     NULL - if a NULL was sent as Node
 *     otherwise, the name of the node.
 */
char* getNameNode(Node node);

/**
 * getNextNode: Returns the next Node which sent Node is pointing to.
 *
 * @param node - The Node which it's next Node is requested
 *
 * @return
 *     NULL - if a NULL was sent as Node
 *     otherwise, the Node which the sent Node is pointing to
 *
 *@note the next Node can be NULL if the sent Node was at the end of the linked list for example
 */
Node getNextNode(Node node);

/**
 * getQuantityNode: Returns the quantity of the node.
 *
 * @param node - The node which it's quantity is requested.
 *
  * @return
 *     -1 if a NULL pointer was sent as Node.
 *     Otherwise the quantity of the Node.
 */
double getQuantityNode(Node node);

/**
 * UpdateQuantityNode: Updates Target Node's quantity.
 *
 * @param node - Target Node that need the update.
 * @param num - the number we need to add to node's quantity
 *
 * @return
 *     NODE_NULL_ARGUMENT - if a NULL was sent as node_to_update
 *     NODE_INSUFFICIENT_AMOUNT - if num is negative and absolute bigger than quantity.
 *     NODE_SUCCESS - if node_to_update's next changed successfully
 *
 *@note num can be negative or zero.
 *      if num>0 - increase quantity
 *      if num<0 (and abs smaller than quantity) - decrease quantity
 *      if num=0 - no change to quantity
 */
NodeResult updateQuantityNode(Node node, const double num);

#endif /*AMOUNT_SET_STR_NODE_H_*/
