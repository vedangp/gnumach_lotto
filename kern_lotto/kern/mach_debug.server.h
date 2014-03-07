#ifndef	_mach_debug_server_
#define	_mach_debug_server_

/* Module mach_debug */

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
#include <mach_debug/mach_debug_types.h>

/* Routine mach_port_get_srights */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_get_srights
(
	ipc_space_t task,
	mach_port_t name,
	mach_port_rights_t *srights
);

/* Routine host_ipc_hash_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_ipc_hash_info
(
	host_t host,
	hash_info_bucket_array_t *info,
	mach_msg_type_number_t *infoCnt
);

/* Routine host_ipc_marequest_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_ipc_marequest_info
(
	host_t host,
	unsigned *max_requests,
	hash_info_bucket_array_t *info,
	mach_msg_type_number_t *infoCnt
);

/* Routine mach_port_space_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_space_info
(
	ipc_space_t task,
	ipc_info_space_t *info,
	ipc_info_name_array_t *table_info,
	mach_msg_type_number_t *table_infoCnt,
	ipc_info_tree_name_array_t *tree_info,
	mach_msg_type_number_t *tree_infoCnt
);

/* Routine mach_port_dnrequest_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_dnrequest_info
(
	ipc_space_t task,
	mach_port_t name,
	unsigned *total,
	unsigned *used
);

/* Routine host_stack_usage */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_stack_usage
(
	host_t host,
	vm_size_t *reserved,
	unsigned *total,
	vm_size_t *space,
	vm_size_t *resident,
	vm_size_t *maxusage,
	vm_offset_t *maxstack
);

/* Routine processor_set_stack_usage */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t processor_set_stack_usage
(
	processor_set_t pset,
	unsigned *total,
	vm_size_t *space,
	vm_size_t *resident,
	vm_size_t *maxusage,
	vm_offset_t *maxstack
);

/* Routine host_virtual_physical_table_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_virtual_physical_table_info
(
	host_t host,
	hash_info_bucket_array_t *info,
	mach_msg_type_number_t *infoCnt
);

/* Routine host_load_symbol_table */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_load_symbol_table
(
	host_t host,
	task_t task,
	symtab_name_t name,
	vm_offset_t symtab,
	mach_msg_type_number_t symtabCnt
);

/* Routine mach_port_kernel_object */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_port_kernel_object
(
	ipc_space_t task,
	mach_port_t name,
	unsigned *object_type,
	vm_offset_t *object_addr
);

/* Routine mach_vm_region_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_vm_region_info
(
	vm_map_t task,
	vm_address_t address,
	vm_region_info_t *region,
	ipc_port_t *object
);

/* Routine mach_vm_object_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_vm_object_info
(
	vm_object_t object,
	vm_object_info_t *info,
	ipc_port_t *shadow,
	ipc_port_t *copy
);

/* Routine mach_vm_object_pages */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_vm_object_pages
(
	vm_object_t object,
	vm_page_info_array_t *pages,
	mach_msg_type_number_t *pagesCnt
);

/* Routine host_slab_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t host_slab_info
(
	host_t host,
	cache_info_array_t *info,
	mach_msg_type_number_t *infoCnt
);

extern mig_routine_t mach_debug_server_routines[];
extern inline mig_routine_t mach_debug_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 3000;

	if ((msgh_id > 22) || (msgh_id < 0))
		return 0;

	return mach_debug_server_routines[msgh_id];
}

#endif	/* not defined(_mach_debug_server_) */
