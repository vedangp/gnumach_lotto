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
 * Project:	Mach Lottery Scheduler (User Programs)
 * Module:      util - common utility operations
 * Author:	Carl Waldspurger
 * History:
 *
 *	15-Jun-94	Defined lotto_time_now().
 *	14-Jun-94	Defined lotto_set_funding().
 *	07-Jun-94	Added issuer_hint to lotto_ticket_reference().
 *	03-Jun-94	Added support for naming currencies by machid mids.
 *	05-May-94	Added need_privileged argument to lotto_pset_port().
 *	31-Jan-94	Added lotto_ticket_reference().
 *	11-Jan-94	Added operations, improved documentation.
 *	09-Jan-94	First version written, based on mkcur.c.
 *
 */

/*
 * includes
 *
 */

#include "lotto_util.h"
#include <sys/time.h>

/*
 * globals
 *
 */

static mach_port_t machid_server_port;
static mach_port_t machid_auth_port;

/*
 * machid operations
 *
 */

static kern_return_t
  lotto_machid_init
  (void)
{
  static boolean_t initialized = FALSE;
  kern_return_t result;
  
  /* done if already initialized */
  if (initialized)
    return(KERN_SUCCESS);
  
  /* obtain machid server port */
  result = netname_look_up(name_server_port,
			   "", 
			   "MachID", 
			   &machid_server_port);
  if (result != KERN_SUCCESS)
    return(result);
  
  /* obtain machid auth port */
  machid_auth_port = mach_host_priv_self();
  if (machid_auth_port == MACH_PORT_NULL)
    machid_auth_port = mach_task_self();

  /* set initialization flag */
  initialized = TRUE;

  /* everything OK */
  return(KERN_SUCCESS);
}


/*
 * utility operations
 *
 */

kern_return_t
  lotto_pset_port
  (mach_port_t *pset_port,
   boolean_t need_privileged)
{
  /*
   * modifies: pset_port
   * effects:  Attempts to obtain a port for the current processor set.
   *           If unable to obtain a privileged port, an ordinary port is
   *	       returned unless need_privileged is set. If successful, sets
   *	       pset_port and returns KERN_SUCCESS, otherwise KERN_FAILURE.
   *
   */

  mach_port_t auth, default_pset, pset;
  kern_return_t result;

  /* obtain priv host port */
  auth = mach_host_priv_self();
  if (!MACH_PORT_VALID(auth))
    {
      /* fail if unable and need priv */
      if (need_privileged)
	return(KERN_FAILURE);
      
      /* fallback to unprivileged auth */
      auth = mach_task_self();
    }
  
  /* obtain default pset port */
  result = processor_set_default(auth, &default_pset);
  if (result != KERN_SUCCESS)
    return(result);
  
  /* obtain priv pset port */
  result = host_processor_set_priv(auth, default_pset, &pset);
  if (result != KERN_SUCCESS)
    {
      /* fail uf unable and need priv */
      if (need_privileged)
	return(result);

      /* fallback to default pset port */
      pset = default_pset;
    }

  /* set pset port */
  *pset_port = pset;

  /* everything OK */
  return(KERN_SUCCESS);
}

kern_return_t
  lotto_parse_currency
  (processor_set_t pset,
   const char *currency_string,
   lotto_currency_id_t *currency_id)
{
  /*
   * modifies: currency_id
   * effects:  !!
   *	       Returns KERN_SUCCESS iff successful.
   *
   */

  lotto_currency_name_t name;

  /* default values */
  *currency_id = LOTTO_CURRENCY_ID_NULL;

  /* copy string to name */
  (void) strncpy(name, currency_string, LOTTO_CURRENCY_NAME_MAX);

  /* cid# represents the currency with id # */
  if (strncmp(name, "cid", 3) == 0)
    {
      int id;

      /* convert string => id; fail if negative */
      id = atoi(name + 3);
      if (id < 0)
	return(KERN_INVALID_ARGUMENT);
      
      /* everything OK */
      *currency_id = id;
      return(KERN_SUCCESS);
    }
  
  /* pid# represents the currency for process # */
  if (strncmp(name, "pid", 3) == 0)
    {
      task_t task;
      int pid;
      
      /* convert Unix pid => Mach task */
      pid = atoi(name + 3);
      task = task_by_pid(pid);
      if (!MACH_PORT_VALID(task))
	return(KERN_FAILURE);
      
      /* find currency associated with Mach task */
      return(lotto_currency_by_task(pset, task, currency_id));
    }

  /* mid# represents the currency for machid # */
  if (strncmp(name, "mid", 3) == 0)
    {
      kern_return_t result;
      mach_type_t type;
      thread_t thread;
      task_t task;
      mach_id_t mid;

      /* initialize machid interface */
      if (lotto_machid_init() != KERN_SUCCESS)
	return(KERN_FAILURE);

      /* determine type of mach object */
      mid = atoi(name + 3);
      result = machid_mach_type(machid_server_port, 
				machid_auth_port,
				mid,
				&type);
      if (result != KERN_SUCCESS)
	return(result);
      
      /* convert mach task or thread => currency */
      switch (type) {
      case MACH_TYPE_TASK:
	result = machid_mach_port(machid_server_port,
				  machid_auth_port,
				  mid,
				  &task);
	if (result != KERN_SUCCESS)
	  return(result);
	return(lotto_currency_by_task(pset, task, currency_id));
      case MACH_TYPE_THREAD:
	result = machid_mach_port(machid_server_port,
				  machid_auth_port,
				  mid,
				  &thread);
	if (result != KERN_SUCCESS)
	  return(result);
	return(lotto_currency_by_thread(pset, thread, currency_id));
      default:
	return(KERN_FAILURE);
      }
    }
  
  /* lookup currency by name */
  return(lotto_currency_by_name(pset, name, currency_id));
}

