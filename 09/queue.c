#include <stdio.h>
#include <stdlib.h>

struct queue {
    int start;
    int finish;
    pid_t storage[1000];

};

void push(struct queue *a, long n){
    a->storage[a->finish] = n;
    (a->finish)++;
}

long pop(struct queue *a) {
    long n = a->storage[a->start];
    (a->start)++;
    return n;
}

int size(struct queue *a) {
    return (a->finish - a->start);
}
