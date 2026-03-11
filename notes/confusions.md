# Confusions & Hard Topics — Hash Map Project

A running log of things that were confusing, with clear explanations.
Added to as the project progresses.

---

## 1. `typedef struct` syntax

**Confusion:** Why is there a `typedef`? Why does `Node` appear twice?

```c
typedef struct Node {
    char *key;
    void *value;
    struct Node *next;
} Node;
```

**Explanation:**
- `typedef` creates an alias for a type so you don't have to keep writing `struct Node` everywhere — just `Node`.
- The first `Node` (in `struct Node`) is the struct's internal name — needed so the struct can reference itself inside its own body.
- The second `Node` (after the closing brace) is the alias being created by `typedef`.
- They happen to be the same word by convention, but they serve different purposes.
- Without `typedef` you'd have to write `struct Node *next` everywhere. With it, just `Node *next`.

**Two ways to write the same thing:**
```c
// Option 1: combined (common)
typedef struct Node { ... } Node;

// Option 2: separate (also valid)
struct Node { ... };
typedef struct Node Node;
```

---

## 2. Why `struct Node *next` inside the typedef

**Confusion:** I thought after typedef you don't need to write `struct` anymore?

**Explanation:**
The alias `Node` doesn't exist until the compiler hits the closing `} Node;` line.
Inside the struct body, the alias hasn't been created yet — so you're forced to use
the full `struct Node *next`. After the typedef is complete, everywhere else in your
code you can just write `Node`.

---

## 3. `char *key` — why a pointer, not a value?

**Confusion:** Isn't `char *key` pointing to a whole string? Why not store the string directly?

**Explanation:**
Strings in C are not built-in value types like `int`. A string is just bytes in memory:
`'A'`, `'l'`, `'i'`, `'c'`, `'e'`, `'\0'` sitting somewhere in RAM one after another.

`char *key` stores the address of the first character. C knows to keep reading
until it hits `'\0'` (null terminator). So yes — the pointer literally points to one
`char`, but that's all you need to find the whole string.

Why not `char key[50]`? Because then every key is capped at 50 characters and every
node wastes space even for short keys. A pointer lets the key be any length.

---

## 4. `void *` vs `void`

**Confusion:** Does `void` always mean "unknown type"?

**Explanation:** No — same word, two completely different meanings:

```c
void print_hello();   // returns NOTHING — not unknown, literally nothing
void *get_data();     // returns a pointer to something of UNKNOWN type
```

- `void` alone = function returns nothing
- `void *` = pointer to an unknown type (generic pointer)

---

## 5. `void *` — type enforcement

**Confusion:** What stops someone from mixing types (store a char for one key, int for another)?

**Explanation:** Nothing. `void *` is a generic pointer — the map just stores addresses
and has no idea what's behind them. If you retrieve the wrong type and cast it wrong,
you get garbage or a crash. C trusts you completely.

To enforce a specific type, change `void *value` to `int value` — then the compiler
enforces it. Tradeoff: flexibility vs safety.

---

## 6. `typedef` for non-structs

**Confusion:** Can `typedef` be used for things other than structs?

**Explanation:** Yes — `typedef` works for any type:

```c
typedef unsigned long long uint64;   // uint64 now means unsigned long long
typedef char* string;                // string now means char*
typedef int* int_ptr;                // int_ptr now means int*
```

The standard library uses this for `uint8_t`, `uint32_t`, `int64_t` etc. in `stdint.h`.

---

## 7. Stack vs Heap — when do variables die?

**Confusion:** As long as something points to a variable, doesn't it stay alive?

**Explanation:** NO — that's Python/JavaScript behavior (garbage collection). C has none of that.

- **Stack** → lives until the function returns. Period. Even if something still points to it.
- **Heap** → lives until you call `free()`. Period.

When a function returns, its stack frame is reclaimed immediately. Any pointer still
pointing at it becomes a **dangling pointer** — holds an address that's no longer valid.
Reading from it is undefined behavior (garbage, crash, or silently wrong).

