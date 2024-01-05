#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define F_OSC 16000000		           /* oscillator-frequency in Hz */
#define UART_BAUD_CALC(x,F_OSC) ((F_OSC)/((x)*16l)-1)
#define SERVICE_RX_INT() SIGNAL(SIG_UART_RECV)


//-------------------- Condition  test parameter data type for display ---------------//
#ifndef TEST_CHAR_TYPE(x)
#define TEST_CHAR_TYPE(x) *x=='%' && (*(x+1)=='c' || *(x+1)=='C')
#endif

#ifndef TEST_INT_TYPE(x)
#define TEST_INT_TYPE(x) *x=='%' && (*(x+1)=='d' || *(x+1)=='D')
#endif

#ifndef TEST_LONG_TYPE(x)
#define TEST_LONG_TYPE(x) *x=='%' && (*(x+1)=='l' || *(x+1)=='L')
#endif

#ifndef TEST_FLOAT_TYPE(x)
#define TEST_FLOAT_TYPE(x) *x=='%' && (*(x+1)=='f' || *(x+1)=='F')
#endif

#ifndef TEST_STRING_TYPE(x)
#define TEST_STRING_TYPE(x) *x=='%' && (*(x+1)=='s' || *(x+1)=='S')
#endif

#ifndef F_PREC
#define F_PREC 3
#endif


unsigned int _baud=9600;
char uart_ini=0;
char _key=0;


#ifndef ENABLE_SERVICE_RX_INT
		
SIGNAL(SIG_UART_RECV) 
{ 	
	_key = UDR;
	
}
#endif


void uart_set_baud(unsigned int baud) 
{
	_baud = baud;
	uart_ini=1;
	// Set baud rate
	UBRRH = (unsigned int)(UART_BAUD_CALC(baud,F_OSC)>>8);
	UBRRL = (unsigned int)UART_BAUD_CALC(baud,F_OSC);

	// Enable receiver and transmitter; enable RX interrupt
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);

	// Asynchronous 8N1
	UCSRC |= (1 << URSEL) | (3 << UCSZ0);
	sei();  // enable interrupts

}

unsigned int uart_gets_baud()
{
	return(_baud);
}

void uart_putc(unsigned char c) 
{
	while(!(UCSRA & (1 << UDRE)));
	UDR = c;    // send character
	
}

void uart_puts(char *s) 
{
	while (*s) 
	{
		uart_putc(*s);
		s++;
	}
}
void uart(char *p,...) 
{
	char *arg,**pp;	// Poter of point
	char *ptr,buff[16]/*,s_arg_offset=0,s_arg_i=0*/;
	pp = &p;
	ptr = p;	// Copy address
	arg = pp;	// Copy address of p point	
	
	arg += 2;	// Cross 2 time go to Origin of first parameter 
	
	if(uart_ini==0)
	{
		uart_ini=1;
		uart_set_baud(_baud);
	}
		
	while(*ptr)                 // Check data pointer = 0?
	{
	 	if(TEST_CHAR_TYPE(ptr))
		{
			uart_putc(toascii(*arg));
			arg+=2;	// Cross address char type
			ptr++;	// Cross %d parameter
		}
		else if(TEST_INT_TYPE(ptr))
		{
			p = ltoa(*(int *)arg,&buff[0],10);
			uart_puts(p);
			arg+=2;	// Cross address int type
			ptr++;	// Cross %d parameter
		}
		else if(TEST_LONG_TYPE(ptr))
		{
			p = ltoa(*(long *)arg,&buff[0],10);
			uart_puts(p);
			arg+=4;	// Cross address long type
			ptr++;	// Cross %l parameter
		}
		else if(TEST_FLOAT_TYPE(ptr))
		{
			p = dtostrf(*(float *)arg,2,F_PREC,&buff[0]);	// Convert float to string(used libm.a)
			uart_puts(p);
			arg+=4;	// Cross address long type
			ptr++;	// Cross %l parameter
		}
		else
		{
			uart_putc(*ptr);             // Send data to LCD
	 	}
		
		ptr++;                      // Increase address 1 time 
	}
}
char uart_getkey()
{
	char _c=0;
	if(uart_ini==0)
	{
		uart_ini=1;
		uart_set_baud(_baud);
	}
	while(!_key);
	_c = _key;
	_key = 0;
	return(_c);	
	
}


