//
// Created by marcel on 28.10.23.
//

#include "io.h"
#include "fb.h"
#include "terminal.h"

void main() {
    uart_init();
    fb_init();

    drawString(0, 200, "Testing printing", 0x0f);

    clearScreen();

    printf("Booting PiServerOS...\n");
    printf("Testing multiple prints\n");

    while (1);
}