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
  int position = 0;
  while(1)
  {
    set_servo(0, position);
    for(int idx = 0; idx < 3000000; idx++){}
    ++position;
    if(position > 9)
    {
      position = 0;
    }
  }
  return(0);
}
