/* 
 * File:   I2C.h
 * Author: AcerV5
 *
 * Created on Ngày 26 tháng 9 n?m 2016, 13:22
 */

#ifndef I2C1_H
#define	I2C1_H

#ifdef	__cplusplus
extern "C" {
#endif


    void Open_I2C1(void);
    void WriteByte_I2C1(unsigned char WriteAddressIC, unsigned char WriteAddressRegister, unsigned char Byte);
    unsigned char ReadByte_I2C (unsigned char ReadAddressIC, unsigned char RegisterAddress);
    void ResetVariables_I2C(void);
    void WaitFlag(void);
    void WaitForACK(void);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C1_H */

