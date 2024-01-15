/*
   Simple machine mode timer driver for RISC-V standard timer.
   SPDX-License-Identifier: Unlicense

   (https://five-embeddev.com/) 

*/

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>


#ifndef __riscv_xlen
	#define __riscv_xlen 64	//jefferry
#endif


#ifndef size_t
	#define size_t	int
#endif


//#define RISCV_MTIMECMP_ADDR (0x2000000 + 0x4000)
//#define RISCV_MTIME_ADDR    (0x2000000 + 0xBFF8)
#define RISCV_MTIMECMP_ADDR (0x18000000 + 0x4000)
#define RISCV_MTIME_ADDR    (0x18000000 + 0xBFF8)

#ifndef MTIME_FREQ_HZ
// Timer for HiFive board
#define MTIME_FREQ_HZ 32768
#endif

#define MTIMER_SECONDS_TO_CLOCKS(SEC)           \
    ((uint64_t)(((SEC)*(MTIME_FREQ_HZ))))

#define MTIMER_MSEC_TO_CLOCKS(MSEC)           \
    ((uint64_t)(((MSEC)*(MTIME_FREQ_HZ))/1000))

#define MTIMER_USEC_TO_CLOCKS(USEC)           \
    ((uint64_t)(((USEC)*(MTIME_FREQ_HZ))/1000000))



/** Set the raw time compare point in system timer clocks.
 * @param clock_offset Time relative to current mtime when 
 * @note The time range of the 64 bit timer is large enough not to consider a wrap around of mtime.
 * An interrupt will be generated at mtime + clock_offset.
 * See http://five-embeddev.com/riscv-isa-manual/latest/machine.html#machine-timer-registers-mtime-and-mtimecmp
 */
void mtimer_set_raw_time_cmp(uint64_t clock_offset);

/** Read the raw time of the system timer in system timer clocks
 */
uint64_t mtimer_get_raw_time(void);


#if __riscv_xlen==32 
typedef uint32_t uint_xlen_t;
typedef uint32_t uint_csr32_t;
typedef uint32_t uint_csr64_t;
#elif __riscv_xlen==64
typedef uint64_t uint_xlen_t;
typedef uint32_t uint_csr32_t;
typedef uint64_t uint_csr64_t;
#else
#error "Unknown XLEN"
#endif
            

/*******************************************
 * mcause - MRW - Machine Exception Cause 
 */
static inline uint_xlen_t csr_read_mcause(void) {
    uint_xlen_t value;        
    __asm__ volatile ("csrr    %0, mcause" 
                      : "=r" (value)  /* output : register */
                      : /* input : none */
                      : /* clobbers: none */);
    return value;
}

/* Register CSR bit set and clear instructions */
static inline void csr_set_bits_mie(uint_xlen_t mask) {
    __asm__ volatile ("csrrs    zero, mie, %0"  
                      : /* output: none */ 
                      : "r" (mask)  /* input : register */
                      : /* clobbers: none */);
}

/* Register CSR bit set and clear instructions */
static inline void csr_set_bits_mstatus(uint_xlen_t mask) {
    __asm__ volatile ("csrrs    zero, mstatus, %0"  
                      : /* output: none */ 
                      : "r" (mask)  /* input : register */
                      : /* clobbers: none */);
}

static inline void csr_write_mtvec(uint_xlen_t value) {
    __asm__ volatile ("csrw    mtvec, %0" 
                      : /* output: none */ 
                      : "r" (value) /* input : from register */
                      : /* clobbers: none */);
}

#define MCAUSE_INTERRUPT_BIT_MASK     (0x1UL << ((__riscv_xlen-1)))


#endif // #ifdef TIMER_H

