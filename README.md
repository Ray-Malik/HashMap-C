# Hash Map in C

A hash map implementation written from scratch in C. Built as part of a systems programming learning path.

## Features

- String keys, `void *` values (store any type)
- djb2 hash function
- Collision handling via chaining (linked list per bucket)
- Full memory management — zero leaks confirmed by Valgrind

## API

```c
HashMap *map_create();                                    // allocate and return a new map
void     map_set(HashMap *map, const char *key, void *value); // insert or overwrite a key
void    *map_get(HashMap *map, const char *key);          // return pointer to value, or NULL
void     map_delete(HashMap *map, const char *key);       // remove a key
void     map_free(HashMap *map);                          // free all memory
```

## How It Works

The map holds an array of buckets. Each key is hashed to an index in that array. If two keys hash to the same bucket (a collision), they form a linked list at that bucket.

```
buckets
  |
  v
[ 0x100 | NULL | 0x200 | NULL | ... ]
   [0]    [1]    [2]     [3]
    |              |
 ["Alice"      ["Carol"
  age=30]       age=42]
```

## Build and Run

```bash
gcc *.c -o main
./main
```

## Memory Check (requires Valgrind on Linux/WSL)

```bash
gcc *.c -o main
valgrind --leak-check=full ./main
```

Expected output:
```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

## Files

| File | Description |
|------|-------------|
| `hash_map.h` | Struct definitions and function declarations |
| `hash_map.c` | Full implementation |
| `main.c` | Demo and tests |