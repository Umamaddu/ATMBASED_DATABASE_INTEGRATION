#ifndef HEADER
#define HEADER  // Define HEADER to prevent future re-inclusion

#include <LPC21xx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "delay.h"
#include "uart.h"
#include "uart_defines.h"
#include "others_functions.h"
#include "bank_functions.h"

#define LED 15
//#define DEBUG
#define MAX_BUF_LEN1 9
#define MAX_BUF_LEN2 100
#define TIMEOUT_FLAG 255  // Special value to indicate timeout
#define MAX_AMOUNT 30000

extern s8 pin[5], *givenpin;
extern u32 flag1, chances,attempts,pinchange;
extern s8 KPMLUT[][4];//={'7','8','9','*','4','5','6','/','1','2','3','-','c','0','=','+'};
extern s8 MENU[][16];



extern volatile s8 uart0_rx_buf[MAX_BUF_LEN2];
extern volatile u32 uart0_rx_index;
extern volatile u32 uart0_rx_complete;

extern volatile s8 uart1_rx_buf[MAX_BUF_LEN1];
extern volatile u32 uart1_rx_index;
extern volatile u32 uart1_rx_complete;
extern volatile u32 uart1_rx_started;
extern volatile u32 uart_checksum ;
extern u32 uart0_rx_valid;

#endif  // HEADER

