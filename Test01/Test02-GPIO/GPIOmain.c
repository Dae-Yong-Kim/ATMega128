/*
 * Test02-GPIO.c
 *
 * Created: 2024-10-10 오전 9:36:52
 * Author : user
 */ 
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

volatile int ival= 200;

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
	while((PING & _BV(SW_STANDBY)) == 0); // == "while((PING & 0X10) == 0);" == "while(1) { if((PING & 0X10) == 1) break; }"
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
	EIMSK |= ((1 << SW1) | (1 << SW2)); // (Enabel Interrupt MaSK) 사용할 인터럽트의 마스크를 1로 변경
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

