/******************************************************************************
 *
 * Module: UltraSonic
 *
 * File Name: UltraSonic.c
 *
 * Description: Source file for the UltraSonic driver
 *
 * Author: Kareem Mohamed
 *
 *******************************************************************************/

#include "UltraSonic.h"
#include "icu.h"
#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <util/delay.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

static uint8 g_edgeCount = 0;  // Store The No of edges
static uint16 g_timeHigh = ECHO_HIGH_PULSE_NOT_READY;  // Store The High Time Of ECHO Pulse


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Description
 * Initialize the ICU driver as required.
 * Setup the ICU call back function.
 * Setup the direction for the trigger pin as output pin through the GPIO driver.
*/
void Ultrasonic_init(void)
{
	/* Create configuration structure for ICU driver */
	   Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

	/* Initialize ICU driver */
	   Icu_init(&Icu_Config);

	/* Set the Call back function pointer in the ICU driver */
	   Icu_setCallBack(Ultrasonic_edgeProcessing);

	/* Setup the direction for the trigger pin as output pin*/
	   GPIO_setupPinDirection(TRIGGER_PORT,TRIGGER_PIN, PIN_OUTPUT);

}


/* Description
 * Send the Trigger pulse to the ultrasonic.
*/
void Ultrasonic_Trigger(void)
{
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_LOW);
}


/* Description
 * This is the call back function called by the ICU driver.
 * This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
*/
void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		*/
			Icu_clearTimerValue();

		/* Detect falling edge */
			Icu_setEdgeDetectionType(FALLING);

	}

	else if(g_edgeCount == 2)
	{
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);

		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();

		Icu_clearTimerValue();

		g_edgeCount = 0;
	}

}


/* Description
 * Send the trigger pulse by using Ultrasonic_Trigger function.
 * Start the measurements by the ICU from this moment.
 * Return: The measured distance in Centimeter.
*/
uint16 Ultrasonic_readDistance(void)
{
	uint16 Distance ;

	/* Send the trigger pulse */
	   Ultrasonic_Trigger();

	   while(g_timeHigh == ECHO_HIGH_PULSE_NOT_READY);

	   Distance = (g_timeHigh / 58.3) + 1;

	   g_timeHigh = ECHO_HIGH_PULSE_NOT_READY;

	   return Distance;
}
