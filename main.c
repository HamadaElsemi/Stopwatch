/*
 * main.c
 *
 *  Created on: Mar 24, 2024
 *      Author: Eng Hamada Ahmed
 */


/*************************LIB**************************/
#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"

/*************************MCAL*************************/
#include "MCAL/DIO/DIO_Interface.h"
#include "MCAL/DIO/DIO_Config.h"
#include "MCAL/DIO/DIO_Private.h"

#include "MCAL/EXTI/EXTI_Private.h"
#include "MCAL/GIE/GIE_Interface.h"

#include "MCAL/TIMERS/TIMERS_Config.h"
#include "MCAL/TIMERS/TIMERS_Interface.h"
#include "MCAL/TIMERS/TIMERS_Private.h"

/*************************Delay*************************/
#include <util/delay.h>



	/*
	 * FCLK = 8 MHZ
	 * PRESCALER = 1024
	 * Preload = 0
	 * Counts = 256 - preload = 256 - 0 = 256
	 * N. of overflows = counts * (Pre-scaler / FCLK) =8.192 msecmsec
	 *  action at -> 1/0.008192= 122 overflows
	 */


/***********Global Variables************/
u8 Global_u8Second=0;
u8 Global_u8Minutes=0;
u8 Global_u8Hours=0;
u8 Global_Flag=0;
u8 Global_u8TickCounts = 0;
/*************GLOBAL MACROS*************/
#define OVERFLOWS		122

/**************Handlers*****************/
void func(void)
{
	if( Global_Flag == 0){
		Global_u8TickCounts++;
		if(Global_u8TickCounts == OVERFLOWS){
			Global_u8TickCounts=0;
			Global_u8Second++;
		}
		if(Global_u8Second == 60){
			Global_u8Minutes++;
			Global_u8Second=0;
		}
		if(Global_u8Minutes == 60){
			Global_u8Hours++;
			Global_u8Minutes=0;
		}
		if(Global_u8Hours == 24){
			Global_u8Hours=0;
		}
	}
	else{
		    /*Nothing*/
	}
}



void Reset(){
	Global_u8Second=0;
	Global_u8Minutes=0;
	Global_u8Hours=0;
	Global_u8TickCounts=0;
	Global_Flag=0;

}

void Resume(){
	Global_Flag=0;
}

void Pause(){
	Global_Flag=1;
}



/********************************************************************************/
/*								ENTRY POINT <main>								*/
/********************************************************************************/
s32 main(void)
{
	/*************************************************************/
	/*				  MODULES INITIALIZATION				     */
	/*************************************************************/

	/*****************EXTI*******************/
	GIE_VidEn();
	EXTI_voidInit();
	EXTI_u8Int0SetCallBack(Reset);
	EXTI_u8Int1SetCallBack(Pause);
	EXTI_u8Int2SetCallBack(Resume);



	/******************TIMER0*****************/
	TIMER0_voidInit();
	/*************************************************************/
	/*				   SET CALLBACK FUNCTION			     	 */
	/*************************************************************/
	TIMER_u8SetCallBack(func,TIMER0_OVF_VECTOR_ID);
	/*************************************************************/

	   //6 pins from PORTA for SSD connections
	DIO_Vid_SetPinDirection(Port_A,pin0,output);   //sec1
	DIO_Vid_SetPinDirection(Port_A,pin1,output);   //sec2
	DIO_Vid_SetPinDirection(Port_A,pin2,output);   //min1
	DIO_Vid_SetPinDirection(Port_A,pin3,output);   //min2
	DIO_Vid_SetPinDirection(Port_A,pin4,output);   //hour1
	DIO_Vid_SetPinDirection(Port_A,pin5,output);   //hour2


	  //4 pins from PORTC for driver connections
	DIO_Vid_SetPinDirection(Port_C,pin0,output);   //sec1
	DIO_Vid_SetPinDirection(Port_C,pin1,output);   //sec2
	DIO_Vid_SetPinDirection(Port_C,pin2,output);   //min1
	DIO_Vid_SetPinDirection(Port_C,pin3,output);   //min2


	DIO_Vid_SetPinDirection(Port_D,pin2,input);  //INT0(Reset)
	DIO_Vid_SetPinDirection(Port_D,pin3,input);  //INT1(Pause)
	DIO_Vid_SetPinDirection(Port_B,pin2,input);  //INT2(Resume)
	while(1){

		                  /*SEC1*/
		DIO_Vid_SetPinValue(Port_A,pin0,PinHigh);
		DIO_Vid_SetPinValue(Port_A,pin1,PinLow);
		DIO_Vid_SetPinValue(Port_A,pin2,PinLow);
		DIO_Vid_SetPinValue(Port_A,pin3,PinLow);
		DIO_Vid_SetPinValue(Port_A,pin4,PinLow);
		DIO_Vid_SetPinValue(Port_A,pin5,PinLow);
		DIO_Vid_SetPortValue(Port_C,(Global_u8Second % 10));

		_delay_ms(1);

						 /*SEC2*/
       DIO_Vid_SetPinValue(Port_A,pin0,PinLow);
       DIO_Vid_SetPinValue(Port_A,pin1,PinHigh);
       DIO_Vid_SetPinValue(Port_A,pin2,PinLow);
       DIO_Vid_SetPinValue(Port_A,pin3,PinLow);
       DIO_Vid_SetPinValue(Port_A,pin4,PinLow);
       DIO_Vid_SetPinValue(Port_A,pin5,PinLow);
       DIO_Vid_SetPortValue(Port_C,(Global_u8Second / 10));

       _delay_ms(1);

                         /*MIN1*/
      DIO_Vid_SetPinValue(Port_A,pin0,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin1,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin2,PinHigh);
      DIO_Vid_SetPinValue(Port_A,pin3,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin4,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin5,PinLow);
      DIO_Vid_SetPortValue(Port_C,(Global_u8Minutes % 10));

      _delay_ms(1);

      	  	  	  	  	  /*MIN2*/
      DIO_Vid_SetPinValue(Port_A,pin0,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin1,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin2,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin3,PinHigh);
      DIO_Vid_SetPinValue(Port_A,pin4,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin5,PinLow);
      DIO_Vid_SetPortValue(Port_C,(Global_u8Minutes / 10));

      _delay_ms(1);

      	  	  	  	  	  /*HOUR1*/
      DIO_Vid_SetPinValue(Port_A,pin0,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin1,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin2,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin3,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin4,PinHigh);
      DIO_Vid_SetPinValue(Port_A,pin5,PinLow);
      DIO_Vid_SetPortValue(Port_C,(Global_u8Hours % 10));

      _delay_ms(1);

      	  	  	  	  	  /*HOUR2*/
      DIO_Vid_SetPinValue(Port_A,pin0,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin1,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin2,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin3,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin4,PinLow);
      DIO_Vid_SetPinValue(Port_A,pin5,PinHigh);
      DIO_Vid_SetPortValue(Port_C,(Global_u8Hours / 10));

      _delay_ms(1);
      DIO_Vid_SetPinValue(Port_A,pin5,PinLow);

	}

	return 0;
}