```
Stack variable:   int x = 5;           // dies when function returns
Heap variable:    int *x = malloc(4);  // lives until free(x)
```

---

## 8. Stack allocation without initialization = garbage

**Confusion:** Does `Node first_node;` initialize the struct?

**Explanation:** No. It only reserves space on the stack. All fields contain whatever
random bytes happened to be in that memory location before. Reading them is undefined behavior.

Safe options:
```c
Node first_node = {NULL, NULL, NULL};  // stack, initialized
Node *first_node = calloc(1, sizeof(Node));  // heap, zeroed by calloc
```

`calloc` always zeros the memory. `malloc` does not.

---

## 9. `Node *first_node = calloc(1, sizeof(Node))` — mental model

**Confusion:** What is actually happening in memory here?

**Explanation:**
```
STACK                    HEAP
─────────────────        ──────────────────────
first_node: 0x4A20  -->  [ key | value | next ]
(pointer variable)       (actual Node struct)
```

- `first_node` lives on the stack — it's just a pointer variable holding an address.
- The actual Node data lives on the heap at that address.
- When the function returns, `first_node` (the stack variable) dies — but the heap memory at `0x4A20` stays alive until you call `free()`.
- To keep access to it, return the pointer or store it somewhere before the function ends.

---

## 10. `Node **buckets` — double pointer

**Confusion:** Why two stars? What does each level mean?

**Explanation:**
`buckets` points to an array where each element is itself a pointer to a Node.
- One star: "it's a pointer"
- Second star: "what it points to is also a pointer"

```
buckets         →  address of buckets[0]       (Node** — pointer to a Node pointer)
*buckets        →  value of buckets[0]          (Node* — address of first Node, or NULL)
**buckets       →  the actual Node struct        (no more pointers)
```

Visual:
```
buckets
  |
  v
[ 0x100 | NULL | 0x200 | NULL ]    ← array of Node pointers (on heap)
   [0]    [1]    [2]     [3]
    |              |
    v              v
  Node           Node               ← actual Node structs (on heap)
```

---

## 11. `->` operator

**Confusion:** When do you use `->` vs `.` to access struct fields?

**Explanation:**
- `.` is used when you have the struct value directly: `node.key`
- `->` is used when you have a pointer to the struct: `node_ptr->key`

`->` is just shorthand for dereferencing then accessing:
```c
node_ptr->key       // same as:
(*node_ptr).key
```

Since we almost always work with pointers to structs in C, `->` is what you'll use most.

---

## 12. Array indexing is O(1) — not a scan

**Confusion:** Does `array[99]` scan from index 0 to 99?

**Explanation:** No — it's instant. Pure arithmetic:
```
address = base_address + (index × sizeof(element))
```
The CPU goes directly to that address. No loop, no scan, no searching.

This is why hash maps are useful — they convert string keys into numbers (indexes)
so you can use this same O(1) trick with arbitrary keys, not just integers.

---

## 13. NULL

**Confusion:** Is `None` available in C?

**Explanation:** No. C equivalents:
- `NULL` — a pointer that points to nothing (address 0)
- `0` — integer with no value
- `'\0'` — end of a string

Dereferencing NULL causes a **segfault** — the OS kills your program because you're
trying to read memory at address 0 which you don't own. Always check `ptr != NULL`
before dereferencing.

---

## 14. Full memory layout of the HashMap

```
STACK              HEAP                         HEAP
──────────         ────────────────             ──────────────────────────────────
map_ptr  ───────►  [ buckets | capacity ]
                       |
                       v
                   [ 0x100 | NULL | NULL | NULL ]   ← buckets array
                      [0]    [1]    [2]    [3]
                       |
                       v
                   [ key | value | next ] ──────►  [ key | value | next ] ──► NULL
                     (Node)                          (Node)
```

Three separate heap allocations:
1. The HashMap struct itself
2. The buckets array (array of Node pointers)
3. Each Node (allocated individually when inserted)
