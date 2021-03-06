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
 * Module:      rmcur - remove a lotto currency
 * Author:	Carl Waldspurger
 * History:
 *
 *	05-May-94	Updated to support lotto_pset_port() interface change.
 *			Updated to support multiple command-line arguments.
 *	09-Jan-94	Updated to use lotto_util operations.
 *	04-Jan-94	Minor modifications to reflect interface changes.
 *	16-Dec-93	Minor modifications.
 *	13-Dec-93	First version written, based on mkcur.c.
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
	 "usage: %s <currency>...\n",
	 argv[0]);

  /* obtain pset port */
  result = lotto_pset_port(&pset, FALSE);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "rmcur: lotto_pset_port: %s\n",
	 mach_error_string(result));

  /* remove each specified <currency> */
  for (arg = 1; arg < argc; arg++)
    {
      lotto_currency_id_t id;

      /* parse command-line arguments */
      result = lotto_parse_currency(pset, argv[arg], &id);
      if (result != KERN_SUCCESS)
	quit(EXIT_FAILURE,
	     "rmcur: lotto_parse_currency: %s\n",
	     mach_error_string(result));
      
      /* destroy currency */
      result = lotto_destroy_currency(pset, id, TRUE);
      if (result != KERN_SUCCESS)
	quit(EXIT_FAILURE,
	     "rmcur: lotto_destroy_currency: %s\n",
	     mach_error_string(result));
      
      /* inform user */
      (void) printf("rmcur: removed currency cid%u\n", id);
    }
  
  /* everything OK */
  exit(EXIT_SUCCESS);
}
