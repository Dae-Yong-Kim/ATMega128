/*
 * Test01.c
 *
 * Created: 2024-10-02 오후 12:17:19
 * Author : user
 */ 

#include <avr/io.h>
// LED Port : PG4

int main(void)
{
    /* Replace with your application code */
	//printf("Hello World!");
	DDRG |= 0x10; // 4번 (0~4) | xxxx xxxx ==> xxx1 xxxx | 0=입력 1=출력 | G4번 핀을 출력으로 만들었다.
    while (1) 
    {
		PORTG |= 0x10; // G4번 핀에 1을 내보낼거다.
    }
}

