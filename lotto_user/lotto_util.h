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
 * Module:      util - common utility operations header file
 * Author:	Carl Waldspurger
 * History:
 *
 *	15-Jun-94	Defined lotto_time_now().
 *	14-Jun-94	Defined lotto_set_funding().
 *	07-Jun-94	Added issuer_hint to lotto_ticket_reference().
 *	03-Jun-94	Added machid inclusions.
 *	05-May-94	Added need_privileged argument to lotto_pset_port().
 *	09-Jan-94	First version written.
 *
 */

#ifndef _LOTTO_UTIL_H
#define _LOTTO_UTIL_H

/*
 * includes
 *
 */

#include <stdlib.h>
#include <mach.h>
#include <mach_error.h>

#include <mach/mach_host.h>
#include <mach/mach_traps.h>

#include <servers/netname.h>
#include <servers/machid.h>
#include <servers/machid_types.h>
#include <servers/machid_lib.h>

#include <mach/mach_lotto_types.h>

/*
 * constants
 *
 */

#define	MAX_BUFFER_LENGTH	(256)

/*
 * types
 *
 */

typedef char buffer[MAX_BUFFER_LENGTH];

/*
 * utility operations
 *
 */

extern kern_return_t
  lotto_pset_port
  (mach_port_t *pset_port,
   boolean_t need_privileged);

extern kern_return_t
  lotto_parse_currency
  (processor_set_t pset,
   const char *currency_string,
   lotto_currency_id_t *currency_id);

extern kern_return_t
  lotto_parse_ticket
  (processor_set_t pset,
   const char *ticket_string,
   lotto_currency_id_t *currency_id,
   lotto_funds_t *amount);

extern kern_return_t
  lotto_ticket_reference
  (processor_set_t pset,
   const char *ticket_string,
   lotto_ticket_id_t *ticket_id,
   lotto_currency_id_t *issuer_hint);

extern kern_return_t
  lotto_set_funding
  (processor_set_t pset,
   lotto_currency_id_t currency_id,
   lotto_ticket_id_t ticket_id,
   lotto_currency_id_t issuer_hint);

extern double
  lotto_time_now
  (void);

#endif	_LOTTO_UTIL_H
