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
#define LED0 PORTG1
#define LED1 PORTG2
#define LED2 PORTG3

volatile int cnt = 0, st = 0, standard = 30;
volatile int ocnt = 0, ost = 0;

ISR(TIMER0_OVF_vect) { // Timer/Count OVerFlow Interrupter
	cnt++;
	if(cnt >= standard) {
		cnt = 0;
		if(st) {
			PORT &= ~(1 << LED2);
			st = 0;
		}
		else {
			PORT |= (1 << LED2);
			st = 1;
		}
	}
}

ISR(TIMER0_COMP_vect) { // Timer/Count Output COMPare Interrupter
	ocnt++;
	if(ocnt >= standard) {
		ocnt = 0;
		if(ost) {
			PORT &= ~(1 << LED1);
			ost = 0;
		}
		else {
			PORT |= (1 << LED1);
			ost = 1;
		}
	}
	TCNT0 = 0;
}

ISR(INT0_vect) { //100ms 느리게 flash
	standard += 6;
}

ISR(INT1_vect) { //100ms 빠르게 flash
	standard -= 6;
	if(standard < 0) {
		standard = 0;
	}
}

int main(void)
{
	DDR |= ((1 << LED0) | (1 << LED1) | (1 << LED2));
	
	PORT &= ~(1 << LED0); // LED0 끄기
	PORT |= ((1 << LED1) | (1 << LED2)); // LED1, 2 켜기
	StandBy();
	PORT &= ~((1 << LED1) | (1 << LED2)); // LED1, 2 끄기
	
	EIMSK |= ((1 << INT0) | (1 << INT1)); // External Interrupt MaSK
	//EICRA = ((EICRA & ~((1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00))) | ((1 << ISC11) | (1 << ISC01)));
	EICRA |= ((1 << ISC11) | (1 << ISC01)); // External Interrupt Control Register
	EICRA &= ~((1 << ISC10) | (1 << ISC00));
	// 8bit counter
	TIMSK |= (1 << TOIE0); // Timer/Counter Overflow Interrupt Enable
	TIMSK |= (1 << OCIE0); // Timer/Counter Output Compare Interrupt Enable
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00); // 분주비 1024 | Timer/Counter Control Register
	OCR0 = 127; // Timer/Counter Output Compare Register
	sei(); // 상태 레지스터 MSB를 1로하여 Interrupt 활성화
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

