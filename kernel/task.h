#ifndef _OS_TASK_H
#define _OS_TASK_H
/***************************************************\
 *
\***************************************************/

#define HW_STACK_SIZE 7

struct os_task_context {
	char			hw_stack_size;
	void*			hw_stack[HW_STACK_SIZE];
	void*			prod;
	char			tablat;
	char			tblptrl;
	char			tblptrh;
	char			tblptru;
	void*			fsr1;
	void*			fsr2;
	char			bsr;
	char			wreg;
	char			status;
};

enum os_task_state {
	TS_Free		= 0,
	TS_Ready	= 1,
	TS_Running	= 2,
	TS_Waiting	= 3
};

struct os_task {
	struct os_task_context	ctx;
	unsigned char			id;
	enum os_task_state		state;		
	struct os_task*			next_task;
};

struct os_task_list {
	struct os_task*		first;
	struct os_task*		iter;
	unsigned char		count;
};

void task_list_init(struct os_task_list* task_list);
void task_list_add_element(struct os_task_list* task_list, struct os_task* task);
void task_list_remove_element(struct os_task_list* task_list, struct os_task* task);
struct os_task* task_list_next_element(struct os_task_list* task_list);
void task_list_clear(struct os_task_list* task_list);

#endif