#include "headers.h"

void Init_UART0(void);
void U0_TxByte(u8 );
u8 	 U0_RxByte(void);
void U0_TxStr(s8 *);
void U0_TxU32(u32 );
void U0_TxS32(s32 );
void U0_TxF32(f32 ,u32 );
s8*  U0_RxStr(void);
void U0_TxHex(u32 );
void UART0_ISR(void) __irq;

void Init_UART1(void);
void U1_TxByte(u8 );
u8 	 U1_RxByte(void);
void U1_TxStr(s8 *);
void U1_TxU32(u32 );
void U1_TxS32(s32 );
void U1_TxF32(f32 ,u32 );
s8*  U1_RxStr(void);
void U1_TxHex(u32 );
void UART1_ISR(void) __irq;
