
/*
EE 474, Embedded Systems
Lab 3 - Oct 28, 2017
Bartholomew Olson
*/

//Function Prototypes
void led_init();
void timer_init(int count);
void sw_init();
void timer_handler_init();
void Timer_Handler();
void Port_F_Handler();
void port_f_handler_init();
void pll_init();
void adc_init();
void ADC_Handler();
void i2c_init();
void set_slave_address(int slave_address);
void set_mode(int mode);
void write_byte(int data_byte, int conditions);

//Register Pointers

//GPIO PORT F
#define RCGCGPIO        (*((int *)0x400FE608)) //GPIO Run Mode Clock Gating Control
#define GPIO_F_DEN      (*((int *)0x4002551C)) //GPIO Digital Enable
#define GPIO_F_DIR      (*((int *)0x40025400)) //GPIO Direction
#define GPIO_F_DATA     (*((int *)0x400253FC)) //GPIO Data
#define GPIO_F_LOCK     (*((int *)0x40025520)) //GPIO Lock
#define GPIO_F_CR       (*((int *)0x40025524)) //GPIO Commit Register
#define GPIO_F_PUR      (*((int *)0x40025510)) //GPIO Pullup Resistor
 
//TIMER
#define RCGCTIMER    (*((int *)0x400FE604)) //RCGC Timer Register - use timer0, bit 0 of register
#define GPTMCTL      (*((int *)0x4003000C)) //Clock Control 16/32 bit timer-0
#define GPTMCFG      (*((int *)0x40030000)) //Clock Configure 16/32 bit timer-0
#define GPTMTAMR     (*((int *)0x40030004)) //Timer A Mode bit 4 = direction, 0 down, 1 up
#define GPTMIMR      (*((int *)0x40030018)) //GPTM interrupt mask
#define GPTMTAILR    (*((int *)0x40030028)) //GPTM Timer A interval Load
#define GPTMRIS      (*((int *)0x4003001C)) //GPTM raw interrupt status
#define GPTMICR      (*((int *)0x40030024)) //GPTM interrupt Clear Register

//INTERRUPT TABLE
#define NVIC         (*((int *)0xE000E100)) //Nested Vector Interrupt Controller Register
//#define NVIC         (*((int *)0xE000E040)) //Nested Vector Interrupt Controller Register


//GPIO INTERRUPT
#define GPIOIS       (*((int *)0x40025404)) //GPIO Interrupt Sense Register
#define GPIOIBE      (*((int *)0x40025408)) //GPIO Interrupt Both Edges
#define GPIOIEV      (*((int *)0x4002540C)) //GPIO Interrupt Event Register
#define GPIOIM       (*((int *)0x40025410)) //GPIO Interrupt Mask
#define GPIOICR      (*((int *)0x4002541C)) //GPIO Interrupt Clear
#define PRI0         (*((int *)0xE000E400)) //Interrupt 0-3 Priority
#define GPIORIS      (*((int *)0x40025414)) //GPIO raw interrupt status

//CLOCK SPEED
#define RCC          (*((int *)0x400FE060)) //Run-Mode Clock Configuration
#define RCC2         (*((int *)0x400FE070)) //Run-Mode Clock Configuration 2
#define RIS          (*((int *)0x400FE050)) //Raw Interrupt Status (for PLL?)

//ADC STUFF
#define RCGCADC       (*((int *)0x400FE638)) //ADC Converter Run Mode Clock Gating Control
#define GPIO_E_AFSEL  (*((int *)0x40024420)) //GPIO alternate function select (PORT E)
#define GPIO_E_DEN    (*((int *)0x4002451C)) //GPIO Digital Enable (PORT E) 
#define GPIO_E_AMSEL  (*((int *)0x40024528)) //GPIO Analog Mode Select
#define ADC_0_ACTSS   (*((int *)0x40038000)) //ADC Active Sample Sequence
#define ADC_0_EMUX    (*((int *)0x40038014)) //ADC Event Multiplexer Select
#define ADC_0_SSMUX3  (*((int *)0x400380A0)) //ADC Sample Sequence Input Multiplexer Select 0
#define ADC_0_SSCTL3  (*((int *)0x400380A4)) //ADC Sample Sequence Control 3
#define ADC_0_PSSI    (*((int *)0x40038028)) //ADC Processor Sample Sequence Initiate
#define ADC_0_RIS     (*((int *)0x40038004)) //ADC Raw Interrupt Status
#define ADC_0_SSFIFO3 (*((int *)0x400380A8)) //ADC Sample Sequence Result FIFO3
#define ADC_0_ISC     (*((int *)0x4003800C)) //ADC Interrupt Status and Clear
#define ADC_0_IM      (*((int *)0x40038008)) //ADC Interrupt Mask

//I2C Stuff
#define SCL_LP        6                      //SCL Low Period
#define SCL_HP        4                      //SCL High Period
#define SCL_CLK       100000                 //Desired I2C clock speed 100 Kbps

