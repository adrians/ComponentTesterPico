#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define PIN_IR_SENSOR 1
#define PIN_IR_LED 3
#define PIN_BTN 15

#define PIN_A_DIRECT 28
#define PIN_A_R1 18
#define PIN_A_R2 17

#define PIN_B_DIRECT 27
#define PIN_B_R1 20
#define PIN_B_R2 19

#define PIN_C_DIRECT 26
#define PIN_C_R1 22
#define PIN_C_R2 21

#define R1_VAL 750
#define R2_VAL 470000

int main()
{
    stdio_init_all();

    int component_pins[] =
        { PIN_A_DIRECT, PIN_A_R1, PIN_A_R2, PIN_B_DIRECT, PIN_B_R1, PIN_B_R2, PIN_C_DIRECT, PIN_C_R1, PIN_C_R2 };

    adc_init();

    for (int i = 0; i < 9; i++) {
        gpio_init(component_pins[i]);
        gpio_set_drive_strength(component_pins[i], GPIO_DRIVE_STRENGTH_12MA);
        gpio_set_slew_rate(component_pins[i], GPIO_SLEW_RATE_FAST);
    }

    /*
     *  (PIN_A_DIRECT)  ---- [ R? ] ----+---- [ R1 ] ---- (PIN_C_R1)
     *      GND                         |                     3V3
     *                             (PIN_C_DIRECT)
     */
    gpio_set_dir(PIN_A_DIRECT, GPIO_OUT);
    gpio_put(PIN_A_DIRECT, 0);

    gpio_set_dir(PIN_C_R1, GPIO_OUT);
    gpio_put(PIN_C_R1, 1);

    adc_select_input(0);

    while (true) {
        uint16_t result = adc_read();
        if (result == 0) {
            printf("ADC output was 0\n");
        } else {
            float resistor = ((float) R1_VAL) / ((((float) 4096) / result) - 1);
            printf("Resistor value is %.2f, ADC output was %u\n", resistor, result);
        }
        sleep_ms(500);
    }
    return 0;
}
