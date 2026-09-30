#include <stdlib.h>
#include <string.h>
#include <dynamic_array.h>
#include <object_manager.h>

// sds_sm_da
void sds_sm_da_create(struct sds_sm_da *create, size_t type_size, sds_da_index_t data_initial_capacity, sds_da_index_t queue_initial_capacity)
{
    create->type_size = type_size;
    create->data_arr = sds_da_create(type_size, data_initial_capacity);
    create->queue_arr = sds_da_create(sizeof(sds_da_index_t), queue_initial_capacity);
}
void sds_sm_da_free(struct sds_sm_da *sm)
{
    sds_da_free(sm->data_arr);
    sds_da_free(sm->queue_arr);
}
sds_da_index_t sds_sm_da_add(struct sds_sm_da *sm)
{
    if (sds_da_get_size(sm->queue_arr) == 0)
    {
        return sds_da_add((void **)(&sm->data_arr), sm->type_size);
    }
    else
    {
        sds_da_pop(sm->queue_arr);
        return (sm->queue_arr[sds_da_get_size(sm->queue_arr)]);
    }
}
void sds_sm_da_delete(struct sds_sm_da *sm, sds_da_index_t index)
{
    sm->queue_arr[sds_da_add((void **)(&(sm->queue_arr)), sizeof(sds_da_index_t))] = index;
}
void *sds_sm_da_get(struct sds_sm_da *sm, sds_da_index_t index)
{
    return (sds_da_get(sm->data_arr, sm->type_size, index));
}
// sds_id_da
void sds_id_da_create(struct sds_id_da *create, size_t type_size, sds_da_index_t data_initial_capacity, sds_da_index_t id_initial_capacity, sds_da_index_t queue_initial_capacity)
{
    sds_sm_da_create(&create->sm, sizeof(sds_da_index_t), id_initial_capacity, queue_initial_capacity);
    create->type_size = type_size;
    create->data_arr = sds_da_create(type_size, data_initial_capacity);
    create->id_arr = sds_da_sync_create(sizeof(sds_da_index_t), data_initial_capacity);
}
void sds_id_da_free(struct sds_id_da *id_da)
{
    sds_sm_da_free(&(id_da->sm));
    sds_da_free(&(id_da->data_arr));
    sds_da_sync_free(&(id_da->id_arr));
}
sds_da_index_t sds_id_da_add(struct sds_id_da *id_da)
{
    sds_da_index_t newid = sds_sm_da_add(&(id_da->sm));
    ((sds_da_index_t *)id_da->sm.data_arr)[newid] = sds_da_sync_add((void **)(&(id_da->id_arr)), sds_da_get_header(id_da->data_arr), sizeof(sds_da_index_t));
    id_da->id_arr[sds_da_add((&id_da->data_arr), id_da->type_size)] = newid;
    return newid;
}
void sds_id_da_delete(struct sds_id_da *id_da, sds_da_index_t id)
{
    sds_da_index_t delete_index = ((sds_da_index_t *)id_da->sm.data_arr)[id];
    sds_da_index_t last_index = sds_da_get_size(id_da->data_arr) - 1;
    sds_da_index_t last_id = id_da->id_arr[last_index];
    if (!(delete_index == last_index))
    {
        memcpy(sds_da_get(id_da->data_arr, id_da->type_size, delete_index), sds_da_get(id_da->data_arr, id_da->type_size, last_index), id_da->type_size);
        id_da->id_arr[delete_index] = last_id;
        ((sds_da_index_t *)id_da->sm.data_arr)[last_id] = delete_index;
    }
    sds_sm_da_delete(&(id_da->sm), id);
    sds_da_pop(id_da->data_arr);
}
void *sds_id_da_get(struct sds_id_da *id_da, sds_da_index_t id)
{
    return sds_da_get(id_da->data_arr, id_da->type_size, ((sds_da_index_t *)id_da->sm.data_arr)[id]);
}
// sds_ecm
void sds_ecm_create(struct sds_id_da *create, sds_da_index_t idarr_initial_capacity, sds_da_index_t id_initial_capacity, sds_da_index_t queue_initial_capacity)
{
    sds_id_da_create(create, sizeof(struct sds_id_da), idarr_initial_capacity, id_initial_capacity, queue_initial_capacity);
}
void sds_ecm_free(struct sds_id_da *ecm)
{
    sds_id_da_free(ecm);
}
sds_om_object_container_id_t sds_ecm_container_add(struct sds_id_da *ecm, size_t type_size, sds_da_index_t data_initial_capacity, sds_da_index_t id_initial_capacity, sds_da_index_t index_initial_capacity)
{
    sds_om_object_container_id_t id = sds_id_da_add(ecm);
    sds_id_da_create(((struct sds_id_da *)sds_id_da_get(ecm, id)), type_size, data_initial_capacity, id_initial_capacity, index_initial_capacity);
    return id;
}
void sds_ecm_container_delete(struct sds_id_da *ecm, sds_om_object_container_id_t container_id)
{
    sds_id_da_free((struct sds_id_da *)sds_id_da_get(ecm, container_id));
    sds_id_da_delete(ecm, container_id);
}
sds_om_object_id_t sds_ecm_component_add(struct sds_id_da *ecm, sds_om_object_container_id_t container_id)
{
    return sds_id_da_add((struct sds_id_da *)sds_id_da_get(ecm, container_id));
}
void sds_ecm_component_delete(struct sds_id_da *ecm, sds_om_object_container_id_t container_id, sds_om_object_id_t object_id)
{
    sds_id_da_delete(((struct sds_id_da *)sds_id_da_get(ecm, container_id)), object_id);
}
void *sds_ecm_component_get(struct sds_id_da *ecm, sds_om_object_container_id_t container_id, sds_om_object_id_t object_id)
{
    return sds_id_da_get(((struct sds_id_da *)sds_id_da_get(ecm, container_id)), object_id);
}
struct sds_id_da *sds_ecm_container_get(struct sds_id_da *ecm, sds_om_object_container_id_t container_id)
{
    return ((struct sds_id_da *)sds_id_da_get(ecm, container_id));
}