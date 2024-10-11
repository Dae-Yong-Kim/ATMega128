/*
 * MyLib.c
 *
 * Created: 2024-10-11 오후 2:38:13
 *  Author: user
 */ 
#include <avr/io.h>

int TestBit(char pin, char mask) { // PINx 레지스터의 값의 mask bit가 0인지 1인지 판별
	if((pin & mask) != 0) return 1;
	return 0;
}

void StandBy() { // PG4 pin으로 프로그램 시작 스위치 연결
	DDRG &= ~0x10; // == "DDRG &= ~0x10;" | G4 입력
	PORTG |= 0x10; // == "PORTG |= 0X10;" | G4 내부저항 Pull Up
	
	// PINx의 초기값은 N/A이다.
	// 5V가 연결되어 있어도 처음 기기가 켜질 때는 1임을 보장할 수 없다. 따라서 stand-by 전에 PING1이 1임을 확인하고 들어가자
	while(!TestBit(PING, 0x10)); // == "while((PING & _BV(SW_STANDBY)) == 0);" == "while((PING & 0X10) == 0);" == "while(1) { if((PING & 0X10) != 0) break; }" == "while(1) { if((PING & 0X10) == 0x10) break; }"
	while(TestBit(PING, 0x10)); // == "while(!((PING & _BV(SW_STANDBY)) == 0));" == "while(!((PING & 0X10) == 0));" == "while(!((PING & (1 << SW_STANDBY)) == 0));" == "while(1) { if(PING & 0X10 == 0) break; }"
}