/*
 * Copyright (c) 1993, 1994  Carl A. Waldspurger.
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARL WALDSPURGER ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION, BUT DISCLAIMS ANY LIABILITY OF ANY KIND FOR ANY DAMAGES
 * WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * It is requested that users of this software return any improvements
 * they make and grant the rights to redistribute these changes to:
 * 
 * Carl Waldspurger			or   carl@lcs.mit.edu
 * MIT Laboratory for Computer Science
 * 545 Technology Square, Room 206
 * Cambridge, MA 02139
 *
 */

/*
 * Project:	Mach Lottery Scheduler (Kernel Modifications)
 * Module:	Lotto Header File
 * Author:	Carl Waldspurger
 * History:
 *
 *	08-Jun-94	Defined lotto_remove_ticket_from_currency()
 *			and lotto_compute_{ticket,currency}_value().
 *	07-Jun-94	Added issuer_hint parameters.
 *	04-Jun-94	Defined lotto_currency_by_thread().
 *	16-May-94	Defined lotto zone-related constants.
 *	05-May-94	Defined lotto_ipc_xfer_abort().
 *	03-May-94	Removed lotto_ticket_set_amount().
 *			Changed LOTTO_PRIORITY from 0 to 7 for 
 *			improved co-existence with priority scheduling.
 *	02-May-94	Added support for GNU extensions.
 *	27-Apr-94	Defined LOTTO_ASSERT().
 *	19-Apr-94	Defined MACH_LOTTO_QUANTUM_METRICS.
 *	18-Apr-94	Removed id field from lotto_ipc_xfer_t.
 *	14-Apr-94	Defined LOTTO_XFER_AMOUNT.
 *	13-Apr-94	Several minor improvements.
 *	12-Apr-94	Updated to support currency-per-thread model.
 *	08-Apr-94	Added ticket field to lotto_ipc_xfer_t.
 *	03-Apr-94	Renamed lotto ipc operations.
 *	01-Apr-94	Defined lotto ipc operation prototypes.
 *	29-Mar-94	Moved MACH_LOTTO_UNUSED from lotto_debug.h.
 *			Added dealloc field to lotto_currency_t and
 *			lotto_ticket_t definitions.
 *	18-Mar-94	Defined MACH_LOTTO_IPC compilation flag.
 *	01-Feb-94	Added wins field to currency structure.
 *	28-Jan-94	Declared lotto_prim_destroy_tickets().
 *			Defined LOTTO_FUNDS_MIN.  Other modifications.
 *	24-Jan-94	Defined LAST_VALID_RUNQ.
 *	22-Jan-94	Moved debugging compilation flags => lotto_debug.h.
 *	18-Jan-94	Fixed minor problem with lotto_currency_info() proto.
 *	12-Jan-94	Removed external declarations for internal routines.
 *	05-Jan-94	Removed id argument from lotto_create_currency().
 *	04-Jan-94	Modified lists and chain fields.  Renamed constants.
 *			Modified _prim creation routines to return objects.
 *	20-Dec-93	Updated to use new lotto id types.
 *			Moved locking macros here from lotto.c.
 *	16-Dec-93	Added lotto_currency_by_name().
 *	13-Dec-93	Added several constants.
 *	09-Dec-93	Added parameters to obtain id from creation operations.
 *	08-Nov-93	Minor improvements.
 *	06-Nov-93	Minor documentation improvements.
 *	04-Nov-93	Updated to use lotto_currency_name_t.
 *	27-Oct-93	Removed lotto_trap.  Started port-based routines.
 *	26-Oct-93	Renamed file, made MACH_LOTTO a kernel config option.
 *	22-Oct-93	Added chain fields to currency, ticket types.
 *	18-Oct-93	Development.  Added "activated" flag to currency.
 *	14-Oct-93	Defined _t pointer types; renamed some functions.
 *			Modified lists to use Mach queue types.
 *	13-Oct-93	Added operation prototypes, constants.
 *	05-Oct-93	Based heavily on standalone lottery scheduler.
 *
 */

#ifndef _LOTTO_H
#define _LOTTO_H

/*
 * compilation flags
 *
 */


/* ipc subsystem */
#define	MACH_LOTTO_IPC			(1)
#define MACH_LOTTO_IPC_METRICS		(1)

