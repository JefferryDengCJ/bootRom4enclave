#include <stddef.h>

#define ED25519_NO_SEED 1



#include "sha3/sha3.h"
/* Adopted from https://github.com/orlp/ed25519
  provides:
  - void ed25519_create_keypair(t_pubkey *public_key, t_privkey *private_key, t_seed *seed);
  - void ed25519_sign(t_signature *signature,
                      const unsigned uint8_t *message,
                      size_t message_len,
                      t_pubkey *public_key,
                      t_privkey *private_key);
*/

#include "ed25519/ed25519.h"
/* adopted from
  provides:
  - int sha3_init(sha3_context * md);
  - int sha3_update(sha3_context * md, const unsigned char *in, size_t inlen);
  - int sha3_final(sha3_context * md, unsigned char *out);
  types: sha3_context
*/

/*

#include "string.h"
#include "time.h"
#include <stdio.h>
*/

#include "timer.h"
#include "riscv-interrupts.h"




void* memcpy(void* dest, const void* src, size_t len);
void* memset(void* dest, int byte, size_t len);
char* itoa(int num,char* str,int radix);

/*
  provides memcpy, memset
*/


#include "../src/uart.h"
#include "../src/spi.h"
#include "../src/sd.h"
#include "../src/gpt.h"
#include "../src/gpio.h"

typedef unsigned char byte;

// Sanctum header fields in DRAM
//extern byte sanctum_dev_public_key[32];
//extern byte sanctum_dev_secret_key[64];
unsigned int sanctum_sm_size = 0x1ff000;
//extern byte sanctum_sm_hash[64];
byte sanctum_sm_hash[64];

//extern byte sanctum_sm_public_key[32];
byte sanctum_sm_public_key[32];


//extern byte sanctum_sm_secret_key[64];
byte sanctum_sm_secret_key[64];


//extern byte sanctum_sm_signature[64];
byte sanctum_sm_signature[64];


#define DRAM_BASE 0x80000000

//-----------------------------------------------------------------------------
#warning Using TEST device root key. No integrity guarantee.
 // unsigned char _sanctum_dev_secret_key[] = {
unsigned char sanctum_dev_secret_key[64] = {
  0x40, 0xa0, 0x99, 0x47, 0x8c, 0xce, 0xfa, 0x3a, 0x06, 0x63, 0xab, 0xc9,
  0x5e, 0x7a, 0x1e, 0xc9, 0x54, 0xb4, 0xf5, 0xf6, 0x45, 0xba, 0xd8, 0x04,
  0xdb, 0x13, 0xe7, 0xd7, 0x82, 0x6c, 0x70, 0x73, 0x57, 0x6a, 0x9a, 0xb6,
  0x21, 0x60, 0xd9, 0xd1, 0xc6, 0xae, 0xdc, 0x29, 0x85, 0x2f, 0xb9, 0x60,
  0xee, 0x51, 0x32, 0x83, 0x5a, 0x16, 0x89, 0xec, 0x06, 0xa8, 0x72, 0x34,
  0x51, 0xaa, 0x0e, 0x4a
};
 const size_t _sanctum_dev_secret_key_len = 64;

//  unsigned char _sanctum_dev_public_key[] = {
unsigned char sanctum_dev_public_key[32] = {
  0x0f, 0xaa, 0xd4, 0xff, 0x01, 0x17, 0x85, 0x83, 0xba, 0xa5, 0x88, 0x96,
  0x6f, 0x7c, 0x1f, 0xf3, 0x25, 0x64, 0xdd, 0x17, 0xd7, 0xdc, 0x2b, 0x46,
  0xcb, 0x50, 0xa8, 0x4a, 0x69, 0x27, 0x0b, 0x4c
};
const size_t _sanctum_dev_public_key_len = 32;
//-----------------------------------------------------------------------------

void bootloader(void);

/* Update this to generate valid entropy for target platform*/
inline byte random_byte(unsigned int i) {
#warning Bootloader does not have entropy source, keys are for TESTING ONLY
  return 0xac + (0xdd ^ i);
}



// Global to hold current timestamp
static volatile uint64_t timestamp = 0;
static uint64_t	g_time_second_count = 0;



static unsigned char g_uncLedStatus = 0;
static unsigned char g_uncLedOnOff = 0;
static unsigned char g_LedNum = 0;

static void irq_entry(void)  {
    uint_xlen_t this_cause = csr_read_mcause();
    if (this_cause &  MCAUSE_INTERRUPT_BIT_MASK) {
        this_cause &= 0xFF;
        // Known exceptions
        switch (this_cause) {
        case RISCV_INT_MASK_MTI :
            // Timer exception, keep up the one second tick.
            mtimer_set_raw_time_cmp(MTIMER_SECONDS_TO_CLOCKS(1));
            timestamp = mtimer_get_raw_time();
	    g_time_second_count ++;
		
	    // ----------------- set led for file copy ----------
		if(g_uncLedOnOff ==1)
		{
			if(g_uncLedStatus )	// is on , set to off
			{
				g_uncLedStatus = 0;
				setLED_Off_0To3(g_LedNum);								
			}
			else{
				SetLED_ON_0To3(g_LedNum);
				g_uncLedStatus = 1;
			}
		}
	    // -----------------end of 	set led for file copy ---

            break;
        }
    }
}

// 开始提示COPY  
void SetIndicateForSDCopy(int LedNum)
{
	init_gpio();
	g_uncLedStatus = 0;
	g_uncLedOnOff = 1;
	g_LedNum = LedNum;
	setLED_Off_0To3(g_LedNum);	
}

