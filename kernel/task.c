#include "task.h"

void task_list_init(struct os_task_list* task_list) {
	task_list->first = 0;
	task_list->iter = 0;
	task_list->count = 0;
}

void task_list_add_element(struct os_task_list* task_list, struct os_task* task) {
	struct os_task*	old_next;
	task_list->count++;
	if (task_list->first == 0) {
		task_list->first = task;
		task_list->iter = task;
		task->next_task = task;
	} else {
		old_next = task_list->first->next_task;
		task_list->first->next_task = task;
		task->next_task = old_next;
		task_list->first = task;
	}
}

void task_list_remove_element(struct os_task_list* task_list, struct os_task* task) {
	unsigned char	cnt = task_list->count;
	struct os_task*	tmp = task->next_task;
	
	while ( (tmp->next_task != task) && (cnt) ) {
		tmp = tmp->next_task;
		cnt--;
	}
	
	if (!cnt)
		return;
		
	if (task == task_list->iter)
		task_list->iter = task->next_task;
		
	if (task == task_list->first)
		task_list->first = task->next_task;
		
	tmp->next_task = task->next_task;
	task->next_task = 0;
	
	task_list->count--;

	if (task_list->count == 1) {
		task->next_task = task;
	}

	if (task_list->count == 0) {
		task_list->first = task_list->iter = 0;
	}
}

struct os_task* task_list_next_element(struct os_task_list* task_list) {
	task_list->iter = task_list->iter->next_task;
	return task_list->iter;
}

void task_list_clear(struct os_task_list* task_list) {
	task_list_init(task_list);
}