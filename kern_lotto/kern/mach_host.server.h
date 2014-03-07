#ifndef	_mach_host_server_
#define	_mach_host_server_

/* Module mach_host */

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

/* Routine host_processors */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_processors
(
	host_t host_priv,
	processor_array_t *processor_list,
	mach_msg_type_number_t *processor_listCnt
);

/* Routine processor_start */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_start
(
	processor_t processor
);

/* Routine processor_exit */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_exit
(
	processor_t processor
);

/* Routine processor_set_default */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_default
(
	host_t host,
	processor_set_t *default_set
);

/* Routine processor_set_create */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_create
(
	host_t host,
	processor_set_t *new_set,
	processor_set_t *new_name
);

/* Routine processor_set_destroy */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_destroy
(
	processor_set_t set
);

/* Routine processor_assign */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_assign
(
	processor_t processor,
	processor_set_t new_set,
	boolean_t wait
);

/* Routine processor_get_assignment */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_get_assignment
(
	processor_t processor,
	processor_set_t *assigned_set
);

/* Routine thread_assign */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_assign
(
	thread_t thread,
	processor_set_t new_set
);

/* Routine thread_assign_default */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_assign_default
(
	thread_t thread
);

/* Routine thread_get_assignment */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_get_assignment
(
	thread_t thread,
	processor_set_t *assigned_set
);

/* Routine task_assign */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_assign
(
	task_t task,
	processor_set_t new_set,
	boolean_t assign_threads
);

/* Routine task_assign_default */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_assign_default
(
	task_t task,
	boolean_t assign_threads
);

/* Routine task_get_assignment */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_get_assignment
(
	task_t task,
	processor_set_t *assigned_set
);

/* Routine host_kernel_version */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_kernel_version
(
	host_t host,
	kernel_version_t kernel_version
);

/* Routine thread_priority */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_priority
(
	thread_t thread,
	int priority,
	boolean_t set_max
);

/* Routine thread_max_priority */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_max_priority
(
	thread_t thread,
	processor_set_t processor_set,
	int max_priority
);

/* Routine task_priority */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_priority
(
	task_t task,
	int priority,
	boolean_t change_threads
);

/* Routine processor_set_max_priority */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_max_priority
(
	processor_set_t processor_set,
	int max_priority,
	boolean_t change_threads
);

/* Routine thread_policy */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_policy
(
	thread_t thread,
	int policy,
	int data
);

/* Routine processor_set_policy_enable */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_policy_enable
(
	processor_set_t processor_set,
	int policy
);

/* Routine processor_set_policy_disable */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_policy_disable
(
	processor_set_t processor_set,
	int policy,
	boolean_t change_threads
);

/* Routine processor_set_tasks */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_tasks
(
	processor_set_t processor_set,
	task_array_t *task_list,
	mach_msg_type_number_t *task_listCnt
);

/* Routine processor_set_threads */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_threads
(
	processor_set_t processor_set,
	thread_array_t *thread_list,
	mach_msg_type_number_t *thread_listCnt
);

/* Routine host_processor_sets */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_processor_sets
(
	host_t host,
	processor_set_name_array_t *processor_sets,
	mach_msg_type_number_t *processor_setsCnt
);

/* Routine host_processor_set_priv */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_processor_set_priv
(
	host_t host_priv,
	processor_set_t set_name,
	processor_set_t *set
);

/* Routine thread_depress_abort */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_depress_abort
(
	thread_t thread
);

/* Routine host_set_time */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_set_time
(
	host_t host_priv,
	time_value_t new_time
);

/* Routine host_adjust_time */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_adjust_time
(
	host_t host_priv,
	time_value_t new_adjustment,
	time_value_t *old_adjustment
);

/* Routine host_get_time */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_get_time
(
	host_t host,
	time_value_t *current_time
);

/* Routine host_reboot */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_reboot
(
	host_t host_priv,
	int options
);

/* Routine vm_wire */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_wire
(
	host_t host_priv,
	vm_map_t task,
	vm_address_t address,
	vm_size_t size,
	vm_prot_t access
);

/* Routine thread_wire */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_wire
(
	host_t host_priv,
	thread_t thread,
	boolean_t wired
);

/* Routine host_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_info
(
	host_t host,
	int flavor,
	host_info_t host_info_out,
	mach_msg_type_number_t *host_info_outCnt
);

/* Routine processor_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_info
(
	processor_t processor,
	int flavor,
	host_t *host,
	processor_info_t processor_info_out,
	mach_msg_type_number_t *processor_info_outCnt
);

/* Routine processor_set_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_info
(
	processor_set_t set_name,
	int flavor,
	host_t *host,
	processor_set_info_t info_out,
	mach_msg_type_number_t *info_outCnt
);

/* Routine processor_control */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_control
(
	processor_t processor,
	processor_info_t processor_cmd,
	mach_msg_type_number_t processor_cmdCnt
);

/* Routine host_get_boot_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_get_boot_info
(
	host_t host_priv,
	kernel_boot_info_t boot_info
);

extern mig_routine_t mach_host_server_routines[];
extern inline mig_routine_t mach_host_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 2600;

	if ((msgh_id > 42) || (msgh_id < 0))
		return 0;

	return mach_host_server_routines[msgh_id];
}

#endif	/* not defined(_mach_host_server_) */
