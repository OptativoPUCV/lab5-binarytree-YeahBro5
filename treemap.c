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

    while(temp != NULL && temp->pair->key != key)
        {
            parent = temp;
            if (tree->lower_than(key, temp->pair->key))
            {
                temp = temp->left;
            }
            else{
                temp = temp->right;
            }
        }

    if (temp == NULL)
    {
        return;
    }

    // Caso 1, no tiene hijos

    if (temp->right == NULL && temp->left == NULL)
    {
        if (temp == tree->root)
            {
                tree->root = NULL;
                free(temp);
            }
        else
        {
            if (parent->right == temp)
            {
                parent->right = NULL;
            }
            else
            {
                parent->left = NULL;
            }
        }
    }
    // Caso 2, tiene 1 hijo
    else if (temp->right == NULL || temp->left == NULL)
    {
        TreeNode *reemplazo;
        if (temp->left != NULL)
        {
            reemplazo =  temp->left;
        }
        else
        {
            reemplazo = temp->right;
        }
        ////
        if (temp == tree->root)
        {
            tree->root = reemplazo;
        }
        else if (parent->left == temp)
        {
            parent->left = reemplazo;
        }
        else
        {
            parent->right = reemplazo;
        }
        free(temp);
        reemplazo->parent = parent;
        
    }

    // Caso 3, tiene 2 hijos

    else{
        TreeNode *sucesor = minimum(temp->right);
        int *sucesorKey = sucesor->pair->key;
        void *sucesorData = sucesor->pair->value;
        removeNode(tree, sucesor);
        temp->pair->key = sucesorKey;
        temp->pair->value = sucesorData;
        
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
/*
La función Pair* upperBound(TreeMap* tree, void* key) retorna el Pair con clave igual a key. En caso de no encontrarlo retorna el primer par asociado a una clave mayor o igual a key. Para implementarla puede realizar una búsqueda normal y usar un puntero a nodo auxiliar ub_node que vaya guardando el nodo con la menor clave mayor o igual a key. Finalmente retorne el par del nodo ub_node.
*/

Pair * upperBound(TreeMap * tree, void* key) 
{
    if (tree == NULL) return NULL;

    unsigned short flag = 0; 
    TreeNode *current = tree->root;
    TreeNode *aux = tree->root;
    while (current != NULL)
        {
            if (is_equal(tree, key, current->pair->key))
            {
                aux = current;
                return aux->pair;
            }
            else if (tree->lower_than(key, current->pair->key))
            {
                if (tree->lower_than(current->pair->key, aux->pair->key))
                {
                    aux = current;
                }
                current = current->left;
            }
            else
            {
                 current = current->right;
            }
        }

    
    
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL) return NULL;
    TreeNode *aux = tree->root;
    while (1)
        {
            if (aux->left == NULL)
            {
                return aux->pair;
            }
            else{
                aux = aux->left;
            }
        }
    
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;  

    TreeNode *current = tree->current;

    // caso 1 si hay un subarbol derecho
    if (current->right != NULL) 
    {
        current = minimum(current->right);
        tree->current = current;
        return current->pair;
    }

    // caso 2 no hay derecho
    TreeNode *parent = current->parent;
    while (parent != NULL && current == parent->right) 
    {
        current = parent;
        parent = parent->parent;
    }
    tree->current = parent;
    if (parent == NULL) return NULL;
    else
    {
        return parent->pair;
    }
}
