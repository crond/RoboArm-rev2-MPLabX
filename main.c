/*
 * File:   main.c
 * Author: Slacker
 *
 * Created on May 20, 2017, 1:54 PM
 */


#include "config.h"
void delay(int count);
void Load_Base();


char *ptr=NULL;
//tuning for MG995 motor 
// 0 degree:
//CCPR2L = 0x02; // reg_ccp[deg_base];
//CCP2CON = 0b00001100; //reg_con[i];
//180 degree:
 //CCPR2L = 0x10; // reg_ccp[deg_base];
 //CCP2CON = 0b00001100; //reg_con[i];
#define FWD 1
#define REV 2
#define MIN_VALUE 0x05
#define MAX_VALUE 0x11
void Load_Base()
{
	static   char i=0,direction=FWD;
    static   char value=MIN_VALUE;
    
    if(direction == FWD)
    {
        if(value < MAX_VALUE)
            value =  value + 1;
        else
           direction = REV; 
    }else 
    {
        if(value > MIN_VALUE)
            value =  value - 1;
        else
           direction = FWD; 
    }
      
     
     CCPR2L = value; // reg_ccp[deg_base];
     CCP2CON = 0b00001100;
       /*     
    switch(i){
        case 1:
            CCPR2L = 0x04; // reg_ccp[deg_base];
            CCP2CON = 0b00001100; //only 5-6 bits are considered 
            direction = FWD;
            break;
        case 2:
            CCPR2L = 0x04; // reg_ccp[deg_base];
            CCP2CON = 0b00001100; //only 5-6 bits are considered 
           break;
        case 3:
             CCPR2L = 0x06; // reg_ccp[deg_base];
            CCP2CON = 0b00001100; //only 5-6 bits are considered 
           break;
        case 4:
             CCPR2L = 0x08; // reg_ccp[deg_base];
            CCP2CON = 0b00001100; //only 5-6 bits are considered 
        break;
        case 5:
             CCPR2L = 0x0A; // reg_ccp[deg_base];
            CCP2CON = 0b00001100; //only 5-6 bits are considered 
        break;
        case 6:
             CCPR2L = 0x0C; // reg_ccp[deg_base];
            CCP2CON = 0b00001100; //only 5-6 bits are considered 
        break;
        case 7:
             CCPR2L = 0x0E; // reg_ccp[deg_base];
            CCP2CON = 0b00001100; //only 5-6 bits are considered 
        break;
        case 8:
            CCPR2L = 0x10; // reg_ccp[deg_base];
            CCP2CON = 0b00001100; //only 5-6 bits are considered 
            direction = REV;
        break;
    }*/
    


}
void delay(int count)
{
    int i=0;
    
    for(i=0;i<count;i++)
    {
        
    }
}
unsigned char  currState,prevState,state;

void main(void) {  
    
    unsigned char ch;
    TRISA = 0x00;
    TRISB=0X00;		
	TRISC=0x00;		// Configuring as Output Port for RC2 17th Pin.
	TRISD=0xff;
	//OSCCON=0X3E;  		// Selecting Internal Oscillator of 500KHZ.
    OSCCON=0b01110010;  		// Selecting Internal Oscillator of 1MHZ
	PR2=0x9B;		// Pr2 Value 155 for Pulse width Period and Pulse width duty Cycle.
	T2CON=0x06;		// Loading Timer2 Control Register with 0x06.
    
   // TRISC =0b01100000; //setting bit 6,7 for UART
    
//-----------------------WristPWM-----------------------------
	PWMCON0	=0x2f;
	PTCON0	=0x00;
	PTPERL	=0xc4;
	PTPERH	=0x09;
	PTMRL	=0xc4;
	PTMRH	=0x09;
//-------------------------------Default Position Value---------------------------

	PTCON1	=0x80;

	eusart_init(ASYNC_MODE, TX_8_BIT, 8000000UL, 9600);
    eeprom_test();
    while(1)
    {
     //   ch=uart_recv_byte(NONBLOCK);
    //    uart_send_byte(ch);
        
              
        
        processSerialData();                
         changeMode();
        state_machine();
       
        delay(9000);            
    }
    return;
}


void state_machine()
{
    static char flag=0;
    switch(state)
    {
        case TEST_MODE:
            getPWMvalue();
            break;
        case SET_MODE:
            break;
            
        default:
            break;
    }
    if(flag)
        PORTAbits.RA1=0b1;               
    else
        PORTAbits.RA1=0b0;
}