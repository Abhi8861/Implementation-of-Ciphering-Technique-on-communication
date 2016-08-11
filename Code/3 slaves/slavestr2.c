#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

// CONFIG1
#pragma config "FOSC = INTRC_NOCLKOUT"          // Oscillator Selection bits (INTRC_NOCLKOUT oscillator: Internal Oscillator with No Clock Output on Pins)
#pragma config "WDTE = OFF"                     // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config "PWRTE = OFF"                    // Power-up Timer Enable bit (PWRT disabled)
#pragma config "MCLRE = ON"                     // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config "CP = OFF"                       // Code Protection bit (Program memory code protection is disabled)
#pragma config "CPD = OFF"                      // Data Code Protection bit (Data memory code protection is disabled)
#pragma config "BOREN = ON"                     // Brown Out Reset Selection bits (BOR disabled)
#pragma config "IESO = OFF"                     // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config "FCMEN = OFF"                    // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config "LVP = OFF"                      // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config "BOR4V = BOR40V"                 // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config "WRT = OFF"                      // Flash Program Memory Self Write Enable bits (Write protection off)


#define _XTAL_FREQ 8000000
#define baud_rate 9600
#define Slave_addr 0xBB

unsigned char addr=0;
//unsigned char datain;

void delay_us(unsigned int);
void delay_ms(unsigned int);
void delay1ms(void);


char UART_Init(long);
void UART_Write(unsigned char);
char UART_TX_Empty(void);
void UART_Write_Text(const char *);
char UART_Data_Ready(void);
char UART_Read(void);
unsigned char * UART_Read_Text(void);
unsigned char getch(void);
unsigned char getche(void);


void i2c_slave_init(void);
void i2c_start_detect(void);
void i2c_address_detect(void);
char i2c_data_detect(void);

void main()
{
unsigned char i;

unsigned char array[80];

const char *main1="\r\n-------------SLAVE 1(0XAA)---------------\r\n";

const char *main2="\r\n-------------SLAVE 2(0XBB)---------------\r\n";

const char *main3="\r\n-------------SLAVE 3(0XCC)---------------\r\n";

const char *msg1="\r\nWaiting for Master\r\n";

const char *t1="\r\n Before start\r\n";

const char *t2="\r\n Address detected\r\n";

const char *t3="\r\n Data recieved??\r\n";

const unsigned char *arr1= "\r\nUART Initialised\r\n";

const unsigned char *arr2= "\r\nSlave I2C initialised:\r\n";

    OSCCONbits.IRCF = 0x07;  // Configure Internal OSC for 8MHz Clock

    while(!OSCCONbits.HTS);   // Wait Until Internal Osc is Stable

    UART_Init(baud_rate);

    UART_Write_Text(main2);//DISPLAYS SLAVE 1 AS THE SLAVE DEVICE

    UART_Write_Text(arr1);

    delay_ms(500);

    UART_Write_Text(msg1);

    i2c_slave_init();

     UART_Write_Text(arr2);

     while(1)
     {
         for(i=0;i<=80;i++)
         {
             array[i]=0;
         }
         i=0;

         //UART_Write(datain);

         UART_Write_Text("\r\n Next line blank array print\r\n");

         UART_Write_Text(array);

         UART_Write_Text("\r\n");

//start i2c routines

            i2c_start_detect();

            PIR1bits.SSPIF=0;

            i2c_address_detect();

            do
            {
                UART_Write(i2c_data_detect());

            }while(!(SSPSTATbits.P));
            UART_Write_Text("\r\nDATA READ\r\n");

            
     }
}




//functions

void i2c_slave_init(void)
{
  TRISCbits.TRISC3 = 1;  //direction register SCL as input pin
    TRISCbits.TRISC4 = 1; //direction register SDA as input pin
    SSPADD = Slave_addr; //device address

    SSPCON = 0X36; //SSPEN =1 and set i2c to slave mode with 7 bit address
    SSPSTAT =0;

    PIR1bits.SSPIF = 0;         //Clear Control Bits
}

void i2c_start_detect(void)
{

   while(!(SSPSTATbits.S));
   //UART_Write_Text("Start detected\r\n"); debug only

}


void i2c_address_detect(void)
{

   static unsigned char condition;

   while(!(SSPSTATbits.BF));

   while(SSPSTATbits.D_A);      /* automatically cleared by hardware */

   addr=SSPBUF;

  //UART_Write_Text("address detected\r\n"); debug only
PIR1bits.SSPIF = 0;



}


char i2c_data_detect(void)
{
    unsigned static char datain='G';
  do
   {
     SSPCONbits.SSPOV = 0;
   }while((!(SSPSTATbits.BF)) && (!(SSPSTATbits.P)));
datain=SSPBUF;


   SSPCONbits.CKP = 0;

   PIR1bits.SSPIF = 0;
   SSPCONbits.CKP = 1;
   return datain;
}

 char UART_Init(long baudrate)
{
  unsigned int x;
  x = (_XTAL_FREQ - baudrate*64)/(baudrate*64); //SPBRG for Low Baud Rate
  if(x>255) //If High Baud Rate required
  {
    x = (_XTAL_FREQ - baudrate*16)/(baudrate*16); //SPBRG for High Baud Rate

    BRGH = 1; //Setting High Baud Rate
    SPBRG = x; //Writing SPBRG register
    SYNC = 0; //Selecting Asynchronous Mode
    SPEN = 1; //Enables Serial Port

    CREN = 1; //Enables Continuous Reception
    TXEN = 1; //Enables Transmission
  }
  if(x<256)
  {
    BRGH = 0; //Setting High Baud Rate
    SPBRG = x; //Writing SPBRG register
    SYNC = 0; //Selecting Asynchronous Mode
    SPEN = 1; //Enables Serial Port
    CREN = 1; //Enables Continuous Reception
    TXEN = 1; //Enables Transmission
    return 1;
  }
  return 0;
}

void UART_Write(unsigned char data)
{
  while(!PIR1bits.TXIF);
  while(!TRMT); //Waiting for Previous Data to Transmit completly
  TXREG = data; //Writing data to Transmit Register, Starts transmission
}

char UART_TX_Empty()
{
  return TRMT; //Returns Transmit Shift Status bit
}

void UART_Write_Text(const char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
    UART_Write(text[i]);

}

//recive part
char UART_Data_Ready()
{
  return RCIF;
}

char UART_Read()
{
  while(!RCIF); //Waits for Reception to complete
  return RCREG; //Returns the 8 bit data
}

unsigned char * UART_Read_Text()
{
  unsigned const char *a="Keyed in \r\n";
  unsigned static char string[20];
  unsigned char x, i = 0;

//receive the characters until ENTER is pressed (ASCII for ENTER = 13)
while((x = UART_Read()) != 13)
{
 //and store the received characters into the array string[] one-by-one
string[i++] = x;
}

//insert NULL to terminate the string
string[i] = '\0';
UART_Write_Text(a);

//return the received string
return(string);
}

void putch(unsigned char byte)
{
    /* output one byte */
    while(!TXIF)    /* set when register is empty */
        continue;
    TXREG = byte;
}

unsigned char getch()
{
    /* retrieve one byte */
    while(!RCIF)    /* set when register is not empty */
        continue;
    return RCREG;
}

unsigned char getche(void)
{
    unsigned char c;
    putch(c = getch());
    return c;
}

//delay funcs

void delay_us(unsigned int i)
{
	for (;i!=0x00;i--);
}

void delay_ms(unsigned int i)
{
	for(;i!=0x00;i--)
		delay1ms();
}

void delay1ms(void)
{
	unsigned int j = 130;
	for (;j!=0x00;j--);
}