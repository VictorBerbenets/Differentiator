#include "include//differentiator.h"
#include "include//Queue.h"

//******************************************************************************************************************************************//
static void SimplifyAdd(Node** node_ptrs, int node_counter);
static void SimplifyMul(Node** node_ptrs, int node_counter);
static void SimplifyDiv(Node** node_ptrs, int node_counter);
static void SimplifyPow(Node** node_ptrs, int node_counter);
static void DeleteInsignificantTreePart(Node* parent, Node* tree_part);
static Node** CollectTreeOperators(Node* tree, int* opers_size);
static void DeleteParentAndChild(Node** parent, Node** child);
static void SetParentConnection(Node** parent, Node** child);
static void LeaveOnlyLeftNode(Node** parent);
static void LeaveOnlyRightNode(Node** parent);
static void CalculateChildes(Node** parent);
//==========================================================================================================================================//

Node* SimplifyTree(Node* tree) {
    if (!tree) {
        return nullptr;
    }
    int opers_size   = 0;
    Node** node_ptrs = CollectTreeOperators(tree, &opers_size);

    int node_counter = 0;
    for (; node_counter < opers_size; ++node_counter) {
        switch (node_ptrs[node_counter]->value.oper) {
            case OP_SUB:
            case OP_ADD:
                SimplifyAdd(node_ptrs, node_counter);
                break;
            case OP_MUL:
                SimplifyMul(node_ptrs, node_counter);
                break;
            case OP_DIV:
                SimplifyDiv(node_ptrs, node_counter);
                break;
            case OP_POW:
                SimplifyPow(node_ptrs, node_counter);
                break;
            default:
                break;
        }
    }
    tree = node_ptrs[node_counter - 1];
    free(node_ptrs);

    return tree;
}

//==========================================================================================================================================//

static void SimplifyAdd(Node** node_ptrs, int node_counter) {
    if (node_ptrs[node_counter]->left_branch->type == NUMBER && node_ptrs[node_counter]->right_branch->type == NUMBER) {
        CalculateChildes(&(node_ptrs[node_counter]));
        return;
    }
    if (node_ptrs[node_counter]->left_branch->type == NUMBER) {
        if (IsEqual(node_ptrs[node_counter]->left_branch->value.number, 0)) {
            LeaveOnlyRightNode(&(node_ptrs[node_counter]));
            return;
        }
    }
    if (node_ptrs[node_counter]->right_branch->type == NUMBER) {
        if (IsEqual(node_ptrs[node_counter]->right_branch->value.number,0)) {
            LeaveOnlyLeftNode(&(node_ptrs[node_counter]));
            return;
        }
    }
}

//==========================================================================================================================================//

static void SimplifyMul(Node** node_ptrs, int node_counter) {
    if (node_ptrs[node_counter]->left_branch->type == NUMBER && node_ptrs[node_counter]->right_branch->type == NUMBER) {
        CalculateChildes(&(node_ptrs[node_counter]));
        return;
    }
    if (node_ptrs[node_counter]->left_branch->type == NUMBER) {
        if (IsEqual(node_ptrs[node_counter]->left_branch->value.number, 0)) {
            if (!node_ptrs[node_counter]->parent) {
                Node* save_node = nullptr;
                save_node = node_ptrs[node_counter]->left_branch;
                DeleteInsignificantTreePart(node_ptrs[node_counter], node_ptrs[node_counter]->right_branch);
                node_ptrs[node_counter] = save_node;
                return;
            }
            SetParentConnection(&node_ptrs[node_counter], &(node_ptrs[node_counter]->left_branch));
            DeleteInsignificantTreePart(node_ptrs[node_counter], node_ptrs[node_counter]->right_branch);
            return;
        }
    }
    if (node_ptrs[node_counter]->right_branch->type == NUMBER) {
        if (IsEqual(node_ptrs[node_counter]->right_branch->value.number, 0)) {
            if (!node_ptrs[node_counter]->parent) {
                Node* save_node = nullptr;
                save_node = node_ptrs[node_counter]->right_branch;
                DeleteInsignificantTreePart(node_ptrs[node_counter], node_ptrs[node_counter]->left_branch);
                node_ptrs[node_counter] = save_node;
                return;
            }
            SetParentConnection(&node_ptrs[node_counter], &(node_ptrs[node_counter]->right_branch));
            DeleteInsignificantTreePart(node_ptrs[node_counter], node_ptrs[node_counter]->left_branch);
            return;
        }
    }
    if (node_ptrs[node_counter]->left_branch->type == NUMBER) {
        if (IsEqual(node_ptrs[node_counter]->left_branch->value.number, 1)) {
            LeaveOnlyRightNode(&(node_ptrs[node_counter]));
            return;
        }
    }
    if (node_ptrs[node_counter]->right_branch->type == NUMBER) {
        if (IsEqual( node_ptrs[node_counter]->right_branch->value.number, 1)) {
            LeaveOnlyLeftNode(&(node_ptrs[node_counter]));
            return;
        }
    }
}

