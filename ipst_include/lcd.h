

#ifndef _LCD_H_
#define _LCD_H_

#include <avr/io.h>
#include <stdlib.h>
#include <ctype.h>
#include <in_out.h>
#include <sleep.h>

#define EN(x) out_c(3,x)	// PC3 connect to EN
#define RS(x) out_c(2,x)	// PC3 connect to EN
#define lcd_clear() lcd_command(1)
#define lcd_origin() lcd_command(2)
#define LCD lcd
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

#ifndef TEST_CLEAR_NEXT_TYPE(x)
#define TEST_CLEAR_NEXT_TYPE(x) *x== '#' && *(x+1)=='c' // Clear display next time
#endif

#ifndef TEST_NEW_LINE_TYPE(x)
#define TEST_NEW_LINE_TYPE(x) *x== '#' && *(x+1)=='n' 	// Go to line 2
#endif

#ifndef F_PREC
#define F_PREC 3
#endif
//------------------------------------------------------------------------------------//
void lcd_init(void);
char lcd_ini=0,_lcd_clear_display=0;

void lcd_clk(void)				/* Pulse LCD clock on EN*/
{
  EN(1);
  sleep(1);
  EN(0);
  sleep(1);
}

void lcd_command(unsigned char outbyte)	/* Send command (4bit operation) */
{
  if(lcd_ini==0)
  {
  	lcd_ini=1;
	lcd_init();
  }
  RS(0);		// Command
  PORTC=(PORTC&0x0f)|(outbyte&0xf0);
  lcd_clk();
  PORTC=(PORTC&0x0f)|((outbyte<<4)&0xf0);
  lcd_clk();
}

void lcd_text(unsigned char outbyte) /* Send Data (4bit operation) */
{
  if(lcd_ini==0)
  {
  	lcd_ini=1;
	lcd_init();
  }
  RS(1);		// Display
  PORTC=(PORTC&0x0f)|(outbyte&0xf0);
  lcd_clk();
  PORTC=(PORTC&0x0f)|((outbyte<<4)&0xf0);
  lcd_clk();
}
//-----------------------------------------------------------------------------------------------------------------------------//
//------------------------------------ Function show string message ----------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------//
void lcd_puts(unsigned char line,char *p)
{
	lcd_origin();         // Set origin address of LCD
	lcd_command(line);			// Set address 00H of LCD
	while(*p)                  // Check data pointer = 0?
	{
	 lcd_text(*p);             // Send data to LCD
	 p++;                      // Increase address 1 time 
	}
}

//-----------------------------------------------------------------------------------------------------------------------------//
//------------------------- Convert integer to ascii for display on LCD ---------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------//
void inttolcd(unsigned char posi, long value)
{
	char buff[16];			// For keep string send to LCD 
	ltoa(value,&buff[0],10);
	lcd_puts(posi,buff);  	// Send integer to LCD
}
//-----------------------------------------------------------------------------------------------------------------------------//
//------------------------- Convert integer to ascii for display on LCD ---------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------//
/*void floattolcd(unsigned char posi, float value)
{
	char buff[16];			// For keep string send to LCD 
	dtostrf(value,16,5,buff);
	lcd_puts(posi,buff);  	// Send integer to LCD
}*/

