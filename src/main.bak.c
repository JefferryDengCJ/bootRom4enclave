#include "uart.h"
#include "spi.h"
#include "sd.h"
#include "gpt.h"
#include "gpio.h"


int main()
{
  
    //volatile int i;
    //volatile uint8_t led=0xFF;
    //delay(500);

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
    /*
    unsigned int *src_base_addr = (volatile unsigned int *)0x80001000;
    unsigned int *dst_base_addr = (volatile unsigned int *)0x80002000;
    for(int i = 0; i < 16; i++){
	*src_base_addr = i;
    }
    for(int i = 0; i < 32; i++){
	*dst_base_addr = *src_base_addr+i;
    }
    */
   
    init_uart(50000000, 115200);
    while (1) {
    	print_uart("Hello World\r\n");
    	print_uart("1-2-3\r\n");
    }
    /* 
    init_gpio();
    
    
    while (1)
    {
    
	    write_gpio(GPIO_BASE,led & 0xFF);
	    for(i=2500000;i>0;i--);
	    led>>=1;
	    if(led == 0)
	    {
	    	led=0xFF;
	    }
	       	
	       	
    }
*/		  
         		
}
void delay(int count)
{
	int i;
	for(i=1;i<=count;i++)
	;
}
//void handle_trap(void)
//{
    // print_uart("trap\r\n");
//}
