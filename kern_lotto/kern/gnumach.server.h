#ifndef	_gnumach_server_
#define	_gnumach_server_

/* Module gnumach */

#include <mach/kern_return.h>
#include <mach/port.h>
#include <mach/message.h>

#include <mach/mig_errors.h>
#include <mach/std_types.h>
#include <kern/ipc_kobject.h>
#include <kern/ipc_tt.h>
#include <kern/ipc_host.h>
#include <kern/task.h>
#include <kern/thread.h>
#include <kern/host.h>
#include <kern/processor.h>
#include <vm/vm_object.h>
#include <vm/vm_map.h>
#include <ipc/ipc_space.h>
#include <mach/mach_types.h>

/* Routine vm_cache_statistics */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_cache_statistics
(
	vm_map_t target_task,
	vm_cache_statistics_data_t *vm_cache_stats
);

/* SimpleRoutine thread_terminate_release */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_terminate_release
(
	thread_t thread,
	task_t task,
	mach_port_t thread_name,
	mach_port_t reply_port,
	vm_address_t address,
	vm_size_t size
);

extern mig_routine_t gnumach_server_routines[];
extern inline mig_routine_t gnumach_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 4200;

	if ((msgh_id > 1) || (msgh_id < 0))
		return 0;

	return gnumach_server_routines[msgh_id];
}

#endif	/* not defined(_gnumach_server_) */
