 #include "config.h"
char uartBuffer[MAX_RECV_BUFF_SIZE];
int recvIdx=0;
int testPWMValue=0;
static unsigned char ch='z',ch2=0;
extern unsigned char currState,prevState,state;

void eusart_init(unsigned char usart_mode, char tx_mode, 
	unsigned long fosc, unsigned int baud)
{
	TRISC6 = 0;		// UART Tx enable
	TRISC7 = 1;		// UART Rx enable
   // RCSTA =0b10010000;
	switch(usart_mode)
	{
		case ASYNC_MODE:
		{
			BRG16 = 0;
            BAUDCTL = 0b00000000; 
            //SPBRG=2 ;//is for 9600 for 500kHZ
			// SPBRG = (int)( (fosc/(64UL * baud)) -1);	// Formula varies based on UART settings
            SPBRG =51;// 51 is for 9600 for 8MHz ; 25 = 19200 8MHZ ; 74  = 
			//TXSTA = 0x24;	// 8-Bit mode, Enable TX
            TXSTA = 0b00100100;
			RCSTA = 0x90;	// Enable serial comm, 8-Bit mode, Enables continous RX
			break;
		}
		case SYNC_MASTER_MODE:
		{
			break;
		}
		case SYNC_SLAVE_MODE:
		{
			break;
		}		
	}
}
 
void uart_send_byte(unsigned char byte)
{
	while(!TXIF)
	{
		continue;
	}
	TXREG = byte;
    delay(1000);
}
unsigned char uart_recv_byte(char flag)
{
	while(!RCIF)
	{
		continue;
	}
	ch2 = RCREG;
    return ch2;
}
void debug(char *str)
{
    int len=0,i=0;
    len=strlen(str);
    for(i=0;i<len;i++)
    {
        uart_send_byte(str[i]);
        __nop;
        __nop;
        delay(100);
    }
    uart_send_byte('\n');
    delay(100);
}
void processSerialData()
{
    ch=uart_recv_byte(NONBLOCK);
    if(ch==START_CHAR)
    {
        for(recvIdx=0;recvIdx<MAX_RECV_BUFF_SIZE;recvIdx++)
                       uartBuffer[recvIdx]='\0';
        recvIdx = 0;
        while(1)
        {
            ch=uart_recv_byte(BLOCK);
            if(ch==STOP_CHAR)
                break;
            else if(ch==EXIT_CHAR)
            {
                debug("change to default mode");
                state = OP_MODE;
            }
            else if(ch=='\0')
                continue;
            else
                uartBuffer[recvIdx]=ch;
            
            if(recvIdx > MAX_RECV_BUFF_SIZE-1)
            {
                debug("Invalid data.");
                debug("change to default mode");
                state = OP_MODE;
            }
            else    
                recvIdx++;
            __nop();
            __nop();
            __nop();
            delay(9000); 
        }
    }
            
}
void changeMode()
{
    if((uartBuffer[0]=='T'))//&&(uartBuffer[1]=='E')&&(uartBuffer[2]=='S')&&(uartBuffer[3]=='T'))
    {
        debug("entering test mode");
        state = TEST_MODE;
    }
    else if((uartBuffer[0]=='S')&&(uartBuffer[1]=='E')&&(uartBuffer[2]=='T'))
    {
        debug("entering set mode");
        state= SET_MODE;
    }
    else if((uartBuffer[0]=='F')&&(uartBuffer[1]=='I')&&(uartBuffer[2]=='N'))
    {
        debug("Going to Default Mode");
        state= OP_MODE;
    }
    else
    {
        //debug("Invalid Command CM");
        state = OP_MODE;
    }
}
void getPWMvalue()
{
    debug("Enter PWM Value");
    __nop;
    delay(9000);
    processSerialData();
    memcpy(&testPWMValue,&uartBuffer[0],4);
    sprintf(uartBuffer,"b:%d",testPWMValue);
    debug(uartBuffer);
  //  sscanf(uartBuffer,"%d",&testPWMValue);
    
}

