//
// Created by marcel on 10.11.23.
//

#include "io.h"

// 16 byte aligned buffer -> only 28 upper bits of address can be passed to mailbox
volatile unsigned int __attribute__((aligned(16))) mbox[36];

enum {
    VIDEOCORE_MBOX = (PERIPHERAL_BASE + 0x0000B880),
    MBOX_READ = (VIDEOCORE_MBOX + 0x0),
    MBOX_POLL = (VIDEOCORE_MBOX + 0x10),
    MBOX_SENDER = (VIDEOCORE_MBOX + 0x14),
    MBOX_STATUS = (VIDEOCORE_MBOX + 0x18),
    MBOX_CONFIG = (VIDEOCORE_MBOX + 0x1C),
    MBOX_WRITE = (VIDEOCORE_MBOX + 0x20),
    MBOX_RESPONSE = 0x80000000,
    MBOX_FULL = 0x80000000,
    MBOX_EMPTY = 0x40000000
};

unsigned int mbox_call(unsigned char ch) {
    // 28-bit address (MSB) and 4-bit value (LSB)
    unsigned int r = ((unsigned int) ((long) &mbox) & ~0xF) | (ch & 0xF);

    // Wait till write is available
    while (mmio_read(MBOX_STATUS) & MBOX_FULL);

    // Write the address of our buffer to the mailbox with the channel appended
    mmio_write(MBOX_WRITE, r);

    while (1) {
        // Reply?
        while (mmio_read(MBOX_STATUS) & MBOX_EMPTY);

        // Reply to our message?
        if (r == mmio_read(MBOX_READ)) {
            return mbox[1] == MBOX_RESPONSE;
        }
    }

    return 0;
}
