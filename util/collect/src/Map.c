#include <Map.h>
#include <List.h>
#include <stdlib.h>

typedef enum{ RED, BLACK} Color;

typedef struct Node{
    const void* key;
    void* value;
    Color color;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

struct TreeMap{
    bool(*key_cmp_fn)(const void*,const void*);
    void(*value_dealloc)(void*,void*);
    void(*key_dealloc)(void*,void*);
    void* alloc_data;
    Node* root;
    LinkedList* serialNodes;
};

void free_dtor(void* alloc_data,void* value){
    free(value);
}

static Node* map_getParent(Node* node){
    return !node?NULL:node->parent;
}

static Node* map_getGrandparent(Node* node){
    return map_getParent(map_getParent(node));
}

static Node* map_getSibling(Node* node){
    Node* parent = map_getParent(node);
    
    if(!parent)
        return NULL;
    else if(parent->left==node)
        return parent->right;
    else
        return parent->left;
}

static Node* map_getUncle(Node* node){
    return map_getSibling(map_getParent(node));
}

static void map_rotateLeft(Node* node){
    Node* nnew = node->right;
    Node* parent = node->parent;
    
    node->right = nnew->left;
    nnew->left = node;
    node->parent = nnew;
    if(node->right)
        node->right->parent = node;
    
    if(parent){
        if(node == parent->left)
            parent->left = nnew;
        else
            parent->right = nnew;
	}
    nnew->parent = parent;
}
static void map_rotateRight(Node* node){
    Node* nnew = node->left;
    Node* parent = node->parent;
    
    node->left = nnew->right;
    nnew->right = node;
    node->parent = nnew;
    if(node->left)
        node->left->parent = node;
    
    if(parent){
        if(node == parent->left)
            parent->left = nnew;
        else
            parent->right = nnew;
    }
    nnew->parent = parent;
}

static void map_repair(Node* node){
    if(!node->parent)
        node->color = BLACK;
    else if(node->parent->color == BLACK)
        return;
    else if(map_getUncle(node) && map_getUncle(node)->color == RED){
        node->parent->color = BLACK;
        map_getUncle(node)->color = BLACK;
        map_getGrandparent(node)->color = RED;
        map_repair(map_getGrandparent(node));
    }else{
        {
            Node* parent = map_getParent(node);
            Node* grandparent = map_getGrandparent(node);
            
            if(node==parent->right && parent == grandparent->left){
                map_rotateLeft(parent);
                node = node->left;
            }else if(node==parent->left && parent == grandparent->right){
                map_rotateRight(parent);
                node = node->right;
            }
        }
        {
           Node* parent = map_getParent(node);
           Node* grandparent = map_getGrandparent(node);
           
           if(node == parent->left)
            map_rotateRight(grandparent);
           else
            map_rotateLeft(grandparent);
           parent->color = BLACK;
           grandparent->color = RED;
        }
    }
}


void map_put(TreeMap* map,const void* key,void* value){
    Node* node = map->root;
    if(!node){
        Node* nnode = (Node*)malloc(sizeof(Node));
        nnode->key = key;
        nnode->value = value;
        nnode->parent = NULL;
        nnode->left = NULL;
        nnode->right = NULL;
        nnode->color = BLACK;
	    map->root = nnode;
	    map->serialNodes = LinkedList_new(NULL);
	    LinkedList_pushBack(map->serialNodes,nnode);
        return;
    }
    while(true){
        if(map->key_cmp_fn(key,node->key)){
            if(node->left)
                node = node->left;
            else
                break;
        }else if(map->key_cmp_fn(node->key,key)){
            if(node->right)
                node = node->right;
            else
                break;
        }else{
            map->value_dealloc(map->alloc_data,node->value);
            node->value = value;
            return;
        }
    }
    Node* nnode = (Node*)malloc(sizeof(Node));
    nnode->key = key;
    nnode->value = value;
    nnode->parent = node;
    if(map->key_cmp_fn(key,node->key))
        node->left = nnode;
    else
        node->right = nnode;
    nnode->left = NULL;
    nnode->right = NULL;
    nnode->color = RED;
    map_repair(nnode);
    LinkedList_pushBack(map->serialNodes,nnode);
}

void* map_get(TreeMap* map,const void* key){
	Node* node = map->root;
	while(node){
		if(map->key_cmp_fn(key,node->key))
			node = node->left;
		else if(map->key_cmp_fn(node->key,key))
			node = node->right;
		else
			return node->value;
	}
	return NULL;
}

TreeMap* map_new(void* data,bool(*cmpFn)(const void*,const void*),void(*value_dtor)(void*,void*),void(*key_dtor)(void*,void*)){
    TreeMap* map = malloc(sizeof(TreeMap));
    map->root = NULL;
    map->key_cmp_fn = cmpFn;
    map->value_dealloc = value_dtor;
    map->key_dealloc = key_dtor;
    map->alloc_data = data;
    map->serialNodes = NULL;
    return map;
}

static void node_free(Node* node,TreeMap* map){
    if(node){
        node_free(node->left,map);
        node_free(node->right,map);
        if(node->value)
            map->value_dealloc(map->alloc_data,node->value);
        if(map->key_dealloc)
            map->key_dealloc(map->alloc_data,(void*)node->key);
        free(node);
    }
}

void map_free(TreeMap* map){
    node_free(map->root,map);
    if(map->serialNodes)
       LinkedList_free(map->serialNodes);
    free(map);
}

MapIterator* map_begin(TreeMap* map){
    return map->serialNodes!=NULL?(MapIterator*)LinkedList_begin(map->serialNodes):NULL;
}
MapIterator* map_next(MapIterator* it){
    return (MapIterator*)LinkedList_next((Iterator*)it);
}
const void* map_deref_it(MapIterator* it){
    return ((Node*)LinkedList_dereference((Iterator*)it))->key;
}

void* map_deref_it_value(MapIterator* it){
    return ((Node*)LinkedList_dereference((Iterator*)it))->value;
}


