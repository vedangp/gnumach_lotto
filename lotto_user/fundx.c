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
 * Module:      fundx - fund and execute a computation
 * Author:	Carl Waldspurger
 * History:
 *
 *	15-Jun-94	Added current time to printed information.
 *	07-Jun-94	Updated to support currency hints for ticket ids.
 *	03-Jun-94	Improved to setuid() back to original user after
 *			completing lotto operations as root.
 *	05-May-94	Updated to support lotto_pset_port() interface change.
 *	22-Apr-94	First version started, based on fund.c.
 *
 */

/*
 * includes
 *
 */

#include <stdio.h>
#include "lotto_util.h"

/*
 * main program
 *
 */

int main
  (int argc,
   char **argv)
{
  lotto_ticket_info tickets[LOTTO_TICKET_INFO_MAX];
  lotto_currency_id_t currency_id, issuer_hint;
  unsigned int tickets_count, wins;
  lotto_ticket_id_t ticket_id;
  lotto_currency_name_t name;
  char *program, **args;
  kern_return_t result;
  mach_port_t pset;
  int i;

  /* fail if bad usage */
  if (argc < 3)
    quit(EXIT_FAILURE, 
	 "usage: %s <ticket> <program> <arg>...\n",
	 argv[0]);
  
  /* obtain pset port */
  result = lotto_pset_port(&pset, FALSE);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "fundx: lotto_pset_port: %s\n",
	 mach_error_string(result));

  /* parse command-line arguments <program> <args> */
  program = argv[2];
  args = &argv[2];

  /* parse command-line argument <ticket> */  
  result = lotto_ticket_reference(pset, argv[1], &ticket_id, &issuer_hint);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "fundx: lotto_ticket_reference: %s\n",
	 mach_error_string(result));
  
  /* determine currency associated with self task */
  result = lotto_currency_by_task(pset, task_self(), &currency_id);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "fundx: lotto_currency_by_task: %s\n",
	 mach_error_string(result));
  
  /* add new ticket to currency */
  result = lotto_move_ticket_to_currency(pset, 
					 ticket_id,
					 issuer_hint,
					 currency_id);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "fundx: lotto_move_ticket_to_currency: %s\n",
	 mach_error_string(result));
  
  /* inform user */
  (void) printf("fundx: funded cid%u with tkt%u at time %lf\n",
		currency_id,
		ticket_id,
		lotto_time_now());
  
  /* query currency to obtain existing tickets */
  tickets_count = LOTTO_TICKET_INFO_MAX;
  result = lotto_currency_info(pset,
			       currency_id,
			       &wins,
			       name,
			       tickets,
			       &tickets_count);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "fundx: lotto_currency_info: %s\n",
	 mach_error_string(result));
  
  /* remove all old tickets from currency */
  for (i = 0; i < tickets_count; i++)
    if (tickets[i].flavor == LOTTO_TICKET_FLAVOR_OWNED)
      if (tickets[i].id != ticket_id)
	{
	  result = lotto_destroy_ticket(pset, 
					tickets[i].id,
					tickets[i].currency_id);
	  (void) printf("fundx: destroyed tkt%u\n", tickets[i].id);
	  if (result != KERN_SUCCESS)
	    quit(EXIT_FAILURE,
		 "fundx: lotto_destroy_ticket: %s\n",
		 mach_error_string(result));
	}

  /* flush output before exec */
  (void) fflush(stdout);

  /* setuid to original user */
  (void) setuid(getuid());

  /* execute specified program */
  execvp(program, args);
  
  /* barf if error */
  quit(EXIT_FAILURE,
       "fundx: unable to exec %s\n",
       program);
}
