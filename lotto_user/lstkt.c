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
 * Module:      lstkt - list tickets for a lotto currency
 * Author:	Carl Waldspurger
 * History:
 *
 *	05-May-94	Updated to support lotto_pset_port() interface change.
 *	01-Feb-94	Added support for currency wins.
 *	31-Jan-94	Minor improvements.
 *	28-Jan-94	Updated to reflect changes to lotto_currency_info().
 *	19-Jan-94	Updated to handle multiple currency arguments.
 *	18-Jan-94	First version written, based on rmcur.c.
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
	 "lstkt: lotto_pset_port: %s\n",
	 mach_error_string(result));

  /* list tickets for each specified currency */
  for (arg = 1; arg < argc; arg++)
    {
      lotto_ticket_info tickets[LOTTO_TICKET_INFO_MAX];
      unsigned int tickets_count;
      lotto_currency_name_t name;
      lotto_currency_id_t id;
      unsigned int wins;
      int i;

      /* parse command-line argument */
      result = lotto_parse_currency(pset, argv[arg], &id);
      if (result != KERN_SUCCESS)
	quit(EXIT_FAILURE,
	     "lstkt: lotto_parse_currency: %s\n",
	     mach_error_string(result));
      
      /* initialize */
      tickets_count = LOTTO_TICKET_INFO_MAX;

      /* list currency info */
      result = lotto_currency_info(pset,
				   id,
				   &wins,
				   name,
				   tickets,
				   &tickets_count);
      if (result != KERN_SUCCESS)
	quit(EXIT_FAILURE,
	     "lstkt: lotto_currency_info: %s\n",
	     mach_error_string(result));
      
      /* list currency id, name */
      (void) printf("currency #%4u (%s): %8u wins\n",
		    id,
		    name,
		    wins);
      
      /* list tickets owned by currency */
      for (i = 0; i < tickets_count; i++)
	if (tickets[i].flavor == LOTTO_TICKET_FLAVOR_OWNED)
	  (void) printf("   o tkt%04u %4u.cid%u\n", 
			tickets[i].id,
			tickets[i].amount,
			tickets[i].currency_id);
      
      /* list tickets issued by currency */
      for (i = 0; i < tickets_count; i++)
	if (tickets[i].flavor == LOTTO_TICKET_FLAVOR_ISSUED)
	  (void) printf("   i tkt%04u %4u.cid%u\n", 
			tickets[i].id,
			tickets[i].amount,
			tickets[i].currency_id);
      
      /* separator */
      (void) printf("\n");
    }
  
  /* everything OK */
  exit(EXIT_SUCCESS);
}
