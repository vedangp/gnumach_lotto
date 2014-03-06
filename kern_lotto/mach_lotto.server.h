#ifndef	_mach_lotto_server_
#define	_mach_lotto_server_

/* Module mach_lotto */

#include <mach/kern_return.h>
#include <mach/port.h>
#include <mach/message.h>

#include <mach/mig_errors.h>
#include <mach/mach_lotto_types.h>
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

/* Routine lotto_create_ticket */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_create_ticket
(
	processor_set_t pset,
	lotto_currency_id_t currency_id,
	lotto_funds_t amount,
	lotto_ticket_id_t *ticket_id
);

/* Routine lotto_create_currency */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_create_currency
(
	processor_set_t pset,
	lotto_currency_name_t name,
	lotto_currency_id_t *currency_id
);

/* Routine lotto_destroy_ticket */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_destroy_ticket
(
	processor_set_t pset,
	lotto_ticket_id_t ticket_id,
	lotto_currency_id_t issuer_hint
);

/* Routine lotto_destroy_currency */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_destroy_currency
(
	processor_set_t pset,
	lotto_currency_id_t currency_id,
	boolean_t destroy_tickets
);

/* Routine lotto_move_ticket_to_currency */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_move_ticket_to_currency
(
	processor_set_t pset,
	lotto_ticket_id_t ticket_id,
	lotto_currency_id_t issuer_hint,
	lotto_currency_id_t currency_id
);

/* Routine lotto_remove_ticket_from_currency */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_remove_ticket_from_currency
(
	processor_set_t pset,
	lotto_ticket_id_t ticket_id,
	lotto_currency_id_t issuer_hint,
	lotto_currency_id_t currency_id
);

/* Routine lotto_currency_by_name */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_currency_by_name
(
	processor_set_t pset,
	lotto_currency_name_t name,
	lotto_currency_id_t *currency_id
);

/* Routine lotto_currency_by_task */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_currency_by_task
(
	processor_set_t pset,
	task_t task,
	lotto_currency_id_t *currency_id
);

/* Routine lotto_currency_by_thread */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_currency_by_thread
(
	processor_set_t pset,
	thread_t task,
	lotto_currency_id_t *currency_id
);

/* Routine lotto_metrics */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_metrics
(
	processor_set_t pset,
	lotto_metrics_info *metrics
);

/* Routine lotto_currency_info */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_currency_info
(
	processor_set_t pset,
	lotto_currency_id_t currency_id,
	unsigned *wins,
	lotto_currency_name_t name,
	lotto_ticket_info_array tickets,
	mach_msg_type_number_t *ticketsCnt
);

/* Routine lotto_currency_ids */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_currency_ids
(
	processor_set_t pset,
	lotto_currency_id_array_t currency_ids,
	mach_msg_type_number_t *currency_idsCnt
);

/* Routine lotto_compute_ticket_value */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_compute_ticket_value
(
	processor_set_t pset,
	lotto_ticket_id_t ticket_id,
	lotto_currency_id_t issuer_hint,
	lotto_funds_t *value
);

/* Routine lotto_compute_currency_value */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_compute_currency_value
(
	processor_set_t pset,
	lotto_currency_id_t currency_id,
	lotto_funds_t *value
);

/* Routine lotto_ipc_metrics */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_ipc_metrics
(
	processor_set_t pset,
	lotto_ipc_metrics_info *metrics
);

/* Routine lotto_quantum_metrics */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_quantum_metrics
(
	processor_set_t pset,
	lotto_quantum_metrics_t metrics
);

/* Routine lotto_ipc_enable */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_ipc_enable
(
	processor_set_t pset,
	boolean_t enable
);

/* Routine lotto_quantum_enable */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_quantum_enable
(
	processor_set_t pset,
	boolean_t enable
);

extern mig_routine_t mach_lotto_server_routines[];
extern inline mig_routine_t mach_lotto_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 711000;

	if ((msgh_id > 21) || (msgh_id < 0))
		return 0;

	return mach_lotto_server_routines[msgh_id];
}

#endif	/* not defined(_mach_lotto_server_) */
