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
#define GDDR    DDRG
#define GPORT   PORTG
#define BUZZER  PORTG0

unsigned char img[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67 }; // 0~9까지의 이미지
char stopwatch_data[4], clock_data[4], timer_data[4];
volatile int stopwatch_num = 0, clock_num = 0, timer_num = 0, stopwatch_st = 0, clock_st = 0, timer_st = 0, mode = 0, tcnt = 0; // st == 0: reset | 1: start | 2 : stop

ISR(INT0_vect) { // Control 모드 버튼
	switch(mode){
		case 0: // Clock
			switch(clock_st) { // Clock: Default 모드, 시간 설정 모드 변경
				case 0: clock_st++; break; // default Clock
				case 1: clock_st--; break; // min 수정
			}
			break;
		case 1: // Stopt-Watch
			switch(stopwatch_st) {
				case 0: stopwatch_st++; break; // stop
				case 1: stopwatch_st--; break; // start
			}
			break;
		case 2: // Timer
			switch(timer_st) {
				case 0: if(timer_num > 0) timer_st++; break; // 시간 세팅
				case 1: timer_st++; break; // Timer 작동
				case 2: if(timer_num > 0) timer_st--; break; // Timer 정지
				case 3: GPORT &= ~(1 << BUZZER); timer_st = 0; break; // Buzzer
			}
			break;
	}
}

ISR(INT1_vect) { // Start Stop + 시간 설정 버튼
	switch(mode) {
		case 0: // Clock 모드
			if(clock_st == 1) {
				clock_num += 60;
				clock_digit(clock_num);
			}
			break;
		case 1: // Stop-Watch 모드
			switch(stopwatch_st) { // Stopt-Watch 리셋
				case 0: stopwatch_num = 0; stopwatch_digit(stopwatch_num); break; // stop
				case 1: break; // start
			}
			break;
		case 2:
			if(timer_st == 0) {
				timer_num += 10;
				timer_digit(timer_num);
			}
			 break; // Timer 모드
	}
}

ISR(INT2_vect) { // Mode 설정
	switch(mode) {
		case 0:	mode = 1; clock_st = 0; break; // default Clock 모드
		case 1:	mode = 2; break; // Stop-Watch 모드
		case 2:	mode = 0; break; // Timer 모드
	}
}

ISR(TIMER0_COMP_vect) { // Stop-Watch Timer 0.01s 단위
	switch(stopwatch_st) {
		case 0: break; // stop
		case 1:	stopwatch_num++; stopwatch_digit(stopwatch_num); break; // start
	}
	TCNT0 = 0;
}

ISR(TIMER1_COMPA_vect) { // Clock & Timer Timer (1s 단위)
	clock_num++;
	clock_digit(clock_num);
	if(timer_st == 1) { // Timer 작동 중
		if(timer_num > 0) {	timer_num--; timer_digit(timer_num);	}
		else {	GPORT |= (1 << BUZZER); timer_st = 3;	}
	}
	TCNT1 = 0;
}

ISR(TIMER2_OVF_vect) { // FND refresh (4ms 단위)
	if(tcnt++ < 1) { // 인터럽트 여러개 생성되는 것 막아줌
		for (int i = 0; i < 4; i++) { // 4자리 출력
			CPORT = 1 << i;
			switch(mode) {
				case 0:	IPORT = ~img[clock_data[i]]; break;
				case 1: IPORT = ~img[stopwatch_data[i]]; break;
				case 2: IPORT = ~img[timer_data[i]]; break;
			}
			if (i == 1) {	IPORT &= ~0x80;	}
			_delay_ms(1);
			tcnt = 0;
		}
	}
}

int clock_digit(int clock_num) { // stopwatch_num 변수의 각자리수 추출하여 clock_data[] 배열에 저장
	if(clock_num > 2599) return 0; // 인수 유효성 Check
	// clock_data[0] = (stopwatch_num / 1000) % 10;
	int min_num = clock_num / 60;
	int sec_num = clock_num % 60;
	clock_data[0] = min_num / 10;
	clock_data[1] = min_num % 10;
	clock_data[2] = sec_num / 10;
	clock_data[3] = sec_num % 10;
	return 1;
}

int stopwatch_digit(int stopwatch_num) { // stopwatch_num 변수의 각자리수 추출하여 stopwatch_data[] 배열에 저장
	if(stopwatch_num > 2599) return 0; // 인수 유효성 Check
	// stopwatch_data[0] = (stopwatch_num / 1000) % 10;
	int min_num = stopwatch_num / 60;
	int sec_num = stopwatch_num % 60;
	stopwatch_data[0] = min_num / 10;
	stopwatch_data[1] = min_num % 10;
	stopwatch_data[2] = sec_num / 10;
	stopwatch_data[3] = sec_num % 10;
	return 1;
}

int timer_digit(int timer_num) { // stopwatch_num 변수의 각자리수 추출하여 clock_data[] 배열에 저장
	if(timer_num > 2599) return 0; // 인수 유효성 Check
	// clock_data[0] = (stopwatch_num / 1000) % 10;
	int min_num = timer_num / 60;
	int sec_num = timer_num % 60;
	timer_data[0] = min_num / 10;
	timer_data[1] = min_num % 10;
	timer_data[2] = sec_num / 10;
	timer_data[3] = sec_num % 10;
	return 1;
}

int main(void)
{
	GDDR |= (1 << BUZZER);
	CDDR |= 0x0F;
	IDDR |= 0xFF;
	
	CPORT = 0x0F; // 모든 자리수 선택
	IPORT = ~0xFF; // 전체 세그먼트 ON ---> [8]
	StandBy();
	//stopwatch_st = 0;
	IPORT = 0xFF;
	
	EIMSK |= ((1 << INT0) | (1 << INT1) | (1 << INT2));
	EICRA |= ((1 << ISC01) | (1 << ISC11) |(1 << ISC21));
	//Timer0: 100ms stop-watch, Timer2: FMD refresh
	TIMSK |= ((1 << OCIE0) | (1 << TOIE2) | (1 << OCIE1A));
	TCCR0 |= ((1 << CS00) | (1 << CS01) | (1 << CS02)); // 분주비 1024
	TCCR1B |= (1 << CS12); // 분주비 256 | 
	TCCR2 |= (1 << CS22); // 분주비 256(100) 1024(101) | (1/16M) * 256 * 256 = 0.004096s
	OCR0 = 127; // (1/16M) * 154 * 1024 = 0.009856 (~10ms)이지만 실제 시간과 달라서 조금 조정
	OCR1A = 62500; //(1/16M) * 62500 * 256 = 1(s)
	sei();
	
	/* Replace with your application code */
    while (1) 
    {
		/* 앞의 숫자 0은 출력 X (코드 최적화 필요)
		for(int j = 0; j < 10000; j++) {
			int r = stopwatch_digit(j);
			if(r) {
				for (int i = 0; i < sizeof(stopwatch_data); i++) { // 4자리 숫자 처리
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
						IPORT = ~img[stopwatch_data[i]];
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
						IPORT = ~img[stopwatch_data[i]];
					}
					else if(j < 1000) {
						if(i == 0) {
							IPORT = 0xFF;
							continue;
						}
						IPORT = ~img[stopwatch_data[i]];
					}
					else {
						IPORT = ~img[stopwatch_data[i]];
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