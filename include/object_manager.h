#ifndef SDS_OBJECT_MANAGER_H
#define SDS_OBJECT_MANAGER_H

#include <stddef.h>
#include <macros.h>
#include <dynamic_array.h>

// sds_sm_da
// space managed dynamic array
struct sds_sm_da
{
    sds_da_index_t *queue_arr;
    void *data_arr;
    size_t type_size;
};
 
void sds_sm_da_create(struct sds_sm_da *create, size_t type_size, sds_da_index_t data_initial_capacity, sds_da_index_t queue_initial_capacity);
void sds_sm_da_free(struct sds_sm_da *sm);

sds_da_index_t sds_sm_da_add(struct sds_sm_da *sm);
void sds_sm_da_delete(struct sds_sm_da *sm, sds_da_index_t index);

void *sds_sm_da_get(struct sds_sm_da *sm, sds_da_index_t index);
// sds_id_da
// id indexed dynamic array
struct sds_id_da
{
    struct sds_sm_da sm;
    sds_da_index_t *id_arr;
    void *data_arr;
    size_t type_size;
};
void sds_id_da_create(struct sds_id_da *create, size_t type_size, sds_da_index_t data_initial_capacity, sds_da_index_t id_initial_capacity, sds_da_index_t queue_initial_capacity);
void sds_id_da_free(struct sds_id_da *id_da);

sds_da_index_t sds_id_da_add(struct sds_id_da *id_da);
void sds_id_da_delete(struct sds_id_da *id_da, sds_da_index_t id);

void *sds_id_da_get(struct sds_id_da *id_da, sds_da_index_t id);
// sds_ecm
// entity component manager
typedef sds_da_index_t sds_om_object_id_t;
typedef sds_da_index_t sds_om_object_container_id_t;

void sds_ecm_create(struct sds_id_da *create, sds_da_index_t idarr_initial_capacity, sds_da_index_t id_initial_capacity, sds_da_index_t queue_initial_capacity);
void sds_ecm_free(struct sds_id_da *ecm);

sds_om_object_container_id_t sds_ecm_container_add(struct sds_id_da *ecm, size_t type_size, sds_da_index_t data_initial_capacity, sds_da_index_t id_initial_capacity, sds_da_index_t index_initial_capacity);
void sds_ecm_container_delete(struct sds_id_da *ecm, sds_om_object_container_id_t container_id);

sds_om_object_id_t sds_ecm_component_add(struct sds_id_da *ecm, sds_om_object_container_id_t container_id);
void sds_ecm_component_delete(struct sds_id_da *ecm, sds_om_object_container_id_t container_id, sds_om_object_id_t object_id);

void *sds_ecm_component_get(struct sds_id_da *ecm, sds_om_object_container_id_t container_id, sds_om_object_id_t object_id);
struct sds_id_da *sds_ecm_container_get(struct sds_id_da *ecm, sds_om_object_container_id_t container_id);
#endif