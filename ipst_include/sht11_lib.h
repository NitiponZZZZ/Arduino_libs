//---------------------------------------------------------------------------------------------//
// Program		: SHT11 Test  
// Description 	: Example for read Temperature/Humidity  
// Frequency	: Fch/4	
// Filename		: sht11.c
// C compiler	: Softune Workbench
//---------------------------------------------------------------------------------------------//


#ifndef _SHT11_LIB_H_
#define _SHT11_LIB_H_

#include <in_out.h>
#include <sleep.h>

#ifndef DATA_out(x)
#define DATA_out(x) out_c(1,x)
#endif
#ifndef DATA_in()
#define DATA_in() in_c(1)
#endif
#ifndef CLK_out(x)
#define CLK_out(x) out_c(0,x)
#endif

#define MEASURE_TEMP 0x03  // Macro for command read Temperature
#define MEASURE_HUMI 0x05  // Macro for command read Humidity

#define TEMP 0  // Macro for select read Temperature 
#define HUMI 1  // Macro for select read Humidity

#define C1 -4.0				// Constant C1 for calculate Humidity(12 bit resolution) 
#define C2 0.0405			// Constant C2 for calculate Humidity(12 bit resolution)
#define C3 -0.0000028	   	// Constant C3 for calculate Humidity(12 bit resolution)

#define T1 0.01			// Constant T1 for calculate Temperature(14 bit resolution)
#define T2 0.00008			// Constant T2 for calculate Temperature(14 bit resolution)

//---------------------------------------------------------------------------------------------//
//-------------------------  Function for delay a few time ------------------------------------//
//---------------------------------------------------------------------------------------------//
void wait()
{
	char x; 				// Variable for counter
	for(x=0;x<8;x++);    	// Loop for delay a few time
	
}

//---------------------------------------------------------------------------------------------//
//-------------------------  Function for write command to sht11 ------------------------------//
//---------------------------------------------------------------------------------------------//
void sht11_write(unsigned char value)
{ 
  unsigned char i;  	// Variable for counter
  for(i=0x80;i>0;i/=2) 	// Loop for bit shift for compare data to send 
  { 
  		if(i & value)   // Data bit AND operation logic "1"?  
 			DATA_out(1);   // Send logic "1" to DATA pin     
    	else 
    		DATA_out(0);
	   	CLK_out(1);
		wait();       	// Wait a few time
    	CLK_out(0);

  }
  DATA_out(1);
  CLK_out(1);
  wait();   			// Wait a few time             
  CLK_out(0);
}

//---------------------------------------------------------------------------------------------//
//------------------------- Function for read data from sht11 ---------------------------------//
//---------------------------------------------------------------------------------------------//
unsigned char sht11_read(char x)
{ 
  unsigned char i=0,value=0;
  DDRC &= ~(2);	// for data input mode at CLK
  for(i=0x80;i>0;i/=2)     	// Loop for bit shift for compare data to send 
  { 
  	CLK_out(1);              	
    if(DATA_in()) 
    	value = value | i;   // Keep data bit  
  	CLK_out(0);  					 
  }
  if(x==1)
  {
  	DATA_out(0);
  }
  wait();
  CLK_out(1);
  wait();
  CLK_out(0);
  
  
  return(value);    // Return data read from sht11
}

//---------------------------------------------------------------------------------------------//
//------------------------- Function for initial a transmission start -------------------------//
//---------------------------------------------------------------------------------------------//
void transmission_start(void)
{  
   DATA_out(1);
   CLK_out(0);
   wait();      // Wait a few time
   CLK_out(1);
   wait();      // Wait a few time 
   DATA_out(0);
   wait();		// Wait a few time
   CLK_out(0);
   wait();		// Wait a few time
   CLK_out(1);
   wait();		// Wait a few time
   DATA_out(1);
   wait();		// Wait a few time
   CLK_out(0);
}
//---------------------------------------------------------------------------------------------//
//------------------------- Function for connection reset sequence ----------------------------//
//---------------------------------------------------------------------------------------------//
void connection_reset()
{  
  unsigned char i; 	// Variable for counter
  DATA_out(1);
  CLK_out(0);
  for(i=0;i<9;i++) 	// Loop for toggle SCK 9 time
  { 
   	CLK_out(1);
	wait();			// Wait a few time
  	CLK_out(0);
  }
  transmission_start();   	//transmission start
}
//---------------------------------------------------------------------------------------------//
//-------------------------  Function for readout the data from sht11(2 byte) -----------------//
//---------------------------------------------------------------------------------------------//
unsigned int sht11_read_measure(unsigned char measure_sel)
{ 
  unsigned int value=0;		// Variable for keep readout data
  
  transmission_start();    // Start connection
  if(measure_sel==TEMP)    // Read Temperature?
  {                     
    	sht11_write(MEASURE_TEMP);	// Send command read Temperature to sht11 
  		sleep(270);
  }
  else if(measure_sel==HUMI)	// Read Humidity?
  {                     //send command to sensor
    	sht11_write(MEASURE_HUMI); 
  		sleep(270);
  }
  value  = sht11_read(1);    // Read MSB byte
  value<<=8;					 // Shift bit 8 time before keep next byte 	
  value = value + sht11_read(2);    // Read LSB byte
  return(value);		// Return readout value from sht11
}
//---------------------------------------------------------------------------------------------//
//-------------------------  Function for convert data from sht11 to float value --------------//
//---------------------------------------------------------------------------------------------//

float read_HT_float(unsigned char measure_sel)
{ 
  float 	so_rh=0.0,		// Variable for keep Humidity readout value
  			so_t=0.0,		// Variable for keep Temperature readout value
  			rh_linear=0.0,  // Variable for RH linear(Calculate by equation sht11 data sheet) 
  			rh_true=0.0,		// Variable for Humidity(Calculate by equation sht11 data sheet)
  			temp=0.0;			// Variable for Temperature(Calculate by equation sht11 data sheet)	 

  	so_rh = sht11_read_measure(HUMI);		// Keep Humidity readout value to so_rh  
  	so_t = sht11_read_measure(TEMP);     // Keep Temperature readout value to so_t
    
  	temp = so_t*0.01 - 40.0;	            // Calculate Temperature
  	rh_linear = C1 + C2*so_rh + C3*so_rh*so_rh ;  	// Calculate RH linear
  	rh_true = (temp-25.0)*(T1+T2*so_rh)+rh_linear;  	// Calculate Humidity

	if(measure_sel==TEMP)  	// Read Temperature? 
		return(temp);        // Return Temperature(float type)
	else if(measure_sel==HUMI) // Read Humidity?
		return(rh_true);		// Return Humidity(float type)
}
int sht11_temp()
{
	connection_reset();
	return((int)read_HT_float(TEMP));
}
int sht11_humi()
{
	connection_reset();
	return((int)read_HT_float(HUMI));
}

#endif
