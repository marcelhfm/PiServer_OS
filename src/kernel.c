//
// Created by marcel on 28.10.23.
//

#include "io.h"
#include "fb.h"
#include "terminal.h"

void main() {
    fb_init();
    uart_init();

    //clearScreen();

    printf("Booting PiServerOS...\n");
    printf("Testing multiple prints\n");

    while (1);
}