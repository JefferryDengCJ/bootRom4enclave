/*

	test ddr capacity. Tue 14 Nov 2023 12:11:56 PM CST , Jefferry.

*/


#include <stdint.h>






#define BUFFER_LEN_		1024
#define MAX_DDR_SIZE_IN_BYTE	((uint64_t)2*1024*1024*1024)	/* 1G BYTES *2  */

/*
	example: 
	uint64_t TestDDR(uint64_t u64_DDRBaseAdd);
	uint64_t divisor =  TestDDR(0x80001000);

*/

uint64_t TestDDR(uint64_t u64_DDRBaseAdd)
{
	uint8_t u8Buf[BUFFER_LEN_];
	uint8_t *p8 = (uint8_t*)u64_DDRBaseAdd;
	uint64_t ddrLen = 0;	
	int i = 0;

	do{
		
		// memcpy(u8Buf,p8, BUFFER_LEN_);
		// memset(p8, 0xaa, BUFFER_LEN_ );			
		for(i = 0; i < BUFFER_LEN_; i++)
		{
			u8Buf[i] = p8[i];
			p8[i]      = 0xaa;	// 0b 1010 1010	
		}
		
		for(i = 0; i < BUFFER_LEN_; i++)
		{	
			if(0xaa == p8[i]){
				ddrLen ++;
				p8[i] = u8Buf[i];
			}
			else i = BUFFER_LEN_+1;		//set exit flag.	
		}
		p8 += BUFFER_LEN_;
	}while( (ddrLen < MAX_DDR_SIZE_IN_BYTE) && (i != BUFFER_LEN_+1) );




	return ddrLen;	/* RETURN DDR IN BYTES. */

}
