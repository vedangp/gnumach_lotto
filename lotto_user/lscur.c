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
 * Module:      lscur - list all lotto currencies
 * Author:	Carl Waldspurger
 * History:
 *
 *	05-May-94	Updated to support lotto_pset_port() interface change.
 *	19-Jan-94	First version written, based on lstkt.c.
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
  lotto_currency_id_array_t ids[LOTTO_CURRENCY_ID_ARRAY_MAX];
  kern_return_t result;
  unsigned int count;
  mach_port_t pset;
  int i;

  /* fail if bad usage */
  if (argc != 1)
    quit(EXIT_FAILURE, 
	 "usage: %s\n",
	 argv[0]);

  /* obtain pset port */
  result = lotto_pset_port(&pset, FALSE);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "lscur: lotto_pset_port: %s\n",
	 mach_error_string(result));

  /* initialize */
  count = LOTTO_CURRENCY_ID_ARRAY_MAX;

  /* get currency ids */
  result = lotto_currency_ids(pset, ids, &count);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "lscur: lotto_currency_ids: %s\n",
	 mach_error_string(result));
  
  /* list currency ids */
  for (i = 0; i < count; i++)
    (void) printf("cid%u\n", ids[i]);
  
  /* everything OK */
  exit(EXIT_SUCCESS);
}
