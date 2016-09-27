#include<p33FJ256MC710.h>
#include <i2c.h>
#include <math.h>
#include "HMC5983L.h"

#define HMC5883L_READ_ADDR       0x3D                                            // Dia chi I2C cua thanh ghi READ
#define HMC5883L_WRITE_ADDR      0x3C                                            // Dia chi I2C cua thanh ghi WRITE  
                            
#define Config_Reg_A             0x00                                            // Dia chi thanh ghi A
#define Config_Reg_B             0x01                                            // Dia chi thanh ghi B
#define Mode_Reg                 0x02                                            // Dia chi thanh ghi Mode
#define X_MSB_Reg                0x03                                            // Dia chi thanh ghi luu 8 bit cao gia tri truc X
#define X_LSB_Reg                0x04                                            // Dia chi thanh ghi luu 8 bit thap gia tri truc X 
#define Z_MSB_Reg                0x05                                            // Dia chi thanh ghi luu 8 bit cao gia tri truc Z 
#define Z_LSB_Reg                0x06                                            // Dia chi thanh ghi luu 8 bit thap gia tri truc Z 
#define Y_MSB_Reg                0x07                                            // Dia chi thanh ghi luu 8 bit cao gia tri truc Y 
#define Y_LSB_Reg                0x08                                            // Dia chi thanh ghi luu 8 bit thap gia tri truc Y 
#define Status_Reg               0x09                    
#define ID_Reg_A                 0x0A              
#define ID_Reg_B                 0x0B                                       
#define ID_Reg_C                 0x0C 
                                                          
#define declination_angle     -1.47193                                           // Goc xich vi trong thien van hoc: Tim thong tin tâi:http://www.ngdc.noaa.gov/geomag-web/#declination                                                                                      
#define PI 3.14

signed long X_axis = 0; 
signed long Y_axis = 0;                                  
signed long Z_axis = 0; 
float m_scale = 1.0;         


/*******************************************************************************
Noi Dung    :   Ham ghep gia tri doc duoc tu thanh ghi - Ghep 2 gia tri 8 bits thanh 16 bits.
Tham Bien   :   HB: 8 bit cao, LB: 8 bit thap.
Tra Ve      :   gia tri do nghieng cua cac thanh ghi truc X,Y,Z.
********************************************************************************/
unsigned long make_word(unsigned char HB, unsigned char LB)
{                                      
   register unsigned long val = 0; 
                               
   val = HB; 
   val <<= 8;                          
   val |= LB;          
   return val; 
}                                  

/*******************************************************************************
Noi Dung    :   Khoi tao ban dau HMC5883L                                
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/ 
void HMC5883L_init() 
{                                        
   HMC5883L_write(Config_Reg_A, 0x70); 
   HMC5883L_write(Config_Reg_B, 0xA0); 
   HMC5883L_write(Mode_Reg, 0x00);  
   HMC5883L_set_scale(1.3);                                                      // Set gia tri Gauss la 1.3
} 
                          
/*******************************************************************************
Noi Dung    :   Doc du lieu gui ve tu cac thanh ghi du lieu 8 bit cua HMC5883L.
Tham Bien   :   reg    :   dia chi thanh ghi du lieu cua HMC5883L can doc gia tri ve.
Tra Ve      :   gia tri tri tu truong.
********************************************************************************/                                   
unsigned char HMC5883L_read(unsigned char reg) 
{                                          
   unsigned char val = 0; 
    
   StartI2C1(); 
   MasterWriteI2C1(HMC5883L_WRITE_ADDR); 
   MasterWriteI2C1(reg); 
   StartI2C1();
   MasterWriteI2C1(HMC5883L_READ_ADDR); 
   val = MasterReadI2C1();                        
   StopI2C1(); 
   return(val);    
} 

/******************************************************************************
Noi Dung    :   Ghi gia tri vao thanh ghi HMC5883L qua I2C.                                  
Tham Bien   :   reg_address: Dia chi thanh ghi can gia tri, value: du lieu ghi vao.
Tra Ve      :   Khong.
********************************************************************************/ 
void HMC5883L_write(unsigned char reg_address, unsigned char value) 
{ 
   StartI2C1(); 
   MasterWriteI2C1(HMC5883L_WRITE_ADDR); 
   MasterWriteI2C1(reg_address); 
   MasterWriteI2C1(value); 
   StopI2C1(); 
}                                            

/*******************************************************************************
Noi Dung    :   Doc du lieu gui ve tu cac thanh ghi du lieu 8 bit cua HMC5883L sau do ghep thanh du lieu 16 bit.
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/                                   
void HMC5883L_read_data() 
{                          
   unsigned char lsb = 0; 
   unsigned char msb = 0; 
    
   StartI2C1(); 
   MasterWriteI2C1(HMC5883L_WRITE_ADDR); 
   MasterWriteI2C1(X_MSB_Reg);            
   StartI2C1(); 
   MasterWriteI2C1(HMC5883L_READ_ADDR); 
    
   msb = MasterReadI2C1(); 
   lsb = MasterReadI2C1(); 
   X_axis = make_word(msb, lsb); 
                           
   msb = MasterReadI2C1(); 
   lsb = MasterReadI2C1(); 
   Z_axis = make_word(msb, lsb); 
                   
   msb = MasterReadI2C1(); 
   lsb = MasterReadI2C1(); 
   Y_axis = make_word(msb, lsb);            
                       
   StopI2C1();    
} 

/*******************************************************************************
Noi Dung    :   Ham set gia tri scale
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/                                   
void HMC5883L_scale_axes() 
{    
   X_axis *= m_scale; 
   Z_axis *= m_scale;                            
   Y_axis *= m_scale; 
} 

/*******************************************************************************
Noi Dung    :   Ham set gia tri Gauss
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/                                   
void HMC5883L_set_scale(float gauss)                      
{ 
   unsigned char value = 0;    
    
    if(gauss == 0.88)        
    {                                            
      value = 0x00;                      
      m_scale = 0.73; 
   }  
    
   else if(gauss == 1.3)    
   { 
      value = 0x01; 
      m_scale = 0.92;      
   }  
    
   else if(gauss == 1.9) 
   { 
      value = 0x02; 
      m_scale = 1.22; 
   } 
    
   else if(gauss == 2.5) 
   { 
      value = 0x03; 
      m_scale = 1.52; 
   }  
    
   else if(gauss == 4.0) 
   { 
      value = 0x04; 
      m_scale = 2.27; 
   } 
    
   else if(gauss == 4.7) 
   { 
      value = 0x05; 
      m_scale = 2.56; 
   }  
    
   else if(gauss == 5.6) 
   { 
      value = 0x06; 
      m_scale = 3.03; 
   }    
    
   else if(gauss == 8.1)                                  
   { 
      value = 0x07; 
      m_scale = 4.35;          
   }        
                                                
   value <<= 5; 
   HMC5883L_write(Config_Reg_B, value); 
}      
                   
/*******************************************************************************
Noi Dung    :   Doi gia tri goc tu RAD sang Do
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/                                                                  
float HMC5883L_heading()          
{ 
   register float heading = 0.0; 
    
   HMC5883L_read_data(); 
   HMC5883L_scale_axes(); 
   heading = atan2(Y_axis, X_axis); 
   heading += declination_angle;
                  
    if(heading < 0.0)         
    { 
      heading += (2.0 * PI);                         
    } 
    
    if(heading > (2.0 * PI))                
    {                            
      heading -= (2.0 * PI); 
    }                    
                    
   heading *= (180.0 / PI);                                                      // Doi goc tu RAD --> Do
                   
   return heading; 
}              

