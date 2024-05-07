#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *tree = malloc(sizeof(TreeMap));
    if (tree == NULL) return NULL;
    
    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;
    
    return tree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    
    TreeNode *newNode  = createTreeNode(key, value);
    if (tree == NULL)
    {
        tree->root = newNode;
        tree->current = tree->root;
    }
    
    tree->current = tree->root;
    while(1)
        {
            if (is_equal(tree, key, tree->current->pair->key))
            {
                break;
            }
            else if (tree->lower_than(key, tree->current->pair->key))
            {
                if(tree->current->left == NULL)
                {
                    tree->current->left = newNode;
                    newNode->left =  NULL;
                    newNode->right =  NULL;
                    newNode->parent = tree->current;
                    break;
                }
                tree->current = tree->current->left;
            }
            else
            {
                if(tree->current->right == NULL)
                    {
                        tree->current->right = newNode;
                        newNode->left =  NULL;
                        newNode->right =  NULL;
                        newNode->parent = tree->current;
                        break;
                    }
                tree->current = tree->current->right;
            }
        }

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    
    while(x->left !=  NULL)
        {
            x = x->left;
        }
    
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode* temp = tree->root;
    TreeNode* parent = NULL;

    Pair *key = node->pair->key;

    // Primero buscamos el nodo a eliminar y su padre
    while(temp != NULL) {
        
        
    }

    if(temp == NULL)  // Si el nodo no se encuentra en el árbol
        return;

    // Caso 1: el nodo es una hoja
    if(temp->left == NULL && temp->right == NULL) {
        if(temp != tree->root) {
            if(parent->left == temp)
                parent->left = NULL;
            else
                parent->right = NULL;
        } else {
            tree->root = NULL;
        }

        free(temp);
    }
    // Caso 2: el nodo tiene un solo hijo
    else if(temp->left == NULL || temp->right == NULL) {
        Tree_Node* child;
        if(temp->left != NULL)
            child = temp->left;
        else
            child = temp->right;

        if(temp != tree->root) {
            if(parent->left == temp)
                parent->left = child;
            else
                parent->right = child;
        } else {
            tree->root = child;
        }

        free(temp);
    }
    // Caso 3: el nodo tiene dos hijos
    else {
        Tree_Node* successor = getMinimumNode(temp->right);
        int succKey = successor->key;
        void* succData = successor->data;
        delete(tree, succKey);  // Eliminamos el sucesor
        temp->key = succKey;
        temp->data = succData;
    }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL) return NULL;
    //if (tree->root->pair->key == key) return tree->root->pair; 
    tree->current = tree->root;
    while (tree->current != NULL)
        {
            if (is_equal(tree, key, tree->current->pair->key))
            {
                return (tree->current->pair);
            }
            else if (tree->lower_than(key, tree->current->pair->key))
            {
                tree->current = tree->current->left;
            }
            else
            {
                 tree->current = tree->current->right;
            }
        }

    
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
