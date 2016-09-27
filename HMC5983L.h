/* 
 * File:   HMC5983L.h
 * Author: AcerV5
 *
 * Created on Ngày 26 tháng 9 n?m 2016, 23:12
 */

#ifndef HMC5983L_H
#define	HMC5983L_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned long make_word(unsigned char HB, unsigned char LB);           
void HMC5883L_init();                                                  
unsigned char HMC5883L_read(unsigned char reg); 
void HMC5883L_write(unsigned char reg_address, unsigned char value); 
void HMC5883L_read_data(); 
void HMC5883L_scale_axes(); 
void HMC5883L_set_scale(float gauss); 
float HMC5883L_heading(); 


#ifdef	__cplusplus
}
#endif

#endif	/* HMC5983L_H */

