#include <stdio.h>

#define CARR_MAP_IMPLEMENTATION
#include "../map.h"

#define CARR_SV_IMPLEMENTATION
#include "../sv.h"

int main(void)
{
    Map freqs;
    map_init(&freqs);

    StringBuilder buf = sb_from_file("examples/fpessoa.txt"); 
    StringView file_view = sv_from_sb(buf);

    while (file_view.len > 0) {
        StringView line_view = sv_chop_line(&file_view);
        while (line_view.len > 0) {
            StringView word = sv_chop_by_space(&line_view);
            sv_strip_space(&word);

            Entry item;
            int* count = malloc(sizeof(int));
            char* key = sv_to_cstr(word);

            map_get(&freqs, key, &item);
            if (item.key == NULL && item.value == NULL) {
                *count = 1;
            } else {
                *count = *(int*)item.value + 1;
            }

            item = (Entry) {
                .key   = key,
                .value = (void*)count,
            };
            map_insert(&freqs, item);
        };
    }

    for (size_t i = 0; i < freqs.cap; ++i) {
        Entry item = freqs.items[i];
        if (item.key == NULL) {
            continue;
        }
        printf("%s: %d\n", item.key, *(int*)item.value);
    }
    return 0;
}

