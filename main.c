//========================================================================
//
//	tccTomazi: main.c
//
//	Application functions implementations
//
//========================================================================

#include "main.h"
#include "spi.h"
#include "usart.h"

//========================================================================
// ADXL345 application functions
//========================================================================

/*
 *	Communicate with ADXL. Return a pointer to received data.
 */

void dataTransmission(	Operation_e operation,
						RegisterAddress_e registerAddress, 
						unsigned char * dataArray,
						unsigned int numberOfBytes){
	unsigned char firstByte = registerAddress;
	int i = 0;

	switch (operation) {
		case READ:
			firstByte |= 0x80;
			break;
		case WRITE:
			firstByte &= ~0x80;
			break;
		default: // should not get here
			break;
	}

	if (numberOfBytes > 1) {
		firstByte |= 0x40;
	} else {
		firstByte &= ~0x40;
	}

	spi_toggleCS(false);
	spi_transmission(firstByte);
	for (i = 0; i < numberOfBytes; i++) {
		dataArray[i] = spi_transmission(dataArray[i]);
	}
	spi_toggleCS(true);
}

/*
 *	Reads DEVID register.
 */

unsigned char getDeviceID() {
	unsigned char dataByte = 0x00;
	dataTransmission(READ, DEVID, &dataByte, 1);
	return dataByte;
}

/*
 *	Configure the ADXL for this application.
 */

void adxl_init(OutputDataRate_e outputDataRate, unsigned int watermark) {
	unsigned char dataArray[2] = {0};
	unsigned char dataByte = 0x80 + watermark; // FIFO is in stream mode

	dataArray[0] = outputDataRate; // set data rate in BW_RATE
	dataArray[1] = 0x08; // set Measure bit in POWER_CTL
	dataTransmission(WRITE, BW_RATE, dataArray, 2);
	dataTransmission(WRITE, FIFO_CTL, &dataByte, 16);
}

/*
 *	Read FIFO_STATUS register, return the number of unread values.
 */

int getNumberOfDataValues() {
	unsigned char dataByte = 0x00;
	dataTransmission(READ, FIFO_STATUS, &dataByte, 1);
	return dataByte & 0x3F;
}

/*
 *	Dump FIFO values through USART.
 */

void dumpFifo(unsigned int numberOfDataValues) {
	unsigned char dataArray[6] = {0};
	int x = 0;
	int y = 0;
	int z = 0;
	int i = 0;
	char tmp[32] = {0};

	for (i = 0; i < numberOfDataValues; i++) {
		dataTransmission(READ, DATAX0, dataArray, 6);
		x = (dataArray[1] & 0x03) << 8 + dataArray[0];
		y = (dataArray[3] & 0x03) << 8 + dataArray[2];
		z = (dataArray[5] & 0x03) << 8 + dataArray[4];
		snprintf(tmp, 32, "%d\t%d\t%d\r\n", x, y, z);
		usart_sendString(tmp);
		memset(tmp, 0x00, 32);
	}
}

//========================================================================
// MAIN (super-loop architecture)
//========================================================================

int main() {
	unsigned int numberOfDataValues = 0;

	_delay_ms(10);
	
	usart_init();
	spi_init(MASTER);
	if (getDeviceID() != 0xE5) {
		usart_sendString("ERROR: wrong device ID. Check connections.\r\n");
		exit(1);
	}
	adxl_init(RATE_50Hz, 16);

	while(true) {
		if (numberOfDataValues = getNumberOfDataValues()) {
			dumpFifo(numberOfDataValues);
		}
	}
}
