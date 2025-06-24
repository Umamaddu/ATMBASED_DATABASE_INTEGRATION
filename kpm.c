#include "headers.h"

u32 ColScan()
{
	return (((IOPIN1>>COL0)&15)<15)?0:1;
}
u32 RowCheck()
{
	u32 r;
	for(r=0;r<4;r++)
	{
		IOPIN1=((IOPIN1&~(15<<ROW0))|((~(1<<r))<<ROW0));
		if(ColScan()==0)
		{
			break;
		}
	}
	IOCLR1=15<<ROW0;
	return r;
}
u32 ColCheck()
{
	u32 c;
	for(c=0;c<4;c++)
	{
		if((IOPIN1>>(COL0+c)&1)==0)
		{
			break;
		}
	}
	return c;
}

u8 Read_key(void) 
{
    u32 row, col;
    s32 timeout = 10000; // Approximate delay for 30 seconds
    while (ColScan() && timeout>0) 
		{
				timeout--;
        delay_ms(1); // Decrement timeout every millisecond
    }

    if (timeout == 0) 
		{
        return TIMEOUT_FLAG; // Indicate timeout occurred
    }

    row = RowCheck();
    col = ColCheck();

    return KPMLUT[row][col];
}
void InitKPM(void)
{
	IODIR1|=15<<ROW0;
}

