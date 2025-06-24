#include "headers.h"

s8* Read_String() {
    static s8 arr[50];
    u8 c;
    s32 i = 0;
    
    for (i = 49; i >= 0; i--) 
        arr[i] = '\0';

    i = 0;
    while (1) {
        c = Read_key();
        
        if (c == TIMEOUT_FLAG)
		{  // Timeout occurred
            return NULL; // Exit without returning partial input
        }

		if (c == '=')
		{
            arr[i] = '\0';
            break;
        } 
		else if (c == 'c') 
		{
            i--;
            CmdLCD(GOTO_LINE2_POS0 + i);
            CharLCD(' ');
            arr[i] = '\0';
            if (i <= 0)
                i = 0;
        }
		else if ((c >= '0') && (c <= '9'))
		{
            arr[i] = c;
            CmdLCD(GOTO_LINE2_POS0 + i);
            CharLCD(c);
            delay_ms(50);
            CmdLCD(GOTO_LINE2_POS0 + i);
            CharLCD(c);
            i++;
        }

        while ((ColScan() & 1) == 0);
				delay_ms(100);
    }
    return arr;
}

s8* Read_PIN(u32 flag) {
    static s8 arry[5];
    u8 c;
    s32 i = 0;

    for (i = 4; i >= 0; i--) 
        arry[i] = '\0';

    i = 0;
    while (i < 4) {
        c = Read_key();

        if (c == TIMEOUT_FLAG) {  // Timeout occurred
            return NULL; // Exit without returning partial input
        }

        if (c == 'c') 
				{
            i--;
            CmdLCD(GOTO_LINE2_POS0 + i);
            CharLCD(' ');
            arry[i] = '\0';
            if (i <= 0)
                i = 0;
        } 
				else if ((c >= '0') && (c <= '9'))
				{
         			   arry[i] = c;
         			   CmdLCD(GOTO_LINE2_POS0 + i);
         			   CharLCD(c);
         			   delay_ms(100);
          			   CmdLCD(GOTO_LINE2_POS0 + i);
						if(flag)
						CharLCD('*');
						else
						CharLCD(c);
            i++;
        }

        while ((ColScan() & 1) == 0);
				delay_ms(100);
    }
    
    arry[i] = '\0';
    return arry;
}
u32 validate_PIN(void)
{
	s8 buffer[30],reference_pin[10];;
	
		for(chances=3;chances>0;chances--)
		{
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD((s8 *)"ENTER PIN:");
			givenpin=Read_PIN(1);
			if(givenpin==NULL)
			{ 
							CmdLCD(CLEAR_LCD);
							CmdLCD(GOTO_LINE1_POS0);
							StrLCD((s8*)"SESSION TIMED OUT ! ");
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("PLEASE TRY AGAIN ");
							delay_s(1);
							CmdLCD(CLEAR_LCD);
							send_buf_to_uart0((s8*)"#TIMEOUT$");
							return 0;
			}
			strcpy((char *)reference_pin,(char *)givenpin);
#ifdef DEBUG
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD(reference_pin);
#endif
			sprintf((char*)buffer,"#p%s$",givenpin);
#ifdef DEBUG
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD(buffer);
			delay_ms(30);
			send_buf_to_uart0(buffer);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD("ENTER UART0 BUFF");
#endif 
			send_buf_to_uart0(buffer);
			recv_buf_from_uart0(buffer,sizeof(buffer));
			delay_s(1);
				if(str_cmp(buffer,(s8*)"@OK$")==0)
				{	

					CmdLCD(CLEAR_LCD);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD((s8*)"LOGIN SUCESSFULL ");
					delay_s(1);
					strcpy((char *)pin,(char *)reference_pin);
					flag1=1;
					CmdLCD(CLEAR_LCD);
					return 1;
				}
				else if(str_cmp(buffer,(s8*)"@ERR#INVLD$")==0)
				{
					IOSET0=1<<LED;//|(1<<ADC);
					CmdLCD(CLEAR_LCD);
					CmdLCD(GOTO_LINE1_POS0);
					StrLCD("INVALID PIN ");
					if(chances==3)
					{
						CmdLCD(GOTO_LINE2_POS0);
						StrLCD("YOU HAVE 2 MOVES ");
						delay_s(1);
					}
					else if(chances==2)
					{
						CmdLCD(GOTO_LINE2_POS0);
						StrLCD("YOU HAVE 1 MOVE ");
						delay_s(1);
					}
				}
				CmdLCD(CLEAR_LCD);
		}
		if(!flag1)
		{
				IOSET0=1<<LED;
				CmdLCD(CLEAR_LCD);
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD((s8 *)"                ");
				StrLCD("OUT OF MOVES ");
				send_buf_to_uart0((s8*)"#BLK$");
				recv_buf_from_uart0(buffer,sizeof(buffer));	
				if(str_cmp(buffer,(s8*)"@BLK$")==0)
				{
					CmdLCD(CLEAR_LCD);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("YOUR CARD BLOCKED ");		
					delay_s(1);
					StrLCD((s8 *)"                ");
					return 0;
				}		
					IOCLR0=1<<LED;				
		}
		return 0;
}
u32  validate_RFID()
{
	s8 buf1[10];
	s8 buf2[30];
#ifdef DEBUG
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("ENTER UART1 BUFF");
#endif
		recv_str_from_uart1(buf1,10);
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD((s8*)"RFID NO:");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD(buf1);
		delay_s(2);
#ifdef DEBUG
	memset((void*)uart1_rx_buf,0,MAX_BUF_LEN1);
	recv_str_from_uart1(buf1,9);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(buf1);
#endif
	sprintf((char*)buf2,"#r%s$",buf1);
#ifdef DEBUG
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD(buf2);
	delay_ms(200);
	send_buf_to_uart0(buf2);
#endif
	send_buf_to_uart0(buf2);
#ifdef DEBUG
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("ENTER UART0 BUFF");
#endif 
	recv_buf_from_uart0(buf2,30);
	CmdLCD(GOTO_LINE2_POS0);
	//StrLCD(buf2);
	delay_ms(200);
#ifdef DEBUG
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("TESTING UART0");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD(buf2);
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(buf2);
	delay_ms(10);
#endif
	if(str_cmp(buf2,(s8 *)"@OK#ACTV$")==0)
	{
		return 1;
	}
	else if(str_cmp(buf2,(s8*)"@BLKD$")==0)
	{
			IOSET0=1<<LED;
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD((s8 *)"CARD BLOCKED !");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("CONTACT MAIN BRANCH");
			delay_s(1);
			CmdLCD(CLEAR_LCD);
			IOCLR0=1<<LED;
	}
	else if(str_cmp(buf2,(s8*)"@ERR#CARD$")==0)
	{
			IOSET0=1<<LED;
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			StrLCD((s8 *)"ERROR");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("INVALID CARD DETAILS ");
		  delay_s(1);
			CmdLCD(CLEAR_LCD);
			IOCLR0=1<<LED;
	}
	return 0;
}
void rotate_string(const char *str,int len,u32 stop)
{
	s32 i,j,pos;
	s8 buffer[16];
	while(1)
	{
		for(i=-16;i<len;i++)
		{
			for(j=0;j<16;j++)
			{
				pos=i+j;
				if(pos<0||pos>=len)
				{
					buffer[j]=' ';
				}
				else
				{
					buffer[j]=str[pos];
				}
				if(uart1_rx_complete)
				{
					break;
				}
			}
			if(uart1_rx_complete)
			{
				CmdLCD(CLEAR_LCD);
				break;
			}
			buffer[j]='\0';
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD(buffer);
			delay_ms(200);
		}
		if(uart1_rx_complete)
		{
			break;
		}
		else if(stop)
		{
			break;
		}
	}
}
void main_MENU()
{
	s8 c;
		while(1)
		{
			CmdLCD(CLEAR_LCD);		
			c=Validate(4,MENU);
			CmdLCD(CLEAR_LCD);
			if(c==NULL)
			{
							CmdLCD(CLEAR_LCD);
							CmdLCD(GOTO_LINE1_POS0);
							StrLCD((s8*)"SESSION TIMED OUT ! ");
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("PLEASE TRY AGAIN ");
							delay_s(1);
							CmdLCD(CLEAR_LCD);
							send_buf_to_uart0((s8*)"#TIMEOUT$");
							break;
			}
			if(c=='1')
			{
				CmdLCD(CLEAR_LCD);
				Bal_Enq();
				CmdLCD(CLEAR_LCD);
			}
			else if(c=='2')
			{
				CmdLCD(CLEAR_LCD);
				if(Deposit())
				{
					break;
				}
				CmdLCD(CLEAR_LCD);
			}
			else if(c=='3')
			{
				CmdLCD(CLEAR_LCD);
				if(Withdraw())
				{
					break;
				}
				CmdLCD(CLEAR_LCD);
			}
			else if(c=='4')
			{
				CmdLCD(CLEAR_LCD);
				if(Pin_Change())
				{
					break;
				}
				CmdLCD(CLEAR_LCD);			
			}
			else if(c=='5')
			{
				CmdLCD(CLEAR_LCD);
				Mini_Statement();
				CmdLCD(CLEAR_LCD);			
			}
			else if(c=='6')
			{
				CmdLCD(CLEAR_LCD);
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD((s8 *)"   EXITED !");
				delay_s(2);
				CmdLCD(CLEAR_LCD);
				send_buf_to_uart0((s8*)"#EXIT$");
				break;
			}
			else
			{
				CmdLCD(CLEAR_LCD);
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD((s8 *)"WRONG INPUT!");
				delay_s(2);
				CmdLCD(CLEAR_LCD);	
			}
	}
		CmdLCD(CLEAR_LCD);
}

