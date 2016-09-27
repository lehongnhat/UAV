/* 
 * File:   UART.h
 * Author: AcerV5
 *
 * Created on Ngày 04 tháng 4 n?m 2016, 17:16
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    void OpenUART1(void);
    char BusyUART1(void);
    void WriteCharToUART1(unsigned char Data);
    void PrintStringUART1(unsigned char *String);
    unsigned char ReadCharUART1(void);
    void OpenUART2(void);
    char BusyUART2(void);
    void WriteCharToUART2(char Data);
    void PrintStringUART2(char *String);    
    char ReadCharUART2(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

