/* Hardware Configuration

	MOTOR1
	- PD2 Connect to 1A port 
	- PD3 Connect to 1B	port
	- PD4 Connect to 1E	port

	MOTOR2
	- PD7 Connect to 2A port
	- PD6 Connect to 2B port
	- PD5 Connect to 2E	port 

*/

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <in_out.h>

#define ALL 3
#define all 3
unsigned char _duty1=0,_duty2=0; 
char pwm_ini =0;

SIGNAL (SIG_OVERFLOW1) // Interval 1 ms
{
	OCR1AL = _duty1; 
	OCR1BL = _duty2;
}

void pwm_init()
{
	TCCR1A |= (1<<WGM10);
	TCCR1B = (1<<CS12)|(1<<CS10)|(1<<WGM12);
	TIFR |= 1<<TOV1; 	//Clear TOV0 / clear
	TIMSK |= 1<<TOIE1; 	//Enable Timer0 Overflow Interrupt
	//timer_enable_int(_BV(TOIE1));
	sei();
}
void pwm(char channel,unsigned int duty)
{
	duty = (duty*255)/100;
	if(pwm_ini==0)
	{
		pwm_init();
		pwm_ini=1;
	}
	if(channel==2)
	{
		TCCR1A |= _BV(COM1A1);
		DDRD |= _BV(PD5);
		OCR1AL = duty;
		_duty1 = duty;
	}
	else if(channel==1)
	{
		TCCR1A |= _BV(COM1B1);
		DDRD |= _BV(PD4);
		OCR1BL = duty;
		_duty2 = duty;
	}
}
void motor(char _channel,int _power)
{
	if(_power>0)
	{
		pwm(_channel,_power);
		if(_channel==2)
		{
			out_d(7,1);		
			out_d(6,0);
		}
		else if(_channel==1)
		{
			out_d(2,1);		
			out_d(3,0);
		}
	}
	else
	{
		
		pwm(_channel,abs(_power));
		if(_channel==2)
		{
			out_d(7,0);
			out_d(6,1);
		}
		else if(_channel==1)
		{
			out_d(2,0);
			out_d(3,1);
		}	
	
	}
}
void motor_stop(char _channel)
{
	pwm(_channel,0);
	if(_channel==2 ||_channel==3)
	{
		out_d(7,0);		
		out_d(6,0);
	}
	if(_channel==1||_channel==3)
	{
		out_d(2,0);		
		out_d(3,0);
	}

	
}

#endif
