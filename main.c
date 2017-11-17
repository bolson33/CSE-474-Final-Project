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
  //led_init();
  i2c_init();

  write_byte(PRE_SCALE, 0x0F); //Set prescale to 4ms pulse-width
  write_byte(MODE1, 0x10); //Set sleep bit in mode1 to 1
  
  write_byte(LED0_ON_L, 0x00);
  write_byte(LED0_ON_H, 0x08);
  
  write_byte(LED0_OFF_L, 0x00);
  write_byte(LED0_OFF_H, 0x08);
  
  while(1)
  {

  }
  return(0);
}