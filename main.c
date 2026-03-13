#include <stdio.h>
#include "hash_map.h"

int main() {
    HashMap *map = map_create();

    // store some integers
    int age1 = 30;
    int age2 = 25;
    int age3 = 42;
    int age4 = 55;
    int age5 = 19;

    map_set(map, "Alice", &age1);
    map_set(map, "Bob",   &age2);
    map_set(map, "Carol", &age3);
    map_set(map, "Eve",   &age4);
    map_set(map, "Frank", &age5);

    // retrieve and print them
    char *names[] = {"Alice", "Bob", "Carol", "Dave"};
    for (int i = 0; i < 4; i++) {
        int *age = map_get(map, names[i]);
        if (age) {
            printf("%s -> %d\n", names[i], *age);
        } else {
            printf("%s -> NOT FOUND\n", names[i]);
        }
    }

    map_delete(map, "Bob");
    printf("\nAfter deleting Bob:\n");

    // print the internal bucket structure visually
    printf("\n--- Bucket Structure ---\n");
    for (int i = 0; i < map->capacity; i++) {
        printf("bucket[%d] -> ", i);
        Node *node = map->buckets[i];
        if (node == NULL) {
            printf("NULL");
        }
        while (node) {
            printf("[\"%s\": %d]", node->key, *((int *)(node->value)));
            if (node->next) printf(" -> ");
            node = node->next;
        }
        printf("\n");
    }

    map_free(map);
    return 0;
}
