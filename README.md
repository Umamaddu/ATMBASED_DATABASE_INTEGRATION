==========================
ATM SYSTEM WITH DATABASE
==========================

PROJECT TITLE:
ATM System Design with RFID Authentication and C-based Database Integration

--------------------------
OBJECTIVE:
--------------------------
To develop a secure ATM system using LPC2148 microcontroller and RFID authentication,
interfacing with a PC-side C program simulating a banking database using file handling and data structures.

--------------------------
HARDWARE REQUIREMENTS:
--------------------------
- LPC2148 Microcontroller
- RFID Reader and RFID Cards
- 16x2 LCD Display
- 4x4 Matrix Keypad
- MAX232 (UART level converter)
- USB-to-UART Converter / DB-9 Cable
- Buzzer

--------------------------
SOFTWARE REQUIREMENTS:
--------------------------
- Embedded C Programming
- KEIL uVision IDE
- FLASH MAGIC (Flashing tool)
- GCC Compiler (PC-side)

--------------------------
PROJECT STRUCTURE:
--------------------------
1. Microcontroller Side:
   - Handles user interaction via LCD and keypad.
   - Reads RFID via UART1.
   - Communicates with PC via UART0.

2. PC-Side Application:
   - Written in C using file handling and linked lists.
   - Maintains users.txt and transactions.txt files.
   - Validates card, processes transactions, logs data.

--------------------------
MICROCONTROLLER FUNCTIONS:
--------------------------
- Display system prompts and menus on LCD.
- Wait for RFID card input.
- Send data to PC in format:
  - `#CARD:12345678$`
  - `#CARD:12345678#PIN:4321$`
- Receive PC responses and update LCD:
  - Balance, deposit, withdraw, PIN change, mini-statement.
- Timeout after 30s of inactivity and reset.

--------------------------
PC-SIDE OPERATIONS:
--------------------------
- Parses incoming strings via UART.
- Validates user from `users.txt`.
- Supports actions:
  - Balance Enquiry
  - Deposit
  - Withdraw
  - PIN Change
  - Mini Statement (Last 3 transactions)
- Logs data in `transactions.txt`.
- Formats and sends response back:
  - `@OK#BALANCE:XXXX.XX$`
  - `@ERR#INVALID_PIN$`
  - `@ERR#INSUFFICIENT_BALANCE$`

--------------------------
SUPPORTED MESSAGE FORMATS:
--------------------------
From MCU to PC:
- `#CARD:12345678$`
- `#CARD:12345678#PIN:4321$`
- `#TXN:DEPOSIT#AMOUNT:1000$`
- `#TXN:BALANCE#REQ$`
- `#PINCHANGE#NEWPIN$`
- `#MINISTMT#REQ$`

From PC to MCU:
- `@OK#VALID_CARD$`
- `@OK#LOGIN_SUCCESS$`
- `@OK#BALANCE:1234.56$`
- `@OK#PINCHANGED$`
- `@OK#MINISTMT:Deposit-100@10:00|Withdraw-50@12:00|Deposit-200@14:00$`
- `@ERR#INVALID_PIN$`
- `@ERR#INSUFFICIENT_BALANCE$`

--------------------------
FILES INCLUDED:
--------------------------
MCU:
- lcd.c / lcd.h
- uart.c / uart.h
- keypad.c / keypad.h
- delay.c / delay.h
- projectmain.c

PC:
- atm.c (Main program)
- users.txt (User data)
- transactions.txt (Transaction logs)

--------------------------
NOTES:
--------------------------
- Ensure correct UART setup: RFID on UART1, PC communication on UART0.
- RFID reader sends 10-byte packets in ASCII format.
- Card number should match entries in users.txt to proceed.
- Timeout mechanism recommended using timer interrupts or counters.

--------------------------
AUTHOR / USAGE:
--------------------------
Developed for embedded systems training and academic use.
Modify or extend as per application requirements.

