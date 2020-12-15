/**
 * This is just a stupid test program to find contiguous blocks
 * of memory that are unused within a given range of addresses. Basically
 * the whole point of this is to find out what a smaller value for
 * STRIP_HOP can be.
 **/

#include "ledstrips.h"
#include "gamma8.h"

int main() {
    strip_config cfg;
    leds_init(&cfg);
    uint8_t *cmd = cfg.base_addr;
    off_t start = 0;
    int i = 0;
    while (i < STRIP_MEM_SIZE + STRIP_HOP) {
        while (cmd[i] == 0 && i < STRIP_MEM_SIZE + STRIP_HOP) {
            i++;
        }
        printf("%#10x - %#10x (%d bytes)\n", start + STRIP_BASE_MEM, i + STRIP_BASE_MEM, i - start);
        while (cmd[i] != 0 && i < STRIP_MEM_SIZE + STRIP_HOP) {
            i++;
            start = i;
        }
    }
    printf("%#08x\n", start+STRIP_BASE_MEM);
    leds_close(&cfg);
    return 0;
}