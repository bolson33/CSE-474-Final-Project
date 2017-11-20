/*
EE 474, Embedded Systems
Final Project - Nov 3, 2017
Bartholomew Olson
Fizza Aslam
Nguyen Lai
Jake Robinson
*/

#include "defines.h"

int main()
{  
  i2c_init();
  i2c_init_PCA9685();
  
  while(1)
  {
    set_servo(0, 0);
    for(int idx = 0; idx < 10000; idx++){}
    set_servo(0, 5);
    for(int idx = 0; idx < 10000; idx++){}
  }
  return(0);
}



/*   If shit breaks, this still should work
    //2ms pulse
  write_byte(LED0_ON_H, 0x0F);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED0_ON_L, 0x86);
  for(int idx = 0; idx < 1000; idx++){}

  write_byte(LED0_OFF_H, 0x00);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED0_OFF_L, 0x7A);
  for(int idx = 0; idx < 1000; idx++){}

  write_byte(LED1_ON_H, 0x0F);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED1_ON_L, 0x86);
  for(int idx = 0; idx < 1000; idx++){}

  write_byte(LED1_OFF_H, 0x00);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED1_OFF_L, 0x7A);
  for(int idx = 0; idx < 1000; idx++){}
  
  for(double idx = 0; idx < 500000; idx++){}
  
  //1ms pulse
  write_byte(LED0_ON_H, 0x0F);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED0_ON_L, 0xC3);
  for(int idx = 0; idx < 1000; idx++){}

  write_byte(LED0_OFF_H, 0x00);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED0_OFF_L, 0x3D);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED1_ON_H, 0x0F);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED1_ON_L, 0xC3);
  for(int idx = 0; idx < 1000; idx++){}

  write_byte(LED1_OFF_H, 0x00);
  for(int idx = 0; idx < 1000; idx++){}
  
  write_byte(LED1_OFF_L, 0x3D);
  for(int idx = 0; idx < 1000; idx++){}
  for(double idx = 0; idx < 500000; idx++){}
*/
