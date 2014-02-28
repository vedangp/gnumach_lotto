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
 * Module:	Lotto Operations
 * Author:	Carl Waldspurger
 * History:
 *
 *	12-Dec-94	Moved lotto_last_selected global to pset structure.
 *	13-Jun-94	Modified lotto_sched_select() to use computed
 *			sum if more than one lottery attempt is needed.
 *			Added support for lazy thread deactivation;
 *			defined lotto_last_selected global.
 *	08-Jun-94	Defined lotto_remove_ticket_from_currency()
 *			and lotto_compute_{ticket,currency}_value().
 *	07-Jun-94	Added support for issuer_hint parameters to 
 *			accelerate ticket lookup by id.
 *	03-Jun-94	Defined lotto_currency_by_thread().
 *	23-May-94	Fixed problem with zone-based allocation by moving
 *			invocation of lotto_init() to setup_main().
 *			Updated to use zalloc()/zfree() code.
 *	17-May-94	Problems booting with zone-based allocation,
 *			so reverting to kalloc()/kfree(); commented out
 *			zalloc()/zfree() code, tagged with "ZZZ".
 *	16-May-94	Updated to support zone-based memory allocation.
 *			Support expanded lotto_metrics_info definition.
 *	06-May-94	Minor improvements and fixes.
 *	05-May-94	Defined lotto_ipc_xfer_abort().  Modified
 *			lotto_thread_credit_win() to avoid credit
 *			when thread currency inactive (e.g. depressed).
 *	03-May-94	Renamed "lotto_quantum_update" => "_reset".
 *			Removed lotto_ticket_set_amount(); updated
 *			lotto depress/undepress code accordingly.
 *	27-Apr-94	Updated to use LOTTO_ASSERT() and vfast_random().
 *	21-Apr-94	Defined lotto_ipc_enable() and lotto_quantum_enable().
 *	19-Apr-94	Defined and use lotto_quantum_update().
 *	18-Apr-94	Improved documentation with "locked" requirements.
 *			Removed unique id generation from lotto ipc xfers.
 *	15-Apr-94	Added code to avoid division by zero in 
 *			lotto_currency_credit_win().
 *	14-Apr-94	Added support for tickets to lotto ipc xfer.
 *	13-Apr-94	Changed lotto_depressed => lotto_depressed_amount.
 *			Several minor improvements to code.
 *	12-Apr-94	Updated to support currency-per-thread model.
 *	11-Apr-94	Added lotto ipc debugging code.
 *	08-Apr-94	Updated to support thread lotto_ipc_reply_port field.
 *			Updated to use expanded lotto_ipc_metrics_info.
 *	06-Apr-94	Minor modifications to lotto ipc operations.
 *	05-Apr-94	Fixed several lotto ipc bugs.
 *	04-Apr-94	Renamed some lotto ipc operations.
 *	03-Apr-94	Updated lotto ipc code.
 *	31-Mar-94	Moved lotto ipc code here from ipc/ipc_mqueue.c.
 *	29-Mar-94	Updated to reflect addition of dealloc field
 *			to ticket and currency structures.
 *	02-Feb-94	Moved invocations of lotto_thread_credit_win().
 *	01-Feb-94	Added support for crediting currency wins.
 *			Updated to support undepress_count metrics.
 *	31-Jan-94	Modified lotto_thread_value() to enforce minimum value.
 *			Other minor improvements.
 *	28-Jan-94	Modified lotto_currency_info() to use a single
 *			variable-size array with "flavor" tags.
 *	27-Jan-94	Minor improvements.
 *	26-Jan-94	Updated lotto_currency_ids() and lotto_currency_info()
 *			to support CountInOut for array parameters.
 *	25-Jan-94	Added more debugging support code.
 *	24-Jan-94	Fixed null currency bug in lotto_prim_destroy_ticket().
 *			Added some assertions to detect runq.low overflow.
 *	22-Jan-94	Updated to use lotto-specific run_queue fields.
 *	18-Jan-94	Minor modifications.  Implemented lotto_currency_info()
 *			operations and fixed associated type error.
 *	13-Jan-94	Defined lotto_metrics_info_init().  Conditionally
 *			compile metrics code with MACH_LOTTO_METRICS.
 *			Updated to use "spl_t" type.
 *	12-Jan-94	Improved documentation.  Declared more routines static.
 *			Updated to support collection of lotto metrics.
 *	09-Jan-94	Minor improvements.
 *	06-Jan-94	Minor modifications.  Added "zero tickets" check
 *			before lottery to avoid division by zero.
 *	05-Jan-94	Removed id argument from lotto_create_currency().
 *			Added sanity checks to lotto_destroy_currency() to
 *			avoid destruction of primitive currencies.
 *	04-Jan-94	Updated to reflect changes to list and chain fields.
 *			Modified _prim creation routines to return objects.
 *	20-Dec-93	Updated to use new lotto id types.
 *			Modified lotto_prim operations to avoid id searches.
 *			Improved some documentation.
 *	16-Dec-93	Added lotto_currency_by_name().
 *	15-Dec-93	Fixed problems in lotto_prim_destroy_currency().
 *			Improved some documentation.
 *	13-Dec-93	Updated to support thread lotto_depressed flag.
 *	09-Dec-93	Added parameters to obtain id from creation operations.
 *	08-Nov-93	Minor improvements.
 *	04-Nov-93	Updated to use lotto_currency_name_t.
 *			Added some debugging operations.
 *	27-Oct-93	Removed lotto_trap.  Started port-based routines.
 *	22-Oct-93	Modifications to support changes to chain fields.
 *			Substantial development of routines to handle traps.
 *	21-Oct-93	Added proper include for splx(), splsched() macros.
 *	20-Oct-93	Started adding support for lotto_trap() system call.
 *	19-Oct-93	Numerous improvements.
 *	18-Oct-93	Development.
 *	14-Oct-93	Updated to use _t types, renamed some functions.
 *			Modified lists to use Mach queue types.
 *	13-Oct-93	Minor fixes and improvements.
 *	06-Oct-93	Based heavily on standalone lottery scheduler.
 *
 */

/*
 * includes
 *
 */


#include <kern/thread.h>
#include <kern/task.h>
#include <kern/slab.h>
#include <kern/kalloc.h>
#include <kern/lotto.h>
#include <kern/lotto_debug.h>
#include <kern/printf.h>


static lotto_funds_t lotto_currency_value_safe(const lotto_currency_t);

/*
 * module globals
 *
 */

static struct kmem_cache lotto_ticket_zone;
static struct kmem_cache lotto_currency_zone;

/*
 * lotto_metrics_info operations
 *
 */

void
  lotto_metrics_info_init
  (lotto_metrics_info_t metrics)
{
  /*
   * modifies: metrics
   * effects:  Initializes all metrics fields to zero.
   *
   */

  metrics->select_count = 0;
  metrics->attempt_count = 0;
  metrics->search_count = 0;
  metrics->idle_count = 0;
  metrics->depress_count = 0;
  metrics->undepress_count = 0;
  metrics->min_value_count = 0;
}

#if	MACH_LOTTO_QUANTUM_METRICS
void
  lotto_quantum_metrics_init
  (lotto_quantum_metrics_t metrics)
{
  /*
   * modifies: metrics
   * effects:  Initializes all metrics fields to zero.
   *
   */

  int i;

  for (i = 0; i < LOTTO_QUANTUM_METRICS_MAX; i++)
    metrics[i] = 0;
}
#endif	/*MACH_LOTTO_QUANTUM_METRICS*/

/*
 * lotto_currency operations
 *
 */

static lotto_currency_t
  lotto_currency_alloc
  (void)
{
  /*
   * modifies: nothing
   * effects:  Allocates and returns currency storage.
   *
   */

  lotto_currency_t new;

  /* dynamically allocate memory */
  new = (lotto_currency_t) kmem_cache_alloc(&lotto_currency_zone);
  /* new = (lotto_currency_t) kalloc(sizeof(lotto_currency)); */
  if (new == LOTTO_CURRENCY_NULL)
    panic("lotto_currency_alloc: no memory");
  
  /* deallocate memory when destroyed */
  new->dealloc = TRUE;

  return(new);
}

static inline void
  lotto_currency_dealloc
  (lotto_currency_t currency)
{
  /*
   * modifies: currency
   * effects:  Deallocates all storage associated with currency.
   *
   */
  
  if (currency->dealloc)
    /* kfree((void *) currency, sizeof(lotto_currency)); */
    kmem_cache_free(&lotto_currency_zone, (vm_offset_t) currency);
}

void
  lotto_currency_init
  (lotto_currency_t currency,
   const char *name,
   lotto_currency_id_t id)
{
  /*
   * modifies: currency
   * effects:  Initializes currency using name and id.  If id is
   *	       LOTTO_CURRENCY_ID_NULL, a unique id is automatically
   *	       generated.
   *
   */

  decl_simple_lock_data(static, lotto_currency_id_lock)
  static lotto_currency_id_t lotto_currency_id = LOTTO_CURRENCY_ID_ALLOC;

  /* set id */
  currency->id = id;

  /* if necessary, generate unique currency id */
  if (id == LOTTO_CURRENCY_ID_NULL)
    {
      simple_lock(&lotto_currency_id_lock);
      currency->id = lotto_currency_id++;
      simple_unlock(&lotto_currency_id_lock);
    }

  /* set name */
  (void) strncpy(currency->name, name, LOTTO_CURRENCY_NAME_MAX);

  /* default values */
  currency->activated = FALSE;
  currency->amount = LOTTO_FUNDS_NULL;
  currency->wins = 0;
  queue_init(&currency->tickets_issued);
  queue_init(&currency->tickets_owned);

  /* list structure */
  currency->chain_pset.next = LOTTO_LINK_NULL;
  currency->chain_pset.prev = LOTTO_LINK_NULL;
}

static inline boolean_t
  lotto_currency_should_activate
  (const lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Returns TRUE iff currency should be activated.
   *
   */
  
  return((!currency->activated) && (currency->amount > LOTTO_FUNDS_NULL));
}

static inline boolean_t
  lotto_currency_should_deactivate
  (const lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Returns TRUE iff currency should be deactivated.
   *
   */
  
  return(currency->activated && (currency->amount == LOTTO_FUNDS_NULL));
}

void
  lotto_currency_activate
  (lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * modifies: currency
   * effects:  Updates currencies associated with currency
   *	       to reflect currency activation.
   *
   */

  lotto_ticket_t ticket;
  
  /* ensure currency not already activated */
  LOTTO_ASSERT(!currency->activated);
  
  if (!currency->activated)
    {
      /* activate tickets owned by currency */
      queue_iterate(&currency->tickets_owned, ticket, lotto_ticket_t, chain_owner)
	lotto_ticket_activate(ticket);
      
      /* set activated flag */
      currency->activated = TRUE;
    }
}