void lcd_init(void)
{
  PORTC = 0x00;
  DDRC = 0xFC;
  EN(0);						// Init. EN = 0
  RS(0);						// Init. RS = 0

  sleep(500);
  PORTC=(PORTC&0x0f)|0x30;	// Force LCD receive 1 times command per pulse
  lcd_clk();
  PORTC=(PORTC&0x0f)|0x30;	// (do 3 times for ensure LCD operation forced)
  lcd_clk();
  PORTC=(PORTC&0x0f)|0x30;
  lcd_clk();

  PORTC=(PORTC&0x0f)|0x20;	// Switch to real 4 bit transfer mode on first
  lcd_clk();

  lcd_command(0x28);		// Send real command to 4 bit operation and set N=1, F=0
  lcd_command(0x01);		// Clear all
  lcd_command(0x06);		// Increase cursor automatic after byte
  lcd_command(0x0C);		// Display on, no cursor, not blink
  lcd_command(0x02);		// Home position
  lcd_command(0x01);		// Clear all again
}
//-----------------------------------------------------------------------------------------------------------------------------//
//------------------------------------ Function LCD display and %d support(Beta version) ----------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------//
void lcd(char *p,...)
{
	char *arg,**pp;	// Poter of point
	char *ptr,char_count=0,buff[16];
	pp = &p;
	ptr = p;	// Copy address
	arg = pp;	// Copy address of p point	
	
	lcd_origin();       // Set origin address of LCD
	lcd_command(0x80);	// Set address 00H of LCD
	if(_lcd_clear_display)
	{
		_lcd_clear_display = 0;
		lcd_clear();
	}
	arg += 2;	// Cross 2 time go to Origin of first parameter 
	
	while(*ptr)                 // Check data pointer = 0?
	{
	 	if(TEST_CHAR_TYPE(ptr))
		{
			char_count++;
			if(char_count==17)
			{
				lcd_origin();
				lcd_command(0xC0);
			}
			if(char_count>32)
			{
				char_count = 0;
			}
			lcd_text(toascii(*arg));             // Send data to LCD
	 	
			arg+=2;	// Cross address char type
			ptr++;	// Cross %d parameter
		}
		else if(TEST_INT_TYPE(ptr))
		{
			p = ltoa(*(int *)arg,&buff[0],10);
			while(*p)           // Check data pointer = 0?
			{
	 			char_count++;
				if(char_count==17)
				{
					lcd_origin();
					lcd_command(0xC0);
				}
				if(char_count>32)
				{
					char_count = 0;
				}
				lcd_text(*p);             // Send data to LCD
	 			p++;                     // Increase address 1 time 
			}
			arg+=2;	// Cross address int type
			ptr++;	// Cross %d parameter
		}
		else if(TEST_LONG_TYPE(ptr))
		{
			p = ltoa(*(long *)arg,&buff[0],10);
			while(*p)           // Check data pointer = 0?
			{
	 			char_count++;
				if(char_count==17)
				{
					lcd_origin();
					lcd_command(0xC0);
				}
				if(char_count>32)
				{
					char_count = 0;
				}
				lcd_text(*p);             // Send data to LCD
	 			p++;                     // Increase address 1 time 
			}
			arg+=4;	// Cross address long type
			ptr++;	// Cross %l parameter
		}
		else if(TEST_FLOAT_TYPE(ptr))
		{
			p = dtostrf(*(float *)arg,2,F_PREC,&buff[0]);	// Convert float to string(used libm.a)
			while(*p)           // Check data pointer = 0?
			{
	 			char_count++;
				if(char_count==17)
				{
					lcd_origin();
					lcd_command(0xC0);
				}
				if(char_count>32)
				{
					char_count = 0;
				}
				lcd_text(*p);             // Send data to LCD
	 			p++;                     // Increase address 1 time 
			}
			arg+=4;	// Cross address long type
			ptr++;	// Cross %l parameter
		}
		else if(TEST_CLEAR_NEXT_TYPE(ptr))
		{
			_lcd_clear_display = 1;	// Clear display next time 
			
			ptr++;	// Cross %l parameter
		}
		else if(TEST_NEW_LINE_TYPE(ptr))
		{
			
			char_count = 17;	// Character start line 2 
			lcd_origin();
			lcd_command(0xC0);		
			ptr++;	// Cross %l parameter
		}
		else
		{
			char_count++;
			if(char_count==17)
			{
				lcd_origin();
				lcd_command(0xC0);
			}
			if(char_count>32)
			{
				char_count = 0;
			}
			lcd_text(*ptr);             // Send data to LCD
	 	}
		
		ptr++;                      // Increase address 1 time 
	}

}

#endif
