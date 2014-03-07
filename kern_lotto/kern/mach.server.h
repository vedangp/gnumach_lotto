#ifndef	_mach_server_
#define	_mach_server_

/* Module mach */

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

/* Routine task_create */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_create
(
	task_t target_task,
	boolean_t inherit_memory,
	task_t *child_task
);

/* Routine task_terminate */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_terminate
(
	task_t target_task
);

/* Routine task_get_emulation_vector */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_get_emulation_vector
(
	task_t task,
	int *vector_start,
	emulation_vector_t *emulation_vector,
	mach_msg_type_number_t *emulation_vectorCnt
);

/* Routine task_set_emulation_vector */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_set_emulation_vector
(
	task_t task,
	int vector_start,
	emulation_vector_t emulation_vector,
	mach_msg_type_number_t emulation_vectorCnt
);

/* Routine task_threads */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_threads
(
	task_t target_task,
	thread_array_t *thread_list,
	mach_msg_type_number_t *thread_listCnt
);

/* Routine task_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_info
(
	task_t target_task,
	int flavor,
	task_info_t task_info_out,
	mach_msg_type_number_t *task_info_outCnt
);

/* Routine thread_terminate */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_terminate
(
	thread_t target_thread
);

/* Routine thread_get_state */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_get_state
(
	thread_t target_thread,
	int flavor,
	thread_state_t old_state,
	mach_msg_type_number_t *old_stateCnt
);

/* Routine thread_set_state */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_set_state
(
	thread_t target_thread,
	int flavor,
	thread_state_t new_state,
	mach_msg_type_number_t new_stateCnt
);

/* Routine thread_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_info
(
	thread_t target_thread,
	int flavor,
	thread_info_t thread_info_out,
	mach_msg_type_number_t *thread_info_outCnt
);

/* Routine vm_allocate */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_allocate
(
	vm_map_t target_task,
	vm_address_t *address,
	vm_size_t size,
	boolean_t anywhere
);

/* Routine vm_deallocate */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_deallocate
(
	vm_map_t target_task,
	vm_address_t address,
	vm_size_t size
);

/* Routine vm_protect */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_protect
(
	vm_map_t target_task,
	vm_address_t address,
	vm_size_t size,
	boolean_t set_maximum,
	vm_prot_t new_protection
);

/* Routine vm_inherit */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_inherit
(
	vm_map_t target_task,
	vm_address_t address,
	vm_size_t size,
	vm_inherit_t new_inheritance
);

/* Routine vm_read */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_read
(
	vm_map_t target_task,
	vm_address_t address,
	vm_size_t size,
	vm_offset_t *data,
	mach_msg_type_number_t *dataCnt
);

/* Routine vm_write */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_write
(
	vm_map_t target_task,
	vm_address_t address,
	vm_offset_t data,
	mach_msg_type_number_t dataCnt
);

/* Routine vm_copy */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_copy
(
	vm_map_t target_task,
	vm_address_t source_address,
	vm_size_t size,
	vm_address_t dest_address
);

/* Routine vm_region */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_region
(
	vm_map_t target_task,
	vm_address_t *address,
	vm_size_t *size,
	vm_prot_t *protection,
	vm_prot_t *max_protection,
	vm_inherit_t *inheritance,
	boolean_t *is_shared,
	ipc_port_t *object_name,
	vm_offset_t *offset
);

/* Routine vm_statistics */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_statistics
(
	vm_map_t target_task,
	vm_statistics_data_t *vm_stats
);

/* Routine mach_ports_register */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_ports_register
(
	task_t target_task,
	mach_port_array_t init_port_set,
	mach_msg_type_number_t init_port_setCnt
);

/* Routine mach_ports_lookup */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t mach_ports_lookup
(
	task_t target_task,
	mach_port_array_t *init_port_set,
	mach_msg_type_number_t *init_port_setCnt
);

/* SimpleRoutine memory_object_data_provided */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_data_provided
(
	vm_object_t memory_control,
	vm_offset_t offset,
	vm_offset_t data,
	mach_msg_type_number_t dataCnt,
	vm_prot_t lock_value
);

/* SimpleRoutine memory_object_data_unavailable */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_data_unavailable
(
	vm_object_t memory_control,
	vm_offset_t offset,
	vm_size_t size
);

