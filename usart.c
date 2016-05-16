//========================================================================
//
//	tccTomazi: usart.c
//
//	USART functions implementations
//
//========================================================================

#define FOSC 16000000UL	// Clock Speed
#define BAUD 9600UL		// baudrate
#define DOUBLE_SPEED	// Asynchronous Double Speed mode

#ifdef DOUBLE_SPEED
#define MYUBRR ((FOSC/(8*BAUD))-1)
#else
#define MYUBRR ((FOSC/(16*BAUD))-1)
#endif

#include "main.h"
#include "usart.h"

/*
 * Sets baudrate and enables communication.
 */

void usart_init() {
#ifdef DOUBLE_SPEED
	UCSR0A |= (1 << U2X0);
#endif /* DOUBLE_SPEED */
	UBRR0 = (int)MYUBRR;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

/*
 * Send byte through USART.
 */

void usart_sendByte(unsigned char byte) {
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = byte;
}

/*
 *	Send string through USART.
 */

void usart_sendString(char * str) {
	unsigned int i = 0;
	unsigned int l = strlen(str);
	for (i = 0; i < l; i++) {
		usart_sendByte(str[i]);
	}
}