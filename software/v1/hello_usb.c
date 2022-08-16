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

void send_data(uint32_t data) {
    // 9 ms of modulation
    for (int i = 0; i < 346; i++) {
        gpio_put(PIN_IR_LED, 1);
        busy_wait_us(13);
        gpio_put(PIN_IR_LED, 0);
        busy_wait_us(13);
    }
    // 4.5ms of silence
    busy_wait_us(4500);

    // 32 bits of data (inverted_payload >> payload >> inverted_addr >> addr)
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 21; j++) {
            gpio_put(PIN_IR_LED, 1);
            busy_wait_us(13);
            gpio_put(PIN_IR_LED, 0);
            busy_wait_us(13);
        }
        if ((data & 1) == 0) {
            busy_wait_us(562);
        } else {
            busy_wait_us(1687);
        }
        data = data >> 1;
    }
    for (int j = 0; j < 21; j++) {
        gpio_put(PIN_IR_LED, 1);
        busy_wait_us(13);
        gpio_put(PIN_IR_LED, 0);
        busy_wait_us(13);
    }
}


int main() {
    stdio_init_all();

    int component_pins[] =
        { PIN_A_DIRECT, PIN_A_R1, PIN_A_R2, PIN_B_DIRECT, PIN_B_R1, PIN_B_R2, PIN_C_DIRECT, PIN_C_R1, PIN_C_R2 };

    adc_init();

    for (int i = 0; i < 9; i++) {
        gpio_init(component_pins[i]);
        gpio_set_drive_strength(component_pins[i], GPIO_DRIVE_STRENGTH_12MA);
        gpio_set_slew_rate(component_pins[i], GPIO_SLEW_RATE_FAST);
    }

#if 0
    gpio_init(PIN_IR_SENSOR);
    gpio_pull_up(PIN_IR_SENSOR);
    int x, oldx;
    absolute_time_t stamp, oldstamp;
    int j = 0;
    while (1) {
        int x = gpio_get(PIN_IR_SENSOR);
        if (x != oldx) {
            oldstamp = stamp;
            stamp = get_absolute_time();

            oldx = x;
            printf("swapped to %u after %lu\n", x, absolute_time_diff_us(oldstamp, stamp));
            if (x == 0) {
                printf("%d--------\n", j);
                j++;
            }
        }
    }
#endif

#if 0
    gpio_init(PIN_IR_LED);
    gpio_set_drive_strength(PIN_IR_LED, GPIO_DRIVE_STRENGTH_12MA);
    gpio_set_slew_rate(PIN_IR_LED, GPIO_SLEW_RATE_FAST);
    gpio_set_dir(PIN_IR_LED, GPIO_OUT);

//    uint32_t data = 0xF708FF00;  // red
    while (1) {
        char input = getchar();

        switch (input) {
        case '+':
            send_data(0xFF00FF00);
            break;
        case '-':
            send_data(0xFD02FF00);
            break;
        case 'I':
            send_data(0xF807FF00);
            break;
        case 'O':
            send_data(0xF906FF00);
            break;
        }
    }
//    uint32_t data = 0xF906FF00;  // power off
    //  send_data(data); 
#endif
    /*
     *  (PIN_A_DIRECT)  ---- [ R? ] ----+---- [ R1 ] ---- (PIN_B_R1)
     *      GND                         |                     3V3
     *                             (PIN_B_DIRECT)
     */
    gpio_set_dir(PIN_A_DIRECT, GPIO_OUT);
    gpio_put(PIN_A_DIRECT, 0);

    gpio_set_dir(PIN_B_R1, GPIO_OUT);
    gpio_put(PIN_B_R1, 1);

    adc_select_input(1);

    while (true) {
        uint16_t result = adc_read();
        if (result == 0) {
            printf("ADC output was 0\n");
        } else {
            float resistor = ((float) R1_VAL) / ((((float) 4096) / result) - 1);
	    float temperature = (resistor - 1600.) * (0.05 * 1.25); // KTY81-220
            printf("Resistance is %.2f, Temperature is %.2f C\n", resistor, temperature);
        }
        sleep_ms(500);
    }
    return 0;
}
