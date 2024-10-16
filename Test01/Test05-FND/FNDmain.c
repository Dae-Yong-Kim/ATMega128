/*
 * Test05-FND.c
 *
 * Created: 2024-10-16 오후 3:12:41
 * Author : user
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define IDDR    DDRA
#define IPORT   PORTA
#define CDDR    DDRB
#define CPORT   PORTB

unsigned char img[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67 };
int deciDigit = 456;
char data[4];

int main(void)
{
	StandBy();
	CDDR |= 0x0F;
	IDDR |= 0xFF;
	
	CPORT = 0x0F; // 모든 자리수 선택
	IPORT = ~0xFF; // 전체 세그먼트 ON ---> [8]
		
	data[3] = deciDigit % 10;
	data[2] = (deciDigit / 10) % 10;
	data[1] = (deciDigit / 100) % 10;
	data[0] = deciDigit / 1000;
	
	
    /* Replace with your application code */
    while (1) 
    {
		/*
		for (int i = 0; i < 4; i++) {
			CPORT = 1 << i;
			IPORT = ~0xFF;
			for (int j = 0; j < 10; j++) {
				IPORT = ~img[j];
				_delay_ms(100);
			}
		}
		*/
		for (int i = 0; i < sizeof(data); i++) { // 4자리 숫자 처리
			CPORT = 1 << i;
			IPORT = ~img[data[i]];
			_delay_ms(1);
		}
	}
}

