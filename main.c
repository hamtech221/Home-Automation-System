#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define pins for relays
#define RELAY_1_PIN 0
#define RELAY_2_PIN 1
#define RELAY_3_PIN 2
#define RELAY_4_PIN 3

// Initialize USART for 9600 baud
void USART_Init() {
	unsigned int ubrr = F_CPU/16/9600-1;  // Set baud rate for 9600 at F_CPU = 16MHz

	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	// Enable transmitter
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	
	// Set frame format: 8 data bits, 1 stop bit
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

// Transmit data via USART
void USART_Transmit(unsigned char data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0))) {
		// Do nothing, just wait
	}
	// Put data into the buffer, which sends it
	UDR0 = data;
}

// Receive data via USART
unsigned char USART_Receive(void) {
	// Wait for data to be received
	while (!(UCSR0A & (1<<RXC0))) {
		// Do nothing, just wait
	}
	// Get and return received data from the buffer
	return UDR0;
}

// Initialize relay pins
void relay_Init() {
	// Set relay pins as outputs
	DDRB |= (1<<RELAY_1_PIN) | (1<<RELAY_2_PIN) | (1<<RELAY_3_PIN) | (1<<RELAY_4_PIN);
	
	// Initialize all relays to off (LOW)
	PORTB &= ~((1<<RELAY_1_PIN) | (1<<RELAY_2_PIN) | (1<<RELAY_3_PIN) | (1<<RELAY_4_PIN));
}

// Control relay based on the received command
void controlRelays(unsigned char command) {
	switch (command) {
		case '1':
		PORTB |= (1<<RELAY_1_PIN);  // Turn on relay 1
		break;
		case '2':
		PORTB |= (1<<RELAY_2_PIN);  // Turn on relay 2
		break;
		case '3':
		PORTB |= (1<<RELAY_3_PIN);  // Turn on relay 3
		break;
		case '4':
		PORTB |= (1<<RELAY_4_PIN);  // Turn on relay 4
		break;
		case '0':
		PORTB &= ~((1<<RELAY_1_PIN) | (1<<RELAY_2_PIN) | (1<<RELAY_3_PIN) | (1<<RELAY_4_PIN));  // Turn off all relays
		break;
		default:
		break;
	}
}

int main(void) {
	// Initialize USART and relays
	USART_Init();
	relay_Init();

	// Infinite loop to receive Bluetooth commands and control relays
	while (1) {
		unsigned char command = USART_Receive();  // Receive data via USART

		// Control relays based on the received command
		controlRelays(command);
	}

	return 0;
}
