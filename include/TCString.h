#ifndef TCSTRING_H
#define TCSTRING_H

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

static inline size_t getClosestPowerOfTwo(size_t size)
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

typedef struct
{
    size_t length;
    char *buffer;
} TCString;

static inline TCString *makeTCString(const char *str)
{
    TCString *object = (TCString *)malloc(sizeof(TCString));
    object->length = strlen(str);
    object->buffer = (char *)malloc(getClosestPowerOfTwo(object->length + 1));
    memcpy(object->buffer, str, object->length + 1);
    return object;
}

static inline TCString *makeTCString_()
{
    TCString *object = (TCString *)malloc(sizeof(TCString));
    object->length = 0;
    object->buffer = (char *)malloc(1);
    object->buffer[0] = '\0';
    return object;
}

static inline TCString *makeTCString_c(char ch)
{
    TCString *object = (TCString *)malloc(sizeof(TCString));
    object->length = 1;
    object->buffer = (char *)malloc(getClosestPowerOfTwo(object->length + 1));
    object->buffer[0] = ch;
    object->buffer[1] = '\0';
    return object;
}

static inline TCString *makeTCString_len(const char *str, size_t len)
{
    TCString *object = (TCString *)malloc(sizeof(TCString));
    object->length = len;
    object->buffer = (char *)malloc(getClosestPowerOfTwo(object->length + 1));
    memcpy(object->buffer, str, object->length);
    object->buffer[object->length] = '\0';
    return object;
}

static inline TCString *makeTCString_capa(size_t capacity)
{
    TCString *object = (TCString *)malloc(sizeof(TCString));
    object->length = 0;
    object->buffer = (char *)malloc(getClosestPowerOfTwo(capacity + 1));
    object->buffer[0] = '\0';
    return object;
}

static inline TCString *makeTCString_tcstr(TCString *str)
{
    TCString *object = (TCString *)malloc(sizeof(TCString));
    object->length = str->length;
    object->buffer = (char *)malloc(getClosestPowerOfTwo(object->length + 1));
    memcpy(object->buffer, str, object->length + 1);
    return object;
}

#define TCString(name, str) TCString *name = makeTCString(str)
#define TCString_(name) TCString *name = makeTCString_()
#define TCString_c(name, ch) TCString *name = makeTCString_c(ch)
#define TCString_len(name, str, len) TCString *name = makeTCString_len(str, len)
#define TCString_capa(name, capacity) TCString *name = makeTCString_capa(capacity)
#define TCString_tcstr(name, tcstr) TCString *name = makeTCString_tcstr(str)

static inline TCString *appendTCString(TCString *this, const char *str)
{
    size_t strLength = strlen(str);
    size_t newLength = this->length + strLength;
    size_t newCapacity = getClosestPowerOfTwo(newLength + 1);
    if (newCapacity > getClosestPowerOfTwo(this->length + 1))
    {
        char *newBuffer = (char *)malloc(newCapacity);
        memcpy(newBuffer, this->buffer, this->length);
        free(this->buffer);
        this->buffer = newBuffer;
    }
    memcpy(this->buffer + this->length, str, strLength + 1);
    this->length = newLength;
    return this;
}

static inline TCString *appendTCString_tcstr(TCString *this, TCString *str)
{
    size_t newLength = this->length + str->length;
    size_t newCapacity = getClosestPowerOfTwo(newLength + 1);
    if (newCapacity > getClosestPowerOfTwo(this->length + 1))
    {
        char *newBuffer = (char *)malloc(newCapacity);
        memcpy(newBuffer, this->buffer, this->length);
        free(this->buffer);
        this->buffer = newBuffer;
    }
    memcpy(this->buffer + this->length, str->buffer, str->length + 1);
    this->length = newLength;
    return this;
}

static inline size_t bufferSizeTCString(const TCString *this)
{
    return getClosestPowerOfTwo(this->length + 1);
}

static inline TCString *clearTCString(TCString *this)
{
    free(this->buffer);
    this->length = 0;
    this->buffer = (char *)malloc(1);
    this->buffer[0] = '\0';
    return this;
}

static inline TCString *freeTCString(TCString *this)
{
    free(this->buffer);
    free(this);
    return this;
}

static inline TCString *reserveTCString(TCString *this, size_t capacity)
{
    free(this->buffer);
    this->length = 0;
    this->buffer = (char *)malloc(getClosestPowerOfTwo(this->length + 1));
    this->buffer[0] = '\0';
    return this;
}

