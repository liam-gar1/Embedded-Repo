/*
// Basic MSP430 Hello World

#include "msp430G2553.h"

#define G_LED BIT6
#define R_LED BIT0

int i,j,delay;

void delay_time(int);
void delay_long(int);

void delay_time (int delay){      //----------------------------------------- Delay---------------------
     
     do delay--;
     while (delay != 0);
}

void delay_long (int val){            //--------------------------------------- Delay long------------------
  int j;                  
  j = 100;
  do{
    delay_time(5000);
    j--;
  }while (j != 0);
}

void main(void){
  WDTCTL = WDTPW + WDTHOLD; // disable watch dog timer
  DCOCTL = CALDCO_16MHZ;    // set internal oscillator at 16MHz
  BCSCTL1 = CALBC1_16MHZ;   // set internal oscillator at 16MHz

  P1OUT = 0x00;
  P2OUT = 0x00;
  P1DIR = 0xFF; // Set all as outputs
  P2DIR = 0xFF; // Set all as outputs

    for(;;) {
      P1OUT |= G_LED;      // Enable = 1                  
      delay_long(delay);
      P1OUT &= ~(G_LED);   // Disable = 0  
      delay_long(delay);                                  
    }                                
}*/

/*
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//drselim MSP430 & Multiple ADC
//Please don't forget to give credits while using the code
//video link for the description of the code:
//https://youtu.be/Ev871bhGFt0
char vt_chara0[5];
char vt_chara1[5];
char vt_chara2[5];
char vt_chara3[5];
char vt_chara4[5];
char vt_chara5[5];
char vt_chara6[5];
char vt_chara7[5];
char volta0[] = " A0:   ";
char volta1[] = " A1(NA): ";//used by UART no connection
char volta2[] = " A2(NA): ";//used by UART no connection
char volta3[] = " A3:   ";
char volta4[] = " A4:   ";
char volta5[] = " A5:   ";
char volta6[] = " A6:   ";
char volta7[] = " A7:   ";
char newline[] = " \r\n";
unsigned int adc[8];
void ser_output(char *str);
void main(void)
{
    ADC10CTL1 = INCH_7 + ADC10DIV_0 + CONSEQ_3 + SHS_0;
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON; //ADC10IE
    ADC10AE0 = BIT7 + BIT6 + BIT5 + BIT4 + BIT3 + BIT0;
    ADC10DTC1 = 8;


    while(1){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc;
       
        if (adc > 0)
        {
          // ADC value is above 0, do something
          P1OUT |= BIT0;  // Turn on the Green LED
        }
        else
        {
          // ADC value is 0 or below, do something else
          P1OUT &= ~(BIT6 + BIT5 + BIT4 + BIT0); //turn the others off
        }
   
        __delay_cycles(100000);
    }
    //drselim MSP430 & Multiple ADC
    //Please don't forget to give credits while using the code
    //video link for the description of the code:
    //https://youtu.be/Ev871bhGFt0
}
void ser_output(char *str){
    while(*str != 0){
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}*/

/*
#include <msp430.h>

void configure_ADC();
void configure_Timer();
void start_ADC_conversion();
void check_ADC_value();

volatile unsigned int adc_value = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
   
    P1DIR |= BIT6 + BIT5 + BIT4 + BIT0;  // Set P1.6, P1.5, P1.4, P1.3 as outputs (Green, Yellow, Red, Blue LEDs)

    __delay_cycles(100000);
    configure_ADC();
    configure_Timer();

    __bis_SR_register(GIE);    // Enable global interrupts

    while (1)
    {
        __no_operation();      // Placeholder for while-loop (low power mode)
    }
}

void configure_ADC()
{
    // Select ADC input pin P1.5 (A5)
    P1SEL |= BIT5;
    P1SEL2 |= BIT5;

    ADC10CTL1 = INCH_5;        // ADC input channel 5
    ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ADC10IE; // Vcc/Vss ref, 64 x ADC10CLKs, ADC on, enable ADC interrupt
}

void configure_Timer()
{
    // Configure Timer A
    TACCR0 = 32768 - 1;        // 1 second (32768 counts for 1 second at 32.768kHz)
    TACTL = TASSEL_1 | MC_1;   // ACLK, up mode
    TACCTL0 = CCIE;            // Enable Timer A interrupt
}

void start_ADC_conversion()
{
    ADC10CTL0 |= ENC | ADC10SC; // Start ADC conversion
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    start_ADC_conversion();
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    adc_value = ADC10MEM;      // Read ADC value
    check_ADC_value();
}

void check_ADC_value()
{
    if (adc_value > 0)
    {
        // ADC value is above 0, do something
        P1OUT |= BIT0;  // Turn on the Green LED
    }
    else
    {
        // ADC value is 0 or below, do something else
        P1OUT &= ~(BIT6 + BIT5 + BIT4 + BIT0); //turn the others off
    }
}*/

