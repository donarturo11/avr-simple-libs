#include "buffer.h"

int buffer_put(char c, BUFFER *b)
{
    uint8_t mask = b->mask;
    uint8_t head = b->head;
    uint8_t headnext = (head+1) & mask;
    uint8_t tail = b->tail;
    uint8_t rv = (headnext==tail) ? BUFFER_FULL : 0;
    if(!rv) {
        b->data[head]=c;
        b->head = headnext;
    }
    return rv;
}

int buffer_get(BUFFER *b)
{
    uint8_t mask = b->mask;
    uint8_t head = b->head;
    uint8_t tail = b->tail;
    uint8_t tailnext = (tail+1) & mask;
    int rv = (tail == head) ? BUFFER_EMPTY : 0;
    if (!rv) {
        rv = b->data[tail] & 0xFF;
        b->tail = tailnext;
    }
    return rv;    
}
