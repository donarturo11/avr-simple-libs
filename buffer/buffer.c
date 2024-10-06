#include "buffer.h"

void buffer_init(buffer_t *b, char *data, uint8_t size)
{
    b->tail = 0;
    b->head = 0;
    b->mask = ~size;
    b->data = data;
}

int buffer_put(buffer_t *b, char c)
{
    uint8_t mask = b->mask;
    uint8_t head = b->head;
    uint8_t tail = b->tail;
    uint8_t rv = ((tail+1)==head) ? BUFFER_FULL : 0;
    if(!rv) {
        b->data[tail]=c;
        b->tail = (++tail) & mask;
    }
    return rv;
}

int buffer_get(buffer_t *b)
{
    uint8_t mask = b->mask;
    uint8_t head = b->head;
    uint8_t tail = b->tail;
    int rv = (tail == head) ? BUFFER_EMPTY : 0;
    if (!rv) {
        rv = b->data[head];
        b->head = (++head) & mask;
    }
    return (unsigned char)rv;    
}
