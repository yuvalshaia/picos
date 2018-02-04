#ifndef _OS_H
#define _OS_H
/***************************************************\
 * + Context switch (timer based - push sched after high_isr)
 * + Multi level call stack
 * + Save more register in context
 * + Mutex
 * + Naming - change all OS function names
 * + Seperate object files (or library) for OS functions
 * + Add exit_task() function which will be call to terminate task
 * - Test on real chip
 * - Split H/W stack to tasks and allow to configure H/W usage, i.e. H/W split or shared H/W stack
 * - Timer(s)
 * - Not such important: Startup function - when done clean task (remove_task())
 * - Some problems while using global (not task) variables
 * - Manage task stack size
\***************************************************/
#include <p18cxxx.h>
#include "task.h"

void os_init(void);
char os_add_task(void* main, char mem_stack_size);
void os_exit_task();
void os_yield(void);
void os_start(void);

// Memory allow 100 (stack for each thread) * F starting from 300 = ~11
#define TASK_COUNT 5

extern struct os_task*			g_running_task;
extern struct os_task_list		g_ready_task_list;

#define CS_BEGIN(intcon_temp) \
	_asm \
		movff	INTCON, intcon_temp\
		bcf		INTCON, 7, 0 \
	_endasm
#define CS_END(intcon_temp) \
	_asm \
		movff	intcon_temp, INTCON \
	_endasm

#endif


