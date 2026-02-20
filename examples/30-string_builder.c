#include <stdio.h>

#define CARR_SV_IMPLEMENTATION
#include "../sv.h"

void sb_print(StringBuilder sb)
{
    printf("CAP: %zu | LEN: %zu \n", sb.cap, sb.len);
    printf("CONTENT:\n%.*s\n", (int)sb.len, sb.data); 
}

typedef struct {
    const char* name;
    int         age;
} Person;

Person people[4] = {
    { "John Cena",                64  },
    { "Elizabeth Mackenzie",      32  },
    { "DarkCat",                  354 },
    { "Luke Lane",                523 },
};

int main(void)
{
    StringBuilder sb = sb_new();
    for (int i = 0; i < 4; ++i) {
        Person p = people[i];
        sb_concatf(&sb, "%s,%d\n", p.name, p.age);
    }
    sb_print(sb);
    sb_free(&sb);
    return 0;
}
