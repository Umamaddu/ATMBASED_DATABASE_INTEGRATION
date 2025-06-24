#include <LPC21xx.h>
#include <string.h>

//types.h
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;

//LCD DEFINES
#define CLEAR_LCD 0x01
#define SET_CUR_RIGHT 0x06
#define DISP_OFF 0x08
#define DISP_ON_CUR_OFF 0x0C
#define DISP_ON_CUR_ON  0x0E
#define DISP_ON_CUR_BLK 0x0F
#define MODE_8BIT_1LINE 0x30
#define MODE_4BIT_1LINE 0x20
#define MODE_8BIT_2LINE 0x38
#define MODE_4BIT_2LINE 0x28
#define GOTO_LINE1_POS0 0x80
#define GOTO_LINE2_POS0 0xC0
#define LCD_DATA 0
#define RS_PIN 8
#define RW_PIN 9
#define EN_PIN 10


//4X4 KEYPAD DEFINES
#define ROW0 16
#define ROW1 17
#define ROW2 18
#define ROW3 19
#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23

//RTC DEFINES
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define PREINT_VAL (int)((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT+1)*32768))
#define RTC_EN 0
#define RTC_RESET 1
#define RTC_CLKSRC 4

#define LED 18
#define ADC 19

//LCD DECLARATIONS
void Init_LCD(void);
void Write_LCD(u8);
void CmdLCD(u8);
void CharLCD(u8);
void StrLCD(u8*);
void U32LCD(u32);

//KEYPAD DECLARATIONS
void Init_KPM(void);
u32 ColScan(void);
u32 RowCheck(void);
u32 ColCheck(void);
u8  Read_key(void);
u8* Read_String(u32);

//DELAY DECLARATIONS 
void dealy_s(u32);
void delay_ms(u32);
void delay_us(u32);


//RTC DECLARATIONS
void RTC_Init(void);
void Display_RTC_Time(void);
void Set_RTC_Time(u32,u32,u32);
void Display_RTC_Date(void);
void Set_RTC_Date(u32,u32,u32);
void Display_RTC_Day(void);
void Set_RTC_Day(u32 );

//EINT0 EINT1 DECLARATIONS
void EXTINT_Init(void);
void EXTINT0_ISR(void)__irq;
void EXTINT1_ISR(void)__irq;

//CHECKING FUNCTIONS
void Check_Time(void);
void Edit_RTC(void);
void Edit_Schedule_Time(void);
u32 Read_num(void);
u8 Validate(u32,u8 *[]);
void New_Password(void);

u8 *MENU[]={"1:EDIT RTC","2:EDIT SCHEDULE","3:EDIT PASSWORD","4:EXIT"};
u8 *SCHEDULE[]={"1:STARTING TIME","2:ENDING TIME","3:EXIT"};
u8 *SCHEDULEHM[]={"1:EDIT HOUR","2:EDIT MIN","3:EXIT"};
u8 *RTC[]={"1:CHANGE HOUR","2:CHANGE MIN","3:CHANGE SEC","4:CHANGE DAY","5:CHANGE YEAR","6:CHANGE MONTH","7:CHANGE DATE","8:EXIT"};
u8 week_day[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
u8 KPMLUT[][4]={'7','8','9','*','4','5','6','/','1','2','3','-','c','0','=','+'};
u8 password[10]="1234",*givenpassword;
s32 chances,flag1=0,shs=9,ehe=18,sms=30,eme=30;
//main
main()
{
	IODIR0|=(1<<LED)|(1<<ADC);
	IOCLR0=1<<LED;//|(1<<ADC);
	Init_LCD();
	Init_KPM();
	RTC_Init();
	Set_RTC_Time(10,40,00);
	Set_RTC_Date(2024,4,1);
	Set_RTC_Day(3);
	EXTINT_Init();
	while(1)
	{
		Display_RTC_Time();
		Display_RTC_Day();
		Display_RTC_Date();

	}
}
void Init_LCD(void)
{
	IODIR0|=(0xFF<<LCD_DATA)|(1<<RS_PIN)|(1<<RW_PIN)|(1<<EN_PIN);
	delay_ms(15);
	CmdLCD(0x30);
	delay_ms(2);
	delay_us(100);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DISP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SET_CUR_RIGHT);
}
void Write_LCD(u8 data)
{
	IOPIN0=((IOPIN0&~(0xFF<<LCD_DATA))|(data<<LCD_DATA));
	IOSET0=1<<EN_PIN;
	delay_ms(1);
	IOCLR0=1<<EN_PIN;
	delay_ms(2);
}
void CmdLCD(u8 data)
{
	IOCLR0=1<<RS_PIN;
	Write_LCD(data);
}
void CharLCD(u8 data)
{
	IOSET0=1<<RS_PIN;
	Write_LCD(data);
}
void StrLCD(u8* str)
{
	while(*str)
	{
		CharLCD(*str++);
	}
}
void U32LCD(u32 num)
{
	s32 i=0;
	s8 arr[10];
	if(num==0)
	{
		CharLCD('0');
	}
	else
	{
		while(num>0)
		{
			arr[i++]=(num%10)+48;
			num/=10;
		}
		for(--i;i>=0;i--)
			CharLCD(arr[i]);
	}
}

