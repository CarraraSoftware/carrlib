#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define CARR_MAP_IMPLEMENTATION
#include "../map.h"

#define CARR_SV_IMPLEMENTATION
#include "../sv.h"

#define CARR_ARENA_IMPLEMENTATION
#include "../arena.h"


int main(void) {
    StringBuilder buf = sb_from_file("examples/fpessoa.txt");
    // Arena with 'sizeof(int)' bytes for every char in buf.
    Arena a = arena_reserve(buf.len * sizeof(int));
    Map m;
    map_init(&m);

    StringView file = sv_from_sb(buf);

    while (file.len > 0) {
        StringView line = sv_chop_line(&file);
        while (line.len > 0) {
            StringView word = sv_chop_by_space(&line);
            sv_strip_space(&word);

            Entry item;
            int *count = arena_alloc(&a, sizeof(int));
            char *key = arena_alloc(&a, word.len + 1);
            for (int i = 0; i < word.len; ++i) {
                key[i] = word.data[i];
            }
            key[word.len] = '\0';

            map_get(&m, key, &item);
            if (item.key == NULL && item.value == NULL) {
                *count = 1;
            } else {
                *count = *(int *)item.value + 1;
            }

            item = (Entry){
                .key = key,
                .value = (void *)count,
            };
            map_insert(&m, item);
        }
    }

    for (int i = 0; i < m.cap; ++i) {
        Entry e = m.items[i];
        if (e.key == NULL) {
            continue;
        }
        printf("%s: %d\n", e.key, *(int *)e.value);
    }

    sb_free(&buf);
    arena_free(&a);
    map_free(&m);

    return 0;
}
