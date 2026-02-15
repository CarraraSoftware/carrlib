#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <float.h>

#include "../vec.h"

typedef struct {
    int age;
} Person;

typedef struct {
    Person*             items;
    size_t              len;
    size_t              cap;
    HeapCompareFunction compare;
} Queue;

bool Person_compare(void* a, void* b)
{
    return ((Person*)a)->age > ((Person*)b)->age;
}

int main(void)
{
    time_t seed = time(NULL);
    srand(seed);
    
    Queue queue;
    heap_new(&queue, Person_compare);

    for (int i = 0; i < 10; ++i) {
        Person v = (Person) {
            .age  = rand() % 100,
        };
        vec_append(&queue, v);
    }

    heapfy(&queue);

    while (queue.len > 0) {
        Person p;
        heap_pop(&queue, &p);
        printf("Next Oldest Person's Age: %d\n", p.age);
    }
    return 0;
}
