#include "os.h"
#include "mutex.h"

//#pragma config DEBUG	= ON

int		i1, i2, i3, i11;
int		plus(int i);
int		real_plus(int i);
struct	os_mutex m1;

void task_loop_1()
{
	int i = 30000;

	i1 = 0;
	i11 = 0;
	while(i--) {
		i1++;
		if (i1 == 10) i11++;
	}
	i = 19;
	os_exit_task();
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
		os_mutex_take(&m1);
		i1++;
		for(i=0; i < 1000000; i++);
		os_mutex_free(&m1);
	}
}
void task_mutex_2()
{
	long i;
	while (1) {
		os_mutex_take(&m1);
		i2++;
		for(i=0; i < 1000000; i++);
		os_mutex_free(&m1);
	}
}

void main(void)
{
	os_init();

	os_add_task((char*)&task_loop_1, 100);
	os_add_task((char*)&task_loop_2, 100);
	os_add_task((char*)&task_loop_3, 100);

	//os_add_task((char*)&task_mutex_1, 100);
	//os_add_task((char*)&task_mutex_2, 100);
	os_mutex_init(&m1);

	os_start();
}