/*
#include <msp430.h>

#define THRESHOLD 0  // Example threshold value (e.g., corresponds to 1.65V if Vcc is 3.3V)

volatile unsigned int adc_value = 0;
volatile unsigned char is_above_threshold = 0;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
   
    P1DIR |= BIT6 + BIT5 + BIT4 + BIT0;  // Set P1.6, P1.5, P1.4, P1.3 as outputs (Green, Yellow, Red, Blue LEDs)

    // Configure ADC10
    ADC10CTL1 = INCH_5 + ADC10DIV_3;          // Channel 5 (A5), ADC10CLK/4
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE; // Vcc & Vss as reference, 64 clock ticks, ADC on, enable interrupt
    ADC10AE0 |= BIT5;                         // P1.5 ADC option select

    __enable_interrupt();                     // Enable global interrupts

    while (1) {
        ADC10CTL0 |= ENC + ADC10SC;           // Sampling and conversion start
        __bis_SR_register(CPUOFF + GIE);      // Enter LPM0 with interrupts enabled

        // Check if the voltage is above the threshold
        if (is_above_threshold) {
            // Voltage is above threshold, do something
            P1OUT |= BIT0;  // Turn on the Green LED
        } else {
            // Voltage is below or equal to threshold, do something else
            P1OUT &= ~(BIT6 + BIT5 + BIT4 + BIT0); //turn the others off
        }
    }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
    adc_value = ADC10MEM;                     // Read ADC value
    if (adc_value > THRESHOLD) {
        is_above_threshold = 1;               // Set flag if voltage is above threshold
    } else {
        is_above_threshold = 0;               // Clear flag if voltage is below or equal to threshold
    }
    __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR) to wake up the main loop
}*/

#include <msp430.h>
#include <stdint.h>

#define RX_BUF_SIZE 11
#define TX_BUF_SIZE 11

//unsigned char rx_buffer[RX_BUF_SIZE];
//unsigned char tx_buffer[TX_BUF_SIZE];

uint8_t rx_buffer[RX_BUF_SIZE];
uint8_t tx_buffer[TX_BUF_SIZE];

uint8_t flag=0;
uint8_t counter=0;

uint8_t motor_state_init=1;

int i,j,delay;

void delay_time(int);
void delay_long(int);

void delay_time (int delay){      //----------------------------------------- Delay---------------------
     
     do delay--;
     while (delay != 0);
}

void delay_long (int val){            //--------------------------------------- Delay long------------------
  int j;                  
  j = 100;
  do{
    delay_time(5000);
    j--;
  }while (j != 0);
}

//uint8_t expected_data[] = {0x7E, 0x01, 0x00, 0x43, 0x01, 0x0F, 0xE4, 0xC1, 0x3A, 0x00, 0x7E}; // Example hexadecimal array to compare against (stop command - frame number 1; will light up LED after pressing start and then stop
//uint8_t expected_data[] = {0x7E, 0x01, 0x00, 0x43, 0x01, 0xCF, 0x54, 0x03, 0x5E, 0x9B, 0x7E}; //medium command, frame number 1
//uint8_t expected_data[] = {0x7E, 0x00, 0x00, 0x43, 0x01, 0x9F, 0x10, 0x7B, 0x55, 0xCD, 0x7E}; //start command, frame number 0

