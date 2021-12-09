/*
 ================================================================================================
 Name        : measure_duty_cycle.c
 Author      : Kareem Mohamed
 Description : Measure The time of high ECHO pulse (From Ultrasonic Driver) Using ICU driver
 	 	 	   and Display The Distance On LCD
 Date        : 23/10/2021
 ================================================================================================
 */
#include "lcd.h"
#include "UltraSonic.h"
#include <avr/io.h> /* To use the SREG register */


int main(void)
{
	/* Enable Global Interrupt I-Bit */
		SREG |= (1<<7);

	/* Initialize the UltraSonic driver */
		Ultrasonic_init();

	/* Initialize the LCD driver */
		LCD_init();

	uint16 distance ;

	LCD_displayString("Distance=    cm");

	while(1)
	{

			distance =	Ultrasonic_readDistance();


			if(distance < 100)
			{
				/* Move Cursor To Column 10 On LCD Screen */
				LCD_moveCursor(0,10);
				/* display the Distance on LCD screen */
				LCD_intgerToString(distance);
				LCD_displayString(" cm ");
			}

			else
			{
				/* Move Cursor To Column 10 On LCD Screen */
				LCD_moveCursor(0,10);
				/* display the Distance on LCD screen */
				LCD_intgerToString(distance);
			}
	}

}
