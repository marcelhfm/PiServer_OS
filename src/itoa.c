//
// Created by marcel on 10.11.23.
//

#include "itoa.h"

// A simple implementation of itoa
void itoa(int num, char *buffer, int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    // Handle negative numbers only if base is 10
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int remainder = num % base;
        buffer[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        num = num / base;
    }

    // Append negative sign for base 10
    if (isNegative && base == 10)
        buffer[i++] = '-';

    buffer[i] = '\0'; // Null-terminate the string
}
