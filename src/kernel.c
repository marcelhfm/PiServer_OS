//
// Created by marcel on 28.10.23.
//
#include "io.h"

void main() {
    uart_init();
    uart_writeText("Hello world!\n");

    while (1);
}