// 
void ExitIndicateForSDCopy(void)
{
	g_uncLedOnOff = 0;
	setLED_Off_0To3(g_LedNum);	

}


void bootloader(void) {
	//*sanctum_sm_size = 0x200;
  // Reserve stack space for secrets
  byte scratchpad[128];
  sha3_ctx_t hash_ctx;

//  time_t	time_start=0, time_end=0;
   int minute =0;	
//   double second = 0;
   char time_string[64];		
   uint64_t 	time_start=0,time_end=0, second = 0;

  int res = 0;	

  // TODO: on real device, copy boot image from memory. In simulator, HTIF writes boot image
  // ... SD card to beginning of memory.
  // sd_init();
  // sd_read_from_start(DRAM, 1024);

  /* Gathering high quality entropy during boot on embedded devices is
   * a hard problem. Platforms taking security seriously must provide
   * a high quality entropy source available in hardware. Platforms
   * that do not provide such a source must gather their own
   * entropy. See the Keystone documentation for further
   * discussion. For testing purposes, we have no entropy generation.
  */

    init_uart(50000000, 115200);
//    print_uart("HW-20240113-cva6_git\r\n");
    print_uart("Hello World,v100\r\n");

/*
    for(res = 0; res < 64; res++) time_string[res]=0;
    sprintf(time_string,"For keystone enclave ,compiled at: %s - %s \r\n", __DATE__ , __TIME__);  //
    print_uart(time_string);		
*/	
	print_uart("For keystone enclave ,compiled at: ");
	print_uart(__DATE__);
	print_uart("---");
	print_uart(__TIME__);
	print_uart("\r\n");

	
	//------------------- set timer -------------------
    // Setup timer for 1 second interval
    timestamp = mtimer_get_raw_time();
    mtimer_set_raw_time_cmp(MTIMER_SECONDS_TO_CLOCKS(1));


    // Enable MIE.MTI
    csr_set_bits_mie(MIE_MTI_BIT_MASK);

    // Global interrupt enable 
    csr_set_bits_mstatus(MSTATUS_MIE_BIT_MASK);

    // Setup the IRQ handler entry point
    csr_write_mtvec((uint_xlen_t) irq_entry);

	//------------------- end of set timer -------------------

//    time_start = clock();	
//    time_start = timestamp;	
	g_time_second_count = 0;
	time_start = g_time_second_count;

	SetIndicateForSDCopy(0);

    	res = gpt_find_boot_partition((uint8_t *)0x80000000UL, 2 * 16384);	// read file from SD TO 


	time_end = g_time_second_count;
	
	ExitIndicateForSDCopy();

    if (res != 0) {
 	print_uart("SD Card Error\r\n");
    }
    else{
		for(res = 0; res < 64; res++) time_string[res]=0;
		second = time_end-time_start;
		minute = (int)(second/60);
		second = second%60;	

		// printf time in consol.
		print_uart("\n--------time cost :");
		itoa(minute,time_string, 10);	
		print_uart(time_string);	// %d
		print_uart("m");
		for(res = 0; res < 64; res++) time_string[res]=0;
		minute=(int)second;
		itoa(minute,time_string, 10);
		print_uart(time_string);
		print_uart("s ----------------\n");
		
	}
//    else{
//              // jump to the address
//        	__asm__ volatile(
//            "li s0, 0x80000000;"
//            "la a1, _dtb;"
//            "jr s0");
//    }

 


  // Create a random seed for keys and nonces from TRNG
  for (unsigned int i=0; i<32; i++) {
    scratchpad[i] = random_byte(i);
  }

  /* On a real device, the platform must provide a secure root device
     keystore. For testing purposes we hardcode a known private/public
     keypair */
  // TEST Device key
  //#include "use_test_keys.h"
  
  // Derive {SK_D, PK_D} (device keys) from a 32 B random seed
  ed25519_create_keypair(sanctum_dev_public_key, sanctum_dev_secret_key, scratchpad);	// jefferry . 01:04:57 ,Sat , Jan 13,2024 

  // Measure SM
  sha3_init(&hash_ctx, 64);
  sha3_update(&hash_ctx, (void*)DRAM_BASE, sanctum_sm_size);
  sha3_final(sanctum_sm_hash, &hash_ctx);

  // Combine SK_D and H_SM via a hash
  // sm_key_seed <-- H(SK_D, H_SM), truncate to 32B
  sha3_init(&hash_ctx, 64);
  sha3_update(&hash_ctx, sanctum_dev_secret_key, sizeof(*sanctum_dev_secret_key));
  sha3_update(&hash_ctx, sanctum_sm_hash, sizeof(*sanctum_sm_hash));
  sha3_final(scratchpad, &hash_ctx);
  // Derive {SK_D, PK_D} (device keys) from the first 32 B of the hash (NIST endorses SHA512 truncation as safe)
  ed25519_create_keypair(sanctum_sm_public_key, sanctum_sm_secret_key, scratchpad);

  // Endorse the SM
  memcpy(scratchpad, sanctum_sm_hash, 64);
  memcpy(scratchpad + 64, sanctum_sm_public_key, 32);
  // Sign (H_SM, PK_SM) with SK_D
  ed25519_sign(sanctum_sm_signature, scratchpad, 64 + 32, sanctum_dev_public_key, sanctum_dev_secret_key);

  // Clean up
  // Erase SK_D
  memset((void*)sanctum_dev_secret_key, 0, sizeof(*sanctum_dev_secret_key));

  // caller will clean core state and memory (including the stack), and boot.
  return;
}
