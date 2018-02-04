#include "mutex.h"
#include "task.h"
#include "os.h"

void os_mutex_init(struct os_mutex* mutex)
{
	char	intcon_temp;

	CS_BEGIN(intcon_temp);
	mutex->owner = 0;
	task_list_init(&(mutex->waiting_tasks_list));
	CS_END(intcon_temp);
}
	
void os_mutex_take(struct os_mutex* mutex)
{
	char	intcon_temp;

	CS_BEGIN(intcon_temp);
	if (!mutex->owner) {
		mutex->owner = g_running_task;
		CS_END(intcon_temp);
	} else {
		g_running_task->state = TS_Waiting;
		task_list_remove_element(&g_ready_task_list, g_running_task);
		task_list_add_element(&(mutex->waiting_tasks_list), g_running_task);
		CS_END(intcon_temp);
		os_yield();
	}
}

void os_mutex_free(struct os_mutex* mutex)
{
	struct os_task*	task_to_free;
	char			intcon_temp;

	CS_BEGIN(intcon_temp);
	
	if (mutex->owner != g_running_task) {
		CS_END(intcon_temp);
		return;
	}
	

	task_to_free = task_list_next_element(&(mutex->waiting_tasks_list));
	if (task_to_free) {
		task_list_remove_element(&(mutex->waiting_tasks_list), task_to_free);
		task_list_add_element(&g_ready_task_list, task_to_free);
		mutex->owner = task_to_free;
		CS_END(intcon_temp);
		os_yield();
	} else {
		mutex->owner = 0;
		CS_END(intcon_temp);
	}
}

void os_mutex_destroy(struct os_mutex* mutex)
{
	char	intcon_temp;

	CS_BEGIN(intcon_temp);
	mutex->owner = 0;
	while (mutex->waiting_tasks_list.count) {
		task_list_remove_element(&(mutex->waiting_tasks_list), mutex->waiting_tasks_list.first);
		task_list_add_element(&g_ready_task_list, mutex->waiting_tasks_list.first);
	}
	CS_END(intcon_temp);
}
