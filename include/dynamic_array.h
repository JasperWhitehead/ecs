#ifndef SDS_DYNAMIC_ARRAY_H
#define SDS_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdlib.h>

#define SDS_DA_REALLOCATE_CAPACITY_MULTIPLIER 2

// dynamic array
typedef size_t sds_da_index_t;

struct sds_da_header
{
    sds_da_index_t capacity;
    sds_da_index_t size;
};

void *sds_da_create(size_t type_size, sds_da_index_t initial_capacity);
sds_da_index_t sds_da_add(void **data, size_t type_size);

sds_da_index_t sds_da_sync_add(void **data, struct sds_da_header *header, size_t type_size);

static inline struct sds_da_header *sds_da_get_header(void *data)
{
    return (((struct sds_da_header *)data) - 1);
}
static inline void *sds_da_sync_create(size_t type_size, sds_da_index_t initial_capacity)
{
    return malloc(type_size * initial_capacity);
}
static inline void *sds_da_reallocate(void *data, size_t type_size, sds_da_index_t new_capacity)
{
    return realloc(sds_da_get_header(data), sizeof(struct sds_da_header) + (type_size * new_capacity));
}
static inline void *sds_da_sync_reallocate(void *data, size_t type_size, sds_da_index_t new_capacity)
{
    return realloc(data, (type_size * new_capacity));
}
static inline void sds_da_sync_free(void *data)
{
    free(data);
}
static inline void sds_da_free(void *data)
{
    free(sds_da_get_header(data));
}
static inline void *sds_da_get(void *data, size_t type_size, sds_da_index_t index)
{
    return ((void *)(((unsigned char *)data) + (type_size * index)));
}
static inline void sds_da_pop(void *data)
{
    sds_da_get_header(data)->size--;
}
static inline void jwcilb_da_clear(void *data)
{
    sds_da_get_header(data)->size = 0;
}
static inline sds_da_index_t sds_da_get_size(void *data)
{
    return sds_da_get_header(data)->size;
}
static inline sds_da_index_t sds_da_get_capacity(void *data)
{
    return sds_da_get_header(data)->capacity;
}
#endif