//uint8_t expected_data[] = {0x7E, 0x00, 0x00, 0x43, 0x01, 0x9F, 0x10, 0x7B, 0x55, 0xCD, 0x7E}; // Example hexadecimal array to compare against
//uint8_t ACK[]={0x7E, 0x00, 0x00, 0x52, 0x01, 0xFF, 0x0F, 0xD3, 0x03, 0x9D, 0x7E}; //ACK response frame with 0x52 control byte and frame number=0
//uint8_t ACK[]={0x7E, 0x00, 0x00, 0x73, 0x01, 0xFF, 0xD8, 0xFF, 0x8C, 0xA4, 0x7E}; //ACK response frame with 0x73 control byte and frame number=0
//uint8_t expected_data[] = {0x7E, 0x67, 0x00, 0x43, 0x01, 0x0F, 0x09, 0x43, 0x48, 0x16, 0x7E}; //Example hexadecimal array to compare against
//uint8_t FrameTimeout[]={0x7E, 0x04, 0x00, 0x53, 0x04, 0x0F, 0x67, 0x00, 0xF0, 0xE5, 0x32, 0xB4, 0x36, 0x7E}; //control byte 0x53, frame number: 32381, embedded frame number: 32257


//uint8_t expected_data[] = {0x7E, 0x00, 0x00, 0x43, 0x01, 0x9F, 0x10, 0x7B, 0x55, 0xCD, 0x7E}; //start, FN=0
//uint8_t ACK[]={0x7E, 0x00, 0x00, 0x52, 0x01, 0xFF, 0x0F, 0xD3, 0x03, 0x9D, 0x7E};

//uint8_t expected_data[] = {0x7E, 0x01, 0x00, 0x43, 0x01, 0x0F, 0xE4, 0xC1, 0x3A, 0x00, 0x7E}; //stop, FN=1
//uint8_t ACK[]={0x7E, 0x01, 0x00, 0x52, 0x01, 0xFF, 0xBF, 0xFA, 0x63, 0xA0, 0x7E};

//uint8_t expected_data[] = {0x7E, 0x02, 0x00, 0x43, 0x01, 0x9F, 0x70, 0x28, 0x95, 0xB7, 0x7E}; //start, FN=2
//uint8_t ACK[]={0x7E, 0x02, 0x00, 0x52, 0x01, 0xFF, 0x6F, 0x80, 0xC3, 0xE7, 0x7E};

//uint8_t expected_data[] = {0x7E, 0x03, 0x00, 0x43, 0x01, 0xCF, 0x34, 0x50, 0x9E, 0xE1, 0x7E}; //med, FN=3
//uint8_t ACK[]={0x7E, 0x03, 0x00, 0x52, 0x01, 0xFF, 0xDF, 0xA9, 0xA3, 0xDA, 0x7E};

//----------------------------------------------------------------//

//uint8_t expected_data[] = {0x7E, 0x01, 0x00, 0x43, 0x01, 0xCF, 0x54, 0x03, 0x5E, 0x9B, 0x7E}; //med, FN=1
//uint8_t ACK[]={0x7E, 0x00, 0x00, 0x52, 0x01, 0xFF, 0x0F, 0xD3, 0x03, 0x9D, 0x7E};  //start pump, FN=0

//uint8_t expected_data[] = {0x7E,0x2E,0x00,0x43,0x01,0x0F,0x34,0xBB,0x9A,0x47,0x7E}; //stop, FN=2
//uint8_t ACK[]={0x7E,0x01,0x00,0x52,0x01,0xFF,0xBF,0xFA,0x63,0xA0,0x7E}; // FN=1

//uint8_t expected_data[] = {0x7E, 0x01, 0x00, 0x43, 0x01, 0x0F, 0xE4, 0xC1, 0x3A, 0x00, 0x7E}; //stop, FN=1
//uint8_t ACK[]={0x7E, 0x00, 0x00, 0x52, 0x01, 0xFF, 0x0F, 0xD3, 0x03, 0x9D, 0x7E};  //FN=0

