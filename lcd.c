#include "headers.h"

void WriteLCD(u8 data)
{
	IOPIN1=((IOPIN1&~(0XFF<<LCD_DATA))|(data<<LCD_DATA));
	IOCLR0=1<<LCD_RW;
	IOSET0=1<<LCD_EN;
	delay_ms(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
}
void CmdLCD(u8 cmd)
{
	IOCLR0=1<<LCD_RS;
	WriteLCD(cmd);
}
void CharLCD(u8 ascii)
{
	IOSET0=1<<LCD_RS;
	WriteLCD(ascii);
}
void InitLCD(void)
{
	IODIR1|=((u32)0xFF<<LCD_DATA);
	IODIR0|=(1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN);
	delay_ms(15);
	CmdLCD(0x30);
	delay_ms(2);
	delay_us(100);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}
void StrLCD(s8 *str)
{
	while(*str)
		CharLCD(*str++);
}
