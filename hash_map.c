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

//search the value of a key in the hashmap. return a pointer to the value 
//becasue we dont actually know the type of it, so return the generic pointer to it
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