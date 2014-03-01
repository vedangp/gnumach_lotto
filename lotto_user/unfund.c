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
 * Module:      unfund - unfund a lotto currency 
 * Author:	Carl Waldspurger
 * History:
 *
 *	07-Jun-94	Updated to support currency hints for ticket ids.
 *	05-May-94	First version started, based on fundx.c.
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
	 "unfund: lotto_pset_port: %s\n",
	 mach_error_string(result));

  /* unfund each specified <currency> */
  for (arg = 1; arg < argc; arg++)
    {
      lotto_ticket_info tickets[LOTTO_TICKET_INFO_MAX];
      unsigned int tickets_count, wins;
      lotto_currency_id_t currency_id;
      lotto_ticket_id_t ticket_id;
      lotto_currency_name_t name;
      int i;
      
      /* parse command-line argument <currency> */
      result = lotto_parse_currency(pset, argv[arg], &currency_id);
      if (result != KERN_SUCCESS)
	quit(EXIT_FAILURE,
	     "unfund: lotto_parse_currency: %s\n",
	     mach_error_string(result));
      
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
	     "unfund: lotto_currency_info: %s\n",
	     mach_error_string(result));
      
      /* remove all old tickets from currency */
      for (i = 0; i < tickets_count; i++)
	if (tickets[i].flavor == LOTTO_TICKET_FLAVOR_OWNED)
	  if (tickets[i].id != ticket_id)
	    {
	      result = lotto_destroy_ticket(pset, 
					    tickets[i].id,
					    tickets[i].currency_id);
	      (void) printf("unfund: destroying ticket %u\n", tickets[i].id);
	      if (result != KERN_SUCCESS)
		quit(EXIT_FAILURE,
		     "unfund: lotto_destroy_ticket: %s\n",
		     mach_error_string(result));
	    }
    }
  
  /* everything OK */
  exit(EXIT_SUCCESS);
}