s8 Validate(u32 j,s8 (*str)[16])
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
				if(c==TIMEOUT_FLAG)
				{
					return NULL;
				}
				else if(c=='*')
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
void recv_str_from_uart1(s8 *str,u32 len)
{
#ifdef DEBUG
	    strcpy((char*)str,"12345678");	
#else
    while(!uart1_rx_complete);
    strncpy((char*)str,(char *)uart1_rx_buf,len > MAX_BUF_LEN1 ? MAX_BUF_LEN1 : len);
#endif 
    uart1_rx_complete=0;
}

void recv_buf_from_uart0(s8 *str,u32 len)
{	
		memset((void*)uart0_rx_buf,0,MAX_BUF_LEN2);
		while(!uart0_rx_complete);
    strncpy((char*)str,(char *)uart0_rx_buf,len);
		uart0_rx_complete=0;
}

void send_buf_to_uart0(s8 *str)
{
	s8 buf1[14]="#OK:CNTD$",buf2[14];
	
			while(1)
			{
				U0_TxStr(buf1);
				recv_buf_from_uart0(buf2,14);
				if(str_cmp(buf2,(s8*)"@OK:CNTD$")==0)
				{
					break;
				}
				else
				{
					CmdLCD(CLEAR_LCD);		
					CmdLCD(GOTO_LINE1_POS0);
					StrLCD((s8*)"WAITING FOR RESPONSE");
				}
			}
			U0_TxStr(str);
}/*
void send_buf_to_uart0(s8 *str) 
{
    s8 buf1[14] = "#OK:CNTD$", buf2[14];

    while (1) 
    {
        uart0_rx_complete = 0;  // Reset flag before transmission
        uart0_rx_valid = 0;  // Ensure fresh validation

        U0_TxStr(buf1);  // Send handshake message

        while (!uart0_rx_complete);  // Wait for response

        recv_buf_from_uart0(buf2, 14);  // Read response

        // Retry if message is incorrect
        if (uart0_rx_valid == 1 && str_cmp(buf2,"@OK:CNTD$") == 0) 
        {
            break;  // Stop retransmitting once correct response arrives
        }
        else 
        {
            CmdLCD(CLEAR_LCD);
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD((s8 *)"WAITING FOR RESPONSE");
            
            // Continue looping until response is valid
        }
    }

    // Once handshake is successful, send actual data
    U0_TxStr(str);
}*/
// Function to compare two strings
u32 str_cmp(s8 *str1, s8 *str2) {
    while (*str1 && (*str1 == *str2))
		{
        str1++;
        str2++;
    }
    return (u32)(*(unsigned char *)str1 - *(unsigned char *)str2);
}

// Function to compare two strings up to a given number of characters
u32 str_ncmp(s8 *str1, s8 *str2, u32 n) {
    while (n-- && *str1 && (*str1 == *str2)) 
		{
        str1++;
        str2++;
    }
    return n == (u32)-1 ? 0 : (u32)(*(unsigned char *)str1 - *(unsigned char *)str2);
}
u32 str_len(s8 *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
int atoi_arm(s8 *str)
{
    int result = 0;
    while (*str)
    {
        if (*str >= '0' && *str <= '9') 
            result = result * 10 + (*str - '0');  // Convert ASCII to integer
        else 
            return -1;  // Invalid input
        str++;
    }
    return result;
}