//==========================================================================================================================================//

static void SimplifyDiv(Node** node_ptrs, int node_counter) {
    if (node_ptrs[node_counter]->left_branch->type == NUMBER && node_ptrs[node_counter]->right_branch->type == NUMBER) {
        CalculateChildes(&(node_ptrs[node_counter]));
        return;
    }
    if (node_ptrs[node_counter]->right_branch->type == NUMBER) {
        if (IsEqual(node_ptrs[node_counter]->right_branch->value.number, 1)) {
            if (!node_ptrs[node_counter]->parent) {
                Node* save_node = nullptr;
                save_node = node_ptrs[node_counter]->left_branch;
                LeaveOnlyLeftNode(&(node_ptrs[node_counter]));
                node_ptrs[node_counter] = save_node;
                return;
            }
            SetParentConnection(&node_ptrs[node_counter], &(node_ptrs[node_counter]->right_branch));
            LeaveOnlyLeftNode(&(node_ptrs[node_counter]));
            return;
        }
    }

}

//==========================================================================================================================================//

static void SimplifyPow(Node** node_ptrs, int node_counter) {
    if (node_ptrs[node_counter]->left_branch->type == NUMBER && node_ptrs[node_counter]->right_branch->type == NUMBER) {
        CalculateChildes(&(node_ptrs[node_counter]));
        return;
    }
    if (node_ptrs[node_counter]->right_branch->type == NUMBER) {

        if (IsEqual(node_ptrs[node_counter]->right_branch->value.number, 1)) {
            Node* save_node = nullptr;
            if (!node_ptrs[node_counter]->parent) {
                save_node = node_ptrs[node_counter]->left_branch;
                LeaveOnlyLeftNode(&(node_ptrs[node_counter]));
                node_ptrs[node_counter] = save_node;
                return;
            }

            SetParentConnection(&node_ptrs[node_counter], &(node_ptrs[node_counter]->left_branch));
            save_node = node_ptrs[node_counter]->left_branch;
            LeaveOnlyLeftNode(&(node_ptrs[node_counter]));
            node_ptrs[node_counter] = save_node;
            return;
        }
    }
    if (node_ptrs[node_counter]->right_branch->type == NUMBER) {

        if (IsEqual(node_ptrs[node_counter]->right_branch->value.number, 0)) {
            elem_t finish_value = 1;
            Node* final_node    = CreateNewNode(NUMBER, &finish_value);
            if (!node_ptrs[node_counter]->parent) {
                DeleteInsignificantTreePart(node_ptrs[node_counter], node_ptrs[node_counter]->left_branch);
                node_ptrs[node_counter] = final_node;
                return;
            }
            SetParentConnection(&node_ptrs[node_counter], &final_node);
            DeleteInsignificantTreePart(node_ptrs[node_counter], node_ptrs[node_counter]->left_branch);
            node_ptrs[node_counter] = final_node;
        }
    }
}