#define RCGCI2C       (*((int *)0x400FE620)) //I2C Run Mode Clock Gating Control
#define I2CMCR        (*((int *)0x40021020)) //I2C Master Configuration Register
#define I2CMTPR       (*((int *)0x4002100C)) //I2C Master Timer Period
#define I2CSOAR       (*((int *)0x40021800)) //Slave Own Address Register
#define I2CMSA        (*((int *)0x40021000)) //Master Slave Address
#define I2CMDR        (*((int *)0x40021008)) //I2C Master Data
#define I2CMCS        (*((int *)0x40021004)) //I2C Master Control/Status

#define MASTER_START    0x03
#define MASTER_CONTINUE 0x01
#define MASTER_STOP     0x05

//TODO SET SYSTEM CLOCK FOR 20 MHz
#define SYSTEM_CLOCK  20000000               //System Clock Speed

//Port A Stuff
#define GPIO_A_DEN    (*((int *)0x4000451C)) //GPIO Port A Digital Enable
#define GPIO_A_AFSEL  (*((int *)0x40004420)) //GPIO Port A Alternate Function Select
#define GPIO_A_DIR    (*((int *)0x40004400)) //GPIO Port A Direction Select
#define GPIO_A_PCTL   (*((int *)0x4000452C)) //GPIO Port A Control
#define GPIO_A_LOCK   (*((int *)0x40004520)) //GPIO Port A Lock
#define GPIO_A_LOCK_ALT (*((int *)0x40058520)) //GPIO Port A Lock AHB 
//TODO change offset for gpiodata
#define GPIO_A_DATA   (*((int *)0x40004000)) //GPIO Port A Data

#define GPIO_A_CR     (*((int *)0x40004524)) //GPIO Port A Commit Register
#define GPIO_A_ODR    (*((int *)0x4000450C)) //GPIO Port A Open Drain Select

#define GPIO_B_DEN    (*((int *)0x4000551C))
#define GPIO_B_DIR    (*((int *)0x40005400))
#define GPIO_B_LOCK   (*((int *)0x40005520))
#define GPIO_B_DATA   (*((int *)0x400053FC))
#define GPIO_B_CR     (*((int *)0x40005524))
#define GPIO_B_AFSEL  (*((int *)0x40005420))
#define GPIO_B_ODR    (*((int *)0x4000550C))

#define GPIOHBCTL     (*((int *)0x400FE06C)) //GPIO High Performance Bus Control

//PCA9685 Registers

#define MODE1         0x00
#define MODE2         0x01

#define SUBADR1       0x02
#define SUBADR2       0x03
#define SUBADR3       0x04

#define ALLCALLADR    0x05

#define LED0_ON_L     0x06
#define LED0_ON_H     0x07
#define LED0_OFF_L    0x08
#define LED0_OFF_H    0x09


#define LED1_ON_L     0x0A
#define LED1_ON_H     0x0B 
#define LED1_OFF_L    0x0C
#define LED1_OFF_H    0x0D

#define LED2_ON_L     0x0E
#define LED2_ON_H     0x0F
#define LED2_OFF_L    0x10
#define LED2_OFF_H    0x11


#define LED3_ON_L     0x12
#define LED3_ON_H     0x13
#define LED3_OFF_L    0x14
#define LED3_OFF_H    0x15


#define LED4_ON_L     0x16
#define LED4_ON_H     0x17
#define LED4_OFF_L    0x18
#define LED4_OFF_H    0x19


#define LED5_ON_L     0x1A
#define LED5_ON_H     0x1B
#define LED5_OFF_L    0x1C
#define LED5_OFF_H    0x1D


#define LED6_ON_L     0x1E
#define LED6_ON_H     0x1F
#define LED6_OFF_L    0x20
#define LED6_OFF_H    0x21


#define LED7_ON_L     0x22
#define LED7_ON_H     0x23
#define LED7_OFF_L    0x24
#define LED7_OFF_H    0x25


#define LED8_ON_L     0x26
#define LED8_ON_H     0x27
#define LED8_OFF_L    0x28
#define LED8_OFF_H    0x29


#define LED9_ON_L     0x2A
#define LED9_ON_H     0x2B
#define LED9_OFF_L    0x2C
#define LED9_OFF_H    0x2D


#define LED10_ON_L    0x2E
#define LED10_ON_H    0x2F
#define LED10_OFF_L   0x30
#define LED10_OFF_H   0x31


#define LED11_ON_L    0x32
#define LED11_ON_H    0x33
#define LED11_OFF_L   0x34
#define LED11_OFF_H   0x35


#define LED12_ON_L    0x36
#define LED12_ON_H    0x37
#define LED12_OFF_L   0x38
#define LED12_OFF_H   0x39

#define LED13_ON_L    0x3A
#define LED13_ON_H    0x3B
#define LED13_OFF_L   0x3C
#define LED13_OFF_H   0x3D


#define LED14_ON_L    0x3E
#define LED14_ON_H    0x3F
#define LED14_OFF_L   0x40
#define LED14_OFF_H   0x41


#define LED15_ON_L    0x42
#define LED15_ON_H    0x43
#define LED15_OFF_L   0x44
#define LED15_OFF_H   0x45

#define ALL_LED_ON_L  0xFA
#define ALL_LED_ON_H  0xFB
#define ALL_LED_OFF_L 0xFC
#define ALL_LED_OFF_H 0xFD

#define PRE_SCALE      0xFE
#define TESTMODE      0xFF