//kpm definitions
void Init_KPM(void)
{
	IODIR1|=0xF<<ROW0;
}
u32 ColScan(void)
{
	return (((IOPIN1>>COL0)&0xF)<15)?0:1;
}
u32 RowCheck(void)
{
	u32 r;
	for(r=0;r<4;r++)
	{
		IOPIN1=((IOPIN1&~(0xF<<ROW0))|((~(1<<r))<<ROW0));
		if(ColScan()==0)
		{
			break;
		}
	}
	IOCLR1=0xF<<ROW0;
	return r;
}
u32 ColCheck(void)
{
	u32 c;
	for(c=0;c<4;c++)
	{
		if(((IOPIN1>>(COL0+c))&1)==0)
			break;
	}
	return c;
}
u8  Read_key(void)
{
	u32 row,col;
	while(ColScan());
	row=RowCheck();
	
	col=ColCheck();
	
	return KPMLUT[row][col];
}
u8* Read_String(u32 flag)
{
	static u8 arr[50];
	u8 c;
	s32 i=0;
	for(i=50;i>=0;i--)
		arr[i]='\0';
	i=0;
	while(1)
	{
		c=Read_key();
		if(c=='=')
		{
			arr[i]='\0';
			break;
		}
		else if(c=='c')
		{
			i--;
			CmdLCD(GOTO_LINE2_POS0+i);
			CharLCD(' ');
			arr[i]='\0';
			if(i<=0)
				i=0;
		}
		else
		{
			arr[i]=c;
			CmdLCD(GOTO_LINE2_POS0+i);
			CharLCD(c);
			delay_ms(100);
			CmdLCD(GOTO_LINE2_POS0+i);
			if(flag)
			CharLCD('*');
			else
				CharLCD(c);
			i++;
		}
		while((ColScan()&1)==0);
	}
	return arr;
}
//delay definitions
void delay_s(u32 delays)
{
	delays*=12000000;
	while(delays--);
}
void delay_ms(u32 delayms)
{
	delayms*=12000;
	while(delayms--);
}
void delay_us(u32 delayus)
{
	delayus*=12;
	while(delayus--);
}
//RTC DEFINITIONS 
void RTC_Init(void)
{
	CCR=1<<RTC_RESET;
	PREINT=PREINT_VAL;
	PREFRAC=PREFRAC_VAL;
	CCR=(1<<RTC_EN)|(1<<RTC_CLKSRC);
}
void Display_RTC_Time()
{
	CmdLCD(GOTO_LINE1_POS0);
	CharLCD((HOUR/10)+48);
	CharLCD((HOUR%10)+48);
	CharLCD(':');
	CharLCD((MIN/10)+48);
	CharLCD((MIN%10)+48);
	CharLCD(':');
	CharLCD((SEC/10)+48);
	CharLCD((SEC%10)+48);
}
void Set_RTC_Time(u32 hour,u32 min,u32 sec)
{
	HOUR=hour;
	MIN=min;
	SEC=sec;
}
void Display_RTC_Date()
{
	CmdLCD(GOTO_LINE2_POS0);
	U32LCD(YEAR);
	CharLCD('/');
	CharLCD((MONTH/10)+48);
	CharLCD((MONTH%10)+48);
	CharLCD('/');
	CharLCD((DOM/10)+48);
	CharLCD((DOM%10)+48);
}
void Set_RTC_Date(u32 year,u32 month,u32 day)
{
	YEAR=year;
	MONTH=month;
	DOM=day;
}
void Display_RTC_Day()
{
	CmdLCD(GOTO_LINE1_POS0+10);
	StrLCD(week_day[DOW]);
}
void Set_RTC_Day(u32 day)
{
	DOW=day;
}
//EXTINT DEFIITIONS
void EXTINT_Init(void)
{
	PINSEL1=((PINSEL1&~(3<<(2*0)))|(1<<(2*0)));
	PINSEL0=((PINSEL0&~(3<<(2*14)))|(2<<(2*14)));	
	VICIntEnable=(1<<14)|(1<<15);
	VICVectCntl0=(1<<5)|14;
	VICVectAddr0=(u32)EXTINT0_ISR;
	VICVectCntl1=(1<<5)|15;
	VICVectAddr1=(u32)EXTINT1_ISR;
	EXTMODE=(1<<0)|(1<<1);
}
void EXTINT0_ISR(void)__irq
{
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("ENTER PASSWORD:");
		for(chances=3;chances>0;chances--)
		{
			givenpassword=Read_String(1);
				if(strcmp((const char *)password,(const char *)givenpassword)==0)
				{
					flag1=1;
					Check_Time();
					break;
				}
				else
				{
					IOSET0=1<<LED;//|(1<<ADC);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("WRONG PASSWORD!");
					delay_ms(500);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("                ");
					IOCLR0=1<<LED;//|(1<<ADC);
				}
	}
			if(!flag1)
			{
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("                ");
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("OUT OF MOVES");
					delay_ms(300);
			}
		EXTINT=1<<0;
		VICVectAddr=0;
		CmdLCD(CLEAR_LCD);
}
void EXTINT1_ISR(void)__irq
{
	u8 c;
	while(1)
	{
		CmdLCD(CLEAR_LCD);		
		c=Validate(2,MENU);
		CmdLCD(CLEAR_LCD);
		if(c=='1')
		{
			CmdLCD(CLEAR_LCD);
			Edit_RTC();
			CmdLCD(CLEAR_LCD);
			break;
		}
		else if(c=='2')
		{
			CmdLCD(CLEAR_LCD);
			Edit_Schedule_Time();
			CmdLCD(CLEAR_LCD);
			break;
		}
		else if(c=='3')
		{
			CmdLCD(CLEAR_LCD);
			New_Password();
			CmdLCD(CLEAR_LCD);			
		}
		else if(c=='4')
		{
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD("   EXITED !");
			delay_ms(300);
			CmdLCD(CLEAR_LCD);
			break;
		}
		else
		{
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD("WRONG INPUT!");
			delay_ms(300);
			CmdLCD(CLEAR_LCD);	
		}
	}
		EXTINT=1<<1;
		VICVectAddr=0;
		CmdLCD(CLEAR_LCD);
}
void Check_Time()
{
	if((HOUR>=shs)&&(HOUR<=ehe))
	{
					CmdLCD(CLEAR_LCD);
					CmdLCD(GOTO_LINE1_POS0);
					StrLCD("DOOR IS OPENED");
					delay_ms(500);	
	}
	else
	{
					CmdLCD(CLEAR_LCD);
					CmdLCD(GOTO_LINE1_POS0);
					StrLCD("INVALID TIME");
					delay_ms(500);
	}
	CmdLCD(CLEAR_LCD);
}
void Edit_RTC()
{
	s32 num;
	u8 c;
	while(1)
	{
		c=Validate(6,RTC);
		CmdLCD(CLEAR_LCD);
		if(c)
		{
			if(c=='1')
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("ENTER HOUR");
				num=Read_num();
				if((num>=0)&&(num<=23))
				{
							HOUR=num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("   UPDATED ");
							delay_ms(100);
				}
				else 
				{
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("INVLAID HOUR");
				delay_ms(200);
				}
			}
			else if(c=='2')
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("ENTER MINUTES");
				num=Read_num();
				if((num>=0)&&(num<=59))
				{
							MIN=num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("   UPDATED ");
							delay_ms(100);					
				}
				else 
				{
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("INVLAID MIN");
					delay_ms(200);
				}
			}
			else if(c=='3')
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("ENTER SECONDS");
				num=Read_num();
				if((num>=0)&&(num<=59))
				{
							SEC=num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("   UPDATED ");
							delay_ms(100);					
				}
				else 
				{
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("INVLAID SEC");
					delay_ms(200);
				}
			}
			else if(c=='4')
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("ENTER WEEK DAY");
				num=Read_num();
				if((num>=0)&&(num<=6))
				{
							DOW=num;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("   UPDATED ");
							delay_ms(100);					
				}
				else 
				{
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("INVLAID DAY");
					delay_ms(200);
				}
			}
			else if(c=='5')
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("ENTER YEAR");
				num=Read_num();
				if(num==2025)
				{
					YEAR=num;
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("   UPDATED ");
					delay_ms(100);					
				}
				else 
				{
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("INVLAID YEAR");
					delay_ms(200);
				}
			}
			else if(c=='6')
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("ENTER MONTH");
				num=Read_num();
				if((num>=1)&&(num<=12))
				{
					MONTH=num;
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("   UPDATED ");
					delay_ms(100);					
				}
				else 
				{
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("INVLAID MONTH");
					delay_ms(200);
				}
			}
			else if(c=='7')
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("ENTER DATE");
				num=Read_num();
				if((num>=1)&&(num<=31))
				{
					DOM=num;
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("   UPDATED ");
					delay_ms(100);					
				}
				else 
				{
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("  INVLAID DAY  ");
					delay_ms(200);
				}
			}
			else if(c=='8')
			{
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("    EXITED !   ");
				delay_ms(300);
				break;			
			}
		}
	}
}
void Edit_Schedule_Time()
{
	u8 c;
	s32 start=0,end=0;
	while(1)
	{
		c=Validate(1,SCHEDULE);
		CmdLCD(CLEAR_LCD);
		if(c=='1')
		{
			while(1)
			{
					c=Validate(1,SCHEDULEHM);
					CmdLCD(CLEAR_LCD);
						if(c=='1')
						{
								CmdLCD(GOTO_LINE1_POS0);
								StrLCD("ENTER HOUR");	
								start=Read_num();
								if((start>=0)&&(start<=23))
								{
											if(ehe==start)
											{
													CmdLCD(GOTO_LINE2_POS0);
													StrLCD("START,END TIME EQUAL");
													delay_ms(200);
													CmdLCD(GOTO_LINE2_POS0);
													StrLCD("                ");													
											}
											else 
											{		
													shs=start;
													CmdLCD(GOTO_LINE2_POS0);
													StrLCD("UPDATED SUCESSFULLY");
													delay_ms(200);
													CmdLCD(GOTO_LINE2_POS0);
													StrLCD("                ");												
											}
								}
								else 
								{
										CmdLCD(GOTO_LINE2_POS0);
										StrLCD("INVLAID HOUR");
										delay_ms(200);
											CmdLCD(GOTO_LINE2_POS0);
											StrLCD("                ");									
								}			
						}
						else if(c=='2')
						{
								CmdLCD(GOTO_LINE1_POS0);
								StrLCD("ENTER MIN");	
								start=Read_num();
								if((start>=0)&&(start<=59))
								{
											sms=start;
											CmdLCD(GOTO_LINE2_POS0);
											StrLCD("UPDATED SUCESSFULLY");
											delay_ms(200);
											CmdLCD(GOTO_LINE2_POS0);
											StrLCD("                ");									
								}
								else 
								{
											CmdLCD(GOTO_LINE2_POS0);
											StrLCD("INVLAID MINUTES");
											delay_ms(200);
											CmdLCD(GOTO_LINE2_POS0);
											StrLCD("                ");										
								}			
						}
						else if(c=='3')
						{
									CmdLCD(CLEAR_LCD);
									CmdLCD(GOTO_LINE1_POS0);
									StrLCD("    EXITED !");
									delay_ms(200);
									break;
						}
					}
				}
				else if(c=='2')
				{
						while(1)
						{
								c=Validate(1,SCHEDULEHM);
								CmdLCD(CLEAR_LCD);
								if(c=='1')
								{
										CmdLCD(GOTO_LINE1_POS0);
										StrLCD("ENTER HOUR");	
										end=Read_num();
										if((end>=0)&&(end<=23))
										{
													if(shs==end)
													{
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("START,END TIME EQUAL");									
															delay_ms(200);	
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("                ");																							
													}
													else 
													{
															ehe=end;
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("UPDATED SUCESSFULLY");
															delay_ms(200);
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("                ");																							
													}
										}
										else 
										{
												CmdLCD(GOTO_LINE2_POS0);
												StrLCD("INVLAID HOUR");
												delay_ms(200);
												CmdLCD(GOTO_LINE2_POS0);
												StrLCD("                ");											
										}			
								}
								else if(c=='2')
								{
												CmdLCD(GOTO_LINE1_POS0);
												StrLCD("ENTER MINUTES");	
												end=Read_num();
												if((end>=0)&&(end<=59))
												{
															eme=end;
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("UPDATED SUCESSFULLY");
															delay_ms(200);
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("                ");														
												}
												else 
												{
														CmdLCD(GOTO_LINE2_POS0);
														StrLCD("INVLAID MINUTES");
														delay_ms(200);
															CmdLCD(GOTO_LINE2_POS0);
															StrLCD("                ");														
												}			
								}
								else if(c=='3')
								{
												CmdLCD(GOTO_LINE1_POS0);
												StrLCD("    EXITED !");
												delay_ms(200);
												break;
								}
						}
				}
				else if(c=='3')
				{
							CmdLCD(CLEAR_LCD);
							CmdLCD(GOTO_LINE1_POS0);
							StrLCD("    EXITED !");
							delay_ms(200);
							break;
				}
	}
CmdLCD(CLEAR_LCD);
}
u32 Read_num()
{
	u8 *str;
	u32 i,num=0;
	str=Read_String(0);
	for(i=0;str[i]!='\0';i++)
	{
		num=(num*10)+(str[i]-48);
	}
	return num;
}
u8 Validate(u32 j,u8 *str[])
{
			u32 i=0;
			u8 c;
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD(str[i]);
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD(str[i+1]);
			while(1)
			{
				c=Read_key();
				while((ColScan()&1)==0);
				if(c=='*')
				{
					if(i<j)
						i++;
					else 
						continue;
				}
				else if(c=='/')
				{
					if(i>0)
						i--;
					else 
						continue;
				}
				else if((c==str[i][0])||(c==str[i+1][0]))
				{
					break;
				}
				CmdLCD(CLEAR_LCD);		
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD(str[i]);
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD(str[i+1]);
			}
			return c;
}
void New_Password()
{
	u8 *oldpassword,newpassword[6];
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("ENTER OLD PASSWORD");
				oldpassword=Read_String(0);
				CmdLCD(CLEAR_LCD);
				if(strcmp((const char *)password,(const char *)oldpassword)==0)
				{
					delay_ms(50);
					CmdLCD(GOTO_LINE1_POS0);
					StrLCD("ENTER NEW PASSWORD");	
					oldpassword=Read_String(0);
					strcpy((char *)newpassword,(const char *)oldpassword);
					delay_ms(50);
					CmdLCD(CLEAR_LCD);	
					CmdLCD(GOTO_LINE1_POS0);
					StrLCD("RE ENTER NEW PASSWORD");	
					oldpassword=Read_String(0);
					CmdLCD(CLEAR_LCD);
					delay_ms(50);
					if(strcmp((const char *)oldpassword,(const char *)newpassword)==0)
					{
						strcpy((char *)password,(const char *)oldpassword);
						CmdLCD(GOTO_LINE1_POS0);
						StrLCD("SUCCEFULLY UPDATED");
						delay_ms(300);
						CmdLCD(CLEAR_LCD);
					}
					else
					{
						CmdLCD(GOTO_LINE1_POS0);
						StrLCD("WRONG NEW PASSWORD!");
						delay_ms(500);
						CmdLCD(GOTO_LINE1_POS0);
						StrLCD("                ");
					}
				}
				else
				{
					CmdLCD(GOTO_LINE1_POS0);
					StrLCD("WRONG OLD PASSWORD!");
					delay_ms(500);
					CmdLCD(GOTO_LINE1_POS0);
					StrLCD("                ");
				}
}