void 
  lotto_currency_deactivate
  (lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * modifies: currency
   * effects:  Updates currencies associated with currency
   *	       to reflect currency deactivation.
   *
   */

  lotto_ticket_t ticket;

  /* ensure currency already activated */
  LOTTO_ASSERT(currency->activated);

  if (currency->activated)
    {
      /* deactivate tickets owned by currency */
      queue_iterate(&currency->tickets_owned, ticket, lotto_ticket_t, chain_owner)
	lotto_ticket_deactivate(ticket);
      
      /* reset activated flag */
      currency->activated = FALSE;
    }
}

lotto_funds_t
  lotto_currency_value
  (const lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * requires: currency is activated
   * modifies: nothing
   * effects:  Returns the current value of currency in 
   *	       the base currency.
   *
   */

  lotto_ticket_t ticket;
  lotto_funds_t value;
  
  /* ensure currency activated */
  LOTTO_ASSERT(currency->activated);

  /* done if currency is base currency */
  if (currency->id == LOTTO_CURRENCY_ID_BASE)
    return(currency->amount);
  
  /* sum values for tickets from all currency parents */
  value = LOTTO_FUNDS_NULL;
  queue_iterate(&currency->tickets_owned, ticket, lotto_ticket_t, chain_owner)
    value += lotto_ticket_value(ticket);
  
  return(value);
}

void
  lotto_currency_add_ticket
  (lotto_currency_t currency,
   lotto_ticket_t ticket)
{
  /*
   * locked:   pset lotto
   * modifies: currency, ticket
   * effects:  Associates ticket with funding for currency.
   *
   */

  /* sanity check */
  LOTTO_ASSERT(ticket->owner_currency == LOTTO_CURRENCY_NULL);

  /* add ticket to currency ticket list */
  queue_enter(&currency->tickets_owned, ticket, lotto_ticket_t, chain_owner);

  /* set backpointer */
  ticket->owner_currency = currency;

  /* activate ticket if currency is active */
  if (currency->activated)
    lotto_ticket_activate(ticket);
}

void
  lotto_currency_remove_ticket
  (lotto_currency_t currency,
   lotto_ticket_t ticket)
{
  /*
   * locked:   pset lotto
   * modifies: currency, ticket
   * effects:  Disassociates ticket from funding for currency.
   *
   */

  /* sanity checks */
  LOTTO_ASSERT(ticket->owner_currency == currency);

  /* deactivate ticket if currency is active */
  if (currency->activated)
    lotto_ticket_deactivate(ticket);

  /* remove ticket from currency ticket list */
  queue_remove(&currency->tickets_owned, ticket, lotto_ticket_t, chain_owner);
  
  /* reset backpointer */
  ticket->owner_currency = LOTTO_CURRENCY_NULL;
}

/*
 * lotto_ticket operations
 *
 */

static lotto_ticket_t
  lotto_ticket_alloc
  (void)
{
  /*
   * modifies: nothing
   * effects:  Allocates and returns ticket storage.
   *
   */

  lotto_ticket_t new;
  
  /* dynamically allocate memory */
  new = (lotto_ticket_t) kmem_cache_alloc(&lotto_ticket_zone);
  /* new = (lotto_ticket_t) kalloc(sizeof(lotto_ticket)); */
  if (new == LOTTO_TICKET_NULL)
    panic("lotto_ticket_alloc: no memory");
  
  /* deallocate memory when destroyed */
  new->dealloc = TRUE;

  return(new);
}

static inline void
  lotto_ticket_dealloc
  (lotto_ticket_t ticket)
{
  /*
   * modifies: ticket
   * effects:  Deallocates all storage associated with ticket.
   *
   */
  
  if (ticket->dealloc)
    /* kfree((void *) ticket, sizeof(lotto_ticket)); */
    kmem_cache_free(&lotto_ticket_zone, (vm_offset_t) ticket);
}

void
  lotto_ticket_init
  (lotto_ticket_t ticket,
   lotto_currency_t currency,
   lotto_funds_t amount)
{
  /*
   * modifies: ticket
   * effects:  Initializes ticket using currency, amount.
   *
   */

  decl_simple_lock_data(static, lotto_ticket_id_lock)
  static lotto_ticket_id_t lotto_ticket_id = LOTTO_TICKET_ID_ALLOC;
  
  /* generate unique ticket id */
  simple_lock(&lotto_ticket_id_lock);
  ticket->id = lotto_ticket_id++;
  simple_unlock(&lotto_ticket_id_lock);

  /* ticket denomination */
  ticket->currency = currency;
  ticket->amount = amount;

  /* list structure */
  ticket->chain_issuer.next = LOTTO_LINK_NULL;
  ticket->chain_issuer.prev = LOTTO_LINK_NULL;
  ticket->chain_owner.next = LOTTO_LINK_NULL;
  ticket->chain_owner.prev = LOTTO_LINK_NULL;

  /* owner backpointer */
  ticket->owner_currency = LOTTO_CURRENCY_NULL;
}

void
  lotto_ticket_activate
  (lotto_ticket_t ticket)
{
  /*
   * locked:   pset lotto
   * modifies: ticket
   * effects:  Updates currency associated with ticket
   *	       to reflect ticket activation.
   *
   */

  /* update active amount */
  ticket->currency->amount += ticket->amount;

  /* propagate activation if necessary */
  if (lotto_currency_should_activate(ticket->currency))
    lotto_currency_activate(ticket->currency);
}

void 
  lotto_ticket_deactivate
  (lotto_ticket_t ticket)
{
  /*
   * locked:   pset lotto
   * modifies: ticket
   * effects:  Updates currency associated with ticket
   *	       to reflect ticket deactivation.
   *
   */

  /* update active amount */
  ticket->currency->amount -= ticket->amount;

  /* propagate if necessary */
  if (lotto_currency_should_deactivate(ticket->currency))
    lotto_currency_deactivate(ticket->currency);
}

lotto_funds_t
  lotto_ticket_value
  (const lotto_ticket_t ticket)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Returns the current value of ticket in 
   *	       the base currency.
   *
   */

  lotto_funds_t value;
  
  /* sanity check */
  LOTTO_ASSERT(ticket->currency != LOTTO_CURRENCY_NULL);

  /* done if ticket amount is zero */
  if (ticket->amount == LOTTO_FUNDS_NULL)
    return(LOTTO_FUNDS_NULL);
  
  /* done if ticket denominated in base currency */
  if (ticket->currency->id == LOTTO_CURRENCY_ID_BASE)
    return(ticket->amount);
  
  /* perform multiplication before division for precision */
  value = ticket->amount * lotto_currency_value(ticket->currency);
  value /= ticket->currency->amount;

  return(value);
}

/*
 * lotto initialization operations
 *
 */

void
  lotto_init
  (void)
{
  /*
   * modifies: lotto_ticket_zone, lotto_currency_zone (globals)
   * effects:  Initialization for lottery scheduler.  Creates 
   *	       lotto zones and initializes lotto ipc.
   *
   */

#if	MACH_LOTTO_DEBUG_VERBOSE
  	printf("lotto_init: about to zinit...\n");
#endif	/*MACH_LOTTO_DEBUG_VERBOSE*/
  
  /* initialize storage zones */
    kmem_cache_init(&lotto_ticket_zone, "lotto ticket", sizeof(lotto_ticket),0,
	  NULL,NULL,NULL,0);

    kmem_cache_init(&lotto_currency_zone,"lotto currency", sizeof(lotto_currency), 0,
	  NULL,NULL,NULL,0);
  
#if	MACH_LOTTO_IPC
  lotto_ipc_init();
#endif	/*MACH_LOTTO_IPC*/

#if	MACH_LOTTO_DEBUG_VERBOSE
  	printf("lotto_init: done\n");
#endif	/*MACH_LOTTO_DEBUG_VERBOSE*/
}

/*
 * lotto search operations
 *
 */

static lotto_currency_t
  lotto_find_currency
  (processor_set_t pset,
   lotto_currency_id_t id)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Finds and returns the currency associated with id in pset.
   *	       Returns LOTTO_CURRENCY_NULL iff no such association exists.
   *
   */

  lotto_currency_t currency;

  /* search for currency */
  queue_iterate(&pset->lotto_currencies, currency, lotto_currency_t, chain_pset)
    if (currency->id == id)
      return(currency);

  /* not found */
  return(LOTTO_CURRENCY_NULL);
}

static lotto_ticket_t
  lotto_find_ticket
  (processor_set_t pset,
   lotto_ticket_id_t id,
   lotto_currency_id_t issuer_hint)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Finds and returns the ticket associated with id in pset.
   *	       Returns LOTTO_TICKET_NULL iff no such association exists.
   *	       If issuer_hint is non-null, it is used to accelerate the
   *	       lookup of id.
   *
   */
  
  lotto_currency_t currency;
  lotto_ticket_t ticket;
  
  /* attempt to use hint */
  if (issuer_hint != LOTTO_CURRENCY_ID_NULL)
    {
      currency = lotto_find_currency(pset, issuer_hint);
      if (currency != LOTTO_CURRENCY_NULL)
	queue_iterate(&currency->tickets_issued, ticket, lotto_ticket_t, chain_issuer)
	  if (ticket->id == id)
	    {
#if	MACH_LOTTO_DEBUG_VERBOSE > 1
	      printf("lotto_find_ticket: hint succeeded\n");
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1 */     
	      return(ticket);
	    }
    }

#if	MACH_LOTTO_DEBUG_VERBOSE > 1
  if (issuer_hint != LOTTO_CURRENCY_ID_NULL)
    printf("lotto_find_ticket: hint failed\n");
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/    
  
  /* search for ticket */
  queue_iterate(&pset->lotto_currencies, currency, lotto_currency_t, chain_pset)
    queue_iterate(&currency->tickets_issued, ticket, lotto_ticket_t, chain_issuer)
      if (ticket->id == id)
	return(ticket);

  /* not found */
  return(LOTTO_TICKET_NULL);
}

static lotto_currency_t
  lotto_find_currency_by_name
  (processor_set_t pset,
   lotto_currency_name_t name)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Finds and returns the currency associated with name in pset.
   *	       Returns LOTTO_CURRENCY_NULL iff no such association exists.
   *
   */

  lotto_currency_t currency;
  
  /* search for currency */
  queue_iterate(&pset->lotto_currencies, currency, lotto_currency_t, chain_pset)
    if (strcmp(currency->name, name) == 0)
      return(currency);
  
  /* not found */
  return(LOTTO_CURRENCY_NULL);
}

static kern_return_t
  lotto_prim_currency_by_name
  (processor_set_t pset,
   lotto_currency_name_t name,
   lotto_currency_id_t *currency_id)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Sets currency_id to the id associated with name in pset, if any.
   *           Returns KERN_SUCCESS iff successful.
   *
   */

  lotto_currency_t currency;

  /* default value */
  *currency_id = LOTTO_CURRENCY_ID_NULL;
  
  /* fail if unable to find currency by name */
  currency = lotto_find_currency_by_name(pset, name);
  if (currency == LOTTO_CURRENCY_NULL)
    return(KERN_INVALID_ARGUMENT);
  
  /* set currency_id */
  *currency_id = currency->id;

  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_currency_by_name
  (processor_set_t pset,
   lotto_currency_name_t name,
   lotto_currency_id_t *currency_id)
{
  /*
   * modifies: pset
   * effects:  Invokes lotto_prim_currency_by_name().
   *
   */

  kern_return_t kr;
  spl_t s;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  kr = lotto_prim_currency_by_name(pset, name, currency_id);

  /* release lock */
  LOTTO_UNLOCK(pset, s);
  
  /* return kernel result */
  return(kr);
}

