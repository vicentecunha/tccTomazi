//========================================================================
//
//	tccTomazi: main.c
//
//	Application header
//
//========================================================================

#ifndef _MAIN_H_
#define _MAIN_H_

//========================================================================
// INCLUDES
//========================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//========================================================================
// TYPEDEFS
//========================================================================

typedef enum { false, true } bool;

typedef enum RegisterAddress_e {
	DEVID 			= 0x00, // Device ID
	THRESH_TAP 		= 0x1D, // Tap threshold
	OFSX 			= 0x1E, // X-axis offset
	OFSY 			= 0x1F, // Y-axis offset
	OFSZ 			= 0x20, // Z-axis offset
	DUR 			= 0x21, // Tap duration
	Latent 			= 0x22, // Tap latency
	Window 			= 0x23, // Tap window
	THRESH_ACT 		= 0x24, // Activity threshold
	THRESH_INACT 	= 0x25, // Inactivity threshold
	TIME_INACT 		= 0x26, // Inactivity time
	ACT_INACT_CTL 	= 0x27, // Axis enable control for activity and inactivity detection
	THRESH_FF 		= 0x28, // Free-fall threshold
	TIME_FF 		= 0x29, // Free-fall time
	TAP_AXES      	= 0x2A, // Axis control for single tap/double tap
	ACT_TAP_STATUS	= 0x2B, // Source of single tap/double tap
	BW_RATE 		= 0x2C, // Data rate and power mode control
	POWER_CTL 		= 0x2D, // Power-saving features control
	INT_ENABLE 		= 0x2E, // Interrupt enable control
	INT_MAP 		= 0x2F, // Interrupt mapping control
	INT_SOURCE 		= 0x30, // Source of interrupts
	DATA_FORMAT 	= 0x31, // Data format control
	DATAX0 			= 0x32, // X-Axis Data 0
	DATAX1 			= 0x33, // X-Axis Data 1
	DATAY0 			= 0x34, // Y-Axis Data 0
	DATAY1 			= 0x35, // Y-Axis Data 1
	DATAZ0 			= 0x36, // Z-Axis Data 0
	DATAZ1 			= 0x37, // Z-Axis Data 1
	FIFO_CTL 		= 0x38, // FIFO control
	FIFO_STATUS 	= 0x39  // FIFO status
} RegisterAddress_e;

typedef enum OutputDataRate_e {
	RATE_0Hz10	= 0x00,
	RATE_0Hz20	= 0x01,
	RATE_0Hz39	= 0x02,
	RATE_0Hz78	= 0x03,
	RATE_1Hz56	= 0x04,
	RATE_3Hz13	= 0x05,
	RATE_6Hz25	= 0x06,
	RATE_12Hz5	= 0x07,
	RATE_25Hz	= 0x08,
	RATE_50Hz	= 0x09,
	RATE_100Hz	= 0x0A,
	RATE_200Hz	= 0x0B,
	RATE_400Hz	= 0x0C,
	RATE_800Hz	= 0x0D,
	RATE_1600Hz	= 0x0E,
	RATE_3200Hz	= 0x0F
} OutputDataRate_e;

typedef enum Operation_e {
	WRITE,
	READ
} Operation_e;


#endif /* _MAIN_H_ */