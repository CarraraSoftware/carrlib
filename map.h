#ifndef CARR_MAP_H_
#define CARR_MAP_H_

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// The user can define this macro to include only the functions 
// with the 'carr_' prefix, as to avoid name collisions.
// If this macro is not defined, all the versions without prefix
// will also be included by default.
#ifndef CARR_MAP_FORCE_PREFIX

#define map_init     carr_map_init
#define map_get      carr_map_get
#define map_insert   carr_map_insert
#define map_delete   carr_map_delete
#define map_free     carr_map_free
#define map_realloc  carr_map_realloc

#endif //CARR_MAP_WITH_PREFIX

typedef struct {
    const char* key;
    void*       value;
} Entry;

typedef struct {
    Entry* items;
    size_t len;
    size_t cap;
} Map;

void carr_map_init(Map* m);
void carr_map_get(Map* m, const char* key, Entry* e);
void carr_map_insert(Map* m, Entry e);
void carr_map_delete(Map* m, const char* key);

#ifdef CARR_MAP_IMPLEMENTATION

#define CARR_MAP_INITIAL_CAP     256
#define CARR_MAP_LOAD_FACTOR     0.7
#define CARR_MAP_TOMBSTONE_VALUE (void*)1
#define CARR_MAP_TOMBSTONE       (Entry){ NULL, CARR_MAP_TOMBSTONE_VALUE }


uint32_t _carr_hash(const char* chars)
{
    uint32_t FNV_prime = 16777619;
    uint32_t hash = 2166136261;
    size_t n = strlen(chars);
    for (size_t i = 0; i < n; i++) {
        hash = hash * FNV_prime;
        hash = hash ^ chars[i];
    }
    return hash;
}

void carr_map_free(Map *m)
{
    free(m->items);
    *m = (Map){0};
}

void carr_map_realloc(Map* m)
{
    size_t new_cap;
    if (m->cap > 0) {
        new_cap = m->cap * 2;
    } else {
        new_cap = CARR_MAP_INITIAL_CAP;
    }

    Map old = *m;

    m->cap = new_cap;
    m->items = (Entry*)malloc(sizeof(m->items[0]) * new_cap);
    for (size_t i = 0; i < old.cap; ++i) {
        Entry item = old.items[i];
        if (item.key == NULL) {
            continue;
        }
        map_insert(m, item);
    }
    map_free(&old);
}

void carr_map_init(Map* m)
{
    *m = (Map){0};
    map_realloc(m);
    // *m->items = (Entry){0};
    // m->cap   = 0;
    // m->len   = 0;
}


void carr_map_get(Map* m, const char* key, Entry* e)
{
    uint32_t idx = _carr_hash(key) % m->cap;
    Entry cur;
    cur = m->items[idx];
    while (
        (cur.key != NULL && strcmp(cur.key, key) != 0)
        || (cur.key == NULL && cur.value == CARR_MAP_TOMBSTONE_VALUE)
    ) {
        idx = (idx + 1) % m->cap;
        cur = m->items[idx];
    }
    if (cur.key == NULL) {
        *e = (Entry){ NULL, NULL };
        return;
    }
    *e = cur;
}

void carr_map_insert(Map* m, Entry e)
{
    if (m->len + 1 >= m->cap * CARR_MAP_LOAD_FACTOR) {
        map_realloc(m);
    }

    uint32_t idx = _carr_hash(e.key) % m->cap;
    Entry exists = {0};
    map_get(m, e.key, &exists);

    Entry* cur;
    cur = &(m->items[idx]);
    if (exists.key == NULL && exists.value == NULL) {
        while (cur->key != NULL) {
            idx = (idx + 1) % m->cap;
            cur = &(m->items[idx]);
        }
        m->len++;
    } else {
        while (strcmp(cur->key, e.key) != 0) {
            idx = (idx + 1) % m->cap;
            cur = &(m->items[idx]);
        }
    }
    m->items[idx] = e;  
}

void carr_map_delete(Map* m, const char* key)
{
    uint32_t idx = _carr_hash(key) % m->cap;
    Entry cur;
    cur = (m->items[idx]);
    while (cur.key != NULL && strcmp(cur.key, key) != 0) {
        idx = (idx + 1) % m->cap;
        cur = (m->items[idx]);
    }
    if (cur.key == NULL) {
        return;
    }
    m->len--;
    m->items[idx] = CARR_MAP_TOMBSTONE;
}

#endif // CARR_MAP_IMPLEMENTATION

#endif // CARR_MAP_H_
