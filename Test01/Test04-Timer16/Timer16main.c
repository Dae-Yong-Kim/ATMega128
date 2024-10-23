/*
 * Test04-Timer16.c
 *
 * Created: 2024-10-16 오전 11:38:40
 * Author : user
 */
#define F_CPU 16000000UL // 없어도 된다 이게 꼭 필요한 경우는 delay를 사용하는 경우

#include <avr/io.h>
#include <avr/interrupt.h>

#define DDR DDRG
#define PORT PORTG
#define LED_R PORTG0
#define LED_B PORTG1
#define LED_Y PORTG2

volatile int st_1 = 0, st_3 = 0;

ISR(TIMER1_OVF_vect) {
	if (st_1) {	PORT &= ~(1 << LEDY); st_1 = 0;	}
	else {	PORT |= (1 << LEDY); st_1 = 1;	}
}

ISR(TIMER3_OVF_vect) {
	if (st_3) {	PORT &= ~(1 << LEDR); st_3 = 0;	}
	else {	PORT |= (1 << LEDR); st_3 = 1;	}
}

int main(void)
{
	DDR |= ((1 << LEDG) | (1 << LEDY) | (1 << LEDR));
	
	PORT |= ((1 << LEDG) | (1 << LEDY) | (1 << LEDR));
	StandBy();
	PORT &= ~((1 << LEDG) | (1 << LEDY) | (1 << LEDR));
    /* Replace with your application code */
	
	TIMSK |= (1 << TOIE1); // 1번 Timer(16bit) 사용
	ETIMSK |= (1 << TOIE3); // 3번 Timer(16bit) 사용
	TCCR1B |= ((1 << CS10) | (1 << CS11)); // 분주비 64 => OVF 인터럽트 주기: (1/16M) * (분주비) * 65536(0~65535) (~250ms)
	TCCR3B |= ((1 << CS30) | (1 << CS31)); // 분주비 64 => OVF 인터럽트 주기: (1/16M) * (분주비) * 65536(0~65535) (~250ms)
	sei();
	
    while (1) 
    {
    }
}