//uint8_t expected_data[] = {0x7E,0x01,0x00,0x43,0x01,0x7F,0xD8,0xB0,0x3F,0x50,0x7E}; //calibrate, FN=1
//uint8_t ACK[]={0x7E,0x01,0x00,0x52,0x01,0x02,0x1E,0x74,0x6F,0x63,0x7E};  //calibration failed, FN=1

//----------------------------------------------------------------//

//uint8_t expected_data[] = {0x7E,0x00,0x00,0x43,0x01,0x20,0x0D,0xD5,0x8B,0x96,0x7E}; //stimulate pressure signal, FN=0
//uint8_t ACK[]={0x7E,0x00,0x00,0x52,0x01,0x10,0xE6,0x2C,0xB6,0xAD,0x7E};  //nevative pressure, FN=0

//----------------------------------------------------------------//

//uint8_t expected_data[] = {0x7E,0x01,0x00,0x43,0x01,0x30,0xD9,0xEC,0x5C,0xB6,0x7E}; //stimulate bubble signal, FN=0
//uint8_t ACK[]={0x7E,0x00,0x00,0x52,0x01,0x60,0xDA,0x5D,0xB3,0xFD,0x7E};  //bubble detected, FN=0

//----------------------------------------------------------------//

//uint8_t expected_data[] = {0x7E,0x00,0x00,0x43,0x01,0xC0,0x75,0x37,0x81,0x36,0x7E}; //stimulate insert cartridge, FN=0
//uint8_t ACK[]={0x7E,0x00,0x00,0x52,0x01,0xB6,0x3B,0x2A,0x03,0x92,0x7E};  //insert cartridge response, FN=0

//----------------------------------------------------------------//

//uint8_t expected_data[] = {0x7E,0x00,0x00,0x43,0x01,0xE0,0xBD,0x17,0xEF,0x0D,0x7E}; //stimulate release cartridge, FN=0
//uint8_t ACK[]={0x7E,0x00,0x00,0x52,0x01,0xA9,0xCE,0x27,0x0B,0x1F,0x7E};  //release cartridge response, FN=0

//----------------------------------------------------------------//

/*
uint8_t expected_data[] = {0x7E,0x01,0x00,0x43,0x01,0x9F,0xA0,0x52,0x35,0xF0,0x7E}; //start, FN=1
uint8_t ACK[]={0x7E,0x01,0x00,0x52,0x01,0xFF,0xBF,0xFA,0x63,0xA0,0x7E}; //ACK, FN=1

uint8_t expected_data1[] = {0x7E,0x01,0x00,0x43,0x01,0x9F,0xA0,0x52,0x35,0xF0,0x7E}; //start, FN=1
uint8_t ACK1[]={0x7E,0x01,0x00,0x52,0x01,0xFF,0xBF,0xFA,0x63,0xA0,0x7E}; //ACK, FN=1

uint8_t expected_data2[] = {0x7E,0x02,0x00,0x43,0x01,0xCF,0x84,0x79,0xFE,0xDC,0x7E}; //med, FN=2
uint8_t ACK2[]={0x7E,0x02,0x00,0x52,0x01,0xFF,0x6F,0x80,0xC3,0xE7,0x7E}; //ACK, FN=2

//uint8_t expected_data3[] = {0x7E,0x03,0x00,0x43,0x01,0x0F,0x84,0x92,0xFA,0x7A,0x7E}; //stop, FN=3
//uint8_t ACK3[]={0x7E,0x03,0x00,0x52,0x01,0xFF,0xDF,0xA9,0xA3,0xDA,0x7E}; //ACK, FN=3

//uint8_t expected_data3[] = {0x7E,0x04,0x00,0x43,0x01,0x0F,0x94,0x4E,0xDA,0xC8,0x7E}; //stop, FN=4
//uint8_t ACK3[]={0x7E,0x04,0x00,0x52,0x01,0xFF,0xCF,0x75,0x83,0x68,0x7E}; //ACK, FN=4

uint8_t expected_data3[] = {0x7E,0x02,0x00,0x43,0x01,0x0F,0x34,0xBB,0x9A,0x47,0x7E}; //stop, FN=2
uint8_t ACK3[]={0x7E,0x02,0x00,0x52,0x01,0xFF,0x6F,0x80,0xC3,0xE7,0x7E}; //ACK, FN=4*/

