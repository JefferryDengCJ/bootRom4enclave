#include "uart.h"
#include "spi.h"
#include "sd.h"
#include "gpt.h"
#include "gpio.h"

int main()
{
  
    volatile int i;
    volatile uint8_t led=0xFF;

    //TestDDR(0x80000000);
    * (volatile unsigned long *)0x80001000 = 0x12345678;
    * (volatile unsigned long *)0x80001010 = 0x10101122;
    * (volatile unsigned long *)0x80001020 = 0x10202222;
    * (volatile unsigned long *)0x80001030 = 0x10303333;
    * (volatile unsigned long *)0x80001040 = 0x10404444;
    * (volatile unsigned long *)0x80002000 = 0x20002000;
    * (volatile unsigned long *)0x80002010 = 0x20102010;
    * (volatile unsigned long *)0x80002020 = 0x20202020;
    * (volatile unsigned long *)0x80002030 = 0x30303030;
    * (volatile unsigned long *)0x80002040 = 0x40404040;
    * (volatile unsigned long *)0x80001000 = * (volatile unsigned long *)0x80002000;//0x20003344
    * (volatile unsigned long *)0x80001010 = * (volatile unsigned long *)0x80002010;//0x20102010
    * (volatile unsigned long *)0x80001020 = * (volatile unsigned long *)0x80002020;//0x20202020
    * (volatile unsigned long *)0x80001030 = * (volatile unsigned long *)0x80002030;//0x30303030
   
    init_uart(50000000, 115200);
    print_uart("Hello World\r\n");
    // GPIO
    init_gpio();
    // Ethernet
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

    while (1) {
	    write_gpio(GPIO_BASE,led & 0xFF);
	    for(i=150000;i>0;i--);
	    led>>=1;
	    if(led == 0)
	    {
	    	led=0xFF;
                print_uart("LED LOOP\r\n");
	    }
    }
}
//void handle_trap(void)
//{
    // print_uart("trap\r\n");
//}
