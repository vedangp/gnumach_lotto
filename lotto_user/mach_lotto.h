#ifndef	_mach_lotto_user_
#define	_mach_lotto_user_

/* Module mach_lotto */

#include <mach/kern_return.h>
#include <mach/port.h>
#include <mach/message.h>

#include <mach/mach_lotto_types.h>
#include <mach/std_types.h>
#include <mach/mach_types.h>

/* Routine lotto_create_ticket */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t lotto_create_ticket
(
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
	mach_port_t task,
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
	mach_port_t pset,
	mach_port_t task,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
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
	mach_port_t pset,
	boolean_t enable
);

#endif	/* not defined(_mach_lotto_user_) */