//----------------------------------------------------------------//
/*
uint8_t expected_data[] = {0x7E,0x00,0x00,0x43,0x01,0x9F,0x10,0x7B,0x55,0xCD,0x7E}; //start, FN=0
uint8_t ACK[]={0x7E,0x00,0x00,0x52,0x01,0xFF,0x0F,0xD3,0x03,0x9D,0x7E}; //ACK, FN=0

uint8_t expected_data1[] = {0x7E,0x00,0x00,0x43,0x01,0x9F,0x10,0x7B,0x55,0xCD,0x7E}; //start, FN=0
uint8_t ACK1[]={0x7E,0x00,0x00,0x52,0x01,0xFF,0x0F,0xD3,0x03,0x9D,0x7E}; //ACK, FN=0

uint8_t expected_data2[] = {0x7E,0x01,0x00,0x43,0x01,0xCF,0x54,0x03,0x5E,0x9B,0x7E}; //med, FN=1
uint8_t ACK2[]={0x7E,0x01,0x00,0x52,0x01,0xFF,0xBF,0xFA,0x63,0xA0,0x7E}; //ACK, FN=1

uint8_t expected_data3[] = {0x7E,0x02,0x00,0x43,0x01,0x0F,0x34,0xBB,0x9A,0x47,0x7E}; //stop, FN=2
uint8_t ACK3[]={0x7E,0x02,0x00,0x52,0x01,0xFF,0x6F,0x80,0xC3,0xE7,0x7E}; //ACK, FN=2*/

//----------------------------------------------------------------//

uint8_t expected_data[] = {0x7E,0x00,0x00,0x43,0x01,0x9F,0x10,0x7B,0x55,0xCD,0x7E}; //start, FN=0
uint8_t ACK[]={0x7E,0x00,0x00,0x52,0x01,0xFF,0x0F,0xD3,0x03,0x9D,0x7E}; //ACK, FN=0

uint8_t expected_data1[] = {0x7E,0x00,0x00,0x43,0x01,0x9F,0x10,0x7B,0x55,0xCD,0x7E}; //start, FN=0
uint8_t ACK1[]={0x7E,0x00,0x00,0x52,0x01,0xFF,0x0F,0xD3,0x03,0x9D,0x7E}; //ACK, FN=0

uint8_t expected_data2[] = {0x7E,0x01,0x00,0x43,0x01,0xdb,0x29,0xd7,0x84,0x81,0x7E}; //low, FN=1 (7e01004301db29d784817e)
uint8_t ACK2[]={0x7E,0x01,0x00,0x52,0x01,0xff,0xbf,0xfa,0x63,0xa0,0x7E}; //ACK, FN=1 (7e01005201ffbffa63a07e)

uint8_t expected_data3[] = {0x7E,0x02,0x00,0x43,0x01,0x0f,0x34,0xbb,0x9a,0x47,0x7E}; //stop, FN=2 (7e020043010f34bb9a477e)
uint8_t ACK3[]={0x7E,0x02,0x00,0x52,0x01,0xff,0x6f,0x80,0xc3,0xe7,0x7E}; //ACK, FN=2 (7e02005201ff6f80c3e77e)

uint8_t expected_data4[] = {0x7E,0x03,0x00,0x43,0x01,0x9f,0xc0,0x01,0xf5,0x8a,0x7E}; //start, FN=3 (7e030043019fc001f58a7e)
uint8_t ACK4[]={0x7E,0x03,0x00,0x52,0x01,0xff,0xdf,0xa9,0xa3,0xda,0x7E}; //ACK, FN=3 (7e03005201ffdfa9a3da7e)

uint8_t expected_data5[] = {0x7E,0x04,0x00,0x43,0x01,0xcf,0x24,0x8c,0xbe,0x53,0x7E}; //med, FN=4 (7e04004301cf248cbe537e)
uint8_t ACK5[]={0x7E,0x04,0x00,0x52,0x01,0xff,0xcf,0x75,0x83,0x68,0x7E}; //ACK, FN=4 (7e04005201ffcf7583687e)

