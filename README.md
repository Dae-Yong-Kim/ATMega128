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
#define F_CPU 16000000UL // 16MHz | 반드시 넣어주자 안넣으면 default로 1G로 문제가 생김
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
//SW 3개로 구성 & G1: SW LED 켜기, G2: flash 빠르게, G3: flash 느리게 & G4: LED
#define F_CPU 16000000UL
#include <avr/io.h>
#define SW1 PORTG1 // switch1 (PORTG1 == 1)
#define SW2 PORTG2 // switch2 (PORTG2 == 2)
#define SW3 PORTG3 // switch3 (PORTG3 == 3)
#define LED1 PORTG4 // led1 (PORTG4 == 4)


int main(void)
{
	// 포트를 정의하는 코드는 관리하기 쉽게 가장 앞부분에 넣어라
	// 포트를 정의할 때는 16진수를 사용하라 이게 표준이다.
	//DDRG |= 0x10; // 4번 (0~4) | xxxx xxxx ==> xxx1 xxxx | 0=입력 1=출력 | G4번 핀을 출력으로 만들었다.
	//DDRG &= ~0x0e; // 1, 2, 3번 (0~4) | xxxx xxxx ==> xxxx 000x | 0=입력 1=출력 | G1, G2, G3번 핀을 입력으로 만들었다.
	// 포트 정의를 위와 같이 쓰는게 좋지만 아래와 같이 써도 됨 BUT 아래는 G0도 입력이 됨
	DDRG = 0x10; // G4번 핀을 출력 G0, G1, G2, G3번 핀을 입력으로 만들었다.
	//DDG4 = 1; // bit에 직접 접근, but Const (상수) = Read Only ==> 0x10 = 1과 같다. (Error)
	
	//PORTG = Ox10;
	//PORTG = (1 << LED1); // Ox10 | 시작되면 LED On
	//C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\sfr_defs.h 파일을 프로젝트에 드레그하여 메크로를 사용할 수 있게 해준다. (그냥 Microchip Studio 화면에 드래그해라)
	PORTG = _BV(LED1); // Ox10 | 메크로로 위와 정확하게 같은 동작을 한다.
	
	while(1) {
		//if(PING1 == 0) { //이런식으로 바로 사용은 불가능하다. (Error)
		//if(PING & 0x02 == 0) { // 이런식으로 사용하는 것은 아래와 같다.
		//if((PING & (1 << SW1)) == 0) { // 0x02
		if((PING & _BV(SW1)) == 0) { // switch1이 눌리면
			PORTG &= ~_BV(LED1);  // LED Off
			break;
		}
	}
    /* Replace with your application code */
    while (1) 
    {
    }
}
```