kern_return_t
  lotto_currency_by_task
  (processor_set_t pset,
   task_t task,
   lotto_currency_id_t *currency_id)
{
  /*
   * modifies: currency_id
   * effects:  Sets currency_id to the task currency associated with task.
   *	       Returns KERN_SUCCESS iff successful.
   *
   */
  
  spl_t s;
  
  /* ensure task valid */
  if (task == TASK_NULL)
    return(KERN_INVALID_ARGUMENT);

  /* acquire locks */
  task_lock(task);
  LOTTO_LOCK(pset, s);
  
  /* get currency id from task */
  *currency_id = task->lotto_task_currency->id;
  
  /* release locks */
  LOTTO_UNLOCK(pset, s);
  task_unlock(task);
  
  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_currency_by_thread
  (processor_set_t pset,
   thread_t thread,
   lotto_currency_id_t *currency_id)
{
  /*
   * modifies: currency_id
   * effects:  Sets currency_id to the thread currency associated with thread.
   *	       Returns KERN_SUCCESS iff successful.
   *
   */
  
  spl_t s;
  
  /* ensure thread valid */
  if (thread == THREAD_NULL)
    return(KERN_INVALID_ARGUMENT);

  /* acquire locks */
  thread_lock(thread);
  LOTTO_LOCK(pset, s);
  
  /* get currency id from thread */
  *currency_id = thread->lotto_thread_currency->id;
  
  /* release locks */
  LOTTO_UNLOCK(pset, s);
  thread_unlock(thread);
  
  /* everything OK */
  return(KERN_SUCCESS);
}

/*
 * lotto info operations
 *
 */

kern_return_t
  lotto_metrics
  (processor_set_t pset,
   lotto_metrics_info *metrics)
{
  /*
   * modifies: metrics
   * effects:  Sets metrics to the current lotto metrics for pset.
   *
   */

  spl_t s;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* copy metrics from processor set */
  *metrics = pset->lotto_metrics;

  /* release lock */
  LOTTO_UNLOCK(pset, s);

  /* everything OK */
  return(KERN_SUCCESS);
}

#if	MACH_LOTTO_QUANTUM_METRICS
kern_return_t
  lotto_quantum_metrics
  (processor_set_t pset,
   lotto_quantum_metrics_t metrics)
{
  /*
   * modifies: metrics
   * effects:  Sets metrics to the current quantum metrics for pset.
   *
   */

  spl_t s;
  int i;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* copy quantum metrics from processor set */
  for (i = 0; i < LOTTO_QUANTUM_METRICS_MAX; i++)
    metrics[i] = pset->lotto_quantum_metrics[i];

  /* release lock */
  LOTTO_UNLOCK(pset, s);

  /* everything OK */
  return(KERN_SUCCESS);
}
#endif	/*MACH_LOTTO_QUANTUM_METRICS*/

static kern_return_t
  lotto_prim_currency_info
  (processor_set_t pset,
   lotto_currency_t currency,
   unsigned int *wins,
   lotto_currency_name_t name,
   lotto_ticket_info_array tickets,
   unsigned int *tickets_count)
{
  /*
   * locked:   pset lotto
   * modifies: wins, name, tickets, tickets_count
   * effects:  Sets wins, name and tickets to the information associated
   *	       with currency, if any.  Sets tickets_count to reflect
   *	       the number of entries set in tickets.
   *	       Returns KERN_SUCCESS iff successful.
   *
   */

  unsigned int count, max_count;
  lotto_ticket_t ticket;

  /* ensure currency valid */
  if (currency == LOTTO_CURRENCY_NULL)
    return(KERN_INVALID_ARGUMENT);
  
  /* set wins */
  *wins = currency->wins;
  
  /* set name */
  (void) strncpy(name, currency->name, LOTTO_CURRENCY_NAME_MAX);
  
  /* initialize */
  max_count = *tickets_count;
  count = 0;
  
  /* set owned tickets */
  queue_iterate(&currency->tickets_owned, ticket, lotto_ticket_t, chain_owner)
    {
      LOTTO_ASSERT(ticket->currency != LOTTO_CURRENCY_NULL);
      if (count >= max_count)
	break;
      tickets[count].id = ticket->id;
      tickets[count].flavor = LOTTO_TICKET_FLAVOR_OWNED;
      tickets[count].currency_id = ticket->currency->id;
      tickets[count].amount = ticket->amount;
      count++;
    }

  /* set issued tickets */
  queue_iterate(&currency->tickets_issued, ticket, lotto_ticket_t, chain_issuer)
    {
      LOTTO_ASSERT(ticket->currency != LOTTO_CURRENCY_NULL);
      if (count >= max_count)
	break;
      tickets[count].id = ticket->id;
      tickets[count].flavor = LOTTO_TICKET_FLAVOR_ISSUED;
      tickets[count].currency_id = ticket->currency->id;
      tickets[count].amount = ticket->amount;
      count++;
    }
  
  /* set count */
  *tickets_count = count;
  
  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_currency_info
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   unsigned int *wins,
   lotto_currency_name_t name,
   lotto_ticket_info_array tickets,
   unsigned int *tickets_count)
{
  /*
   * modifies: wins, name, tickets, tickets_count
   * effects:  Invokes lotto_prim_currency_info().
   *
   */

  lotto_currency_t currency;
  kern_return_t kr;
  spl_t s;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  currency = lotto_find_currency(pset, currency_id);
  kr = lotto_prim_currency_info(pset, 
				currency,
				wins,
				name,
				tickets, 
				tickets_count);

  /* release lock */
  LOTTO_UNLOCK(pset, s);

  /* return status */
  return(kr);
}

kern_return_t
  lotto_currency_ids
  (processor_set_t pset,
   lotto_currency_id_array_t currency_ids,
   unsigned int *currency_ids_count)
{
  /*
   * modifies: currency_ids, currency_ids_count
   * effects:  Sets currency_ids to the ids for all lotto currencies.
   *	       Sets currency_ids_count to the number of currency_ids.
   *
   */

  unsigned int count, max_count;
  lotto_currency_t currency;
  spl_t s;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* initialize */
  max_count = *currency_ids_count;
  count = 0;
  
  /* set currency_ids */
  queue_iterate(&pset->lotto_currencies, currency, lotto_currency_t, chain_pset)
    {
      if (count >= max_count)
	break;
      currency_ids[count] = currency->id;
      count++;
    }
  
  /* set currency_ids_count */
  *currency_ids_count = count;
  
  /* release lock */
  LOTTO_UNLOCK(pset, s);
  
  /* return status */
  return(KERN_SUCCESS);
}

static lotto_funds_t
  lotto_ticket_value_safe
  (const lotto_ticket_t ticket)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Returns the current value of ticket in the base currency.
   *	       Uses lotto_currency_value_safe() to avoid problems with
   *	       deactivated currencies; deactivated currency has no value.
   *
   */

  //static lotto_funds_t lotto_currency_value_safe(const lotto_currency_t);
  lotto_funds_t value, currency_value;
  
  /* sanity check */
  LOTTO_ASSERT(ticket->currency != LOTTO_CURRENCY_NULL);

  /* done if ticket amount is zero */
  if (ticket->amount == LOTTO_FUNDS_NULL)
    return(LOTTO_FUNDS_NULL);
  
  /* done if ticket denominated in base currency */
  if (ticket->currency->id == LOTTO_CURRENCY_ID_BASE)
    return(ticket->amount);
  
  /* safely compute currency value, done if zero */
  currency_value = lotto_currency_value_safe(ticket->currency);
  if (currency_value == LOTTO_FUNDS_NULL)
    return(LOTTO_FUNDS_NULL);

  /* perform multiplication before division for precision */
  value = ticket->amount * currency_value;
  value /= ticket->currency->amount;

  return(value);
}

kern_return_t
  lotto_compute_ticket_value
  (processor_set_t pset,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint,
   lotto_funds_t *value)
{
  /*
   * modifies: value
   * effects:  Sets value to the value of the ticket associated with ticket_id
   *	       in the base currency.  The value of a ticket issued in a
   *	       deactivated currency is zero.
   *
   */

  lotto_ticket_t ticket;
  spl_t s;

#if	MACH_LOTTO_DEBUG_VERBOSE > 1
  	printf("lotto_compute_ticket_value: ticket_id=%d\n", ticket_id);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* lookup ticket */
  ticket = lotto_find_ticket(pset, ticket_id, issuer_hint);

  /* set value */
  if (ticket != LOTTO_TICKET_NULL)
    *value = lotto_ticket_value_safe(ticket);
  else
    *value = LOTTO_FUNDS_NULL;

  /* release lock */
  LOTTO_UNLOCK(pset, s);

#if	MACH_LOTTO_DEBUG_VERBOSE > 1
  	printf("lotto_compute_ticket_value: ticket_id=%d, value=%d\n",
		ticket_id,
		*value);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/

  /* everything OK */
  return(KERN_SUCCESS);
}

static lotto_funds_t
  lotto_currency_value_safe
  (const lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * requires: currency is activated
   * modifies: nothing
   * effects:  Returns the current value of currency in the base currency.
   *	       A deactivated currency is considered to have no value.
   *
   */

  lotto_ticket_t ticket;
  lotto_funds_t value;
  
  /* deactivated currency has no value */
  if (!currency->activated)
    {
#if	MACH_LOTTO_DEBUG_VERBOSE > 1
     	 printf("lotto_currency_value_safe: cid=%d, inactive\n",
		    currency->id);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/
      return(LOTTO_FUNDS_NULL);
    }
  
  /* done if currency is base currency */
  if (currency->id == LOTTO_CURRENCY_ID_BASE)
    return(currency->amount);
  
  /* sum values for tickets from all currency parents */
  value = LOTTO_FUNDS_NULL;
  queue_iterate(&currency->tickets_owned, ticket, lotto_ticket_t, chain_owner)
    value += lotto_ticket_value(ticket);
  
  return(value);
}

kern_return_t
  lotto_compute_currency_value
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   lotto_funds_t *value)
{
  /*
   * modifies: value
   * effects:  Sets value to the value of the currency associated with
   *	       currency_id, measured in the base currency.  The value of
   *	       a deactivated currency is zero.
   *
   */

  lotto_currency_t currency;
  spl_t s;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* lookup currency */
  currency = lotto_find_currency(pset, currency_id);

  /* set value */
  if (currency != LOTTO_CURRENCY_NULL)
    *value = lotto_currency_value_safe(currency);
  else
    *value = LOTTO_FUNDS_NULL;

  /* release lock */
  LOTTO_UNLOCK(pset, s);

  /* everything OK */
  return(KERN_SUCCESS);
}

/*
 * lotto creation operations
 *
 */