//==========================================================================================================================================//

static Node** CollectTreeOperators(Node* tree, int* opers_size) {
    Queue queue = {};
    Queue queue_save = {};
    QueueInit(&queue, QueueInitSize);
    QueueInit(&queue_save, QueueInitSize);
    QueuePush(&queue, tree);
    QueuePush(&queue_save, tree);

    while (queue.size) {
        Node* ptr = QueuePop(&queue);
        if (ptr->left_branch) {
            QueuePush(&queue, ptr->left_branch);
            if (ptr->left_branch->type == OPER) {
                QueuePush(&queue_save, ptr->left_branch);
            }
        }
        if (ptr->right_branch) {
            QueuePush(&queue, ptr->right_branch);
            if (ptr->right_branch->type == OPER) {
                QueuePush(&queue_save, ptr->right_branch);
            }
        }
    }
    *opers_size      = queue_save.size;
    Node** node_ptrs = (Node**)calloc(*opers_size, sizeof(Node*));

    for (int node_counter = *opers_size; node_counter > 0; --node_counter) {
        node_ptrs[node_counter - 1] = QueuePop(&queue_save);
    }
    QueueDtor(&queue);
    QueueDtor(&queue_save);

    return node_ptrs;
}

//==========================================================================================================================================//

static void DeleteInsignificantTreePart(Node* parent, Node* tree_part) {
    DeleteTree(tree_part);
    free(parent);
}

//==========================================================================================================================================//

static void CalculateChildes(Node** parent) {
    elem_t calculated_value = CalculateNumbers(*parent);
    Node* grand_parent = (*parent)->parent;
    Node* new_parent = CreateNewNode(NUMBER, &calculated_value);
    if (!grand_parent) {
        DeleteTree(*parent);
        *parent = new_parent;
        return;
    }
    new_parent->parent = grand_parent;
    if (grand_parent->left_branch == *parent) {
        grand_parent->left_branch = new_parent;
    } else if (grand_parent->right_branch == *parent) {
        grand_parent->right_branch = new_parent;
    }

    free((*parent)->left_branch);
    free((*parent)->right_branch);
    free(*parent);

    *parent = new_parent;
}

//==========================================================================================================================================//

static void LeaveOnlyRightNode(Node** parent) {
    if (!(*parent)->parent) {
        Node* save_child = (*parent)->right_branch;
        DeleteParentAndChild(parent, &((*parent)->left_branch));
        *parent = save_child;
        return;
    }
    SetParentConnection(parent, &(*parent)->right_branch);
    DeleteParentAndChild(&((*parent)), &((*parent)->left_branch));
}

//==========================================================================================================================================//

static void LeaveOnlyLeftNode(Node** parent) {
    if (!(*parent)->parent) {
        Node* save_child = (*parent)->left_branch;
        DeleteParentAndChild(parent, &((*parent)->right_branch));
        *parent = save_child;
        return;
    }
    SetParentConnection(parent, &(*parent)->left_branch);
    DeleteParentAndChild(&((*parent)), &((*parent)->right_branch));
}

//==========================================================================================================================================//

static void DeleteParentAndChild(Node** parent, Node** child) {
    if (*child) {
        free(*child);
        *child = nullptr;
    }
    if (*parent) {
        free(*parent);
        *parent = nullptr;
    }
}

//==========================================================================================================================================//

static void SetParentConnection(Node** parent, Node** child) {
    (*child)->parent = (*parent)->parent;
    if ((*parent)->parent->left_branch == (*parent)) {
        (*parent)->parent->left_branch = *child;
    }
    else if ((*parent)->parent->right_branch == (*parent)) {
        (*parent)->parent->right_branch = *child;
    }
}

//==========================================================================================================================================//
