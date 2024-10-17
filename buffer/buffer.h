#ifndef BUFFER_H
#define BUFFER_H
#include <stdint.h>

#define BUFFER_EMPTY -1
#define BUFFER_FULL   1

#define BUFFER_SETUP(DATA, SIZE)      \
    {                                 \
        DATA,                         \
	~SIZE & 0xFF,                 \
	0,                            \
	0                             \
    }

struct __buffer {
    char *data;
    uint8_t mask;
    uint8_t tail;
    uint8_t head;
};

typedef struct __buffer BUFFER;

int buffer_put(char c, BUFFER*);
int buffer_get(BUFFER*);

#endif
