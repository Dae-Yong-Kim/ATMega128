/*
 * Test01.c
 *
 * Created: 2024-10-02 오후 12:17:19
 * Author : user
 */ 
#define F_CPU 16000000UL // 16MHz | delay 사용시 필요
#include <avr/io.h>
#include <util/delay.h>
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
	char v = PING & 0x08;
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
	DDRG |= 0x10; // 4번 (0~4) | xxxx xxxx ==> xxx1 xxxx | 0=입력 1=출력 | G4번 핀을 출력으로 만들었다.
	DDRG &= ~0x0e; // 3번 (0~4) | xxxx xxxx ==> xxxx 000x | 0=입력 1=출력 | G3번 핀을 입력으로 만들었다.
	//DDG4 = 1; // bit에 직접 접근, but Const (상수) = Read Only ==> 0x10 = 1과 같다. (Error)
	
	//char v; // 변수를 while 밖에서 선언
	int toggle = 0; // toggle=0 : disable, toggle=1 : active
	int numOfDelay = 20;
	/*
	########################## SW가 눌리면 flash 시작 ##########################
	*/
	//while(1)
	//{
		////int v = PING & 0x08; // Error 발생 int는 32bit PING는 8bit
		//v = PING & 0x08;
		//if(v == 0)
		//{
			//break;
		//}
	//}
		
    while (1) 
    {
		/*
		########################## G4 pin에 1을 출력하여 LED On ##########################
		*/
		//PORTG |= 0x10; // G4번 핀에 1을 내보낼거다.
		
		/*
		########################## G4 pin에 500ms 간격으로 LED On Off - 1 ##########################
		*/
		//PORTG |= 0x10;
		//_delay_ms(200);
		//PORTG &= ~0x10;
		//_delay_ms(200);
		
		/*
		########################## G4 pin에 500ms 간격으로 LED On Off - 2 ##########################
		*/
		//if (PORTG & 0x10) // LED가 켜져있다면
		//{
			//PORTG &= ~0x10; // 끄고
		//} else // 아니라면
		//{
			//PORTG |= 0x10; // 켜라
		//}
		//_delay_ms(200);
		
		/*
		########################## SW가 눌리면 flash 안눌리면 아무일도 없다. ##########################
		*/
		//v = PING & 0x08;
		//if (v == 0){
			//if (PORTG & 0x10) // LED가 켜져있다면
			//{
				//PORTG &= ~0x10; // 끄고
			//} else // 아니라면
			//{
				//PORTG |= 0x10; // 켜라
			//}
			//_delay_ms(200);
		//} else
		//{
			//PORTG &= ~0x10;
		//}
		
		/*
		########################## SW가 눌리면 flash와 아무일 변경 ##########################
		*/
		//if(Check()){
			//PORTG &= ~0x10;
			//if (toggle == 1)	toggle = 0;
			//else				toggle = 1;
			//_delay_ms(300);
		//}
		//
		//if(toggle){
			//if (PORTG & 0x10) // LED가 켜져있다면
			//{
				//PORTG &= ~0x10; // 끄고
			//} else // 아니라면
			//{
				//PORTG |= 0x10; // 켜라
			//}
			//_delay_ms(200);
		//}
		
		/*
		########################## PG1: start, PG2: fast, PG3: slow ##########################
		*/
		if(AjustSpeed(&numOfDelay)){
			PORTG &= ~0x10;
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
				_delay_ms(10);
			}
		}
	}
}