#ifndef CARR_ARENA_H_
#define CARR_ARENA_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#ifndef CARR_ARENA_MALLOC
#define CARR_ARENA_MALLOC malloc
#endif

#ifndef CARR_ARENA_FREE
#define CARR_ARENA_FREE free
#endif

#ifndef CARR_ARENA_FORCE_PREFIX

#define Arena CarrArena

#define arena_reserve carr_arena_reserve
#define arena_alloc   carr_arena_alloc
#define arena_clear   carr_arena_clear
#define arena_free    carr_arena_free
#define arena_save    carr_arena_save
#define arena_restore carr_arena_restore

#endif //CARR_ARENA_FORCE_PREFIX

// For now, once instantiated, the Arena
// will always have the same capacity.
// TODO: expand the arena (reserve/commit?, mmap?)
typedef struct {
    uint8_t* data;
    size_t   len;
    size_t   cap;
    size_t   checkpoint;
} CarrArena;

CarrArena carr_arena_reserve(size_t cap);
void*     carr_arena_alloc(CarrArena* a, size_t size);
void      carr_arena_save(CarrArena* a);
void      carr_arena_restore(CarrArena* a);
void      carr_arena_clear(CarrArena* a);
void      carr_arena_free(CarrArena* a);

#ifdef CARR_ARENA_IMPLEMENTATION

CarrArena carr_arena_reserve(size_t cap) {
    uint8_t* data = (uint8_t*)CARR_ARENA_MALLOC(cap);
    if (data == NULL) {
        printf("ERROR: failed to reserve ARENA\n");
        exit(1);
    }
    return (CarrArena){
        .data = data,
        .len  = 0,
        .cap  = cap,
        .checkpoint = 0,
    };
}

void* carr_arena_alloc(CarrArena* a, size_t size) 
{
    if (a->len + size > a->cap) {
        printf("ERROR: arena overflow\n");
        return NULL;
    }
    void* res = a->data + a->len;
    a->len += size;
    return res;
}

void carr_arena_save(CarrArena* a)
{
    a->checkpoint = a->len;
}

void carr_arena_restore(CarrArena* a)
{
    a->len = a->checkpoint;
}

void carr_arena_clear(CarrArena* a) 
{
    a->len        = 0; 
    a->checkpoint = 0; 
}

void carr_arena_free(CarrArena* a) 
{
    CARR_ARENA_FREE(a->data);
    a->data       = NULL;
    a->cap        = 0;
    a->len        = 0;
    a->checkpoint = 0;
}


#endif // CARR_ARENA_IMPLEMENTATION

#endif // CARR_ARENA_H_