/* metrics */
#define	MACH_LOTTO_METRICS		(1)
#define MACH_LOTTO_QUANTUM_METRICS	(1)

/* code elimination */
#define MACH_LOTTO_UNUSED		(0)
#define MACH_LOTTO_IPC_UNUSED		(0)

/*
 * includes
 *
 */

#include <kern/macro_help.h>
#include <machine/machspl.h>
#include <mach/message.h>

#include <mach/mach_lotto_types.h>

/*
 * types
 *
 */

typedef struct {
  /* identity */
  lotto_currency_name_t name;
  lotto_currency_id_t id;

  /* allocation */
  boolean_t dealloc;

  /* ticket supply */
  boolean_t activated;
  lotto_funds_t amount;	
  
  /* issued tickets */
  queue_head_t tickets_issued;

  /* funding tickets */
  queue_head_t tickets_owned;

  /* metrics */
  unsigned int wins;

  /* list structure */
  queue_chain_t chain_pset;
} lotto_currency;
typedef lotto_currency *lotto_currency_t;

typedef struct {
  /* identity */
  lotto_ticket_id_t id;

  /* allocation */
  boolean_t dealloc;

  /* denomination */
  lotto_currency_t currency;
  lotto_funds_t amount;

  /* list structure */
  queue_chain_t chain_issuer;
  queue_chain_t chain_owner;

  /* owner backpointer */
  lotto_currency_t owner_currency;
} lotto_ticket;
typedef lotto_ticket *lotto_ticket_t;

typedef struct {
  unsigned int seed;
} lotto_random;
typedef lotto_random *lotto_random_t;

typedef enum {
  LOTTO_IPC_NONE,
  LOTTO_IPC_POST,
  LOTTO_IPC_TAKE,
  LOTTO_IPC_RETURN
} lotto_ipc_state_t;

typedef struct {
  /* identity */
  mach_msg_header_t header;

  /* current state */
  lotto_ipc_state_t state;

  /* participants */
  thread_t sender;
  thread_t receiver;

  /* ticket */
  lotto_ticket_t ticket;

  /* list structure */
  queue_chain_t chain;
} lotto_ipc_xfer;
typedef lotto_ipc_xfer *lotto_ipc_xfer_t;

/*
 * constants
 *
 */

/* primitive currency ids */
#define LOTTO_CURRENCY_ID_BASE		(1)
#define LOTTO_CURRENCY_ID_SYSTEM	(2)
#define LOTTO_CURRENCY_ID_USER		(3)

/* primitive currency names */
#define LOTTO_CURRENCY_NAME_BASE	"base"
#define LOTTO_CURRENCY_NAME_SYSTEM	"system"
#define LOTTO_CURRENCY_NAME_USER	"user"

/* id allocation base values */
#define LOTTO_CURRENCY_ID_ALLOC		(100)
#define LOTTO_TICKET_ID_ALLOC		(100)

/* special values */
#define	LOTTO_FUNDS_MIN			(1)
#define LOTTO_RANDOM_SEED		(1)
#define LOTTO_DEFAULT_AMOUNT		(1000)
#define LOTTO_IPC_XFER_AMOUNT		(LOTTO_DEFAULT_AMOUNT)

/* null values */
#define LOTTO_TICKET_NULL		((lotto_ticket_t) 0)
#define LOTTO_CURRENCY_NULL		((lotto_currency_t) 0)
#define LOTTO_FUNDS_NULL		((lotto_funds_t) 0)
#define LOTTO_LINK_NULL			((queue_entry_t) 0)

/* ipc null values */
#define LOTTO_IPC_ID_NULL		((lotto_ipc_id_t) 0)
#define LOTTO_IPC_XFER_NULL		((lotto_ipc_xfer_t) 0)

/* priority values */
#define LOTTO_PRIORITY			(7)
#define LAST_VALID_RUNQ			(49)

/* zone sizes */
#define LOTTO_TICKET_ZONE_MAX		(1024)
#define LOTTO_TICKET_ZONE_CHUNK		(16)
#define LOTTO_CURRENCY_ZONE_MAX		(1024)
#define LOTTO_CURRENCY_ZONE_CHUNK	(16)
#define LOTTO_IPC_XFER_ZONE_MAX		(512)
#define LOTTO_IPC_XFER_ZONE_CHUNK	(16)

