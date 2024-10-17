# ATMega128
ATMega128 applications

## Explanation of Folder
### Test01
LED On using G4 Port
```
/*
 * Test01.c
 *
 * Created: 2024-10-02 오후 12:17:19
 * Author : user
 */ 
#define F_CPU 16000000UL // 16MHz | 반드시 넣어주자 안넣으면 default로 1M로 문제가 생김
#include <avr/io.h>
#include <util/delay.h> // delay를 사용하기 위해서는 넣어줘라
//  SW : PG3, Input
// LED : PG4, Output

//int Check()
//{
	//char v = PING & 0x08;
	//if (v == 0) return 1;
	//return 0;
//}

int main(void)
{
	/* Replace with your application code */
	//printf("Hello World!");
	DDRG |= 0x10; // 4번 (0~4) | xxxx xxxx ==> xxx1 xxxx | 0=입력 1=출력 | G4번 핀을 출력으로 만들었다.
	DDRG &= ~0x0e; // 1, 2, 3번 (0~4) | xxxx xxxx ==> xxxx 000x | 0=입력 1=출력 | G3번 핀을 입력으로 만들었다.
	//DDG4 = 1; // bit에 직접 접근, but Const (상수) = Read Only ==> 0x10 = 1과 같다. (Error)
	
	char v; // 변수를 while 밖에서 선언
	/*
	########################## SW가 눌리면 flash 시작 ##########################
	*/
	while(1)
	{
		//int v = PING & 0x08; // Error 발생 int는 32bit PING는 8bit
		v = PING & 0x08;
		if(v == 0)
		{
			break;
		}
	}

	while (1) 
	{
		/*
		########################## G4 pin에 200ms 간격으로 LED On Off - 2 ##########################
		*/
		if (PORTG & 0x10) // LED가 켜져있다면
		{
			PORTG &= ~0x10; // 끄고
		} else // 아니라면
		{
			PORTG |= 0x10; // 켜라
		}
		_delay_ms(200); // delay의 사용법
	}
}
```
### Test02-GPIO
```
//SW 3개로 구성 & G4: Program Start (standby), D1: flash 빠르게, D0: flash 느리게 & G1: LED
#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL // 16MHz | 반드시 넣어주자 안넣으면 default로 1G로 문제가 생김
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define LED1 PORTG3 // led1 (PORTG3 == 3)
#define SW1 PORTD0 // switch1 (PORTD0 == 0)
#define SW2 PORTD1 // switch2 (PORTD1 == 1)
#define SW_STANDBY PORTG4 // switch for standby (PORTG4 == 4)

volatile int ival= 200; // interrupt에서 사용하려면 volatile 써서 compiler가 최적화 못하게 전역변수로 선언

//Interrupt를 통해서 timing 이슈 해결
ISR(INT0_vect){ // 느리게
	ival += 500;
}
ISR(INT1_vect){ // 빠르게
	ival -= 500;
	if (ival < 0) ival = 0;
}

void StandBy() { // PG4 pin으로 프로그램 시작 스위치 연결
	DDRG &= ~_BV(SW_STANDBY); // == "DDRG &= ~0x10;" | G4 입력
	PORTG |= _BV(SW_STANDBY); // == "PORTG |= 0X10;" | G4 내부저항 Pull Up
	
	// PINx의 초기값은 N/A이다.
	// 5V가 연결되어 있어도 처음 기기가 켜질 때는 1임을 보장할 수 없다. 따라서 stand-by 전에 PING1이 1임을 확인하고 들어가자
	while((PING & _BV(SW_STANDBY)) == 0); // == "while((PING & 0X10) == 0);" == "while(1) { if((PING & 0X10) != 0) break; }" == "while(1) { if((PING & 0X10) == 0x10) break; }"
	while(1) { // stand-by
		//if(PING1 == 0) { //이런식으로 바로 사용은 불가능하다. (Error)
		if((PING & _BV(SW_STANDBY)) == 0) { // == "if(PING & 0X10 == 0) {" == "if((PING & (1 << SW_STANDBY)) == 0) {"
			break;
		}
	}
}

int main(void)
{
	// 포트를 정의는 16진수로 위에 적어라
	DDRG |= _BV(LED1); // G1 출력
	//DDG1 = 1; // bit에 직접 접근, but Const (상수) = Read Only ==> 0x10 = 1과 같다. (Error)
	DDRG &= ~(_BV(SW1) | _BV(SW2)); // G2, G3 입력
	// Input Pin의 PORTX는 내부 Pull Up/Down을 정함
	PORTG |= 0x0c; // G2, G3 내부 Pull Up
	
	// Interrupt 사용 설정
	EIMSK |= ((1 << INT0) | (1 << INT1)); // (External Interrupt MaSK) 사용할 인터럽트의 마스크를 1로 변경
	EICRA |= ((1 << ISC11) | (1 << ISC01)); // (External Interrupt Control Register A) INT0 ~ INT3까지의 컨트롤 방식 설정
	sei();
	
	//PORTG |= Ox10; // LED On
	//PORTG |= (1 << LED1); // LED On
	//C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\sfr_defs.h 파일을 프로젝트에 드레그하여 메크로를 사용할 수 있게 해준다. (그냥 Microchip Studio 화면에 드래그해라)
	PORTG |= _BV(LED1); // LED On
	
	StandBy();
	
	PORTG &= ~_BV(LED1); // LED Off
	
    /* Replace with your application code */
    while (1) // main loop
    {
		PORTG |= _BV(LED1); // LED On
		_delay_ms(ival);
		PORTG &= ~_BV(LED1); // LED Off
		_delay_ms(ival);
		
		// timing 이슈 발생 SW가 눌릴 때 위의 딜레이가 아닌 이 부분이 수행되어야함
		// ival은 _delay_ms의 파라미터인데 이 값이 바뀌면 에러 -> 해결 방법: __DELAY_BACKWARD_COMPATIBLE__를 define 해준다.
		/*
		if((PING & _BV(SW2)) == 0) { // switch2가 눌리면
			ival -= 500; // flash 빠르게
			if (ival <= 0){
				ival = 0;
			}
		}
		
		if((PING & _BV(SW1)) == 0) { // switch1이 눌리면
			ival += 500; // flash 느리게
		}
		*/
    }
}
```
### Test03-Timer
```
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
```
### Test04-Timer16
```
#define F_CPU 16000000UL // 없어도 된다 이게 꼭 필요한 경우는 delay를 사용하는 경우

#include <avr/io.h>
#include <avr/interrupt.h>

#define DDR DDRG
#define PORT PORTG
#define LEDG PORTG1
#define LEDY PORTG2
#define LEDR PORTG3

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
```
### Test05-FND
```
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
		if(st == 1) {	num++;	}
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
			if (i == 1) {	IPORT |= 0x80;	} // 소수점 출력
#else
			CPORT = 1 << i;
			IPORT = ~img[data[i]];
			if (i == 1) {	IPORT &= ~0x80;	}
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
						if(i == 0) {	IPORT = 0xFF; continue;	}
						if(i == 1) {	IPORT = 0xFF; continue;	}
						if(i == 2) {	IPORT = 0xFF; continue;	}
						IPORT = ~img[data[i]];
					}
					else if(j < 100) {
						if(i == 0) {	IPORT = 0xFF; continue;	}
						if(i == 1) {	IPORT = 0xFF; continue;	}
						IPORT = ~img[data[i]];
					}
					else if(j < 1000) {
						if(i == 0) {	IPORT = 0xFF; continue;	}
						IPORT = ~img[data[i]];
					}
					else {	IPORT = ~img[data[i]];	}
					_delay_ms(1);
				}
			}
			else {	break;	}
			_delay_ms(10);
		}
		*/
	}
}
```
