#ifndef _OS_MUTEX_H
#define _OS_MUTEX_H
/***************************************************\
 *
\***************************************************/

#include "task.h"

struct os_mutex {
	struct os_task*			owner;
	struct os_task_list		waiting_tasks_list;
};

void os_mutex_init(struct os_mutex* mutex);
void os_mutex_take(struct os_mutex* mutex);
void os_mutex_free(struct os_mutex* mutex);
void os_mutex_destroy(struct os_mutex* mutex);

#endif
