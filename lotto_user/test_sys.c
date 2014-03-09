#include "lotto_util.h"
#include <hurd/process_request.h>

int main()
{
	mach_port_t task;
	proc_pid2task_request(108,task);
	printf("task : %d\n",task);//syscall(-33,108));
	
} 