kern_return_t
  lotto_parse_ticket
  (processor_set_t pset,
   const char *ticket_string,
   lotto_currency_id_t *currency_id,
   lotto_funds_t *amount)
{
  /*
   * modifies: currency_id, amount
   * effects:  !!
   *	       Returns KERN_SUCCESS iff successful.
   *
   */

  char *s, *currency;
  buffer buf;
  int n;

  /* default values */
  *currency_id = LOTTO_CURRENCY_ID_NULL;
  *amount = 0;
  
  /* copy string into buffer */
  (void) strcpy(buf, ticket_string);
  
  /* initialize */
  currency = NULL;

  /* format is amount.currency */
  for (s = buf; *s != '\0'; s++)
    if (*s == '.')
      {
	/* found separator: replace with NUL */
	*s = '\0';
	currency = ++s;
	break;
      }

  /* fail if separator not found */
  if (currency == NULL)
    return(KERN_INVALID_ARGUMENT);

  /* parse amount; fail if negative */
  n = atoi(buf);
  if (n < 0)
    return(KERN_INVALID_ARGUMENT);
  *amount = n;  
  
  /* parse currency */
  return(lotto_parse_currency(pset, currency, currency_id));
}

kern_return_t
  lotto_ticket_reference
  (processor_set_t pset,
   const char *ticket_string,
   lotto_ticket_id_t *ticket_id,
   lotto_currency_id_t *issuer_hint)
{
  /*
   * modifies: ticket_id
   * effects:  Sets ticket_id to the ticket represented by ticket_string.
   *	       If ticket_string has format "tkt#", ticket_id is set to #.
   *	       If ticket_string has format "amount.currency", then a new
   *	       ticket with the specified amount and currency will be
   *	       created, and ticket_id will be set to its id.  May set 
   *	       issuer_hint to the id associated with currency, or to
   *	       LOTTO_CURRENCY_ID_NULL if the id is not easily accessible.
   *           Returns KERN_SUCCESS iff successful.
   *
   */

  lotto_currency_id_t currency_id;
  lotto_funds_t amount;
  kern_return_t result;

  /* default values */
  *ticket_id = LOTTO_TICKET_ID_NULL;
  *issuer_hint = LOTTO_CURRENCY_ID_NULL;

  /* tkt# represents the ticket with id # */
  if (strncmp(ticket_string, "tkt", 3) == 0)
    {
      int id;
      
      /* convert string => id; fail if negative */
      id = atoi(ticket_string + 3);
      if (id < 0)
	return(KERN_INVALID_ARGUMENT);
      
      /* everything OK */
      *ticket_id = id;
      return(KERN_SUCCESS);
    }
  
  /* parse ticket with format amount.currency */
  result = lotto_parse_ticket(pset, ticket_string, &currency_id, &amount);
  if (result != KERN_SUCCESS)
    return(result);
  
  /* set hint */
  *issuer_hint = currency_id;

  /* create new ticket */
  return(lotto_create_ticket(pset, currency_id, amount, ticket_id));
}

kern_return_t
  lotto_set_funding
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint)
{
  /*
   * modifies: nothing
   * effects:  Sets the funding for currency_id to ticket_id.
   *
   */

  lotto_ticket_info tickets[LOTTO_TICKET_INFO_MAX];
  unsigned int tickets_count, wins;
  lotto_currency_name_t name;
  kern_return_t result;
  int i;

  /* add new ticket to currency */
  result = lotto_move_ticket_to_currency(pset, 
					 ticket_id,
					 issuer_hint,
					 currency_id);
  if (result != KERN_SUCCESS)
    return(result);
  
  /* query currency to obtain existing tickets */
  tickets_count = LOTTO_TICKET_INFO_MAX;
  result = lotto_currency_info(pset,
			       currency_id,
			       &wins,
			       name,
			       tickets,
			       &tickets_count);
  if (result != KERN_SUCCESS)
    return(result);
  
  /* remove all old tickets from currency */
  for (i = 0; i < tickets_count; i++)
    if (tickets[i].flavor == LOTTO_TICKET_FLAVOR_OWNED)
      if (tickets[i].id != ticket_id)
	{
	  result = lotto_destroy_ticket(pset, 
					tickets[i].id,
					tickets[i].currency_id);
	  if (result != KERN_SUCCESS)
	    return(result);
	}
  
  /* everything OK */
  return(KERN_SUCCESS);
}

double
  lotto_time_now
  (void)
{
  struct timeval t;
  double now;
  
  /* obtain current time */
  gettimeofday(&t, NULL);
  
  /* convert to double */
  now = (double) t.tv_sec + (double) t.tv_usec * 1.0e-6;
  
  return(now);
}
