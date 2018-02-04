#include <p18cxxx.h>
#include <delays.h>

#include "os.h"
#include "mutex.h"

#define LED_1              LATCbits.LATC0
#define LED_2              LATCbits.LATC1

int		i1, i2, i3, i11;
int		plus(int i);
int		real_plus(int i);
struct	Mutex m1;

void enableWDT(int secs)
{
	_asm
		bsf WDTCON,7,ACCESS
		bsf INTCON,5,ACCESS
	_endasm
}

void initLeds()
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

	LED_1 = 0;
	LED_2 = 0;
}

void task_loop_1()
{
	i1 = 0;
	i11 = 0;
	while(1) {
		i1++;
		if (i1 == 10) i11++;
	}
}

void task_loop_2()
{
	i2 = 0;
	while(1) {
		i2++;
		//i2 = plus(i2);
	}
}

int real_plus(int i)
{
	return i + 1;
}

int plus(int i) {
	return real_plus(i);
}

void task_loop_3()
{
	i3 = 0;
	while(1) {
		i3 = plus(i3);
		//i3++;
	}
}

void task_mutex_1()
{
	long i;
	while (1) {
		mutex_take(&m1);
		for(i=0; i < 1000000; i++);
		mutex_free(&m1);
	}
}
void task_mutex_2()
{
	long i;
	while (1) {
		mutex_take(&m1);
		for(i=0; i < 1000000; i++);
		mutex_free(&m1);
	}
}

void task_led_1()
{
	while (1) {
		LED_1 = 1;
		Delay10KTCYx(100);
		LED_1 = 0;
		Delay10KTCYx(250);
	}
}

void task_led_2()
{
	Delay10KTCYx(100);
	while (1) {
		LED_2 = 1;
		//LED_1 = 0;
		Delay10KTCYx(100);
		LED_2 = 0;
		//LED_1 = 1;
		Delay10KTCYx(250);
	}
}

void simulateLoad()
{
	int		i = 0;
	int		cnt = 10;

	LED_2 = 0;

	initLeds();

	while(cnt--)
    {
		LED_1 = 1;
		Delay10KTCYx(250);
		LED_1 = 0;
		Delay10KTCYx(250);
    }
}

void main(void)
{
	//initLeds();

	//simulateLoad();

	os_init();

	//add_task((char*)&task_led_1, 100);
	//add_task((char*)&task_led_2, 100);

	//add_task((char*)&task_loop_1, 100);
	//add_task((char*)&task_loop_2, 100);
	//add_task((char*)&task_loop_3, 100);

	add_task((char*)&task_mutex_1, 100);
	add_task((char*)&task_mutex_2, 100);
	mutex_init(&m1);

	os_start();
}