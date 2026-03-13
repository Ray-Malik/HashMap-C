#include "hash_map.h"
#include <stdlib.h>
#include <string.h>


//input the key, output the bucket in the map it belongs in
unsigned int hash(const char *key, int capacity) {
    unsigned int h = 5381;
    while (*key) {
        h = h * 33 + *key;
        key++;
    }
    return h % capacity;
}

//create the memory for the hashmap and return the pointer to it
HashMap * map_create() {
    HashMap * map = calloc(1, sizeof(HashMap));
    map->buckets = calloc(8, sizeof(Node*));
    map->capacity = 8;

    return map;
}


//insert a Node into the map given it's key and value 
void map_set(HashMap *map, const char *key, void *value) {
    //first get the hash value 
    unsigned int hash_value = hash(key, map->capacity);

    //create a new node, whose memory address is going to be in a bucket
    Node * node = calloc(1, sizeof(Node));

    //set its fields
    //if you do node->key = key, it's risky becasue what if you call free()
    //on the pointer you passed into the parameter after you call this map_set() function?

    node->key = strdup(key);
    //we cant do strdup() on the value parameter because we dont know what
    //type it is. it could be an int, char, or anything else.
    node->value = value;
    //place that node pointer into the bucket that it belongs in
    node->next = map->buckets[hash_value];
    map->buckets[hash_value] = node;

}

//input a key, return a pointer to the value 
//since we dont actually know the type of the value we are returning, so return the generic pointer to it
//and returning a poitner allows the caller to change the value if they want (more flexibility)
void *map_get(HashMap *map, const char *key) {
    //get the index of the value
    unsigned int hash_value = hash(key, map->capacity);
    
    //get the first node at that location
    Node * current_node = map->buckets[hash_value];

    //walk the linked list and check the value of each one
    while (current_node) {
        if (strcmp(current_node->key, key) == 0) {
            return current_node->value;
        } 
        else {
            current_node = current_node->next;
        }
    }
    return NULL;
}

//1. free each nodes key (becasue we used strdup and WE allocated memory for it. the caller allocated memory for the value 
// so no need to free the value pointer.)
//2. free each node itself
//3. free the buckets pointer
//4. free the hashmap pointer itself
void map_free(HashMap *map) {
    //iterate through the buckets array
    for (int i = 0; i < map->capacity; i++) {
        //save the node you are on right now
        Node * current_node = map->buckets[i];
        while (current_node) {
            //save next node
            Node * next_node = current_node->next;
            //free current node and it's key
            free(current_node->key);
            free(current_node);
            //now move to the next node
            current_node = next_node;
        }
    }
    //now free the buckets pointer
    free(map->buckets);
    //free the hashmap
    free(map);
}

//remove the node with the given key
void map_delete(HashMap * map, const char *key) {
    //get the index of the value
    unsigned int hash_value = hash(key, map->capacity);

    //get the first node at that location
    Node * current_node = map->buckets[hash_value];

    //edge case: bucket is empty
    if (current_node == NULL) return;

    
    //initialize a variable that stores the node that is before the current one
    Node * before_node;


    //edge case: the node you want to delete is the first node in the bucket
    if (strcmp(current_node->key, key) == 0) {
            map->buckets[hash_value] = current_node->next;
            //free the memory
            free(current_node->key);
            free(current_node);
            //end loop
            return;
    } 

    //walk the linked list and check the value of each one
    while (current_node) {
        if (strcmp(current_node->key, key) == 0) {
            before_node->next = current_node->next;
            //free the memory
            free(current_node->key);
            free(current_node);
            //stop the loop (the while loop could keep running on garbage memory even after freeing)
            // free() only tells the heap allocator this block is available.
            // the pointer still holds the same address and the data is still there
            // until something else overwrites it. so we must return immediately
            // to avoid accessing that now-invalid memory.
            return;
        } 
        else {
            before_node = current_node;
            current_node = current_node->next;
        }
    }
}