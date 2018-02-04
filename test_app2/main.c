#include <p18cxxx.h>
#include <delays.h>

#include "os.h"

//#pragma config DEBUG	= ON

#define LED_1              LATCbits.LATC0
#define LED_2              LATCbits.LATC1

void enableWDT(int secs);
void enableWDT(int secs)
{
	_asm
		bsf WDTCON,7,ACCESS
		bsf INTCON,5,ACCESS
	_endasm
}

void twoLeds(int cnt);
void twoLeds(int cnt)
{
	int i;
	i = 0;

	while(cnt--)
    {
		LED_2 = 1;
		Delay10KTCYx(100);
		LED_2 = 0;
		Delay10KTCYx(250);

		if (i++ == 3)
		{
			LED_1 ^= 1;
			i = 0;
		}
    }
}

void oneLed(int cnt);
void oneLed(int cnt)
{
	int i;
	i = 0;

	LED_2 = 0;

	while(cnt--)
    {
		LED_1 = 1;
		Delay10KTCYx(250);
		LED_1 = 0;
		Delay10KTCYx(250);
    }
}

void simulateLoad();
void simulateLoad()
{
	oneLed(10);
}

void simulateMainLoop();
void simulateMainLoop()
{
	while (1)
	{
		twoLeds(20);
		_asm clrwdt	_endasm
		Delay10KTCYx(250);
	}
}

void main1(void)
{
	// Default all pins to digital
    ADCON1 |= 0x0F;

	// Start out both LEDs off
	LED_1 = 0;
	LED_2 = 0;

	// Make both LED I/O pins be outputs
	TRISCbits.TRISC0 = 0;
	TRISCbits.TRISC1 = 0;

	enableWDT(12);

	simulateLoad();
	LED_1 = 0;
	LED_2 = 0;
	Delay10KTCYx(250);
	simulateMainLoop();
}

void led1_loop()
{
	int i;
	i = 0;

	while(1)
    {
		Delay10KTCYx(350);

		if (i++ == 3)
		{
			LED_1 ^= 1;
			i = 0;
		}
    }
}

void led2_loop()
{
	while(1)
    {
		LED_2 = 1;
		Delay10KTCYx(100);
		LED_2 = 0;
		Delay10KTCYx(250);
    }
}

void main(void)
{
	// Default all pins to digital
    ADCON1 |= 0x0F;

	// Start out both LEDs off
	LED_1 = 0;
	LED_2 = 0;
led1_loop();
	// Make both LED I/O pins be outputs
	TRISCbits.TRISC0 = 0;
	TRISCbits.TRISC1 = 0;

	//enableWDT(12);

	os_init();

	//os_add_task((char*)&led1_loop, 100);
	//os_add_task((char*)&led2_loop, 100);

	//os_start();
}
