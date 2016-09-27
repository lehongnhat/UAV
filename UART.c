//#include <xc.h>
#include<p33FJ256MC710.h>
#include "UART.h"

#define FOSC 20000000
#define FCY (FOSC/2)
#define BAUD 9600
#define U1BRGVAL ((FCY/BAUD/16)-1)
#define U2BRGVAL ((FCY/BAUD/16)-1)

void OpenUART1(void)
{
    unsigned int i;
    U1MODEbits.STSEL = 0; // 1 Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
    //U2MODEbits.PDSEL = 1; // even Parity, 8 data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U1MODEbits.BRGH = 0; // Low Speed mode
    U1BRG = U1BRGVAL;
    
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 0; // Enable UART TX Interrupt
    IEC0bits.U1RXIE = 1;
    U1STAbits.URXISEL = 0; // Interrupt after one RX character is received;
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
    /* wait at least 104 usec (1/9600) before sending first char */
    for(i = 0; i < 4200; i++){
        Nop();
    }
}

char BusyUART1(void)
{
    /*  flag for transission    */
    if (!IFS0bits.U1TXIF)
        return 1;
    else
    {
        IFS0bits.U1TXIF = 0; /* clear flag  */
        return 0;
    }
        
}

void PrintStringUART1(unsigned char *String)
{
    while (*String)
    {
        WriteCharToUART1(*String++);
    }
}

void WriteCharToUART1(unsigned char Data)
{
    while(U1STAbits.UTXBF);
    U1TXREG = Data;
}

unsigned char ReadCharUART1()
{
    return U1RXREG;
}

//UART2
void OpenUART2(void)
{
    unsigned int i;
    U2MODEbits.STSEL = 0; // 1 Stop bit
    U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
    //U2MODEbits.PDSEL = 1; // even Parity, 8 data bits
    U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U2MODEbits.BRGH = 0; // Low Speed mode
    U2BRG = U2BRGVAL;
    
    U2STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
    U2STAbits.UTXISEL1 = 0;
    IEC1bits.U2TXIE = 0; // Enable UART TX Interrupt
    IEC1bits.U2RXIE = 1;
    U2STAbits.URXISEL = 0; // Interrupt after one RX character is received;
    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART TX
    /* wait at least 104 usec (1/9600) before sending first char */
    for(i = 0; i < 4200; i++){
        Nop();
    }
}

void PrintStringUART2(char *String)
{
    while (*String)
    {
        WriteCharToUART2(*String++);
    }
}

void WriteCharToUART2(char Data)
{
    while(U2STAbits.UTXBF);
    U2TXREG = Data;
}

char ReadCharUART2()
{
    return U2RXREG;
}
