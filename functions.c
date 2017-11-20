
/*
EE 474, Embedded Systems
Lab 3 - Oct 28, 2017
Bartholomew Olson
*/

#include "defines.h"

//ISR for Timer setup function
void timer_handler_init(void)
{
  GPTMIMR = 0x11; //set the TATOIM bit, time out interrupt mask
  NVIC |= (1 << 19); //set interrupt 19 to trigger
}

//ISR for Timer 
void Timer_Handler(void)
{
  GPTMICR = 0x01; //clear timer interrupt
  ADC_0_PSSI |= 0x08; //Start a conversion Sequence 
}

//ISR for Port F (sw1 and 2) setup
void port_f_handler_init(void)
{
  GPIOIS &= ~0x11; //Make bit 4, 0 edge sensitive
  GPIOIBE &= ~0x11; //Trigger is controlled by IEV
  GPIOIEV = ~0x11; //Falling Edge Trigger
  GPIOICR |= 0x11; //Clear any interrupts
  GPIOIM |= 0x11; //Unmask Interrupt
  NVIC |= (1 << 30); //enable IRQ30
  PRI0 = (3 << 21); //Set interrupt priority to 3
}

//ISR for Port F (sw 1 and 2)
void Port_F_Handler(void)
{
   int sw_one = 0x10;
   int sw_two = 0x01;

   int count_4m = 0x003D0900;
   int count_80m = 0x4C4B400;
  if(GPIORIS & sw_one) //check for sw 1 interrupt set
  {
    RCC2 = (RCC2 & ~0x1FC00000) + (99 << 22); //set pll to 4 MHz
    timer_init(count_4m);
  }
  else if(GPIORIS & sw_two) //check for sw 2 interrupt set
  {
    RCC2 = (RCC2 & ~0x1FC00000) + (4 << 22);  //set pll to 80 MHz
    timer_init(count_80m);
  }
  GPIOICR |= 0x11; //Clear any interrupts
}

//initialization for Port F leds and switches
void led_init()
{
  int port_f_enable = 0x20;
  int all_colors = 0x0E;
  
  //RCGCGPIO |= port_f_enable; //enable Port F GPIO
  //RCGCGPIO = 0x1D;
  RCGCGPIO |= 0x20;
  for(int idx = 0; idx < 1000; idx++){};
  //GPIO_F_LOCK = 0x4C4F434B; //enable GPIO_F_LOCK to access other registers
  //GPIO_F_CR = 0xFF; //turn on all commit registers
  GPIO_F_DIR = all_colors; //set Port F bits 3:1 as outputs, 4, 0 as inputs
  GPIO_F_DEN = 0x1F; // enable digital PORT F
  GPIO_F_DATA = 0; //clear all PORT F
};

//initialization for timer
void timer_init(int count)
{
  RCGCTIMER = 0x01; //set timer0 bit to 0
  GPTMCTL = 0x00; //disable timer
  GPTMCFG = 0x00000000; //select 32 bit timer
  GPTMTAMR = 0x02; //Set timer A mode to count down, timer type to periodic
  GPTMTAILR = count; //Set Timer A interval to 4 million clock cycles
  GPTMCTL |= 0x01; //Enable counter to start counting
};

//initialization for switches
void sw_init()
{
  GPIO_F_PUR = 0x11; //turn on pullups for PF0 and PF4
  GPIO_F_DEN = 0x1F; // enable digital PORT F
  GPIO_F_DATA = 0; //clear all PORT F
};

//initialization for phase-lock-loop
void pll_init()
{
  RCC = 0x00;
  RCC2 |= 0x80000000; //Use RCC2
  RCC2 |= 0x00000800; //System clock is derived from osc source
  RCC = (RCC & ~0x000007C0) + 0x00000540; //Clear XTAL field, bits 10-6 10101, configure for 16 MHz Crystal
  RCC2 &= ~0x00000070; //configure for main oscillator source
  RCC2 &= ~0x00002000; //Activate PLL by clearing PWRDN
  RCC2 |= 0x40000000; //Use 400 MHz PLL
  RCC2 = (RCC2 & ~0x1FC00000) + (99 << 22); //Clear system clock divider, configure for 4 MHz clock
  while((RIS & 0x00000040) == 0){}; //wait for PLLRIS bit
  RCC2 &= ~0x00000800; //enable use of PLL by clearing BYPASS
}

