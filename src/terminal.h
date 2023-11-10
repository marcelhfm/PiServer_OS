//
// Created by marcel on 10.11.23.
//

#ifndef PISERVER_OS_TERMINAL_H
#define PISERVER_OS_TERMINAL_H

void printf(const char *str);

void clearScreen();

extern unsigned int vgapal[];

enum {
    FONT_WIDTH = 8,
    FONT_HEIGHT = 8,
    FONT_BPG = 8,  // Bytes per glyph
    FONT_BPL = 1,  // Bytes per line
    FONT_NUMGLYPHS = 224
};

extern unsigned char font[FONT_NUMGLYPHS][FONT_BPG];

#endif //PISERVER_OS_TERMINAL_H
