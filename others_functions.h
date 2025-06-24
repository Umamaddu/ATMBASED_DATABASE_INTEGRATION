#include "headers.h"

s8* Read_String(void);
u8 Read_key(void);
s8* Read_PIN(u32);
void Display_Wel(void);
u32  validate_RFID(void);
u32  validate_PIN(void);
void recv_str_from_uart1(s8 *,u32 );
void recv_buf_from_uart0(s8 *,u32);
void send_buf_to_uart0(s8 *);
void rotate_string(const char *,int ,u32 );
void main_MENU(void);
s8 	 Validate(u32 ,s8 (*)[]);
u32 str_cmp(s8 *,s8 *);
u32 str_ncmp(s8 *,s8 *,u32 );
u32 str_len(s8 *);
int atoi_arm(s8*);
