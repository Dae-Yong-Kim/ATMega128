/*
 * Test03-Timer.c
 *
 * Created: 2024-10-11 오후 2:32:59
 * Author : user
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define DDR DDRG
#define PORT PORTG
#define LED PORTG3

volatile int cnt = 0, st = 0, standard = 30;

ISR(TIMER0_OVF_vect) { // Timer Overflow Interrupter
	cnt++;
	if(cnt >= standard) {
		cnt = 0;
		if(st) {
			PORT &= ~(1 << LED);
			st = 0;
		}
		else {
			PORT |= (1 << LED);
			st = 1;
		}
	}
}

ISR(INT0_vect) {
	standard += 6;
}

ISR(INT1_vect) {
	standard -= 6;
	if(standard < 0) {
		standard = 0;
	}
}

int main(void)
{
	DDR |= (1 << LED);
	
	PORT |= (1 << LED);
	StandBy();
	PORT &= ~(1 << LED);
	
	EIMSK |= ((1 << INT0) | (1 << INT1));
	//EICRA = ((EICRA & ~((1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00))) | ((1 << ISC11) | (1 << ISC01)));
	EICRA |= ((1 << ISC11) | (1 << ISC01));
	EICRA &= ~((1 << ISC10) | (1 << ISC00));
	TIMSK |= (1 << TOIE0);
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00); // 분주비 1024
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

