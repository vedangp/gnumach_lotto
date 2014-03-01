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
 * Module:      fund - fund a lotto currency
 * Author:	Carl Waldspurger
 * History:
 *
 *	15-Jun-94	Added current time to printed information.
 *	07-Jun-94	Updated to support currency hints for ticket ids.
 *	05-May-94	Updated to support lotto_pset_port() interface change.
 *	31-Jan-94	Updated to handle multiple ticket arguments.
 *			Updated to parse general ticket references.
 *	11-Jan-94	First version started.
 *
 */

/*
 * includes
 *
 */

#include "lotto_util.h"

/*
 * main program
 *
 */

int main
  (int argc,
   char **argv)
{
  lotto_currency_id_t currency_id, issuer_hint;
  lotto_ticket_id_t ticket_id;
  kern_return_t result;
  mach_port_t pset;
  int arg;

  /* fail if bad usage */
  if (argc < 3)
    quit(EXIT_FAILURE, 
	 "usage: %s <currency> <ticket>...\n",
	 argv[0]);
  
  /* obtain pset port */
  result = lotto_pset_port(&pset, FALSE);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "fund: lotto_pset_port: %s\n",
	 mach_error_string(result));

  /* parse command-line argument <currency> */
  result = lotto_parse_currency(pset, argv[1], &currency_id);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "fund: lotto_parse_currency: %s\n",
	 mach_error_string(result));
  
  /* fund currency with each specified ticket */
  for (arg = 2; arg < argc; arg++)
    {
      /* parse command-line argument <ticket> */  
      result = lotto_ticket_reference(pset,
				      argv[arg],
				      &ticket_id,
				      &issuer_hint);
      if (result != KERN_SUCCESS)
	quit(EXIT_FAILURE,
	     "fund: lotto_ticket_reference: %s\n",
	     mach_error_string(result));
  
      /* associate ticket with currency */
      result = lotto_move_ticket_to_currency(pset,
					     ticket_id,
					     issuer_hint,
					     currency_id);
      if (result != KERN_SUCCESS)
	quit(EXIT_FAILURE,
	     "fund: lotto_move_ticket_to_currency: %s\n",
	     mach_error_string(result));
      
      /* inform user */
      (void) printf("fund: funded cid%u with tkt%u at time %lf\n",
		    currency_id,
		    ticket_id,
		    lotto_time_now());
    }
  
  /* everything OK */
  exit(EXIT_SUCCESS);
}
