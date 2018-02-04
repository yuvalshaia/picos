#include <p18cxxx.h>

#include "os.h"
#include "task.h"

struct os_task*			g_running_task;
struct os_task_list		g_ready_task_list;
char 					gm_intcon_temp;
struct os_task			gm_task_arr[TASK_COUNT];
unsigned char			gm_current_task_id;
struct os_task_list		gm_free_task_list;
char					WREG_TEMP;
char					STKPTR_TEMP;
char					TOSL_TEMP;
char					TOSH_TEMP;

void init_tasks_list(void);
void sched_init(void);
void sched(void);
void save_context(struct os_task* task);
void restore_context(struct os_task* task);

//#pragma config WDT		= OFF
//#pragma config DEBUG	= ON

void timer_isr(void);

void* p_sched = (void*)sched;
#pragma code high_vector=0x08
void high_interrupt(void)
{
	_asm
		// Clear int flag
		bcf		INTCON, 2, 0

		// TODO: Add check here if this is timer int
		// Push sched addr to h/w stack
		incf	STKPTR, 1, 0
		movff	p_sched, WREG
		movwf	TOSL, 0
		movff	p_sched + 1, WREG
		movwf	TOSH, 0

		// Enable interrupt
		bsf		INTCON, 7, 0
	_endasm
}
#pragma code

void sched_init(void) {
	gm_current_task_id = 0;
	g_running_task = 0;
}

void sched(void)
{
	char	old_task_id;
	char	intcon_temp;

	CS_BEGIN(intcon_temp);
	old_task_id = g_running_task->id;
	g_running_task->state = TS_Ready;
	g_running_task = task_list_next_element(&g_ready_task_list);
	g_running_task->state = TS_Running;
	gm_current_task_id = g_running_task->id;
	if (gm_current_task_id != old_task_id) {
		save_context(&gm_task_arr[old_task_id]);
		restore_context(&gm_task_arr[gm_current_task_id]);
	}
	CS_END(intcon_temp);
}

void save_context(struct os_task* task)
{
	_asm
		movlw	0xfd
		movff	PLUSW2, FSR0L
		movlw	0xfe
		movff	PLUSW2, FSR0H
		// Save h/w stack pointer
		movff	STKPTR, STKPTR_TEMP
		// Save backup of W as we will use it here
		movwf	WREG_TEMP, 0

		// Save call-stack
		decf	STKPTR, 1, 0
		movff	STKPTR, POSTINC0
	loop:
		movff	TOSL, POSTINC0
		movff	TOSH, POSTINC0
		decf	STKPTR, 1, 0
		movlw	0x2
		XORWF	STKPTR, 0, 0
		BNZ		loop
		movff	STKPTR_TEMP, STKPTR

		// Jump to first field after stack (task + F)
		movlw	0xfd
		movff	PLUSW2, FSR0L
		movlw	0xfe
		movff	PLUSW2, FSR0H
		movlw	0xf
		addwf	FSR0L, 1, 0
		movlw	0
		addwfc	FSR0H, 1, 0

		// Save other registers
		movff	PRODL, POSTINC0
		movff	PRODH, POSTINC0
		movff	TABLAT, POSTINC0
		movff	TBLPTRL, POSTINC0
		movff	TBLPTRH, POSTINC0
		movff	TBLPTRU, POSTINC0
		movff	FSR1L, POSTINC0
		movff	FSR1H, POSTINC0
		movff	FSR2L, POSTINC0
		movff	FSR2H, POSTINC0
		movff	BSR, POSTINC0
		movff	WREG_TEMP, POSTINC0
		movff	STATUS, POSTINC0
	_endasm
}