uint8_t expected_data6[] = {0x7E,0x05,0x00,0x43,0x01,0x0f,0x24,0x67,0xba,0xf5,0x7E}; //stop, FN=5 (7e050043010f2467baf57e)
uint8_t ACK6[]={0x7E,0x05,0x00,0x52,0x01,0xff,0x7f,0x5c,0xe3,0x55,0x7E}; //ACK, FN=5 (7e05005201ff7f5ce3557e)

uint8_t expected_data7[] = {0x7E,0x06,0x00,0x43,0x01,0x9f,0xb0,0x8e,0x15,0x42,0x7E}; //start, FN=6 (7e060043019fb08e15427e)
uint8_t ACK7[]={0x7E,0x06,0x00,0x52,0x01,0xff,0xaf,0x26,0x43,0x12,0x7E}; //ACK, FN=6 (7e06005201ffaf2643127e)

uint8_t expected_data8[] = {0x7E,0x07,0x00,0x43,0x01,0xab,0xb5,0x53,0xc1,0x5e,0x7E}; //high, FN=7 (7e07004301abb553c15e7e)
uint8_t ACK8[]={0x7E,0x07,0x00,0x52,0x01,0xff,0x1f,0x0f,0x23,0x2f,0x7E}; //ACK, FN=7 (7e07005201ff1f0f232f7e)

uint8_t expected_data9[] = {0x7E,0x08,0x00,0x43,0x01,0x0f,0x95,0xa3,0x2a,0x0d,0x7E}; //stop, FN=8 (7e080043010f95a32a0d7e)
uint8_t ACK9[]={0x7E,0x08,0x00,0x52,0x01,0xff,0xce,0x98,0x73,0xad,0x7E}; //ACK, FN=8 (7e08005201ffce9873ad7e)

uint8_t expected_data10[] = {0x7E,0x00,0x00,0x43,0x01,0xa9,0x89,0xee,0xef,0x02,0x7E}; //release cartridge, FN=0 (7e00004301a989eeef027e)
uint8_t ACK10[]={0x7E,0x00,0x00,0x52,0x01,0xff,0x0f,0xd3,0x03,0x9d,0x7E}; //ACK, FN=0 (7e00005201ff0fd3039d7e)

void init_uart() {
    // Configure UART
    P1SEL = BIT1 + BIT2;          // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2;         // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;         // Use SMCLK
    UCA0BR0 = 26;                // 1MHz - set to 104 for 9600 and 8 for 115200
    UCA0BR1 = 0;                  // 1MHz 115200 or 9600.. 0 in both cases
    UCA0MCTL |= UCBRS2 + UCBRS0; // Modulation UCBRSx = 1 //this was UCA0MCTL = UCBRS0; when baud was 9600
    UCA0CTL1 &= ~UCSWRST;         // Initialize USCI state machine
    IE2 |= UCA0RXIE;            // Enable USCI_A0 RX interrupt
}