//initialization for the ADC
void adc_init()
{
  RCGCGPIO |= 0x10; //Enable Port E
  NVIC |= (1 << 17); //set interrupt 17 to trigger (ADC)
  
  GPTMCTL |= 0x10; //output Timer A ADC trigger is enabled
  RCGCADC |= 0x01; //Enable clock to ADC0
  for(int i = 0; i < 1000; i++){}; //Delay a lil' bit
  GPIO_E_AFSEL |= 0x08; //enable alternate function
  GPIO_E_DEN &= ~0x08; //Disable digital function
  GPIO_E_AMSEL |= 0x08; //Enable analog function
  ADC_0_ACTSS &= ~0x08; //Disable SS3 during configuration
  ADC_0_EMUX &= ~0xF000; //Software Trigger Conversion
  //ADC_0_SSMUX3 = 0x00; //get input from channel 0
  ADC_0_SSCTL3 |= (1<<3); //read from internal temperature sensor
  ADC_0_SSCTL3 |= 0x06; //take one sample at a time, set flag at first sample
  ADC_0_ACTSS |= 0x08; //enable ADC0 sequencer 3
  ADC_0_IM |= (1 << 3); //Interrupt Mask
}

//ADC Interrupt Handler
void ADC_Handler()
{
  int red = 0x13;
  int blue = 0x15;
  int violet = 0x17;
  int green = 0x19;
  int yellow = 0x1B;
  int l_blue = 0x1D;
  int white = 0x1F;
  int result = 0;
  float temp = 0;
  
  while((ADC_0_RIS & 0x08) == 0){}; //wait for conversion complete
  result = ADC_0_SSFIFO3; //Read the conversion result
    
  temp = 147.5 - ((75 * 3.3 * result ) / 4096);
  
  if(temp >= 0 && temp <= 16)
  {
    GPIO_F_DATA = red;
  }
  else if(temp >= 17 && temp <= 18)
  {
    GPIO_F_DATA = blue;
  }
  else if(temp >= 19 && temp <= 20)
  {
    GPIO_F_DATA = violet;
  }
  else if(temp >= 21 && temp <= 22)
  {
    GPIO_F_DATA = green;
  }
  else if(temp >= 23 && temp <= 24)
  {
    GPIO_F_DATA = yellow;
  }
  else if(temp >= 25 && temp <= 26)
  {
    GPIO_F_DATA = l_blue;
  }
  else if(temp >= 27 && temp <= 40)
  {
    GPIO_F_DATA = white;
  }
  ADC_0_ISC = 0x08; //clear completion flag
}

void i2c_init()
{  
  RCGCI2C = (1<<1); //Using I2C Module 1
  RCGCGPIO |= (1<<0); //Enable and provide a clock to GPIO Port A
  GPIO_A_AFSEL = (1<<6)|(1<<7); //Set Port A pins 6,7 to alternate function
  GPIO_A_DEN = (1<<6)|(1<<7); //Enable digital signals for Port A pin 6,7
  GPIO_A_ODR = (1<<7); //Set Port A pin 6,7 to open drain
  GPIO_A_PCTL = 0x33000000; //Set GPIO Port A pin 6,7 to be I2C
  I2CMCR = (1<<4); //Enable Master Mode
  I2CMTPR = 0x0F; //Clock speed at 100 Kbps

}


void i2c_init_PCA9685()
{
  write_byte(MODE1, 0x10); //Put chip to sleep
  for(int idx = 0; idx < 10000; idx++){}

  //CA for 30 Hz 
  write_byte(PRE_SCALE, 0xCA);
  for(int idx = 0; idx < 1000; idx++){}

  write_byte(MODE1, 0x81); //Put into Restart mode, AllCall enabled
  for(int idx = 0; idx < 10000; idx++){}
  
  write_byte(MODE2, 0x05); //configure with totem-pole structure
  for(int idx = 0; idx < 1000; idx++){}
}


void write_byte(int register_byte, int data_byte)
{
  while((I2CMCS & (1<<0)));
  
  //TODO Unsure of slave address - 0x06, 0x20, 0x40?
  I2CMSA = WRITE; //Set master slave address to write
  I2CMDR = register_byte; //load master data register with register_byte
  I2CMCS = MASTER_START; //set master control register to start followed by Transmit
  
  while((I2CMCS & (1<<0))); //wait until not busy

  I2CMDR = data_byte;
  I2CMCS = MASTER_STOP;
}

