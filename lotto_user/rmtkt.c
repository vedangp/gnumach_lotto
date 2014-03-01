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
 * Module:      rmtkt - remove a lotto ticket
 * Author:	Carl Waldspurger
 * History:
 *
 *	07-Jun-94	Updated to support currency hints for ticket ids.
 *	05-May-94	Updated to support lotto_pset_port() interface change.
 *			Updated to support multiple command-line arguments.
 *	09-Jan-94	Updated to use lotto_util operations.
 *	16-Dec-93	First version written, based on rmcur.c.
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
  kern_return_t result;
  mach_port_t pset;
  int arg;

  /* fail if bad usage */
  if (argc < 2)
    quit(EXIT_FAILURE, 
	 "usage: %s <id>...\n",
	 argv[0]);

  /* obtain pset port */
  result = lotto_pset_port(&pset, FALSE);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "rmtkt: lotto_pset_port: %s\n",
	 mach_error_string(result));

  /* remove each specified <id> */
  for (arg = 1; arg < argc; arg++)
    {
      lotto_ticket_id_t id;

      /* parse command-line arguments */
      id = atoi(argv[arg]);
      
      /* destroy ticket */
      result = lotto_destroy_ticket(pset, id, LOTTO_CURRENCY_ID_NULL);
      if (result != KERN_SUCCESS)
	quit(EXIT_FAILURE,
	     "rmtkt: lotto_destroy_ticket: %s\n",
	     mach_error_string(result));
      
      /* inform user */
      (void) printf("rmtkt: removed ticket %u\n", id);
    }

  /* everything OK */
  exit(EXIT_SUCCESS);
}
