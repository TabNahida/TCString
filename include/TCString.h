#ifndef TCSTRING_H
#define TCSTRING_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct TCString
{
    size_t length;
    char *buffer;
} TCString;

size_t getClosestPowerOfTwo(size_t size)
{
    if (size <= 1)
        return 1;
    size--;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
#if SIZE_MAX > 0xFFFFFFFF
    size |= size >> 32;
#endif
    size++;
    return size;
}

inline TCString *TCStringConstructor(const char *str)
{
    TCString *object = malloc(sizeof(TCString));
    object->length = strlen(str);
    object->buffer = malloc(getClosestPowerOfTwo(object->length + 1));
    memcpy(object->buffer, str, object->length + 1);
    return object;
}

inline TCString *TCStringConstructor_()
{
    TCString *object = malloc(sizeof(TCString));
    object->length = 0;
    object->buffer = malloc(1);
    object->buffer[0] = '\0';
    return object;
}

inline TCString *TCStringConstructor_c(char ch)
{
    TCString *object = malloc(sizeof(TCString));
    object->length = 1;
    object->buffer = malloc(getClosestPowerOfTwo(object->length + 1));
    object->buffer[0] = ch;
    object->buffer[1] = '\0';
    return object;
}

inline TCString *TCStringConstructor_len(const char *str, size_t len)
{
    TCString *object = malloc(sizeof(TCString));
    object->length = len;
    object->buffer = malloc(getClosestPowerOfTwo(object->length + 1));
    memcpy(object->buffer, str, object->length);
    object->buffer[object->length] = '\0';
    return object;
}

inline TCString *TCStringConstructor_TCStr(TCString *str)
{
    TCString *object = malloc(sizeof(TCString));
    object->length = str->length;
    object->buffer = malloc(getClosestPowerOfTwo(object->length + 1));
    memcpy(object->buffer, str, object->length + 1);
    return object;
}

#endif