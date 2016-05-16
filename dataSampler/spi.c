//========================================================================
//
//	tccTomazi: spi.c
//
//	SPI functions implementations
//
//========================================================================

#include "main.h"
#include "spi.h"

/*
 * Toggle chip select.
 */

void spi_toggleCS(bool level) {
	if (level) {
		PORTB |= (1 << PORTB2);
	} else {
		PORTB &= ~(1 << PORTB2);
	}
}

/*
 * Configure SPI as Master or Slave.
 */

void spi_init(Spi_operation_e operation) {
	switch(operation) {
		case MASTER:
			DDRB |= (1<<DDB3)|(1<<DDB5)|(1<<DDB2); // MOSI, SCK and CS
			SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
			break;
		case SLAVE:
			DDRB |= (1<<DDB4); // MISO
			SPCR |= (1<<SPE);
			break;		
		default: // should not get here
			break;
	}
}

/*
 * SPI full-duplex communication.
 */

unsigned char spi_transmission(unsigned char byte) {
	SPDR = byte;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}