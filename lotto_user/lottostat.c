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
 * Module:      lottostat - lotto scheduler metrics
 * Author:	Carl Waldspurger
 * History:
 *
 *	16-May-94	Updated to support expanded metrics.
 *	05-May-94	Updated to support lotto_pset_port() interface change.
 *	20-Apr-94	Updated to support quantum metrics.
 *	08-Apr-94	Updated to reflect expanded ipc metrics.
 *	04-Apr-94	Added support for ipc metrics.
 *	01-Feb-94	Added support for undepress count.
 *	13-Jan-94	Minor modifications.
 *	12-Jan-94	First version started.
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
  lotto_quantum_metrics_t quantum_metrics;
  lotto_ipc_metrics_info ipc_metrics;
  lotto_metrics_info metrics;
  kern_return_t result;
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
	 "lottostat: lotto_pset_port: %s\n",
	 mach_error_string(result));

  /* query metrics */
  result = lotto_metrics(pset, &metrics);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "lottostat: lotto_metrics: %s\n",
	 mach_error_string(result));
  
  /* inform user */
  (void) printf("select    = %8u\n"
		"attempt   = %8u\n"
		"idle      = %8u\n"
		"search    = %8u\n"
		"depress   = %8u\n"
		"undepress = %8u\n"
		"min value = %8u\n",
		metrics.select_count,
		metrics.attempt_count,
		metrics.idle_count,
		metrics.search_count,
		metrics.depress_count,
		metrics.undepress_count,
		metrics.min_value_count);
  
  /* query ipc metrics */
  result = lotto_ipc_metrics(pset, &ipc_metrics);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "lottostat: lotto_ipc_metrics: %s\n",
	 mach_error_string(result));

  /* inform user */
  (void) printf("\n"
		"post      = %8u\n"
		"unpost    = %8u\n"
		"take      = %8u\n"
		"return    = %8u\n"
		"ua_post   = %8u\n"
		"ua_take   = %8u\n"
		"ua_retn   = %8u\n"
		"ua_other  = %8u\n"
		"search    = %8u\n"
		"sfail     = %8u\n"
		"cx_rq     = %8u\n"
		"cx_reply  = %8u\n"
		"pap       = %8u\n"
		"tap       = %8u\n"
		"tat       = %8u\n",
		ipc_metrics.post_count,
		ipc_metrics.unpost_count,
		ipc_metrics.take_count,
		ipc_metrics.return_count,
		ipc_metrics.unpost_after_post_count,
		ipc_metrics.unpost_after_take_count,
		ipc_metrics.unpost_after_return_count,
		ipc_metrics.unpost_after_other_count,
		ipc_metrics.search_count,
		ipc_metrics.search_fail_count,
		ipc_metrics.complex_request_count,
		ipc_metrics.complex_reply_count,
		ipc_metrics.post_after_post_count,
		ipc_metrics.take_after_post_count,
		ipc_metrics.take_after_take_count);

  /* query quantum metrics */
  result = lotto_quantum_metrics(pset, quantum_metrics);
  if (result != KERN_SUCCESS)
    quit(EXIT_FAILURE,
	 "lottostat: lotto_quantum_metrics: %s\n",
	 mach_error_string(result));

  /* inform user */
  (void) printf("\n");
  for (i = 0; i < LOTTO_QUANTUM_METRICS_MAX; i++)
    (void) printf("quantum[%2d] = %8u\n", 
		  i,
		  quantum_metrics[i]);

  /* everything OK */
  exit(EXIT_SUCCESS);
}
  