//Sets one of 9 servos to one of 6 positions - 0 is full right, 5 is full left
void set_servo(int servo, int position)
{
  //servo 0-8, position 0-5
  int servo_on_H[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  int servo_on_L[] = {0xF6, 0xDD, 0xC5, 0xAC, 0x93, 0x7B};
  int servo_off_H[] = {0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F};
  int servo_off_L[] = {0x0A, 0x23, 0x3B, 0x54, 0x6D, 0x85};
    
  switch(servo)
  {
    case 0:
      write_byte(LED0_ON_H, servo_on_H[servo]);
      write_byte(LED0_ON_L, servo_on_L[servo]);
      write_byte(LED0_OFF_H, servo_off_H[servo]);
      write_byte(LED0_OFF_L, servo_off_L[servo]);
      break;
      
    case 1: 
      write_byte(LED1_ON_H, servo_on_H[servo]);
      write_byte(LED1_ON_L, servo_on_L[servo]);
      write_byte(LED1_OFF_H, servo_off_H[servo]);
      write_byte(LED1_OFF_L, servo_off_L[servo]);
      break;
      
    case 2:
      write_byte(LED2_ON_H, servo_on_H[servo]);
      write_byte(LED2_ON_L, servo_on_L[servo]);
      write_byte(LED2_OFF_H, servo_off_H[servo]);
      write_byte(LED2_OFF_L, servo_off_L[servo]);
      break;
      
    case 3:
      write_byte(LED3_ON_H, servo_on_H[servo]);
      write_byte(LED3_ON_L, servo_on_L[servo]);
      write_byte(LED3_OFF_H, servo_off_H[servo]);
      write_byte(LED3_OFF_L, servo_off_L[servo]);
      break;
      
    case 4:
      write_byte(LED4_ON_H, servo_on_H[servo]);
      write_byte(LED4_ON_L, servo_on_L[servo]);
      write_byte(LED4_OFF_H, servo_off_H[servo]);
      write_byte(LED4_OFF_L, servo_off_L[servo]);
      break;
      
    case 5:
      write_byte(LED5_ON_H, servo_on_H[servo]);
      write_byte(LED5_ON_L, servo_on_L[servo]);
      write_byte(LED5_OFF_H, servo_off_H[servo]);
      write_byte(LED5_OFF_L, servo_off_L[servo]);
      break;
      
    case 6:
      write_byte(LED6_ON_H, servo_on_H[servo]);
      write_byte(LED6_ON_L, servo_on_L[servo]);
      write_byte(LED6_OFF_H, servo_off_H[servo]);
      write_byte(LED6_OFF_L, servo_off_L[servo]);
      break;
      
    case 7:
      write_byte(LED7_ON_H, servo_on_H[servo]);
      write_byte(LED7_ON_L, servo_on_L[servo]);
      write_byte(LED7_OFF_H, servo_off_H[servo]);
      write_byte(LED7_OFF_L, servo_off_L[servo]);
      break;
      
    case 8:
      write_byte(LED8_ON_H, servo_on_H[servo]);
      write_byte(LED8_ON_L, servo_on_L[servo]);
      write_byte(LED8_OFF_H, servo_off_H[servo]);
      write_byte(LED8_OFF_L, servo_off_L[servo]);
      break;
      
    default:
      break;
  }
/*  This is important, don't delete it or I'll kill someone  
30 cycles per second
33.333 ms/cycle

position 5
1 ms pulse 
1/33.333 => 3% duty cycle
0.03 * 4096 = 122.88 => 123
4096 - 122.88 = 3973.12 => 3973
high time = 0x7B
low time = 0xF85

position 4
1.2 ms pulse
1.2/33.333 => 3.6% duty cycle
0.036 * 4096 = 147.456 => 147
4096 - 147.456 = 3948.544 => 3949
high time = 0x93
low time = 0xF6D

position 3
1.4 ms pulse
1.4/33.333 => 4.2% duty cycle
0.042 * 4096 = 172.032 => 172
4096 - 172.032 = 3923.968 => 3924
high time = 0xAC
low time = 0xF54

position 2
1.6 ms pulse
1.6/33.333 => 4.8% duty cycle
0.048 * 4096 = 196.608 => 197
4096 - 196.608 = 3899.392 => 3899
high time = 0xC5
low time = 0xF3B

position 1
1.8 ms pulse
1.8/33.333 => 5.4% duty cycle
0.054 * 4096 = 221.184 => 221
4096 - 221.184 = 3874.816 => 3875
high time = 0xDD
low time = 0xF23

position 0
2 ms pulse
2/33.333 => 6% duty cycle
0.06 * 4096 = 245.76 => 246
4096 - 245.76 = 3850.24 => 3850
high time = 0xF6
low time = 0xF0A
*/
}