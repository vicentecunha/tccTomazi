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
 *	BW_RATE = normal operation
 *	POWER_CTL = measurement mode
 *	INT_ENABLE = enable Overrun INT1
 *	FIFO_CTL = Stream mode
 */

void adxl_init(OutputDataRate_e outputDataRate) {
	unsigned char dataArray[3] = {outputDataRate, 0x08, 0x01};
	unsigned char dataByte = 0x80; // FIFO is in stream mode
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

/*
 *	Configure external interrupt 0 to trigger on rising edge.
 */

interruptsInit() {
	EICRA = 0x03; // The rising edge of INT0 generates an interrupt request.
	EIMSK = 0x01; //  External Interrupt Request 0 Enables
}

/*
 *	External Interrupt 0: Overrun
 */

ISR(INT0_vect) {
	usart_sendString("WARNING: FIFO overrun. Data samples were lost\r\nTry increasing baudrate or lowering sampling frequency.\r\nIf this does not solve the issue, send data in binary mode.\r\n");
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
	adxl_init(RATE_50Hz);

	interruptsInit();
	sei(); // enable global interrupts
	while(true) {
		if (numberOfDataValues = getNumberOfDataValues()) {
			dumpFifo(numberOfDataValues);
		}
	}
}
