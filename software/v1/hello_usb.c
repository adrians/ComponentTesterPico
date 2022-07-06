/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;

    gpio_init(LED_PIN);
    gpio_set_drive_strength(LED_PIN, GPIO_DRIVE_STRENGTH_12MA);
    gpio_set_slew_rate(LED_PIN, GPIO_SLEW_RATE_FAST);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        printf("Hello, world!\n");
        gpio_put(LED_PIN, 1);
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
    }
    return 0;
}

