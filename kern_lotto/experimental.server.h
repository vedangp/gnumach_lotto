#ifndef	_experimental_server_
#define	_experimental_server_

/* Module experimental */

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

/* Routine device_intr_register */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t experimental_device_intr_register
(
	ipc_port_t master_port,
	int line,
	int id,
	int flags,
	ipc_port_t receive_port
);

/* Routine device_intr_enable */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t experimental_device_intr_enable
(
	ipc_port_t master_port,
	int line,
	char status
);

/* Routine vm_allocate_contiguous */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t experimental_vm_allocate_contiguous
(
	host_t host_priv,
	vm_map_t target_task,
	vm_address_t *vaddr,
	vm_address_t *paddr,
	vm_size_t size
);

extern mig_routine_t experimental_server_routines[];
extern inline mig_routine_t experimental_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 424242;

	if ((msgh_id > 3) || (msgh_id < 0))
		return 0;

	return experimental_server_routines[msgh_id];
}

#endif	/* not defined(_experimental_server_) */
