// ============================================================
// Hash Map - Header
// Structs and function declarations
// ============================================================

#ifndef HASH_MAP_H
#define HASH_MAP_H

// ------------------------------------------------------------
// Node
// One element in a bucket's linked list.
// Holds a key (string), a value (anything), and a pointer
// to the next node in the chain.
//
// Example chain in one bucket:
//   [key="Alice", value=42, next] --> [key="Bob", value=7, next] --> NULL
// ------------------------------------------------------------
typedef struct Node {
    char *key;
    void *value;
    struct Node *next;
} Node;

// ------------------------------------------------------------
// HashMap
// The map itself. Holds an array of bucket pointers and
// tracks how many buckets exist (capacity).
//
// buckets is a pointer to the first slot of an array of Node pointers :
//   buckets
//     |
//     v
//   [ 0x100 | NULL | 0x200 | NULL ]
//      [0]    [1]    [2]     [3]
//       |              |
//     Node           Node
//
// Each slot is either NULL (empty bucket) or points to the
// first Node in that bucket's linked list.
// ------------------------------------------------------------
typedef struct {
    Node **buckets;
    int capacity;
} HashMap;




unsigned int hash(const char *key, int capacity);

HashMap *map_create();

void map_set(HashMap *map, const char *key, void *value);

void *map_get(HashMap *map, const char *key);

void map_free(HashMap *map);

void map_delete(HashMap *map, const char *key);



#endif
