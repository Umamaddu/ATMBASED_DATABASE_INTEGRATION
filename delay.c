#include "headers.h"

void delay_us(u32 delayus)
{
	delayus *=12;
	while(delayus--);
}
void delay_ms(u32 delayms)
{
		delayms *=12000;
		while(delayms--);
}
void delay_s(u32 delays)
{
		delays *= 12000000;
	  while(delays--);
}   
