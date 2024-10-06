#ifndef BUFFER_H
#define BUFFER_H
#include <stdint.h>

#define BUFFER_EMPTY -1
#define BUFFER_FULL   1

typedef struct {
    uint8_t tail;
    uint8_t head;
    uint8_t mask;
    char *data;
} buffer_t;

void buffer_init(buffer_t *b, char *data, uint8_t size);
int buffer_put(buffer_t *b, char c);
int buffer_get(buffer_t *b);

#endif