kern_return_t
  lotto_prim_create_ticket
  (processor_set_t pset,
   lotto_currency_t currency,
   lotto_funds_t amount,
   lotto_ticket_t storage,
   lotto_ticket_t *ticket)
{
  /*
   * locked:   pset lotto
   * modifies: pset, ticket
   * effects:  Creates a new lotto ticket with value amount denominated in
   *	       currency.  The new ticket is not owned by any object.  
   *	       Memory for the new ticket is provided by storage; memory
   *	       is dynamically allocated iff storage is LOTTO_TICKET_NULL.
   *	       Sets ticket to the new lotto ticket and returns KERN_SUCCESS
   *	       iff successful.
   *
   */

  lotto_ticket_t new;

  /* default value */
  *ticket = LOTTO_TICKET_NULL;

  /* ensure currency valid */
  if (currency == LOTTO_CURRENCY_NULL)
    return(KERN_INVALID_ARGUMENT);
  
  /* create ticket */
  new = storage;
  if (new == LOTTO_TICKET_NULL)
    new = lotto_ticket_alloc();
  else
    new->dealloc = FALSE;
  lotto_ticket_init(new, currency, amount);
  
  /* link ticket into currency list */
  queue_enter(&currency->tickets_issued, new, lotto_ticket_t, chain_issuer);
  
  /* set ticket */
  *ticket = new;
  
  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_create_ticket
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   lotto_funds_t amount,
   lotto_ticket_id_t *ticket_id)
{
  /*
   * modifies: pset, ticket_id
   * effects:  Invokes lotto_prim_create_ticket().
   *
   */

  lotto_currency_t currency;
  lotto_ticket_t ticket;
  kern_return_t kr;
  spl_t s;
  
  /* default value */
  *ticket_id = LOTTO_TICKET_ID_NULL;
  
  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  currency = lotto_find_currency(pset, currency_id);
  kr = lotto_prim_create_ticket(pset,
				currency,
				amount,
				LOTTO_TICKET_NULL,
				&ticket);
  if (kr == KERN_SUCCESS)
    *ticket_id = ticket->id;

  /* release lock */
  LOTTO_UNLOCK(pset, s);
  
  /* return kernel result */
  return(kr);
}

