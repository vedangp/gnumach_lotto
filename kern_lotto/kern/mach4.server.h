#ifndef	_mach4_server_
#define	_mach4_server_

/* Module mach4 */

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

/* Routine task_enable_pc_sampling */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_enable_pc_sampling
(
	task_t host,
	int *tick,
	sampled_pc_flavor_t flavor
);

/* Routine task_disable_pc_sampling */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_disable_pc_sampling
(
	task_t host,
	int *samplecnt
);

/* Routine task_get_sampled_pcs */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_get_sampled_pcs
(
	task_t host,
	sampled_pc_seqno_t *seqno,
	sampled_pc_array_t sampled_pcs,
	mach_msg_type_number_t *sampled_pcsCnt
);

/* Routine thread_enable_pc_sampling */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_enable_pc_sampling
(
	thread_t host,
	int *tick,
	sampled_pc_flavor_t flavor
);

/* Routine thread_disable_pc_sampling */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_disable_pc_sampling
(
	thread_t host,
	int *samplecnt
);

/* Routine thread_get_sampled_pcs */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_get_sampled_pcs
(
	thread_t host,
	sampled_pc_seqno_t *seqno,
	sampled_pc_array_t sampled_pcs,
	mach_msg_type_number_t *sampled_pcsCnt
);

/* Routine memory_object_create_proxy */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_create_proxy
(
	ipc_space_t task,
	vm_prot_t max_protection,
	memory_object_array_t object,
	mach_msg_type_number_t objectCnt,
	vm_offset_array_t offset,
	mach_msg_type_number_t offsetCnt,
	vm_offset_array_t start,
	mach_msg_type_number_t startCnt,
	vm_offset_array_t len,
	mach_msg_type_number_t lenCnt,
	ipc_port_t *proxy
);

extern mig_routine_t mach4_server_routines[];
extern inline mig_routine_t mach4_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 4000;

	if ((msgh_id > 10) || (msgh_id < 0))
		return 0;

	return mach4_server_routines[msgh_id];
}

#endif	/* not defined(_mach4_server_) */
