#pragma once

#include <esp_err.h>

#define EFREE_INITIAL_CAPACITY 3


#define EFREE_MULTIPLE_PUSH(efree_addr, function_free,...) \
    do { \
        void* data[] = { __VA_ARGS__ }; \
        for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); i++) { \
            efree_push(efree_addr, data[i], (efree_fn)function_free); \
        } \
    } while (0)

typedef void (*efree_fn)(void*);

typedef struct {
    void *ptr;
    efree_fn free_function;
} efree_data;

typedef struct {
    efree_data ** stack;
    unsigned length;
    unsigned capacity;
} eFree;

esp_err_t efree_init(eFree *efree);

esp_err_t efree_push(eFree *efree, void *ptr, efree_fn free_function);

void efree_free(eFree *efree);
