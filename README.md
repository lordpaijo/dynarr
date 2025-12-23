# DynArr

A simple, generic dynamic array implementation in C.

## Features

- Generic storage for any data type
- Automatic capacity management with exponential growth
- Optional destructor callbacks for element cleanup
- Rich API with common vector operations
- Zero dependencies beyond standard C library

## Building

### Prerequisites

- CMake 3.10 or higher
- A C compiler (GCC, Clang, or MSVC)

### Build Steps

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Install (may require sudo/admin privileges)
sudo cmake --install .
```

### Build Options

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Specify install prefix
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
```

## Usage

### Basic Example

```c
#include <dynarr.h>
#include <stdio.h>

int main(void) {
    DynArr arr;
    
    // Initialize with int elements, no destructor needed
    dynarr_init(&arr, sizeof(int), NULL);
    
    // Push elements
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        dynarr_push(&arr, &values[i]);
    }
    
    // Access elements
    for (size_t i = 0; i < dynarr_size(&arr); i++) {
        int *val = dynarr_get(&arr, i);
        printf("%d ", *val);
    }
    printf("\n");
    
    // Clean up
    dynarr_free(&arr);
    
    return 0;
}
```

### With Custom Destructor

```c
#include <dynarr.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int age;
} Person;

void person_drop(void *elem) {
    Person *p = (Person *)elem;
    free(p->name);
}

int main(void) {
    DynArr people;
    dynarr_init(&people, sizeof(Person), person_drop);
    
    Person p = {
        .name = strdup("Alice"),
        .age = 30
    };
    dynarr_push(&people, &p);
    
    // Destructor automatically called on free
    dynarr_free(&people);
    
    return 0;
}
```

## API Reference

### Initialization & Cleanup

- `dynarr_init(v, elem_size, drop)` - Initialize array
- `dynarr_free(v)` - Free array and call destructors
- `dynarr_clear(v)` - Remove all elements, keep capacity

### Adding/Removing Elements

- `dynarr_push(v, elem)` - Add element to end
- `dynarr_pop(v)` - Remove last element
- `dynarr_insert(v, index, elem)` - Insert at position
- `dynarr_remove(v, index)` - Remove at position

### Access

- `dynarr_get(v, index)` - Get pointer to element
- `dynarr_back(v)` - Get pointer to last element
- `dynarr_size(v)` - Get number of elements
- `dynarr_empty(v)` - Check if empty

### Capacity Management

- `dynarr_reserve(v, new_cap)` - Reserve capacity
- `dynarr_resize(v, new_size)` - Resize array
- `dynarr_shrink_to_fit(v)` - Reduce capacity to fit elements

### Operations

- `dynarr_copy(dest, src)` - Deep copy array
- `dynarr_move(dest, src)` - Move array (O(1))
- `dynarr_swap(v1, v2)` - Swap two arrays
- `dynarr_extend(dest, src)` - Append src to dest
- `dynarr_find(v, elem)` - Linear search by memcmp
- `dynarr_sort(v, cmp)` - Sort with comparator

## Linking

### CMake Project

```cmake
find_package(dynarr REQUIRED)
target_link_libraries(your_target PRIVATE dynarr::dynarr)
```

### Manual Compilation

```bash
gcc your_program.c -ldynarr -o your_program
```

## Notes

- Elements are stored by value (copied via `memcpy`)
- The `drop` callback is optional (pass `NULL` if not needed)
- `dynarr_copy` treats destination as uninitialized
- `dynarr_move` invalidates the source array
- `dynarr_find` uses `memcmp`, so it works for simple types but may not be suitable for complex structures

## License

This project is provided as-is for educational and personal use.
