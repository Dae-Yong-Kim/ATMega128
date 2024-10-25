/*
 * Test07-UART.c
 *
 * Created: 2024-10-25 오전 9:41:46
 * Author : user
 */ 
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

char buf[256];
char *str = "Button Pushed!!!!\r\n";
int n = 0;

void UART0_init() {
	// UART Register setting - 9600(Baud rate) N(None parity bit) 8(Data bit) 1(Stop bit)
	UCSR0A |= (1 << U2X0);								// 2배속 설정
	UBRR0H = 0;
	UBRR0L = 207;										// Baud rate : 9600
	
	UCSR0C &= ~((1 << UPM00) | (1 << UPM01));			// Parity - 00: None(default), 10: Even, 11: Odd
	
	UCSR0B &= ~(1 << UCSZ02);
	UCSR0C |= ((1 << UCSZ00) | (1 << UCSZ01));			// Data bit - 000: 5bit, 001: 6bit, 010: 7bit, 011: 8bit(default), 111: 9bit
	
	UCSR0C &= ~(1 << USBS0);							// Stop bit - 0: 1bit(default), 1: 2bit
	
	UCSR0B |= ((1 << RXEN0) | (1 << TXEN0));			// RX와 TX Open
}

void UART0_putc(char c) {
	while(!(UCSR0A & (1 << UDRE0))){} // 입출력 버퍼가 준비될 때까지 대기
	UDR0 = c; // send
}

void UART0_puts(char *str) {
	while(*str){
		UART0_putc(*str++);
	}
}

ISR(INT0_vect) {
	sprintf(buf, "Button Pushed!!!! - %d times\r\n", ++n);
	UART0_puts(buf);
}

int main(void)
{
    /* Replace with your application code */
	UART0_init();
	// ESC = "\033(8진수)"
	UART0_puts("\033[2J"); // screen clear - ESC[2J
	UART0_puts("\033[0;0H"); // move cursor to LeftRop - ESC[0;0H
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC01);
	
	sei();
	
    while (1) 
    {
    }
}

