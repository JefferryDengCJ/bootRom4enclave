#include "uart.h"
#include "spi.h"
#include "sd.h"
#include "gpt.h"
#include "gpio.h"

int main() {
    init_uart(50000000, 115200);
    print_uart("Hello World\r\n");
    int res = gpt_find_boot_partition((uint8_t *)0x80000000UL, 2 * 16384);
	
    if (res == 0) {
        // jump to the address
        __asm__ volatile(
            "li s0, 0x80000000;"
            "la a1, _dtb;"
            "jr s0");
    }
    else{
        print_uart("SD Card Error\r\n");
    }
}
