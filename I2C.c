
#include<p33FJ256MC710.h>
#include <i2c.h>
#include "I2C1.h"

void Open_I2C1(void)
{
    OpenI2C1((I2C1_ON & I2C1_IDLE_CON & I2C1_CLK_HLD & I2C1_IPMI_DIS &
             I2C1_7BIT_ADD & I2C1_SLW_EN & I2C1_SM_DIS &
             I2C1_GCALL_DIS & I2C1_STR_DIS &
             I2C1_NACK & I2C1_ACK_DIS & I2C1_RCV_DIS &
             I2C1_STOP_DIS & I2C1_RESTART_DIS &
             I2C1_START_DIS), 0x4E);
    
    IdleI2C1();

}

void WriteByte_I2C1(unsigned char WriteAddressIC, unsigned char WriteAddressRegister, unsigned char Byte)
{
    ResetVariables_I2C();
    StartI2C1();
    WaitFlag();

      //Write Slave address and set master for transmission
    MasterWriteI2C1(WriteAddressIC);
      // Wait till address is transmitted
    WaitFlag();
    WaitForACK();
      
    MasterWriteI2C1(WriteAddressRegister);
    WaitFlag();
    WaitForACK();

    MasterWriteI2C1(Byte);
    WaitFlag();
    WaitForACK();
    ResetVariables_I2C();

    StopI2C1();
    WaitFlag();
    WaitForACK();
    IdleI2C1();
}

unsigned char ReadByte_I2C (unsigned char ReadAddressIC, unsigned char RegisterAddress)
{
    unsigned char Byte = 0;

    ResetVariables_I2C();
    StartI2C1();
    WaitFlag();

    //Write Slave address and set master for transmission
    MasterWriteI2C1(ReadAddressIC);
      
    WaitFlag();
    WaitForACK();

    MasterWriteI2C1(RegisterAddress);
    WaitFlag();
    WaitForACK();
    ResetVariables_I2C();
    RestartI2C1();
    WaitFlag();
    
    MasterWriteI2C1(ReadAddressIC + 1);
    WaitFlag();
    WaitForACK();
    ResetVariables_I2C();

    Byte = MasterReadI2C1();
    WaitFlag();
    ResetVariables_I2C();
    NotAckI2C1();
    WaitFlag();
    ResetVariables_I2C();
    StopI2C1();
    WaitFlag();
    IdleI2C1();

    return Byte;

}

void ResetVariables_I2C(void)
{
    I2C1CONbits.ACKEN=0;
    I2C1CONbits.PEN=0;
    I2C1CONbits.RCEN=0;
    I2C1CONbits.RSEN=0;
    I2C1CONbits.SEN=0;
}

void WaitFlag(void)
{
    while(!IFS1bits.MI2C1IF);           // wait for flag to be high
    IFS1bits.MI2C1IF=0;
}
void WaitForACK(void)
{
    while(I2C1STATbits.ACKSTAT);        // wait for ack receive from slave
}