/* Routine memory_object_get_attributes */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_get_attributes
(
	vm_object_t memory_control,
	boolean_t *object_ready,
	boolean_t *may_cache,
	memory_object_copy_strategy_t *copy_strategy
);

/* Routine vm_set_default_memory_manager */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_set_default_memory_manager
(
	host_t host_priv,
	ipc_port_t *default_manager
);

/* SimpleRoutine memory_object_lock_request */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_lock_request
(
	vm_object_t memory_control,
	vm_offset_t offset,
	vm_size_t size,
	memory_object_return_t should_return,
	boolean_t should_flush,
	vm_prot_t lock_value,
	ipc_port_t reply_to,
	mach_msg_type_name_t reply_toPoly
);

/* Routine task_suspend */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_suspend
(
	task_t target_task
);

/* Routine task_resume */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_resume
(
	task_t target_task
);

/* Routine task_get_special_port */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_get_special_port
(
	task_t task,
	int which_port,
	ipc_port_t *special_port
);

/* Routine task_set_special_port */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_set_special_port
(
	task_t task,
	int which_port,
	ipc_port_t special_port
);

/* Routine thread_create */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_create
(
	task_t parent_task,
	thread_t *child_thread
);

/* Routine thread_suspend */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_suspend
(
	thread_t target_thread
);

/* Routine thread_resume */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_resume
(
	thread_t target_thread
);

/* Routine thread_abort */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_abort
(
	thread_t target_thread
);

/* Routine thread_get_special_port */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_get_special_port
(
	thread_t thread,
	int which_port,
	ipc_port_t *special_port
);

/* Routine thread_set_special_port */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t thread_set_special_port
(
	thread_t thread,
	int which_port,
	ipc_port_t special_port
);

/* Routine task_set_emulation */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_set_emulation
(
	task_t target_port,
	vm_address_t routine_entry_pt,
	int routine_number
);

/* Routine task_ras_control */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t task_ras_control
(
	task_t target_task,
	vm_address_t basepc,
	vm_address_t boundspc,
	int flavor
);

/* Routine vm_map */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_map
(
	vm_map_t target_task,
	vm_address_t *address,
	vm_size_t size,
	vm_address_t mask,
	boolean_t anywhere,
	ipc_port_t memory_object,
	vm_offset_t offset,
	boolean_t copy,
	vm_prot_t cur_protection,
	vm_prot_t max_protection,
	vm_inherit_t inheritance
);

/* SimpleRoutine memory_object_data_error */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_data_error
(
	vm_object_t memory_control,
	vm_offset_t offset,
	vm_size_t size,
	kern_return_t error_value
);

/* SimpleRoutine memory_object_set_attributes */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_set_attributes
(
	vm_object_t memory_control,
	boolean_t object_ready,
	boolean_t may_cache,
	memory_object_copy_strategy_t copy_strategy
);

/* SimpleRoutine memory_object_destroy */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_destroy
(
	vm_object_t memory_control,
	kern_return_t reason
);

/* SimpleRoutine memory_object_data_supply */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_data_supply
(
	vm_object_t memory_control,
	vm_offset_t offset,
	vm_offset_t data,
	mach_msg_type_number_t dataCnt,
	vm_prot_t lock_value,
	boolean_t precious,
	ipc_port_t reply_to,
	mach_msg_type_name_t reply_toPoly
);

/* SimpleRoutine memory_object_ready */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_ready
(
	vm_object_t memory_control,
	boolean_t may_cache,
	memory_object_copy_strategy_t copy_strategy
);

/* SimpleRoutine memory_object_change_attributes */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t memory_object_change_attributes
(
	vm_object_t memory_control,
	boolean_t may_cache,
	memory_object_copy_strategy_t copy_strategy,
	ipc_port_t reply_to,
	mach_msg_type_name_t reply_toPoly
);

/* Routine vm_machine_attribute */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t vm_machine_attribute
(
	vm_map_t target_task,
	vm_address_t address,
	vm_size_t size,
	vm_machine_attribute_t attribute,
	vm_machine_attribute_val_t *value
);

extern mig_routine_t mach_server_routines[];
extern inline mig_routine_t mach_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 2000;

	if ((msgh_id > 100) || (msgh_id < 0))
		return 0;

	return mach_server_routines[msgh_id];
}

#endif	/* not defined(_mach_server_) */
