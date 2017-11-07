
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

//Register Pointers

//GPIO PORT F
#define RCGCPIO      (*((int *)0x400FE608)) //GPIO Run Mode Clock Gating Control
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


//TODO SET SYSTEM CLOCK FOR 20 MHz
#define SYSTEM_CLOCK  20000000               //System Clock Speed



//Port A Stuff
#define GPIO_A_DEN    (*((int *)0x4000451C)) //GPIO Port A Digital Enable
#define GPIO_A_AFSEL  (*((int *)0x40004420)) //GPIO Port A Alternate Function Select
#define GPIO_A_DIR    (*((int *)0x40004400)) //GPIO Port A Direction Select
#define GPIO_A_PCTL   (*((int *)0x4000452C)) //GPIO Port A Control

//TODO change offset for gpiodata
#define GPIO_A_DATA   (*((int *)0x40004000)) //GPIO Port A Data

#define GPIO_A_CR     (*((int *)0x40004524)) //GPIO Port A Commit Register
#define GPIO_A_ODR    (*((int *)0x4000450C)) //GPIO Port A Open Drain Select
