static inline TCString *subTCString(const TCString *this, size_t pos, size_t len)
{
    if (pos > this->length)
    {
        return NULL;
    }
    size_t actualLen = min(len, this->length - pos);
    TCString *result = makeTCString_capa(actualLen);
    memcpy(result->buffer, this->buffer + pos, actualLen);
    result->buffer[actualLen] = '\0';
    result->length = actualLen;
    return result;
}

static inline TCString *subTCString_end(const TCString *this, size_t pos)
{
    if (pos > this->length)
    {
        return NULL;
    }
    size_t actualLen = this->length - pos;
    TCString *result = makeTCString_capa(actualLen);
    memcpy(result->buffer, this->buffer + pos, actualLen);
    result->buffer[actualLen] = '\0';
    result->length = actualLen;
    return result;
}

#define substrTCString(this, pos, len, name) TCString *name = subTCString(this, pos, len)
#define substrTCString_end(this, pos, name) TCString *name = subTCString_end(this, pos)

static inline uint32_t hashFNV(const void *key, size_t len)
{
    uint32_t hash = 2166136261u;
    const uint8_t *p = key;

    for (size_t i = 0; i < len; i++)
    {
        hash ^= p[i];
        hash *= 16777619;
    }

    return hash;
}

static inline int32_t hashMurmur(const void *key, size_t len, uint32_t seed)
{
    const uint8_t *data = key;
    const int nblocks = len / 4;
    uint32_t h1 = seed;
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;

    for (int i = 0; i < nblocks; i++)
    {
        uint32_t k1 = *(uint32_t *)(data + i * 4);
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> (32 - 13));
        h1 = h1 * 5 + 0xe6546b64;
    }

    const uint8_t *tail = data + nblocks * 4;
    uint32_t k1 = 0;

    switch (len & 3)
    {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;
        h1 ^= k1;
    }

    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

void hashMD5(void *data, size_t size, uint8_t hash[16])
{
    uint32_t s[4] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
    uint32_t k[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d9, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0x0c9c9a99, 0x1f83d9ab, 0x3b6c1c2d, 0x5be0cd19,
        0x10e32f67, 0x5a82810f, 0x6d9d8f3b, 0x74c35e73, 0x87c91ef5, 0x9f6c9d85, 0x1a2d64e7, 0x2e0281b2,
        0x7e7f26e2, 0x2aa6e860, 0x547c7c4f, 0x5c80f32d, 0xd1d2cd11, 0xf2ee8f4a, 0x8fa4c1c5, 0x213b15c4,
        0x9b50c41e, 0x9a4a3d6f, 0x8af5dc4e, 0x7c032500, 0x3a56c1d4, 0x0c5cb9a0, 0x0fa7a6b3, 0x38216f0f,
        0xe1e5b65f, 0x8c1e5608, 0x5d6d8886, 0x2da7056a, 0xb0a2c7f3, 0xa8ee6054, 0xf8e8d417, 0x6df8f421,
    };

    uint32_t r[64] = {7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21};

    uint8_t *input = data;
    uint64_t bit_len = size * 8;

    size_t new_size = size + 1 + ((56 - (size + 1) % 64) % 64) + 8;
    uint8_t *buffer = (uint8_t *)malloc(new_size);
    memcpy(buffer, input, size);
    buffer[size] = 0x80;

    memset(buffer + size + 1, 0, new_size - size - 9);
    memcpy(buffer + new_size - 8, &bit_len, 8);

    for (size_t i = 0; i < new_size / 64; i++)
    {
        uint32_t a = s[0], b = s[1], c = s[2], d = s[3];

        for (size_t j = 0; j < 64; j++)
        {
            uint32_t f, g;
            if (j < 16)
            {
                f = (b & c) | (~b & d);
                g = j;
            }
            else if (j < 32)
            {
                f = (d & b) | (~d & c);
                g = (5 * j + 1) % 16;
            }
            else if (j < 48)
            {
                f = b ^ c ^ d;
                g = (3 * j + 5) % 16;
            }
            else
            {
                f = c ^ (b | ~d);
                g = (7 * j) % 16;
            }

            f = f + a + k[j] + (*(uint32_t *)(buffer + i * 64 + g * 4));
            a = d;
            d = c;
            c = b;
            b = b + ((f << r[j % 4]) | (f >> (32 - r[j % 4])));
        }

        s[0] += a;
        s[1] += b;
        s[2] += c;
        s[3] += d;
    }

    for (size_t i = 0; i < 4; i++)
    {
        hash[i * 4 + 0] = (s[i] & 0xFF);
        hash[i * 4 + 1] = (s[i] >> 8) & 0xFF;
        hash[i * 4 + 2] = (s[i] >> 16) & 0xFF;
        hash[i * 4 + 3] = (s[i] >> 24) & 0xFF;
    }

    free(buffer);
}

#endif
