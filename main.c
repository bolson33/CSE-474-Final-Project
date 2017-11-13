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
  int register_byte = 0x00;
  int data_byte = 0x00;
  while(1)
  {
    write_byte(register_byte, data_byte);
    ++register_byte;
    ++data_byte;
  }
  return(0);
}