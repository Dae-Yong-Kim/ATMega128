# ATMega128
ATMega128 applications

## Explanation of Folder
### Test 01
LED On using G4 Port
```
#define F_CPU 16000000UL // 16MHz | 반드시 넣어주자 안넣으면 default로 1G로 문제가 생김
#include <avr/io.h>
#include <util/delay.h>// delay 사용시 필요
//  SW : PG3, Input
// LED : PG4, Output

//int Check()
//{
	//char v = PING & 0x08;
	//if (v == 0) return 1;
	//return 0;
//}

int AjustSpeed(int* numOfDelay)
{
	char v = PING & 0x08; // PINX는 입력에 대한 레지스터
	if (v == 0)	*numOfDelay += 5;
	
	v =  PING & 0x04;
	if (v == 0)	*numOfDelay -= 5;
	if(*numOfDelay < 0) *numOfDelay = 0;
	
	v =  PING & 0x02;
	if (v == 0) return 1;
	return 0;
}

int main(void)
{
    /* Replace with your application code */
	//printf("Hello World!");
	DDRG |= 0x10; // Data Direction Register G | 4번 (0~4) | xxxx xxxx ==> xxx1 xxxx | 0=입력 1=출력 | G4번 핀을 출력으로 만들었다.
	DDRG &= ~0x0e; // Data Direction Register G | 3번 (0~4) | xxxx xxxx ==> xxxx 000x | 0=입력 1=출력 | G3번 핀을 입력으로 만들었다. | 계산 귀찮으니 ~ 연산 사용
	//DDG4 = 1; // bit에 직접 접근, but Const (상수) = Read Only ==> 0x10 = 1과 같다. (Error)
	
	//char v; // 변수를 while 밖에서 선언
	int toggle = 0; // toggle=0 : disable, toggle=1 : active
	int numOfDelay = 20;

    while (1) 
    {
		
		/*
		########################## PG1: start, PG2: fast, PG3: slow ##########################
		*/
		if(AjustSpeed(&numOfDelay)){
			PORTG &= ~0x10; // PORTX는 출력에 대한 레지스터
			if (toggle == 1)	toggle = 0;
			else				toggle = 1;
			_delay_ms(300);
		}
		
		if(toggle){
			if (PORTG & 0x10) // LED가 켜져있다면
			{
				PORTG &= ~0x10; // 끄고
			} else // 아니라면
			{
				PORTG |= 0x10; // 켜라
			}
			for(int i = 0; i < numOfDelay; i++){
				_delay_ms(10); // delay 함수
			}
		}
	}
}
```
