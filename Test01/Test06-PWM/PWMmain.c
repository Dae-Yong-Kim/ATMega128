/*
 * Test06-PWM.c
 *
 * Created: 2024-10-23 오전 9:47:53
 * Author : user
 */ 
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define DDR DDRG
#define PORT PORTG
#define LED_R PORTG0
#define LED_B PORTG1
#define LED_Y PORTG2

int period = 10, DutyRate = 50;
int led_y_st = 0, cnt = 0, cnt_ref = 0;

ISR(INT0_vect) {
	//period -= 10;
	//if(period < 0) period = 500;
	OCR1A += 10000;
}

ISR(INT1_vect) {
	//DutyRate += 10;
	//if(DutyRate > 90) DutyRate = 10;
	OCR1B += 10000;
}

ISR(INT2_vect) {
	//OCR0 += 10;
	OCR1C += 10000;
}

ISR(TIMER0_COMP_vect) {
	
}
ISR(TIMER1_COMPA_vect) {
	
}
ISR(TIMER1_COMPB_vect) {
	
}
ISR(TIMER1_COMPC_vect) {
	
}

int main(void)
{
	DDR |= ((1 << LED_R) | (1 << LED_B) | (1 << LED_Y));
	DDRB |= ((1 << PORTB4) | (1 << PORTB5) | (1 << PORTB6) | (1 << PORTB7));
	PORT |= ((1 << LED_R) | (1 << LED_B) | (1 << LED_Y));
	led_y_st = 1;
	StandBy();
	PORT &= ~((1 << LED_R) | (1 << LED_B) | (1 << LED_Y));
	led_y_st = 0;
	
	EIMSK |= ((1 << INT2) | (1 << INT1) | (1 << INT0));
	EICRA |= ((1 << ISC01) | (1 << ISC11) | (1 << ISC21));
	TIMSK |= ((1 << OCIE0) | (1 << OCIE1A) | (1 << OCIE1B));
	ETIMSK |= (1 << OCIE1C);
	TCCR0 |= ((1 << CS02) | (1 << CS01) | (1 << CS00)); // 분주비 1024 (1 / 16000000) * 1024 = 64us
	TCCR0 |= ((1 << WGM01) | (1 << WGM00)); // 0b11: 고속 PWM | 0b10: CTC (TCNT = 0) | 0b01 : 위상 교정 PWM | 0b00 : 정상
	TCCR0 |= (1 << COM01); // 0b10: 비반전 모드 | 0b11: 반전 모드
	OCR0 = 156; // 0.000064 * 156 = 0.009984 ~ 10ms
	TCCR1B |= ((1 << CS10)); // 분주비 1
	TCCR1A |= ((1 << WGM11));
	TCCR1B |= ((1 << WGM13) | (1 << WGM12));
	TCCR1A |= ((1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1)); // 0b10: 비반전 모드 | 0b11: 반전 모드
	OCR1A = 45000;
	OCR1B = 30000;
	OCR1C = 15000;
	ICR1 = 65535;
	sei();
    /* Replace with your application code */
    while (1) 
    {
		PORT |= ((1 << LED_R) | (1 << LED_B));		_delay_ms(period * DutyRate / 100);
		PORT &= ~((1 << LED_R) | (1 << LED_B));		_delay_ms(period * (100 - DutyRate) / 100);
    }
}