/*
 * Map GNU extensions => nothing if not using gcc.
 *
 */

#ifndef	__GNUC__
#define inline  
#endif	__GNUC__

/*
 * macros
 *
 */

#define LOTTO_LOCK(pset, s)			\
  MACRO_BEGIN					\
  /* set interrupt priority */			\
  s = splsched();				\
						\
  /* acquire lotto lock */			\
  simple_lock(&pset->lotto_lock);		\
  MACRO_END

#define LOTTO_UNLOCK(pset, s)			\
  MACRO_BEGIN					\
  /* release lotto lock */			\
  simple_unlock(&pset->lotto_lock);		\
						\
  /* restore interrupt priority */		\
  (void) splx(s);				\
  MACRO_END

#define LOTTO_IPC_LOCK()			\
  MACRO_BEGIN					\
  /* acquire lotto ipc lock */			\
  simple_lock(&lotto_ipc_lock);			\
  MACRO_END

#define LOTTO_IPC_UNLOCK()			\
  MACRO_BEGIN					\
  /* release lotto ipc lock */			\
  simple_unlock(&lotto_ipc_lock);		\
  MACRO_END

//#define LOTTO_ASSERT(cond)			\
//  MACRO_BEGIN					\
//  extern void Assert();				\		
//  Assert("cond", __FILE__, __LINE__);			\
//  MACRO_END

#define LOTTO_ASSERT(cond) assert(cond)

/*
 * operations
 *
 */

/* lotto */

extern void
  lotto_init
  (void);

/* primitive lotto routines */

extern kern_return_t
  lotto_prim_create_ticket
  (processor_set_t pset,
   lotto_currency_t currency,
   lotto_funds_t amount,
   lotto_ticket_t storage,
   lotto_ticket_t *ticket);

extern kern_return_t
  lotto_prim_destroy_ticket
  (processor_set_t pset,
   lotto_ticket_t ticket);

extern void
  lotto_prim_destroy_tickets
  (processor_set_t pset,
   queue_t ticket_queue);

extern kern_return_t
  lotto_prim_create_currency
  (processor_set_t pset,
   lotto_currency_name_t name,
   lotto_currency_id_t id,
   lotto_currency_t storage,
   lotto_currency_t *currency);

extern kern_return_t
  lotto_prim_destroy_currency
  (processor_set_t pset,
   lotto_currency_t currency,
   boolean_t destroy_tickets);

extern kern_return_t
  lotto_prim_move_ticket_to_currency
  (processor_set_t pset,
   lotto_ticket_t ticket,
   lotto_currency_t currency);

extern kern_return_t
  lotto_prim_remove_ticket_from_currency
  (processor_set_t pset,
   lotto_ticket_t ticket,
   lotto_currency_t currency);

/* lotto routines */

extern kern_return_t
  lotto_create_ticket
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   lotto_funds_t amount,
   lotto_ticket_id_t *ticket_id);

extern kern_return_t
  lotto_create_currency
  (processor_set_t pset,
   lotto_currency_name_t name,
   lotto_currency_id_t *currency_id);

extern kern_return_t
  lotto_destroy_ticket
  (processor_set_t pset,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint);

extern kern_return_t
  lotto_destroy_currency
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   boolean_t destroy_tickets);

extern kern_return_t
  lotto_move_ticket_to_currency
  (processor_set_t pset,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint,
   lotto_currency_id_t currency_id);

extern kern_return_t
  lotto_remove_ticket_from_currency
  (processor_set_t pset,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint,
   lotto_currency_id_t currency_id);

extern kern_return_t
  lotto_currency_by_name
  (processor_set_t pset,
   lotto_currency_name_t name,
   lotto_currency_id_t *currency_id);

extern kern_return_t
  lotto_currency_by_task
  (processor_set_t pset,
   task_t task,
   lotto_currency_id_t *currency_id);

extern kern_return_t
  lotto_currency_by_thread
  (processor_set_t pset,
   thread_t thread,
   lotto_currency_id_t *currency_id);

extern kern_return_t
  lotto_metrics
  (processor_set_t pset,
   lotto_metrics_info *metrics);

