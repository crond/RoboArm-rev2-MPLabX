 #include "config.h"
unsigned int readByte;
extern char uartBuffer[];
int  eeprom_test()
{
    unsigned char ch=(unsigned char)1,address=0x01;
    int i=0;
    EEPGD=0;
    CFGS=0;
    debug("writing values");
    /*
    eeprom_write_custom(0x01,'X');
    delay(9000);
    delay(9000);
    debug("reading values  : "); 
    delay(9000);
    delay(9000);
    ch = eeprom_read_custom(0x01);  
    uart_send_byte(readByte);
    delay(9000);
    delay(9000);
    eeprom_write_custom(0x02,'Y');
    delay(9000);
    delay(9000);
    debug("reading values  : "); 
    delay(9000);
    delay(9000);
    ch = eeprom_read_custom(0x02);  
    uart_send_byte(readByte);
    */
    
    /*
    for(i=0;i<250;i++)
    {
        eeprom_write_custom(address,i);
        delay(9000);
        delay(9000);
        delay(9000);
        ch++;
        address++;
    }
    */
    debug("reading EEPROM values\n");
    delay(9000); 
    for(i=0,address=0x01;i<250;i++,address++)
    {
        ch = eeprom_read_custom(address);  
        
        sprintf(uartBuffer,"0x%x ",readByte);
        debug(uartBuffer);
        delay(9000);
        delay(9000);
        delay(9000);
    }
    debug("\n");
   debug("reading done");
    return 1;
}
 unsigned char eeprom_read_custom( unsigned char badd )
 {
     EEADR = (badd & 0x0ff);
       EECON1bits.CFGS = 0;
     EECON1bits.EEPGD = 0;
     EECON1bits.RD = 1;
     readByte = EEDATA;
     return 1;              // return with read byte
 }
 void eeprom_write_custom( unsigned char badd,unsigned int data )
 {
       //EEADR =0x00;     
       EEADR = (badd & 0x0ff);
    
       EEDATA =data;
       EECON1bits.EEPGD = 0;
     EECON1bits.CFGS = 0;
     EECON1bits.WREN = 1;
     INTCONbits.GIE = 0;
     EECON2 = 0x55;
     EECON2 = 0xAA;
     EECON1bits.WR = 1;
    while(EECON1bits.WR);
     INTCONbits.GIE = 1;
     EECON1bits.WREN = 0;
 }