void read_and_write_message() {
    // Read 9-byte message from RX buffer
    unsigned int i;
    int adjustedSize=0;
    for (i = 0; i < RX_BUF_SIZE; i++) {
        while (!(IFG2 & UCA0RXIFG)); // Wait until a byte is received
        rx_buffer[i] = UCA0RXBUF; // Read byte from RX buffer
    }
    if (memcmp(rx_buffer, expected_data, RX_BUF_SIZE) == 0) {
       //P1OUT |= BIT6;  // Turn on the Red LED
       //P1OUT &= ~(BIT5 + BIT4 + BIT0); //turn the others off
       //P1OUT |= BIT0;  // Turn on the Green LED
       //P1OUT &= ~(BIT6 + BIT5 + BIT4); //turn the others off
    }
    else{
       // Turn off all LEDs if none of the specific strings match
       P1OUT &= ~(BIT6 + BIT5 + BIT0);
    }
   
    for (i = 0; i < TX_BUF_SIZE; i++) {
        tx_buffer[i] = rx_buffer[TX_BUF_SIZE - 1 - i];
    }
   
    for (i = 0; i < 11; i++) {
        while (!(IFG2 & UCA0TXIFG)); // Wait until TX buffer is ready
        UCA0TXBUF = ACK[i]; // Write byte to TX buffer
    }
   
    if (memcmp(expected_data10, rx_buffer, RX_BUF_SIZE) == 0) {
      P1OUT |= BIT6; //turns on red LED
     
      memcpy(expected_data, expected_data1, 11 * sizeof(int));
      memcpy(ACK, ACK1, 11 * sizeof(int));
    }
    if (memcmp(expected_data9, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT4; //sets stop to high
      P2OUT &= ~(BIT1+BIT2+BIT3);
     
      memcpy(expected_data, expected_data10, 11 * sizeof(int));
      memcpy(ACK, ACK10, 11 * sizeof(int));
    }
    if (memcmp(expected_data8, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT1; //sets high to high
      P2OUT &= ~(BIT2+BIT3+BIT4);
     
      memcpy(expected_data, expected_data9, 11 * sizeof(int));
      memcpy(ACK, ACK9, 11 * sizeof(int));
    }
    if (memcmp(expected_data7, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT3; //sets low to high
      P2OUT &= ~(BIT1+BIT2+BIT4);
     
      memcpy(expected_data, expected_data8, 11 * sizeof(int));
      memcpy(ACK, ACK8, 11 * sizeof(int));
    }
    if (memcmp(expected_data6, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT4; //sets stop to high
      P2OUT &= ~(BIT1+BIT2+BIT3);
     
      memcpy(expected_data, expected_data7, 11 * sizeof(int));
      memcpy(ACK, ACK7, 11 * sizeof(int));
    }
    if (memcmp(expected_data5, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT2; //sets med to high
      P2OUT &= ~(BIT1+BIT3+BIT4);
     
      memcpy(expected_data, expected_data6, 11 * sizeof(int));
      memcpy(ACK, ACK6, 11 * sizeof(int));
    }
    if (memcmp(expected_data4, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT3; //sets low to high
      P2OUT &= ~(BIT1+BIT2+BIT4);
      P1OUT |= BIT6; //turns on red LED
     
      memcpy(expected_data, expected_data5, 11 * sizeof(int));
      memcpy(ACK, ACK5, 11 * sizeof(int));
    }
    if (memcmp(expected_data3, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT4; //sets stop to high
      P2OUT &= ~(BIT1+BIT2+BIT3);
     
      memcpy(expected_data, expected_data4, 11 * sizeof(int));
      memcpy(ACK, ACK4, 11 * sizeof(int));
    }
    if (memcmp(expected_data2, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT2; //sets med to high
      P2OUT &= ~(BIT1+BIT3+BIT4);
      P1OUT |= BIT6; //turns on red LED

      memcpy(expected_data, expected_data3, 11 * sizeof(int));
      memcpy(ACK, ACK3, 11 * sizeof(int));
    }
    if (memcmp(expected_data1, rx_buffer, RX_BUF_SIZE) == 0) {
      P2OUT |= BIT3; //sets low to high
      P2OUT &= ~(BIT1+BIT2+BIT4);

      memcpy(expected_data, expected_data2, 11 * sizeof(int));
      memcpy(ACK, ACK2, 11 * sizeof(int));
    }
}

void checkADC(){
   ADC10CTL0 |= ENC + ADC10SC;  // Enable and start conversion
   while (ADC10CTL1 & ADC10BUSY); // Wait for conversion to complete
   unsigned int value = ADC10MEM; // Read ADC value
   
   uint8_t temp1[]={0x7E,0x00,0x00,0x52,0x01,0xB6,0x3B,0x2A,0x03,0x92,0x7E};  //insert cartridge response, FN=0
   uint8_t temp2[]={0x7E,0x09,0x00,0x52,0x01,0x10,0x97,0x4e,0xa6,0xa0,0x7E}; //pressure detected, FN=9 (7e0900520110974ea6a07e)
   uint8_t temp3[]={0x7E,0x0a,0x00,0x52,0x01,0x60,0x7b,0x45,0x03,0xb7,0x7E}; //bubble detected, FN=10 (7e0a005201607b4503b77e)
   uint8_t temp4[]={0x7E,0x01,0x00,0x43,0x01,0x1c,0x3a,0x80,0x84,0x84,0x7E}; //cartridge removed, FN=1 (7e010043011c3a8084847e)
   
   if (value > 100) {
     if(flag==1){
        P1OUT |= BIT0;  // Turn on the Green LED
        //P1OUT &= ~(BIT6 + BIT5 + BIT4); //turn the others off
       
        uint8_t temp[]={0x7E,0x00,0x00,0x52,0x01,0xB6,0x3B,0x2A,0x03,0x92,0x7E};  //insert cartridge response, FN=0
       
        if(counter==0){
        for (i = 0; i < 11; i++) {
        while (!(IFG2 & UCA0TXIFG)); // Wait until TX buffer is ready
          UCA0TXBUF = temp1[i]; // Write byte to TX buffer
        }
        }
        if(counter==1){
        for (i = 0; i < 11; i++) {
        while (!(IFG2 & UCA0TXIFG)); // Wait until TX buffer is ready
          UCA0TXBUF = temp2[i]; // Write byte to TX buffer
        }
        }
        if(counter==2){
        for (i = 0; i < 11; i++) {
        while (!(IFG2 & UCA0TXIFG)); // Wait until TX buffer is ready
          UCA0TXBUF = temp3[i]; // Write byte to TX buffer
        }
        }
        if(counter==3){
        for (i = 0; i < 11; i++) {
        while (!(IFG2 & UCA0TXIFG)); // Wait until TX buffer is ready
          UCA0TXBUF = temp4[i]; // Write byte to TX buffer
        }
        }
       
        if(counter==0){
        for (int i = 0; i < 11; i++) {
          expected_data[i] = expected_data1[i];
          ACK[i] = ACK1[i];
        }
        }
     
        counter++;
        flag=0;
       
        if(counter==4){
          counter=0;
        }
     }
     else{
        //P1OUT &= ~(BIT6 + BIT5 + BIT4 + BIT0); //turn the others off
       P1OUT &= ~(BIT5 + BIT0); //turn the others off
     }
   } else {
     
      // Voltage on P1.5 is lower than or equal to the threshold, do something else
     
      //P1OUT |= BIT0;  // Turn on the Green LED
      //P1OUT &= ~(BIT6 + BIT5 + BIT4 + BIT0); //turn the others off
     P1OUT &= ~(BIT5 + BIT0); //turn the others off
      flag=1;
   }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    read_and_write_message();
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    P1DIR |= BIT6 + BIT5 + BIT0;  // Set P1.6, P1.5, P1.4, P1.3 as outputs (Green, Yellow, Red, Blue LEDs)
    P2DIR |= BIT1 + BIT2 + BIT3 + BIT4;
   
    // Initialize UART
    init_uart();

    ADC10CTL1 = INCH_5;         // ADC input select: channel 5 (A5)
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON; // Vcc & Vss as reference, sample and hold time, ADC on
   
    //checkADC();
   
    __enable_interrupt();       // Enable global interrupts

   //DCOCTL = CALDCO_16MHZ;    // set internal oscillator at 16MHz
   //BCSCTL1 = CALBC1_16MHZ;   // set internal oscillator at 16MHz

    P1OUT = 0x00;
    P2OUT = 0x00;
    P1DIR = 0xFF; // Set all as outputs
    P2DIR = 0xFF; // Set all as outputs
 
    __bis_SR_register(GIE); // Enable global interrupts
   
    //P1OUT &= ~(BIT6);
    //P2OUT &= ~(BIT1+BIT2+BIT3+BIT4);
    //P2OUT |= BIT4;
    //P2OUT &= ~(BIT1+BIT2+BIT3);
   
    if(motor_state_init==1){
      P2OUT |= BIT4;
      P2OUT &= ~(BIT1+BIT2+BIT3);
      motor_state_init=0;
    }
     
    while(1){
      //read_and_write_message();
      checkADC();
      //delay_long(delay);
    }  
   
    return 0;
}