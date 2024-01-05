//------------------------------------------------------------------------------------------------//
// Program	: Library for gennerate sound   
// Description 	: Library for gennerate easy sound
// Frequency	: Crytal 12 MHz at PLL 5x(CCLK = 60 MHz),PCLK = 30 MHz 
// Filename	: sound.h
// C compiler	: Keil CARM Compiler
//------------------------------------------------------------------------------------------------//
#ifndef _SOUND_H_
#define _SOUND_H_

#include <avr/io.h>
#include <in_out.h>
//------------------------------------------------------------------------------------------------//
//---------------------------- Function delay 100 us per count -----------------------------------//
//------------------------------------------------------------------------------------------------//
void delay_sound(unsigned int ms)
{
	unsigned int i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<80;j++);
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function generate sound -------------------------------------------//
//------------------------------------------------------------------------------------------------//
void sound_a(char ch,int freq,int time)
{
	int dt=0,m=0;	// Keep value and 
    dt = 5000/freq;   	// Keep active logic delay
	time = (5*time)/dt;	// Keep counter for generate sound
	for(m=0;m<time;m++) // Loop for generate sound(Toggle logic P0.12)   
	{
        out_a(ch,1);
		delay_sound(dt);	// Delay for sound	
        out_a(ch,0);
		delay_sound(dt);	// Delay for sound
	}		
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function generate sound -------------------------------------------//
//------------------------------------------------------------------------------------------------//
void sound_b(char ch,int freq,int time)
{
	int dt=0,m=0;	// Keep value and 
    dt = 5000/freq;   	// Keep active logic delay
	time = (5*time)/dt;	// Keep counter for generate sound
	for(m=0;m<time;m++) // Loop for generate sound(Toggle logic P0.12)   
	{
        out_b(ch,1);
		delay_sound(dt);	// Delay for sound	
        out_b(ch,0);
		delay_sound(dt);	// Delay for sound
	}		
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function generate sound -------------------------------------------//
//------------------------------------------------------------------------------------------------//
void sound_c(char ch,int freq,int time)
{
	int dt=0,m=0;	// Keep value and 
    dt = 5000/freq;   	// Keep active logic delay
	time = (5*time)/dt;	// Keep counter for generate sound
	for(m=0;m<time;m++) // Loop for generate sound(Toggle logic P0.12)   
	{
        out_c(ch,1);
		delay_sound(dt);	// Delay for sound	
        out_c(ch,0);
		delay_sound(dt);	// Delay for sound
	}		
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function generate sound -------------------------------------------//
//------------------------------------------------------------------------------------------------//
void sound_d(char ch,int freq,int time)
{
	int dt=0,m=0;	// Keep value and 
    dt = 5000/freq;   	// Keep active logic delay
	time = (5*time)/dt;	// Keep counter for generate sound
	for(m=0;m<time;m++) // Loop for generate sound(Toggle logic P0.12)   
	{
        out_d(ch,1);
		delay_sound(dt);	// Delay for sound	
        out_d(ch,0);
		delay_sound(dt);	// Delay for sound
	}		
}

//------------------------------------------------------------------------------------------------//
//---------------------------- Function generate sound beep default ------------------------------//
//------------------------------------------------------------------------------------------------//
void beep_a(char _ch)
{
	sound_a(_ch,3000,100);	// Generate sound default frequency
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function generate sound beep default ------------------------------//
//------------------------------------------------------------------------------------------------//
void beep_b(char _ch)
{
	sound_b(_ch,3000,100);	// Generate sound default frequency
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function generate sound beep default ------------------------------//
//------------------------------------------------------------------------------------------------//
void beep_c(char _ch)
{
	sound_c(_ch,3000,100);	// Generate sound default frequency
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function generate sound beep default ------------------------------//
//------------------------------------------------------------------------------------------------//
void beep_d(char _ch)
{
	sound_d(_ch,3000,100);	// Generate sound default frequency
}
#endif



