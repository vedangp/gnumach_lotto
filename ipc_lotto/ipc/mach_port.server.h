#ifndef	_mach_port_server_
#define	_mach_port_server_

/* Module mach_port */

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

/* Routine mach_port_names */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_names
(
	ipc_space_t task,
	mach_port_array_t *names,
	mach_msg_type_number_t *namesCnt,
	mach_port_type_array_t *types,
	mach_msg_type_number_t *typesCnt
);

/* Routine mach_port_type */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_type
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_type_t *ptype
);

/* Routine mach_port_rename */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_rename
(
	ipc_space_t task,
	mach_port_t old_name,
	mach_port_t new_name
);

/* Routine mach_port_allocate_name */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_allocate_name
(
	ipc_space_t task,
	mach_port_right_t right,
	mach_port_t name
);

/* Routine mach_port_allocate */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_allocate
(
	ipc_space_t task,
	mach_port_right_t right,
	mach_port_t *name
);

/* Routine mach_port_destroy */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_destroy
(
	ipc_space_t task,
	mach_port_t name
);

/* Routine mach_port_deallocate */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_deallocate
(
	ipc_space_t task,
	mach_port_t name
);

/* Routine mach_port_get_refs */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_get_refs
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_right_t right,
	mach_port_urefs_t *refs
);

/* Routine mach_port_mod_refs */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_mod_refs
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_right_t right,
	mach_port_delta_t delta
);

/* Routine old_mach_port_get_receive_status */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t old_mach_port_get_receive_status
(
	ipc_space_t task,
	mach_port_t name,
	old_mach_port_status_t *status
);

/* Routine mach_port_set_qlimit */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_set_qlimit
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_msgcount_t qlimit
);

/* Routine mach_port_set_mscount */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_set_mscount
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_mscount_t mscount
);

/* Routine mach_port_get_set_status */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_get_set_status
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_array_t *members,
	mach_msg_type_number_t *membersCnt
);

/* Routine mach_port_move_member */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_move_member
(
	ipc_space_t task,
	mach_port_t member,
	mach_port_t after
);

/* Routine mach_port_request_notification */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_request_notification
(
	ipc_space_t task,
	mach_port_t name,
	mach_msg_id_t id,
	mach_port_mscount_t sync,
	ipc_port_t notify,
	ipc_port_t *previous
);

/* Routine mach_port_insert_right */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_insert_right
(
	ipc_space_t task,
	mach_port_t name,
	ipc_port_t poly,
	mach_msg_type_name_t polyPoly
);

/* Routine mach_port_extract_right */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_extract_right
(
	ipc_space_t task,
	mach_port_t name,
	mach_msg_type_name_t msgt_name,
	ipc_port_t *poly,
	mach_msg_type_name_t *polyPoly
);

/* Routine mach_port_get_receive_status */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_get_receive_status
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_status_t *status
);

/* Routine mach_port_set_seqno */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_set_seqno
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_seqno_t seqno
);

extern mig_routine_t mach_port_server_routines[];
extern inline mig_routine_t mach_port_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 3200;

	if ((msgh_id > 18) || (msgh_id < 0))
		return 0;

	return mach_port_server_routines[msgh_id];
}

#endif	/* not defined(_mach_port_server_) */
