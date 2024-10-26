#include "TCString.h"
#include <stdint.h>
#include <stdio.h>

void printTCString(const TCString *str)
{
    if (str)
    {
        printf("String: %s, Length: %zu, Buffer Size: %zu\n", str->buffer, str->length, bufferSizeTCString(str));
    }
}

int main()
{
    // Test 1: Create TCString from a regular string
    TCString(str1, "Hello, World!");
    printTCString(str1);

    // Test 2: Create empty TCString
    TCString_(str2);
    printTCString(str2);

    // Test 3: Create TCString from a single character
    TCString_c(str3, 'A');
    printTCString(str3);

    // Test 4: Append to TCString
    appendTCString(str1, " Welcome to TCString!");
    printTCString(str1);

    // Test 5: Create TCString from a length-limited substring
    TCString_len(str4, "Substring Test", 9);
    printTCString(str4);

    // Test 6: Substring
    substrTCString(str1, 7, 5, subStr);
    printTCString(subStr);

    // Test 7: Substring to the end
    substrTCString_end(str1, 7, endSubStr);
    printTCString(endSubStr);

    // Test 8: Hash functions
    uint32_t fnvHash = hashFNV(str1->buffer, str1->length);
    printf("FNV Hash: %u\n", fnvHash);

    uint32_t murmurHash = hashMurmur(str1->buffer, str1->length, 0);
    printf("Murmur Hash: %u\n", murmurHash);

    // Test 9: Clear TCString
    clearTCString(str2);
    printTCString(str2);

    // Test 10: Free all TCStrings
    freeTCString(str1);
    freeTCString(str2);
    freeTCString(str3);
    freeTCString(str4);
    freeTCString(subStr);
    freeTCString(endSubStr);

    return 0;
}
