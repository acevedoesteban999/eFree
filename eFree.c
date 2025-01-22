#include "eFree.h"

esp_err_t efree_init(eFree *efree) {
    if (efree) {
        return ESP_ERR_INVALID_ARG;
    }

    efree->stack = malloc(sizeof(efree_data*) * EFREE_INITIAL_CAPACITY);
    if (!efree->stack) {
        return ESP_ERR_NO_MEM;
    }

    efree->length = 0;
    efree->capacity = EFREE_INITIAL_CAPACITY;
    return ESP_OK;
}

esp_err_t efree_push(eFree *efree, void *ptr, efree_fn free_function) {
    if (!efree || !ptr)
        return ESP_ERR_INVALID_ARG;

    if (efree->length >= efree->capacity) {
        unsigned new_capacity = efree->capacity + EFREE_INITIAL_CAPACITY;
        efree_data **new_items = realloc(efree->stack, sizeof(efree_data*) * new_capacity);
        
        if (!new_items)
            return ESP_ERR_NO_MEM;

        efree->stack = new_items;
        efree->capacity = new_capacity;
    }

    efree_data *item = malloc(sizeof(efree_data));
    if (!item)
        return ESP_ERR_NO_MEM;

    item->ptr = ptr;
    item->free_function = free_function;
    
    efree->stack[efree->length++] = item;
    return ESP_OK;
}

void efree_free(eFree *efree) {
    if (!efree) return;

    for (int i = efree->length - 1; i >= 0; i--) {
        if (efree->stack[i]) {
            if (efree->stack[i]->free_function)
                efree->stack[i]->free_function(efree->stack[i]->ptr);
            else
                free(efree->stack[i]->ptr);
            free(efree->stack[i]);
        }
    }

    free(efree->stack);
    efree->length = 0;
    efree->capacity = 0;
}