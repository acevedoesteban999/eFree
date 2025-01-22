#include "eFree.h"

esp_err_t efree_init(eFree *deSTRoyer) {
    if (deSTRoyer) {
        return ESP_ERR_INVALID_ARG;
    }

    deSTRoyer->stack = malloc(sizeof(efree_data*) * efree_INITIAL_CAPACITY);
    if (!deSTRoyer->stack) {
        return ESP_ERR_NO_MEM;
    }

    deSTRoyer->length = 0;
    deSTRoyer->capacity = efree_INITIAL_CAPACITY;
    return ESP_OK;
}

esp_err_t efree_push(eFree *deSTRoyer, void *ptr, efree_fn deSTRuctor) {
    if (!deSTRoyer || !ptr) {
        return ESP_ERR_INVALID_ARG;
    }

    if (deSTRoyer->length >= deSTRoyer->capacity) {
        unsigned new_capacity = deSTRoyer->capacity + efree_INITIAL_CAPACITY;
        efree_data **new_items = realloc(deSTRoyer->stack, sizeof(efree_data*) * new_capacity);
        
        if (!new_items) {
            return ESP_ERR_NO_MEM;
        }

        deSTRoyer->stack = new_items;
        deSTRoyer->capacity = new_capacity;
    }

    efree_data *item = malloc(sizeof(efree_data));
    if (!item) {
        return ESP_ERR_NO_MEM;
    }

    item->ptr = ptr;
    item->deSTRuctor = deSTRuctor;
    
    deSTRoyer->stack[deSTRoyer->length++] = item;
    return ESP_OK;
}

void efree_free(eFree *deSTRoyer) {
    if (!deSTRoyer) return;

    for (int i = deSTRoyer->length - 1; i >= 0; i--) {
        if (deSTRoyer->stack[i]) {
            if (deSTRoyer->stack[i]->deSTRuctor) {
                deSTRoyer->stack[i]->deSTRuctor(deSTRoyer->stack[i]->ptr);
            } else {
                free(deSTRoyer->stack[i]->ptr);
            }
            free(deSTRoyer->stack[i]);
        }
    }

    free(deSTRoyer->stack);
    deSTRoyer->length = 0;
    deSTRoyer->capacity = 0;
}