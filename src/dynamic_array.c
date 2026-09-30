#include <stdlib.h>
#include <dynamic_array.h>

void *sds_da_create(size_t type_size, sds_da_index_t initial_capacity)
{
    struct sds_da_header *header = malloc(sizeof(struct sds_da_header) + (type_size * initial_capacity));
    header->capacity = initial_capacity;
    header->size = 0;
    return (void *)(header + 1);
}
sds_da_index_t sds_da_add(void **data, size_t type_size)
{
    struct sds_da_header *header = sds_da_get_header(*data);
    
    if (header->size >= header->capacity)
    {
        header->capacity = (header->capacity == 0) ? 1 : (header->capacity * SDS_DA_REALLOCATE_CAPACITY_MULTIPLIER);
        data = sds_da_reallocate(*data, type_size, header->capacity);
    }
    return header->size++;
}
sds_da_index_t sds_da_sync_add(void **data, struct sds_da_header *header, size_t type_size)
{
    if (header->size >= header->capacity)
    {
        data = sds_da_sync_reallocate(*data, type_size, ((header->capacity == 0) ? 1 : (header->capacity * SDS_DA_REALLOCATE_CAPACITY_MULTIPLIER)));
    }
    return header->size;
}