/*
 * Test05-FND.c
 *
 * Created: 2024-10-16 오후 3:12:41
 * Author : user
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IDDR    DDRA
#define IPORT   PORTA
#define CDDR    DDRB
#define CPORT   PORTB

unsigned char img[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67 }; // 0~9까지의 이미지
char data[4];
int num = 0, st = 0, cnt = 0; // st == 0: reset | 1: start | 2 : stop

ISR(INT0_vect) { // Control 버튼
	switch(st) {
		case 0:	st = 1; break;
		case 1:	st = 2; break;
		case 2:	st = 0; num = 0; break;
	}
}

ISR(TIMER0_COMP_vect) { // Timer용 ((0.01 / 7)s 단위)
	cnt++;
	if(cnt == 7) {
		if(st == 1) {
			num++;
		}
		digit(num);
		cnt = 0;
	}
	TCNT0 = 0;
}

int tcnt = 0;

ISR(TIMER2_OVF_vect) { // FND refresh (4ms 단위)
	if(tcnt++ < 1) { // 인터럽트 여러개 생성되는 것 막아줌
		for (int i = 0; i < 4; i++) { // 4자리 출력
#ifdef FND_TYPE_CATHOD
			CPORT = ~(1 << i);
			IPORT = img[data[i]];
			if (i == 1) { // 소수점 출력
				IPORT |= 0x80;
			}
#else
			CPORT = 1 << i;
			IPORT = ~img[data[i]];
			if (i == 1) {
				IPORT &= ~0x80;
			}
#endif
		_delay_ms(1);
		tcnt = 0;
		}
	}
}

int digit(int num) { // num 변수의 각자리수 추출하여 data[] 배열에 저장
	if(num > 9999) return 0; // 인수 유효성 Check
	// data[0] = (num / 1000) % 10;
	data[0] = num / 1000;
	data[1] = (num / 100) % 10;
	data[2] = (num / 10) % 10;
	data[3] = num % 10;
	return 1;
}

int main(void)
{
	CDDR |= 0x0F;
	IDDR |= 0xFF;
	
	CPORT = 0x0F; // 모든 자리수 선택
	IPORT = ~0xFF; // 전체 세그먼트 ON ---> [8]
	StandBy();
	//st = 0;
	IPORT = 0xFF;
	
	EIMSK |= (1 << INT0);
	EICRA |= ((1 << ISC00) | (1 << ISC01));
	//Timer0: 100ms stop-watch, Timer2: FMD refresh
	TIMSK |= ((1 << OCIE0) | (1 << TOIE2));
	TCCR0 |= ((1 << CS00) | (1 << CS01) | (1 << CS02)); // 분주비 1024
	TCCR2 |= (1 << CS22); // 분주비 256(100) 1024(101) | (1/16M) * 256 * 256 = 0.004096s
	OCR0 = 22; // (1/16M) * 22 * 1024 * 7 = 0.009856 (~10ms)
	sei();
	
	/* Replace with your application code */
    while (1) 
    {
		/* 앞의 숫자 0은 출력 X (코드 최적화 필요)
		for(int j = 0; j < 10000; j++) {
			int r = digit(j);
			if(r) {
				for (int i = 0; i < sizeof(data); i++) { // 4자리 숫자 처리
					CPORT = 1 << i;
					if(j < 10) {
						if(i == 0) {
							IPORT = 0xFF;
							continue;
						}
						if(i == 1) {
							IPORT = 0xFF;
							continue;
						}
						if(i == 2) {
							IPORT = 0xFF;
							continue;
						}
						IPORT = ~img[data[i]];
					}
					else if(j < 100) {
						if(i == 0) {
							IPORT = 0xFF;
							continue;
						}
						if(i == 1) {
							IPORT = 0xFF;
							continue;
						}
						IPORT = ~img[data[i]];
					}
					else if(j < 1000) {
						if(i == 0) {
							IPORT = 0xFF;
							continue;
						}
						IPORT = ~img[data[i]];
					}
					else {
						IPORT = ~img[data[i]];
					}
					_delay_ms(1);
				}
			}
			else {
				break;
			}
			_delay_ms(10);
		}
		*/
	}
}

