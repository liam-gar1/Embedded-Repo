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

uint8_t expected_data[] = {0x7E,0x00,0x00,0x43,0x01,0x9F,0x10,0x7B,0x55,0xCD,0x7E};
uint8_t ACK[]={0x7E,0x00,0x00,0x52,0x01,0xFF,0x0F,0xD3,0x03,0x9D,0x7E};

uint8_t expected_data1[] = {0x7E,0x00,0x00,0x43,0x01,0x9F,0x10,0x7B,0x55,0xCD,0x7E};
uint8_t ACK1[]={0x7E,0x00,0x00,0x52,0x01,0xFF,0x0F,0xD3,0x03,0x9D,0x7E};

uint8_t expected_data2[] = {0x7E,0x01,0x00,0x43,0x01,0xdb,0x29,0xd7,0x84,0x81,0x7E};
uint8_t ACK2[]={0x7E,0x01,0x00,0x52,0x01,0xff,0xbf,0xfa,0x63,0xa0,0x7E};

uint8_t expected_data3[] = {0x7E,0x02,0x00,0x43,0x01,0x0f,0x34,0xbb,0x9a,0x47,0x7E};
uint8_t ACK3[]={0x7E,0x02,0x00,0x52,0x01,0xff,0x6f,0x80,0xc3,0xe7,0x7E};

uint8_t expected_data4[] = {0x7E,0x03,0x00,0x43,0x01,0x9f,0xc0,0x01,0xf5,0x8a,0x7E};
uint8_t ACK4[]={0x7E,0x03,0x00,0x52,0x01,0xff,0xdf,0xa9,0xa3,0xda,0x7E};

uint8_t expected_data5[] = {0x7E,0x04,0x00,0x43,0x01,0xcf,0x24,0x8c,0xbe,0x53,0x7E};
uint8_t ACK5[]={0x7E,0x04,0x00,0x52,0x01,0xff,0xcf,0x75,0x83,0x68,0x7E};

uint8_t expected_data6[] = {0x7E,0x05,0x00,0x43,0x01,0x0f,0x24,0x67,0xba,0xf5,0x7E};
uint8_t ACK6[]={0x7E,0x05,0x00,0x52,0x01,0xff,0x7f,0x5c,0xe3,0x55,0x7E};

uint8_t expected_data7[] = {0x7E,0x06,0x00,0x43,0x01,0x9f,0xb0,0x8e,0x15,0x42,0x7E};
uint8_t ACK7[]={0x7E,0x06,0x00,0x52,0x01,0xff,0xaf,0x26,0x43,0x12,0x7E};

uint8_t expected_data8[] = {0x7E,0x07,0x00,0x43,0x01,0xab,0xb5,0x53,0xc1,0x5e,0x7E};
uint8_t ACK8[]={0x7E,0x07,0x00,0x52,0x01,0xff,0x1f,0x0f,0x23,0x2f,0x7E};

uint8_t expected_data9[] = {0x7E,0x08,0x00,0x43,0x01,0x0f,0x95,0xa3,0x2a,0x0d,0x7E};
uint8_t ACK9[]={0x7E,0x08,0x00,0x52,0x01,0xff,0xce,0x98,0x73,0xad,0x7E};

uint8_t expected_data10[] = {0x7E,0x00,0x00,0x43,0x01,0xa9,0x89,0xee,0xef,0x02,0x7E};
uint8_t ACK10[]={0x7E,0x00,0x00,0x52,0x01,0xff,0x0f,0xd3,0x03,0x9d,0x7E};

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
