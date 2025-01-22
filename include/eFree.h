#pragma once

#include <esp_err.h>

#define efree_INITIAL_CAPACITY 3

typedef void (*efree_fn)(void*);

typedef struct {
    void *ptr;
    efree_fn deSTRuctor;
} efree_data;

typedef struct {
    efree_data ** stack;
    unsigned length;
    unsigned capacity;
} eFree;

esp_err_t efree_init(eFree *deSTRoyer);

esp_err_t efree_push(eFree *deSTRoyer, void *ptr, efree_fn deSTRuctor);

void efree_free(eFree *deSTRoyer);