extern kern_return_t
  lotto_currency_info
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   unsigned int *wins,
   lotto_currency_name_t name,
   lotto_ticket_info_array tickets,
   unsigned int *tickets_count);

extern kern_return_t
  lotto_currency_ids
  (processor_set_t pset,
   lotto_currency_id_array_t currency_ids,
   unsigned int *currency_ids_count);

extern kern_return_t
  lotto_compute_ticket_value
  (processor_set_t pset,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint,
   lotto_funds_t *value);

extern kern_return_t
  lotto_compute_currency_value
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   lotto_funds_t *value);

/* metrics */

extern void
  lotto_metrics_info_init
  (lotto_metrics_info_t metrics);

/* ticket */

extern void
  lotto_ticket_init
  (lotto_ticket_t ticket,
   lotto_currency_t currency,
   lotto_funds_t amount);

extern void
  lotto_ticket_activate
  (lotto_ticket_t ticket);

extern void 
  lotto_ticket_deactivate
  (lotto_ticket_t ticket);

extern lotto_funds_t
  lotto_ticket_value
  (const lotto_ticket_t ticket);

/* currency */

extern void
  lotto_currency_init
  (lotto_currency_t currency,
   const char *name,
   lotto_currency_id_t id);

extern void
  lotto_currency_activate
  (lotto_currency_t currency);

extern void 
  lotto_currency_deactivate
  (lotto_currency_t currency);

extern lotto_funds_t
  lotto_currency_value
  (const lotto_currency_t currency);

extern void
  lotto_currency_add_ticket
  (lotto_currency_t currency,
   lotto_ticket_t ticket);

extern void
  lotto_currency_remove_ticket
  (lotto_currency_t currency,
   lotto_ticket_t ticket);

/* thread */

extern void
  lotto_thread_activate
  (thread_t thread);

extern void 
  lotto_thread_deactivate
  (thread_t thread);

extern lotto_funds_t
  lotto_thread_value
  (const thread_t thread);

extern void
  lotto_thread_depress
  (thread_t thread);

extern void
  lotto_thread_undepress
  (thread_t thread);

/* random */

extern void
  lotto_random_init
  (lotto_random_t gen,
   unsigned int seed);

extern unsigned
  lotto_random_next
  (lotto_random_t gen);

/* scheduling */

extern thread_t
  lotto_sched_select
  (processor_t proc,
   boolean_t idle);

/* quantum */

extern void
  lotto_quantum_reset
  (thread_t thread);

/* lotto ipc operations */

#if	MACH_LOTTO_IPC
extern void
  lotto_ipc_init
  (void);

extern boolean_t
  lotto_ipc_is_request
  (const mach_msg_header_t *header);

extern boolean_t
  lotto_ipc_is_reply
  (const mach_msg_header_t *header);

extern void
  lotto_ipc_prim_xfer_post
  (thread_t thread,
   const mach_msg_header_t *header);

extern void
  lotto_ipc_xfer_post
  (thread_t thread,
   const mach_msg_header_t *header);

extern void
  lotto_ipc_prim_xfer_unpost
  (thread_t thread);

extern void
  lotto_ipc_xfer_unpost
  (thread_t thread);

extern void
  lotto_ipc_prim_xfer_take
  (thread_t thread,
   const mach_msg_header_t *header);

extern void
  lotto_ipc_xfer_take
  (thread_t thread,
   const mach_msg_header_t *header);

extern void
  lotto_ipc_prim_xfer_return
  (thread_t thread);

extern void
  lotto_ipc_xfer_return
  (thread_t thread);

extern void
  lotto_ipc_xfer_abort
  (thread_t thread);

extern kern_return_t
  lotto_ipc_metrics
  (processor_set_t pset,
   lotto_ipc_metrics_info *metrics);
#endif	MACH_LOTTO_IPC

#if	MACH_LOTTO_QUANTUM_METRICS
extern void
  lotto_quantum_metrics_init
  (lotto_quantum_metrics_t metrics);

extern kern_return_t
  lotto_quantum_metrics
  (processor_set_t pset,
   lotto_quantum_metrics_t metrics);
#endif	MACH_LOTTO_QUANTUM_METRICS

#endif _LOTTO_H