void restore_context(struct os_task* task)
{
	_asm
		movlw	0xfd
		movff	PLUSW2, FSR0L
		movlw	0xfe
		movff	PLUSW2, FSR0H

		// Restore call-stack
		movff	TOSL, TOSL_TEMP
		movff	TOSH, TOSH_TEMP
		movff	POSTINC0, STKPTR
		movff	STKPTR, STKPTR_TEMP
	loop:
		movff	POSTINC0, WREG
		movwf	TOSL, 0
		movff	POSTINC0, WREG
		movwf	TOSH, 0
		decf	STKPTR, 1, 0
		movlw	0x2
		XORWF	STKPTR, 0, 0
		BNZ		loop
		movff	STKPTR_TEMP, STKPTR
		incf	STKPTR, 1, 0
		movff	TOSL_TEMP, WREG
		movwf	TOSL, 0
		movff	TOSH_TEMP, WREG
		movwf	TOSH, 0

		// Jump to first field after stack (task + F)
		movlw	0xfd
		movff	PLUSW2, FSR0L
		movlw	0xfe
		movff	PLUSW2, FSR0H
		movlw	0xf
		addwf	FSR0L, 1, 0
		movlw	0
		addwfc	FSR0H, 1, 0
		
		// Restore rest of the fields
		movff	POSTINC0, PRODL
		movff	POSTINC0, PRODH
		movff	POSTINC0, TABLAT
		movff	POSTINC0, TBLPTRL
		movff	POSTINC0, TBLPTRH
		movff	POSTINC0, TBLPTRU
		movff	POSTINC0, FSR1L
		movff	POSTINC0, FSR1H
		movff	POSTINC0, FSR2L
		movff	POSTINC0, FSR2H
		movff	POSTINC0, BSR
		movff	POSTINC0, WREG
		movff	POSTINC0, STATUS
	_endasm
}

void os_init(void)
{
	init_tasks_list();
	sched_init();
}

void init_tasks_list(void)
{
	int	i;
	task_list_init(&gm_free_task_list);
	task_list_init(&g_ready_task_list);
	for (i = TASK_COUNT - 1; i >= 0; i--) {
		gm_task_arr[i].id = i;
		gm_task_arr[i].state = TS_Free;
		task_list_add_element(&gm_free_task_list, &gm_task_arr[i]);
	}
}

char os_add_task(void* main, char mem_stack_size)
{
	struct os_task*	task;
	int				i;
	void*			p;

	if (mem_stack_size > 100)
		return -1;

	// For some strange reason when using function scope variable it messed up in CS_END */
	CS_BEGIN(gm_intcon_temp);
	task = gm_free_task_list.first;
	task_list_remove_element(&gm_free_task_list, task);

	task->state = TS_Ready;
	task->ctx.hw_stack_size = 3;
	task->ctx.hw_stack[0] = main;
	for (i = 1; i < HW_STACK_SIZE; i++)
		task->ctx.hw_stack[i] = 0;
	task->ctx.prod		= 0;
	task->ctx.tablat	= 0;
	task->ctx.tblptrl	= 0;
	task->ctx.tblptrh	= 0;
	task->ctx.tblptru	= 0;
	task->ctx.bsr		= 0;
	task->ctx.wreg		= 0;
	task->ctx.status	= 0;
	task->ctx.fsr1		= (void*)((task->id+3) * 0x100);
	task->ctx.fsr2		= task->ctx.fsr1;
	task->ctx.fsr1		+= 10;

	for(p = task->ctx.fsr1; p < task->ctx.fsr2; i++)
		*((char*)p) = 0;
	
	task_list_add_element(&g_ready_task_list, task);

	CS_END(gm_intcon_temp);
	
	return (task->id);
}

void os_exit_task()
{
	char	intcon_temp;

	CS_BEGIN(intcon_temp);
	task_list_remove_element(&g_ready_task_list, g_running_task);
	task_list_add_element(&gm_free_task_list, g_running_task);
	CS_END(intcon_temp);
	sched();
}

void os_yield(void) {
	sched();
}

void os_start(void)
{
	void	(*task1main)(void);

	if (g_ready_task_list.count == 0)
		return;

	_asm
		// Internal instruction cycle clock
		bcf		T0CON, 5, 0
		// Prescalar
		bcf		T0CON, 3, 0
		bcf		T0CON, 1, 0

		// Enable Timer0
		bsf		INTCON, 5, 0
		// Enable interrupt
		bsf		INTCON, 7, 0
	_endasm

	g_running_task = task_list_next_element(&g_ready_task_list);

	task1main = (void(*)())(g_running_task->ctx.hw_stack[0]);
	(*task1main)();
}
