#include "../vec.h"

typedef struct {
    const char* name;
    int         age;
} Person;

typedef struct {
    Person* items;
    size_t  len;
    size_t  cap;
} PersonVec;

int main(void) 
{
    PersonVec v;
    vec_init(&v);

    for (int i = 0; i < 10; i++) {
        Person p = {
            .name = "aboba",
            .age = i,
        };
        vec_append(&v, p);
    }

    for (size_t i = 0; i < v.len; i++) {
        printf("--\n");
        printf("Person{ ");
        printf("Name: %s | ", v.items[i].name);
        printf("Age : %d", v.items[i].age);
        printf(" }\n");
    }

    vec_free(&v);
    return 0;
}