kern_return_t
  lotto_prim_create_currency
  (processor_set_t pset,
   lotto_currency_name_t name,
   lotto_currency_id_t id,
   lotto_currency_t storage,
   lotto_currency_t *currency)
{
  /*
   * locked:   pset lotto
   * modifies: pset
   * effects:  Creates a new lotto currency with the specified name and id.
   *	       Memory for the new currency is provided by storage; memory
   *	       is dynamically allocated iff storage is LOTTO_CURRENCY_NULL.
   *	       Sets currency to the new lotto currency and returns 
   *	       KERN_SUCCESS iff successful.
   *
   */

  lotto_currency_t new;

  /* default value */
  *currency = LOTTO_CURRENCY_NULL;

  /* fail if currency name isn't unique */
  new = lotto_find_currency_by_name(pset, name);
  if (new != LOTTO_CURRENCY_NULL)
    return(KERN_INVALID_ARGUMENT);

  /* create currency */
  new = storage;
  if (new == LOTTO_CURRENCY_NULL)
    new = lotto_currency_alloc();
  else
    new->dealloc = FALSE;
  lotto_currency_init(new, name, id);
  
  /* link currency into pset list */
  queue_enter(&pset->lotto_currencies, new, lotto_currency_t, chain_pset);
    
#if	MACH_LOTTO_DEBUG_VERBOSE > 1
  	printf("create: ");
  lotto_currency_print(new);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/

  /* set currency */
  *currency = new;

  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_create_currency
  (processor_set_t pset,
   lotto_currency_name_t name,
   lotto_currency_id_t *currency_id)
{
  /*
   * modifies: pset
   * effects:  Invokes lotto_prim_create_currency().
   *
   */

  lotto_currency_t currency;
  kern_return_t kr;
  spl_t s;

  /* default value */
  *currency_id = LOTTO_CURRENCY_ID_NULL;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  kr = lotto_prim_create_currency(pset,
				  name,
				  LOTTO_CURRENCY_ID_NULL,
				  LOTTO_CURRENCY_NULL,
				  &currency);
  if (kr == KERN_SUCCESS)
    *currency_id = currency->id;

  /* release lock */
  LOTTO_UNLOCK(pset, s);

  /* return kernel result */
  return(kr);
}  

/*
 * lotto destruction operations 
 *
 */

kern_return_t
  lotto_prim_destroy_ticket
  (processor_set_t pset,
   lotto_ticket_t ticket)
{
  /*
   * locked:   pset lotto
   * modifies: pset, ticket
   * effects:  Removes ticket from pset, and reclaims all associated storage.
   *
   */

  /* ensure ticket valid */
  if (ticket == LOTTO_TICKET_NULL)
    return(KERN_INVALID_ARGUMENT);

  /* sanity check */
  LOTTO_ASSERT(ticket->currency != LOTTO_CURRENCY_NULL);

  /* remove it from its present owner, if any */
  if (ticket->owner_currency != LOTTO_CURRENCY_NULL)
    lotto_currency_remove_ticket(ticket->owner_currency, ticket);
  
  /* unlink ticket from issuing currency */
  queue_remove(&ticket->currency->tickets_issued, ticket, lotto_ticket_t, chain_issuer);
  
  /* destroy ticket values */
  ticket->amount = LOTTO_FUNDS_NULL;
  ticket->currency = LOTTO_CURRENCY_NULL;

  /* deallocate ticket */
  lotto_ticket_dealloc(ticket);

  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_destroy_ticket
  (processor_set_t pset,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint)
{
  /*
   * modifies: pset
   * effects:  Removes the ticket associated with ticket_id in pset, if any.
   *	       Reclaims all storage associated with the removed ticket.
   *	       If issuer_hint is non-null, it is used to accelerate the
   *	       lookup of ticket_id.
   *
   */

  kern_return_t kr;
  spl_t s;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  kr = lotto_prim_destroy_ticket(pset,
				 lotto_find_ticket(pset, ticket_id, issuer_hint));

  /* release lock */
  LOTTO_UNLOCK(pset, s);

  /* return kernel result */
  return(kr);
}  

void
  lotto_prim_destroy_tickets
  (processor_set_t pset,
   queue_t ticket_queue)
{
  /*
   * locked:   pset lotto
   * modifies: pset, ticket_queue
   * effects:  Destroys all tickets in ticket_queue using 
   *	       lotto_prim_destroy_ticket().
   *
   */

  /* destroy each ticket in ticket_queue */
  while (!queue_empty(ticket_queue))
    (void) lotto_prim_destroy_ticket(pset,
				     (lotto_ticket_t) queue_first(ticket_queue));
}

kern_return_t
  lotto_prim_destroy_currency
  (processor_set_t pset,
   lotto_currency_t currency,
   boolean_t destroy_tickets)
{
  /*
   * locked:   pset lotto
   * modifies: pset, currency
   * effects:  Removes currency from pset, and reclaims all associated storage.
   *	       If destroy_tickets is set, all tickets denominated in currency
   *	       will also be destroyed.
   *
   */

  /* ensure currency valid */
  if (currency == LOTTO_CURRENCY_NULL)
    return(KERN_INVALID_ARGUMENT);

#if	MACH_LOTTO_DEBUG_VERBOSE > 1
  	printf("destroy: ");
  lotto_currency_print(currency);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/

  /* ensure all tickets denominated in currency already removed */
  if (!queue_empty(&currency->tickets_issued))
    {
      /* fail if not authorized to remove tickets */
      if (!destroy_tickets)
	return(KERN_INVALID_ARGUMENT);
      
      /* destroy each ticket issued by currency */
      lotto_prim_destroy_tickets(pset, &currency->tickets_issued);
    }

  /* deactivate currency, if necessary */
  if (currency->activated)
    lotto_currency_deactivate(currency);
  
  /* destroy each ticket owned by currency */
  lotto_prim_destroy_tickets(pset, &currency->tickets_owned);

  /* unlink currency from pset list */
  queue_remove(&pset->lotto_currencies, currency, lotto_currency_t, chain_pset);
  
  /* deallocate currency */
  lotto_currency_dealloc(currency);

  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_destroy_currency
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   boolean_t destroy_tickets)
{
  /*
   * modifies: pset
   * effects:  Removes currency associated with currency_id in pset, if any.
   *	       Reclaims all storage associated with the removed ticket.
   *
   */
  
  lotto_currency_t currency;
  kern_return_t kr;
  spl_t s;
  
  /* fail if attempt to remove primitive currency */
  if (currency_id == LOTTO_CURRENCY_ID_BASE ||
      currency_id == LOTTO_CURRENCY_ID_SYSTEM ||
      currency_id == LOTTO_CURRENCY_ID_USER)
    return(KERN_INVALID_ARGUMENT);

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  currency = lotto_find_currency(pset, currency_id);
  kr = lotto_prim_destroy_currency(pset, currency, destroy_tickets);

  /* release lock */
  LOTTO_UNLOCK(pset, s);

  /* return kernel result */
  return(kr);
}  

/*
 * lotto association operations
 *
 */

kern_return_t
  lotto_prim_move_ticket_to_currency
  (processor_set_t pset,
   lotto_ticket_t ticket,
   lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * modifies: pset, ticket, currency
   * effects:  Disassociates ticket from current owner, if any.
   *	       Associates ticket with currency.
   *
   */

  /* ensure ticket valid */
  if (ticket == LOTTO_TICKET_NULL)
    return(KERN_INVALID_ARGUMENT);

  /* ensure currency valid */
  if (currency == LOTTO_CURRENCY_NULL)
    return(KERN_INVALID_ARGUMENT);

  /* remove ticket from its present owner, if any */
  if (ticket->owner_currency != LOTTO_CURRENCY_NULL)
    lotto_currency_remove_ticket(ticket->owner_currency, ticket);
  
  /* attach ticket to currency */
  lotto_currency_add_ticket(currency, ticket);
  
  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_move_ticket_to_currency
  (processor_set_t pset,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint,
   lotto_currency_id_t currency_id)
{
  /*
   * modifies: pset
   * effects:  Invokes lotto_prim_move_ticket_to_currency().
   *
   */

  lotto_currency_t currency;
  lotto_ticket_t ticket;
  kern_return_t kr;
  spl_t s;
  
  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  ticket = lotto_find_ticket(pset, ticket_id, issuer_hint);
  currency = lotto_find_currency(pset, currency_id);
  kr = lotto_prim_move_ticket_to_currency(pset, ticket, currency);

  /* release lock */
  LOTTO_UNLOCK(pset, s);
  
  /* return kernel result */
  return(kr);
}  

kern_return_t
  lotto_prim_remove_ticket_from_currency
  (processor_set_t pset,
   lotto_ticket_t ticket,
   lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * modifies: pset, ticket, currency
   * effects:  Disassociates ticket from currency, if any.
   *
   */

  /* ensure ticket valid */
  if (ticket == LOTTO_TICKET_NULL)
    return(KERN_INVALID_ARGUMENT);

  /* ensure currency valid */
  if (currency == LOTTO_CURRENCY_NULL)
    return(KERN_INVALID_ARGUMENT);

  /* check that currency owns ticket */
  if (ticket->owner_currency != currency)
    return(KERN_INVALID_ARGUMENT);
  
  /* remove ticket from its present owner, if any */
  lotto_currency_remove_ticket(ticket->owner_currency, ticket);
  
  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_remove_ticket_from_currency
  (processor_set_t pset,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint,
   lotto_currency_id_t currency_id)
{
  /*
   * modifies: pset
   * effects:  Invokes lotto_prim_remove_ticket_from_currency().
   *
   */

  lotto_currency_t currency;
  lotto_ticket_t ticket;
  kern_return_t kr;
  spl_t s;
  
  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  ticket = lotto_find_ticket(pset, ticket_id, issuer_hint);
  currency = lotto_find_currency(pset, currency_id);
  kr = lotto_prim_remove_ticket_from_currency(pset, ticket, currency);

  /* release lock */
  LOTTO_UNLOCK(pset, s);
  
  /* return kernel result */
  return(kr);
}  

/*
 * thread operations
 *
 */

void
  lotto_thread_activate
  (thread_t thread)
{
  /*
   * locked:   pset lotto
   * modifies: thread
   * effects:  Updates ticket associated with thread
   *	       to reflect thread activation.
   *
   */

  //lotto_ticket_t ticket;
  
  /* ensure thread not already activated */
  LOTTO_ASSERT(!thread->lotto_activated);
  
  if (!thread->lotto_activated)
    {
      /* activate ticket */
      lotto_ticket_activate(thread->lotto_thread_ticket);

      /* set activated flag */
      thread->lotto_activated = TRUE;
    }
}

void 
  lotto_thread_deactivate
  (thread_t thread)
{
  /*
   * locked:   pset lotto
   * modifies: thread
   * effects:  Updates ticket associated with thread
   *	       to reflect thread deactivation.
   *
   */

  //lotto_ticket_t ticket;

  /* ensure thread already activated */
  LOTTO_ASSERT(thread->lotto_activated);

  if (thread->lotto_activated)
    {
      /* deactivate ticket */
      lotto_ticket_deactivate(thread->lotto_thread_ticket);
      
      /* reset activated flag */
      thread->lotto_activated = FALSE;
    }
}

inline lotto_funds_t
  lotto_thread_value
  (const thread_t thread)
{
  /*
   * locked:   pset lotto
   * modifies: nothing
   * effects:  Returns the current value of thread in
   *	       the base currency.
   *
   */
  
  lotto_funds_t value;
  
  /* sanity check */
  LOTTO_ASSERT(thread->policy == POLICY_LOTTO);
  
  /* compute value of thread ticket */
  value = lotto_ticket_value(thread->lotto_thread_ticket);
  
  /* ensure minimum, non-zero value */
  if (value < LOTTO_FUNDS_MIN)
    {
#if	MACH_LOTTO_METRICS
      processor_set_t pset;
      
      /* convenient abbreviation */
      pset = thread->processor_set;
      
      /* update metrics: min value count */
      pset->lotto_metrics.min_value_count++;
#endif	/*MACH_LOTTO_METRICS*/

      value = LOTTO_FUNDS_MIN;
    }
  
  return(value);
}

static void
  lotto_prim_thread_depress
  (processor_set_t pset,
   thread_t thread)
{
  /*
   * locked:   thread, pset lotto
   * modifies: pset, thread
   * effects:  Depresses thread by setting flag and deactivating tickets.
   *
   */

  //lotto_ticket_t ticket;

  /* ensure thread not already depressed */
  LOTTO_ASSERT(thread->lotto_depressed_amount == LOTTO_FUNDS_NULL);

  if (thread->lotto_depressed_amount == LOTTO_FUNDS_NULL)
    {
      /* store depressed amount */
      thread->lotto_depressed_amount = thread->lotto_thread_ticket->amount;
      
      /* keep count */
      pset->lotto_depress_count++;
      
#if	MACH_LOTTO_METRICS
      /* update metrics: depress count */
      pset->lotto_metrics.depress_count++;
#endif	/*MACH_LOTTO_METRICS*/

      /* zero ticket value, handling activation appropriately */
      if (thread->lotto_activated)
	{
	  lotto_ticket_deactivate(thread->lotto_thread_ticket);
	  thread->lotto_thread_ticket->amount = LOTTO_FUNDS_NULL;
	  lotto_ticket_activate(thread->lotto_thread_ticket);
	}
      else
	{
	  thread->lotto_thread_ticket->amount = LOTTO_FUNDS_NULL;
	}
      
      /* maintain lotto priority */
      thread->sched_pri = LOTTO_PRIORITY;
    }
}

void
  lotto_thread_depress
  (thread_t thread)
{
  /*
   * locked:   thread
   * modifies: thread
   * effects:  Invokes lotto_prim_thread_depress().
   *
   */
   
  processor_set_t pset;
  spl_t s;

  /* convenient abbreviation */
  pset = thread->processor_set;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive operation */
  lotto_prim_thread_depress(pset, thread);

  /* release lock */
  LOTTO_UNLOCK(pset, s);
}

static void
  lotto_prim_thread_undepress
  (processor_set_t pset,
   thread_t thread)
{
  /*
   * locked:   thread, pset lotto
   * modifies: pset, thread
   * effects:  Undepresses thread by resetting flag and reactivating tickets.
   *
   */

  //lotto_ticket_t ticket;

  /* ensure thread already depressed */
  LOTTO_ASSERT(thread->lotto_depressed_amount != LOTTO_FUNDS_NULL);

  if (thread->lotto_depressed_amount != LOTTO_FUNDS_NULL)
    {
      /* restore ticket value, handling activation appropriately */
      if (thread->lotto_activated)
	{
	  lotto_ticket_deactivate(thread->lotto_thread_ticket);
	  thread->lotto_thread_ticket->amount = thread->lotto_depressed_amount;
	  lotto_ticket_activate(thread->lotto_thread_ticket);
	}
      else
	{
	  thread->lotto_thread_ticket->amount = thread->lotto_depressed_amount;
	}
      
      /* reset depressed amount */
      thread->lotto_depressed_amount = LOTTO_FUNDS_NULL;
      
      /* keep count */
      pset->lotto_depress_count--;

#if	MACH_LOTTO_METRICS
      /* update metrics: undepress count */
      pset->lotto_metrics.undepress_count++;
#endif	/*MACH_LOTTO_METRICS*/
      
      /* maintain lotto priority */
      thread->sched_pri = LOTTO_PRIORITY;
    }
}

void
  lotto_thread_undepress
  (thread_t thread)
{
  /*
   * locked:   thread
   * modifies: thread
   * effects:  Invokes lotto_prim_thread_undepress().
   *
   */

  processor_set_t pset;
  spl_t s;

  /* convenient abbreviation */
  pset = thread->processor_set;

  /* acquire lock */
  LOTTO_LOCK(pset, s);

  /* invoke primitive */
  lotto_prim_thread_undepress(pset, thread);

  /* release lock */
  LOTTO_UNLOCK(pset, s);
}

static void
  lotto_currency_credit_win
  (processor_set_t pset,
   lotto_currency_t currency)
{
  /*
   * locked:   pset lotto
   * effects:  Credits lottery win by propagating information back
   *	       through the funding chain.
   *
   */

  lotto_funds_t value, winner, sum;
  lotto_ticket_t ticket;
  
  /* sanity check */
  LOTTO_ASSERT(currency != LOTTO_CURRENCY_NULL);

  /* credit currency win */
  currency->wins++;

  /* done if at base currency */
  if (currency->id == LOTTO_CURRENCY_ID_BASE)
    return;

  /* done if no backing currencies */
  if (queue_empty(&currency->tickets_owned))
    return;

  /* propagate to backing currencies */

  /* optimize common case: single-ticket funding */
  ticket = (lotto_ticket_t) queue_first(&currency->tickets_owned);
  if (ticket == (lotto_ticket_t) queue_last(&currency->tickets_owned))
    {
      lotto_currency_credit_win(pset, ticket->currency);
      return;
    }
  
  /* credit win using last random number and currency value */
  winner = LOTTO_FUNDS_NULL;
  value = lotto_currency_value(currency);
  if (value != LOTTO_FUNDS_NULL)
    winner = pset->lotto_rnd.seed % value;
  
  /* find winning ticket */
  sum = 0;
  queue_iterate(&currency->tickets_owned, ticket, lotto_ticket_t, chain_owner)
    {
      sum += lotto_ticket_value(ticket);
      if (sum >= winner)
	{
	  lotto_currency_credit_win(pset, ticket->currency);
	  return;
	}
    }
  
#if	MACH_LOTTO_DEBUG
  	printf("lotto_currency_credit_win: unable to credit win\n");
#endif	/*MACH_LOTTO_DEBUG*/
}

static void
  lotto_thread_credit_win
  (processor_set_t pset,
   thread_t thread)
{
  /*
   * locked:   pset lotto
   * effects:  Credits lottery win by propagating information back
   *	       through the funding chain.
   *
   */
  
  /* punt if thread not activated */
  if (!thread->lotto_activated)
    return;
  
  /* credit win to thread's currency, if activated (may be depressed)  */
  if (thread->lotto_thread_currency->activated)
    lotto_currency_credit_win(pset, thread->lotto_thread_currency);
}

void
  lotto_quantum_reset
  (thread_t thread)
{
  /*
   * locked:   pset lotto
   * modifies: thread
   * effects:  Resets quantum information associated with thread.
   *	       Sets quantum cost to be (uncompensated) thread value.
   *	       Resets quantum usage and compensation.
   *
   */

  /* remove previous quantum ticket compensation */
  if (thread->lotto_quantum_ticket->owner_currency != LOTTO_CURRENCY_NULL)
    {
      /* remove ticket from thread currency */
      lotto_currency_remove_ticket(thread->lotto_thread_currency, 
				   thread->lotto_quantum_ticket);
      
      /* reset ticket amount */
      thread->lotto_quantum_ticket->amount = LOTTO_FUNDS_NULL;
    }
  
  /* compute quantum cost (ignoring previous compensation) */
  thread->lotto_quantum_cost = lotto_thread_value(thread);

  /* reset quantum usage */
  thread->lotto_quantum_used = 0;
}

kern_return_t 
  lotto_quantum_enable
  (processor_set_t pset,
   boolean_t enable)
{
  /*
   * modifies: pset lotto_quantum_enabled flag
   * effects:  Sets lotto_quantum_enabled to enable.
   *
   */

  spl_t s;

  /* aqcuire lotto lock */
  LOTTO_LOCK(pset, s);

  /* set global flag */
  pset->lotto_quantum_enabled = enable;

  /* release lotto ipc lock */
  LOTTO_UNLOCK(pset, s);

  /* everything OK */
  return(KERN_SUCCESS);
}


/*
 * random operations
 *
 */

void
  lotto_random_init
  (lotto_random_t gen, 
   unsigned int seed)
{
  /*
   * modifies: gen
   * effects:  Initializes gen with specified seed.
   *
   */

  gen->seed = seed;
}

#ifdef	mips
inline unsigned
  lotto_random_next
  (lotto_random_t gen)
{
  /*
   * requires: MIPS architecture
   * modifies: gen
   * effects:  Generates the next random number in the pseudo-random
   *           sequence defined by the multiplicative linear congruential
   *           generator S' = 16807 * S mod (2^31 - 1).  This invokes an
   *	       optimized MIPS assembly-language version of this generator
   *	       called vfast_random().
   *
   */
  
  unsigned vfast_random(unsigned);
  
  gen->seed = vfast_random(gen->seed);
  return(gen->seed);
}
#else
unsigned
  lotto_random_next
  (lotto_random_t gen)
{
  /*
   * requires: "unsigned long long" is 64 bits, "unsigned" is 32 bits
   * modifies: gen
   * effects:  Generates the next random number in the pseudo-random
   *           sequence defined by the multiplicative linear congruential
   *           generator S' = 16807 * S mod (2^31 - 1).  This is the ACM
   *           "minimal standard random number generator".  Based on
   *           method described by D.G. Carta in CACM, January 1990.
   *
   */

  unsigned long long product;
  unsigned product_lo, product_hi;
  long test;

  product = 33614 * (long long) gen->seed;

  product_lo = product & (0xffffffff);
  product_hi = product >> 32;
  product_lo >>= 1;

  test = product_lo + product_hi;

  if (test > 0)
    gen->seed = test;
  else
    {
      test &= 0x7fffffff;
      gen->seed = test + 1;
    }

  return(gen->seed);
}
#endif	/*mips*/

/*
 * scheduling operations
 *
 */

static thread_t
  lotto_idle_thread
  (processor_set_t pset,
   processor_t proc)
{
  /*
   * modifies: pset, proc
   * effects:  Set processor proc idle, and return its idle thread.
   *	       Based on vanilla code from choose_pset_thread().
   *
   */

  /* acquire pset idle lock */
  simple_lock(&pset->idle_lock);
  
  /* only update running processor */
  if (proc->state == PROCESSOR_RUNNING)
    {
      /* update processor state */
      proc->state = PROCESSOR_IDLE;
      
      /* put master on end of queue so it gets used last */
      if (proc == master_processor) 
	{
	  queue_enter(&pset->idle_queue, 
		      proc,
		      processor_t,
		      processor_queue);
	}
      else 
	{
	  queue_enter_first(&pset->idle_queue,
			    proc,
			    processor_t,
			    processor_queue);
	}
      
      /* update idle count */
      pset->idle_count++;
    }
  
  /* release pset idle lock */
  simple_unlock(&pset->idle_lock);
  
  /* return idle thread for proc */
  return(proc->idle_thread);
}

thread_t
  lotto_sched_select
  (processor_t proc,
   boolean_t idle)
{
  /*
   * requires: At splsched() interrupt level.
   * modifies: proc
   * effects:  Holds a lottery to select a thread for proc to run.
   *	       May select the current thread.  If idle is TRUE and
   *	       there are no runnable threads, an idle thread will be
   *	       returned.
   *
   */

  thread_t thread, current, selected;
  lotto_funds_t total, winner, sum;
  boolean_t current_runnable;
  unsigned int count, attempt;
  processor_set_t pset;

#if	MACH_HOST
  pset = proc->processor_set;
#else	/*MACH_HOST*/
  pset = &default_pset;
#endif	/*MACH_HOST*/

  /* initialize */
  current_runnable = FALSE;
  sum = LOTTO_FUNDS_NULL;
  attempt = 0;

  /* acquire lotto lock */
  simple_lock(&pset->lotto_lock);
  
  /* support lazy deactivation */
  if (pset->lotto_last_selected != THREAD_NULL)
    {
      /* need to deactivate if no longer runnable */
      if (pset->lotto_last_selected->runq == RUN_QUEUE_NULL &&
	  pset->lotto_last_selected->state != TH_RUN)
	if (pset->lotto_last_selected->lotto_activated)
	  lotto_thread_deactivate(pset->lotto_last_selected);
      
      /* reset last selected */
      pset->lotto_last_selected = THREAD_NULL;
    }
  
#if	MACH_LOTTO_METRICS
  /* update metrics: select count */
  pset->lotto_metrics.select_count++;

#if	MACH_LOTTO_DEBUG_VERBOSE > 1
  if ((pset->lotto_metrics.select_count & 4095) == 0)
    	printf("lotto_sched_select: select_count = %d\n",
		  pset->lotto_metrics.select_count);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/

#endif	/*MACH_LOTTO_METRICS*/

  /* consider current thread, if runnable */
  current = current_thread();
  if ((current->state == TH_RUN) &&
#ifdef	MACH_HOST
      (current->processor_set == pset) &&
#endif	/*MACH_HOST*/
      (current->policy == POLICY_LOTTO) &&
      ((current->bound_processor == PROCESSOR_NULL) ||
       (current->bound_processor == proc)))
    {
      /* set flag */
      current_runnable = TRUE;
    }

  /* loop because (single) retry may be needed if rounding errors (rare) */
  do
    {
      /* update attempt */
      attempt++;
      
#if	MACH_LOTTO_METRICS
      /* update metrics: attempt count */
      pset->lotto_metrics.attempt_count++;
      
      /* update metrics: search count */
      if (attempt > 1)
	pset->lotto_metrics.search_count += count;
#endif	/*MACH_LOTTO_METRICS*/

#if	MACH_LOTTO_DEBUG_VERBOSE 
      if (attempt > 2)
		printf("lotto_sched_select: "
		      "attempt = %u, "
		      "total = %u, "
		      "winner = %u, "
		      "sum = %u, "
		      "count = %u\n",
		      attempt,
		      total,
		      winner,
		      sum,
		      count);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE*/ 

      /* should never exceed two attempts since computed sum used on retry */
      LOTTO_ASSERT(attempt <= 2);
      
      /* initialize */
      selected = THREAD_NULL;
      count = 0;
      
      if (sum == LOTTO_FUNDS_NULL)
	{
	  /* conservatively overestimate total value in base currency */
	  total = pset->lotto_base_currency.amount;
	  total += LOTTO_FUNDS_MIN * pset->lotto_count;
	}
      else
	{
	  /* use actual computed sum from previous attempt */
	  total = sum;
#if	MACH_LOTTO_DEBUG_VERBOSE > 1
	 	printf("lotto_sched_select: using computed sum=%d\n", sum);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/
	}
      
      /* zero total => idle (avoid division by zero) */
      if (total == LOTTO_FUNDS_NULL)
	break;

      /* randomly select winning ticket */
      winner = lotto_random_next(&pset->lotto_rnd) % total;
      
#if	MACH_LOTTO_DEBUG_VERBOSE > 1
      printf("lotto_sched_select: "
                    "total = %u, "
		    "winner = %u\n", 
		    total,
		    winner);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/

      /* search for winner */
      
      /* check current thread, if runnable */
      if (current_runnable)
	{
	  /* update running sum */
	  sum += lotto_thread_value(current);
	  
	  /* count runnable threads */
	  count++;
	  
	  /* done if current is winner */
	  if (sum >= winner)
	    {
	      selected = current;
	      pset->lotto_last_selected = selected;
#if	MACH_LOTTO_CREDIT_WIN
	      lotto_thread_credit_win(pset, selected);
#endif	/*MACH_LOTTO_CREDIT_WIN*/
	      lotto_quantum_reset(selected);
	    }
	}
      
      /* check pset runq (common case) */
      if (selected == THREAD_NULL)
	{
	  queue_t pset_runq_lotto;
	  run_queue_t pset_runq;

	  /* convenient abbreviations */
	  pset_runq = &pset->runq;
	  pset_runq_lotto = &pset_runq->lotto_runq;
	  
	  /* acquire pset runq lock */
	  simple_lock(&pset_runq->lock);
	  
	  /* sanity checks */
	  LOTTO_ASSERT(pset_runq->low <= LAST_VALID_RUNQ);

#if	MACH_LOTTO_DEBUG_VERBOSE  
	  /* consistency check */
	  if (pset->lotto_count != pset_runq->lotto_count)
	   	 printf("lotto_sched_select: lotto count mismatch: "
			  "pset = %d, "
			  "lotto = %d\n",
			  pset->lotto_count,
			  pset_runq->lotto_count);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE*/
	  
	  /* search pset runq for winner */
	  queue_iterate(pset_runq_lotto, thread, thread_t, links)
	    {
	      /* update running sum */
	      sum += lotto_thread_value(thread);
	      
	      /* count runnable threads */
	      count++;
	      
	      /* done if thread is winner */
	      if (sum >= winner)
		{
		  selected = thread;
		  pset->lotto_last_selected = selected;
#if	MACH_LOTTO_CREDIT_WIN
		  lotto_thread_credit_win(pset, selected);
#endif	/*MACH_LOTTO_CREDIT_WIN*/
		  lotto_quantum_reset(selected);
		  queue_remove(pset_runq_lotto, selected, thread_t, links);
		  /* lotto_thread_deactivate(selected); */
		  selected->runq = RUN_QUEUE_NULL;
		  pset_runq->count--;
		  pset_runq->lotto_count--;
		  pset->lotto_count--;
		  break;
		}
	    }
	  
	  /* release pset runq lock */
	  simple_unlock(&pset_runq->lock);
	}
      
      /* check proc runq (uncommon case) */
      if (selected == THREAD_NULL)
	{
	  queue_t proc_runq_lotto;
	  run_queue_t proc_runq;

	  /* convenient abbreviations */
	  proc_runq = &proc->runq;
	  proc_runq_lotto = &proc_runq->lotto_runq;
	  
	  /* acquire proc runq lock */
	  simple_lock(&proc_runq->lock);
	  
	  /* sanity check */
	  LOTTO_ASSERT(proc_runq->low <= LAST_VALID_RUNQ);
	  
	  /* search proc runq for winner */
	  queue_iterate(proc_runq_lotto, thread, thread_t, links)
	    {
	      /* update running sum */
	      sum += lotto_thread_value(thread);
	      
	      /* count runnable threads */
	      count++;
	      
	      /* done if thread is winner */
	      if (sum >= winner)
		{
		  selected = thread;
		  pset->lotto_last_selected = selected;
#if	MACH_LOTTO_CREDIT_WIN
		  lotto_thread_credit_win(pset, selected);
#endif	/*MACH_LOTTO_CREDIT_WIN*/
		  lotto_quantum_reset(selected);
		  queue_remove(proc_runq_lotto, selected, thread_t, links);
		  /* lotto_thread_deactivate(selected); */
		  selected->runq = RUN_QUEUE_NULL;
		  proc_runq->count--;
		  proc_runq->lotto_count--;
		  pset->lotto_count--;
		  break;
		}
	    }
	  
	  /* release proc runq lock */
	  simple_unlock(&proc_runq->lock);
	}

#if	MACH_LOTTO_DEBUG_VERBOSE > 1
      		printf("lotto_sched_select: "
		    "count=%d, total=%u, winner=%u, selected = %x\n",
		    count, 
		    total,
		    winner,
		    selected);
#endif	/*MACH_LOTTO_DEBUG_VERBOSE > 1*/

    } while (selected == THREAD_NULL && count > 0);
  
#if	MACH_LOTTO_METRICS
  /* update metrics: idle count */
  if (selected == THREAD_NULL)
    pset->lotto_metrics.idle_count++;

  /* update metrics: search count */
  pset->lotto_metrics.search_count += count;
#endif	/*MACH_LOTTO_METRICS*/

  /* release lotto lock */
  simple_unlock(&pset->lotto_lock);
  
  /* select idle thread, if specified */
  if (selected == THREAD_NULL && idle)
    selected = lotto_idle_thread(pset, proc);

  /* return winner */
  return(selected);
}

#if	MACH_LOTTO_DEBUG
/*
 * debugging operations
 *
 */

void
  lotto_currency_print
  (lotto_currency_t currency)
{
  /*
   * modifies: stdout
   * effects:  Prints a human-readable representation of currency.
   *
   */

  	printf("[currency: "
		"name = %s, "
		"id = %u, "
		"activated = %d, "
		"amount = %u]\n",
		currency->name,
		currency->id,
		currency->activated,
		currency->amount);
}

void
  lotto_ticket_print
  (lotto_ticket_t ticket)
{
  /*
   * modifies: stdout
   * effects:  Prints a human-readable representation of ticket.
   *
   */

  	printf("[ticket: "
		"id = %u, "
		"currency = %u, "
		"amount = %u, "
		"ownerc = %x]\n",
		ticket->id,
		ticket->currency->id,
		ticket->amount,
		ticket->owner_currency);
}
#endif	/*MACH_LOTTO_DEBUG*/

#if	MACH_LOTTO_IPC
/*
 * lotto ipc globals
 *
 */

/* should probably associate with pset instead */
decl_simple_lock_data(static, lotto_ipc_lock)
static queue_head_t lotto_ipc_queue;
static lotto_ipc_metrics_info lotto_ipc_metrics_data;
static struct kmem_cache lotto_ipc_xfer_zone;
boolean_t lotto_ipc_enabled;

/*
 * lotto ipc operations 
 *
 */

static void
  lotto_ipc_metrics_init
  (lotto_ipc_metrics_t metrics)
{
  /*
   * modifies: metrics
   * effects:  Initializes all metrics fields to zero.
   *
   */

  /* basic operation counts */
  metrics->post_count = 0;
  metrics->unpost_count = 0;
  metrics->take_count = 0;
  metrics->return_count = 0;

  /* final state counts */
  metrics->unpost_after_post_count = 0;
  metrics->unpost_after_take_count = 0;
  metrics->unpost_after_return_count = 0;
  metrics->unpost_after_other_count = 0;
  
  /* search counts */
  metrics->search_count = 0;
  metrics->search_fail_count = 0;

  /* unusual condition counts */
  metrics->complex_request_count = 0;
  metrics->complex_reply_count = 0;
  metrics->post_after_post_count = 0;
  metrics->take_after_post_count = 0;
  metrics->take_after_take_count = 0;
}

kern_return_t
  lotto_ipc_metrics
  (processor_set_t pset,
   lotto_ipc_metrics_info *metrics)
{
  /*
   * modifies: metrics
   * effects:  Sets metrics to the current lotto ipc metrics.
   *
   */

  /* acquire lotto ipc lock */
  LOTTO_IPC_LOCK();

  /* copy metrics from global */
  *metrics = lotto_ipc_metrics_data;

  /* release lotto ipc lock */
  LOTTO_IPC_UNLOCK();

  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t 
  lotto_ipc_enable
  (processor_set_t pset,
   boolean_t enable)
{
  /*
   * modifies: lotto_ipc_enabled flag (global)
   * effects:  Sets lotto_ipc_enabled to enable.
   *
   */

  /* aqcuire lotto ipc lock */
  LOTTO_IPC_LOCK();

  /* set global flag */
  lotto_ipc_enabled = enable;

  /* release lotto ipc lock */
  LOTTO_IPC_UNLOCK();

  /* everything OK */
  return(KERN_SUCCESS);
}

void
  lotto_ipc_init
  (void)
{
  /*
   * modifies: lotto ipc globals
   * effects:  Initializes lotto ipc globals.
   *
   */

  /* initialize lotto ipc globals */
  simple_lock_init(&lotto_ipc_lock);
  queue_init(&lotto_ipc_queue);
  lotto_ipc_enabled = TRUE;

  /* initialize lotto ipc metrics */
  lotto_ipc_metrics_init(&lotto_ipc_metrics_data);

  /* initialize storage zone */
    kmem_cache_init(&lotto_ipc_xfer_zone, "lotto ipc xfer" , sizeof(lotto_ipc_xfer),0, 
	  NULL, NULL, NULL, 0);

#if	MACH_LOTTO_DEBUG_VERBOSE
  	printf("lotto_ipc_init: done\n");
#endif	/*MACH_LOTTO_DEBUG_VERBOSE*/
}

static lotto_ipc_xfer_t
  lotto_ipc_xfer_alloc
  (void)
{
  /*
   * modifies: nothing
   * effects:  Allocates and returns xfer storage.
   *
   */

  lotto_ipc_xfer_t new;

  /* dynamically allocate memory */
  new = (lotto_ipc_xfer_t) kmem_cache_alloc(&lotto_ipc_xfer_zone);
  /* new = (lotto_ipc_xfer_t) kalloc(sizeof(lotto_ipc_xfer)); */
  if (new == LOTTO_IPC_XFER_NULL)
    panic("lotto_ipc_xfer_alloc: no memory");

  return(new);
}

static inline void
  lotto_ipc_xfer_dealloc
  (lotto_ipc_xfer_t xfer)
{
  /*
   * modifies: xfer
   * effects:  Deallocates all storage associated with xfer.
   *
   */

  /* kfree((void *) xfer, sizeof(lotto_ipc_xfer)); */
  kmem_cache_free(&lotto_ipc_xfer_zone, (vm_offset_t) xfer);
}

void
  lotto_ipc_xfer_init
  (lotto_ipc_xfer_t xfer,
   lotto_ipc_state_t state,
   const mach_msg_header_t *header,
   thread_t sender,
   thread_t receiver)
{
  /*
   * modifies: xfer
   * effects:  Initializes xfer using state, header, sender, and receiver.
   *
   */

  /* set state */
  xfer->state = state;

  /* set header */
  xfer->header = *header;

  /* set participants */
  xfer->sender = sender;
  xfer->receiver = receiver;

  /* set ticket */
  xfer->ticket = LOTTO_TICKET_NULL;

  /* list structure */
  xfer->chain.next = LOTTO_LINK_NULL;
  xfer->chain.prev = LOTTO_LINK_NULL;
}

lotto_ipc_xfer_t
  lotto_ipc_xfer_create
  (lotto_ipc_state_t state,
   const mach_msg_header_t *header)
{
  /*
   * modifies: nothing
   * effects:  Creates and returns a new lotto_ipc_xfer using state and header.
   *
   */

  lotto_ipc_xfer_t new;

  /* allocate storage */
  new = lotto_ipc_xfer_alloc();

  /* initialize */
  lotto_ipc_xfer_init(new,
		      state,
		      header,
		      THREAD_NULL, 
		      THREAD_NULL);

  return(new);
}

boolean_t
  lotto_ipc_is_request
  (const mach_msg_header_t *header)
{
  /*
   * modifies: nothing
   * effects:  Returns TRUE iff header represents a request message.
   *
   */

  /* check if request message */
  if (header->msgh_bits == 
      MACH_MSGH_BITS(MACH_MSG_TYPE_PORT_SEND, MACH_MSG_TYPE_PORT_SEND_ONCE))
    return(TRUE);

  /* check if complex request message */
  if (header->msgh_bits ==
      (MACH_MSGH_BITS_COMPLEX |
       MACH_MSGH_BITS(MACH_MSG_TYPE_PORT_SEND, MACH_MSG_TYPE_PORT_SEND_ONCE)))
    {
#if	MACH_LOTTO_IPC_METRICS
      /* update metrics */
      LOTTO_IPC_LOCK();
      lotto_ipc_metrics_data.complex_request_count++;
      LOTTO_IPC_UNLOCK();
#endif	/*MACH_LOTTO_IPC_METRICS*/

      return(TRUE);
    }

  /* not a request message */
  return(FALSE);
}

boolean_t
  lotto_ipc_is_reply
  (const mach_msg_header_t *header)
{
  /*
   * modifies: nothing
   * effects:  Returns TRUE iff header represents a reply message.
   *
   */

  /* check if reply message */
  if (header->msgh_bits ==
      MACH_MSGH_BITS(MACH_MSG_TYPE_PORT_SEND_ONCE, 0))
    return(TRUE);

  /* check if complex reply message */
  if (header->msgh_bits ==
      (MACH_MSGH_BITS_COMPLEX |
       MACH_MSGH_BITS(MACH_MSG_TYPE_PORT_SEND_ONCE, 0)))
    {
#if	MACH_LOTTO_IPC_METRICS
      /* update metrics */
      LOTTO_IPC_LOCK();
      lotto_ipc_metrics_data.complex_reply_count++;
      LOTTO_IPC_UNLOCK();
#endif	/*MACH_LOTTO_IPC_METRICS*/

      return(TRUE);
    }

  /* not a reply message */
  return(FALSE);
}

lotto_ipc_xfer_t 
  lotto_ipc_xfer_find
  (const mach_msg_header_t *header)
{
  /*
   * locked:   lotto ipc
   * modifies: nothing
   * effects:  Searches for lotto ipc xfer associated with header.
   *	       Returns xfer if found, otherwise LOTTO_IPC_XFER_NULL.
   *
   */

  lotto_ipc_xfer_t xfer;

  /* search for match with header */
  queue_iterate(&lotto_ipc_queue, xfer, lotto_ipc_xfer_t, chain)
    {
#if	MACH_LOTTO_IPC_METRICS
      /* update metrics */
      lotto_ipc_metrics_data.search_count++;
#endif	/*MACH_LOTTO_IPC_METRICS*/
      
      /* return xfer if appropriate header fields match */
      if ((xfer->header.msgh_remote_port == header->msgh_remote_port) &&
	  (xfer->header.msgh_local_port == header->msgh_local_port) &&
	  (xfer->header.msgh_bits == header->msgh_bits))
	return(xfer);
    }
  
#if	MACH_LOTTO_IPC_METRICS
  lotto_ipc_metrics_data.search_fail_count++;
#endif	/*MACH_LOTTO_IPC_METRICS*/

  /* not found */
  return(LOTTO_IPC_XFER_NULL);
}

void
  lotto_ipc_prim_xfer_post
  (thread_t thread,
   const mach_msg_header_t *header)
{
  /*
   * locked:   lotto ipc
   * modifies: thread 
   * effects:  Posts funding for the receiver of a synchronous request.
   *	       Intended to be called from ipc_mqueue_send() when
   *	       sending a request message.
   *
   */

  lotto_ipc_xfer_t xfer;
  processor_set_t pset;
  spl_t s;

  /* !! need to lock thread? */
  if (thread->policy != POLICY_LOTTO)
    return;

  /* done if post not enabled */
  if (!thread->lotto_ipc_post_enable)
    return;

  /* unpost previous xfer; disallow nested posts */
  if (thread->lotto_ipc_send != LOTTO_IPC_XFER_NULL)
    {
#if	MACH_LOTTO_IPC_METRICS
      /* update metrics */
      lotto_ipc_metrics_data.post_after_post_count++;
#endif	/*MACH_LOTTO_IPC_METRICS*/
      
      lotto_ipc_prim_xfer_unpost(thread);
    }
  
  /* sanity check */
  LOTTO_ASSERT(thread->lotto_ipc_send == LOTTO_IPC_XFER_NULL);
  
#if	MACH_LOTTO_IPC_METRICS
  /* update metrics */
  lotto_ipc_metrics_data.post_count++;
#endif	/*MACH_LOTTO_IPC_METRICS*/
  
  /* create xfer */
  xfer = lotto_ipc_xfer_create(LOTTO_IPC_POST, header);

  /* convenient abbreviation */
  pset = thread->processor_set;

  /* acquire lotto lock */
  LOTTO_LOCK(pset, s);

  /* create ticket */
  (void) lotto_prim_create_ticket(pset,
				  thread->lotto_thread_currency,
				  LOTTO_IPC_XFER_AMOUNT,
				  LOTTO_TICKET_NULL,
				  &xfer->ticket);

  /* release lotto lock */
  LOTTO_UNLOCK(pset, s);

  /* associate xfer with sender thread */
  thread->lotto_ipc_send = xfer;
  xfer->sender = thread;
  
  /* add xfer to queue (add at head to exploit locality) */
  queue_enter_first(&lotto_ipc_queue, xfer, lotto_ipc_xfer_t, chain);
}

void
  lotto_ipc_xfer_post
  (thread_t thread,
   const mach_msg_header_t *header)
{
  /*
   * modifies: thread 
   * effects:  Invokes lotto_ipc_prim_xfer_post().
   *
   */

  /* acquire lotto ipc lock */
  LOTTO_IPC_LOCK();

  /* invoke primitive */
  lotto_ipc_prim_xfer_post(thread, header);

  /* release lotto ipc lock */
  LOTTO_IPC_UNLOCK();
}

void
  lotto_ipc_prim_xfer_unpost
  (thread_t thread)
{
  /*
   * locked:   lotto ipc
   * modifies: thread 
   * effects:  Unposts funding for the receiver of a synchronous request
   *	       that was posted via lotto_ipc_xfer_post().
   *
   */

  lotto_ipc_xfer_t xfer;

  /* convenient abbreviation */
  xfer = thread->lotto_ipc_send;

  /* done if no xfer to unpost */
  if (xfer == LOTTO_IPC_XFER_NULL)
    return;

  /* sanity check */
  LOTTO_ASSERT(xfer->sender == thread);

#if	MACH_LOTTO_IPC_METRICS
  /* update metrics */
  lotto_ipc_metrics_data.unpost_count++;

  switch (xfer->state) {
  case LOTTO_IPC_POST:
    lotto_ipc_metrics_data.unpost_after_post_count++;
    break;
  case LOTTO_IPC_TAKE:
    lotto_ipc_metrics_data.unpost_after_take_count++;
    break;
  case LOTTO_IPC_RETURN:
    lotto_ipc_metrics_data.unpost_after_return_count++;
    break;
  default:
    lotto_ipc_metrics_data.unpost_after_other_count++;
    break;
  }
#endif	/*MACH_LOTTO_IPC_METRICS*/

  /* remove xfer from queue */
  if (xfer->state == LOTTO_IPC_POST)
    queue_remove(&lotto_ipc_queue, xfer, lotto_ipc_xfer_t, chain);
  
  /* disassociate xfer from sender thread */
  thread->lotto_ipc_send = LOTTO_IPC_XFER_NULL;

  /* disassociate xfer from receiver thread, if any */
  if (xfer->receiver != THREAD_NULL)
    {
      LOTTO_ASSERT(xfer->receiver->lotto_ipc_recv == xfer);
      xfer->receiver->lotto_ipc_recv = LOTTO_IPC_XFER_NULL;
    }      

  /* destroy transferred ticket, if any */
  if (xfer->ticket != LOTTO_TICKET_NULL)
    {
      processor_set_t pset;
      spl_t s;

      /* convenient abbreviation */
      pset = thread->processor_set;

      /* acquire lotto lock */
      LOTTO_LOCK(pset, s);

      /* destroy ticket */
      lotto_prim_destroy_ticket(pset, xfer->ticket);
      xfer->ticket = LOTTO_TICKET_NULL;

      /* release lotto lock */
      LOTTO_UNLOCK(pset, s);
    }

  /* deallocate xfer */
  lotto_ipc_xfer_dealloc(xfer);
}

void
  lotto_ipc_xfer_unpost
  (thread_t thread)
{
  /*
   * modifies: thread 
   * effects:  Invokes lotto_ipc_prim_xfer_unpost().
   *
   */

  /* acquire lotto ipc lock */
  LOTTO_IPC_LOCK();

  /* invoke primitive */
  lotto_ipc_prim_xfer_unpost(thread);

  /* release lotto ipc lock */
  LOTTO_IPC_UNLOCK();
}

void
  lotto_ipc_prim_xfer_take
  (thread_t thread,
   const mach_msg_header_t *header)
{
  /*
   * locked:   lotto ipc
   * modifies: thread
   * effects:  Obtains funding from the sender of a synchronous request.
   *	       Intended to be called from ipc_mqueue_recv() when 
   *	       receiving a request message.
   *
   */

  lotto_ipc_xfer_t xfer;
  processor_set_t pset;
  spl_t s;

  /* !! need to lock thread? */
  if (thread->policy != POLICY_LOTTO)
    return;

  /* sanity check: disallow take after post */
  if (thread->lotto_ipc_send != LOTTO_IPC_XFER_NULL)
    {
#if	MACH_LOTTO_IPC_METRICS
      /* update metrics */
      lotto_ipc_metrics_data.take_after_post_count++;
#endif	/*MACH_LOTTO_IPC_METRICS*/

      lotto_ipc_prim_xfer_unpost(thread);
    }

  /* find xfer associated with header */
  xfer = lotto_ipc_xfer_find(header);

  /* done if not found */
  if (xfer == LOTTO_IPC_XFER_NULL)
    return;

  /* remove xfer from queue */
  queue_remove(&lotto_ipc_queue, xfer, lotto_ipc_xfer_t, chain);
  
#if	MACH_LOTTO_IPC_METRICS
  /* update metrics */
  lotto_ipc_metrics_data.take_count++;
#endif	/*MACH_LOTTO_IPC_METRICS*/
  
  /* return previous xfer; disallow nested takes */
  if (thread->lotto_ipc_recv != LOTTO_IPC_XFER_NULL)
    {
#if	MACH_LOTTO_IPC_METRICS
      /* update metrics */
      lotto_ipc_metrics_data.take_after_take_count++;
#endif	/*MACH_LOTTO_IPC_METRICS*/
      
      lotto_ipc_prim_xfer_return(thread);
    }
  
  /* sanity checks */
  LOTTO_ASSERT(thread->lotto_ipc_recv == LOTTO_IPC_XFER_NULL);
  LOTTO_ASSERT(xfer->state == LOTTO_IPC_POST);
  
  /* associate xfer with receiver thread */
  xfer->receiver = thread;
  xfer->state = LOTTO_IPC_TAKE;
  thread->lotto_ipc_recv = xfer;
  
  /* sanity check */
  LOTTO_ASSERT(xfer->ticket != LOTTO_TICKET_NULL);
  
  /* convenient abbreviation */
  pset = thread->processor_set;
  
  /* acquire lotto lock */
  LOTTO_LOCK(pset, s);
  
  /* transfer ticket to receiver */
  lotto_currency_add_ticket(thread->lotto_thread_currency, xfer->ticket);
  
  /* release lotto lock */
  LOTTO_UNLOCK(pset, s);
}

void
  lotto_ipc_xfer_take
  (thread_t thread,
   const mach_msg_header_t *header)
{
  /*
   * modifies: thread 
   * effects:  Invokes lotto_ipc_prim_xfer_take().
   *
   */

  /* acquire lotto ipc lock */
  LOTTO_IPC_LOCK();

  /* invoke primitive */
  lotto_ipc_prim_xfer_take(thread, header);

  /* release lotto ipc lock */
  LOTTO_IPC_UNLOCK();
}

void
  lotto_ipc_prim_xfer_return
  (thread_t thread)
{
  /*
   * locked:   lotto ipc
   * modifies: thread
   * effects:  Returns funding that was previously obtained via
   *	       lotto_ipc_xfer_take().  Intended to be called from
   *	       ipc_mqueue_send() when sending a reply message.
   *
   */

  lotto_ipc_xfer_t xfer;
  processor_set_t pset;
  spl_t s;

  /* convenient abbreviation */
  xfer = thread->lotto_ipc_recv;

  /* done if no xfer to return */
  if (xfer == LOTTO_IPC_XFER_NULL)
    return;

#if	MACH_LOTTO_IPC_METRICS
  /* update metrics */
  lotto_ipc_metrics_data.return_count++;
#endif	/*MACH_LOTTO_IPC_METRICS*/

  /* sanity checks */
  LOTTO_ASSERT(xfer->receiver == thread);
  LOTTO_ASSERT(xfer->ticket != LOTTO_TICKET_NULL);
  LOTTO_ASSERT(xfer->state == LOTTO_IPC_TAKE);

  /* convenient abbreviation */
  pset = thread->processor_set;

  /* acquire lotto lock */
  LOTTO_LOCK(pset, s);

  /* destroy transferred ticket */
  lotto_prim_destroy_ticket(pset, xfer->ticket);
  xfer->ticket = LOTTO_TICKET_NULL;

  /* release lotto lock */
  LOTTO_UNLOCK(pset, s);

  /* disassociate receiver thread and xfer */
  xfer->receiver = THREAD_NULL;
  xfer->state = LOTTO_IPC_RETURN;
  thread->lotto_ipc_recv = LOTTO_IPC_XFER_NULL;
}

void
  lotto_ipc_xfer_abort
  (thread_t thread)
{
  /*
   * modifies: thread
   * effects:  Aborts all lotto ipc xfers associated with thread.
   *
   */

  /* acquire lotto ipc lock */
  LOTTO_IPC_LOCK();

  /* return incoming xfers */
  lotto_ipc_prim_xfer_return(thread);

  /* revoke outgoing xfers */
  lotto_ipc_prim_xfer_unpost(thread);

  /* release lotto ipc lock */
  LOTTO_IPC_UNLOCK();
}

void
  lotto_ipc_xfer_return
  (thread_t thread)
{
  /*
   * modifies: thread 
   * effects:  Invokes lotto_ipc_prim_xfer_return().
   *
   */

  /* acquire lotto ipc lock */
  LOTTO_IPC_LOCK();

  /* invoke primitive */
  lotto_ipc_prim_xfer_return(thread);

  /* release lotto ipc lock */
  LOTTO_IPC_UNLOCK();
}
#endif	/*MACH_LOTTO_IPC*/
