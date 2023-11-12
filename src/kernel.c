//
// Created by marcel on 28.10.23.
//

#include "io.h"
#include "fb.h"
#include "terminal.h"
#include "breakout.h"

void main() {
    fb_init();
    uart_init();

    clearScreen();

    startBreakout();
    while (1);
}