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
 * Project:	Mach Lottery Scheduler (Kernel Modifications)
 * Module:	Lotto Type Definitions (for MIG-based IPC)
 * Author:	Carl Waldspurger
 * History:
 *	
 *	16-May-94	Expanded lotto_metrics_info definition.
 *	19-Apr-94	Defined lotto_quantum_metrics_t.
 *	12-Apr-94	Defined lotto_ipc_id_t.
 *	08-Apr-94	Expanded lotto_ipc_metrics_info definition.
 *	03-Apr-94	Modified lotto_ipc_metrics_info definition.
 *	01-Apr-94	Defined lotto_ipc_metrics_info.
 *	01-Feb-94	Added undepress_count field to lotto_metrics_info.
 * 	28-Jan-94	Added flavor field to lotto_ticket_info.
 *	18-Jan-94	Minor modifications.
 *	13-Jan-94	Defined lotto_currency_id_array_t.
 *	12-Jan-94	Modified info and metric types.
 *	09-Jan-94	Defined info types.
 *	04-Jan-94	Defined id null constants.
 *	20-Dec-93	Defined id types.
 *	29-Oct-93	First version started.
 *
 */

#ifndef _MACH_LOTTO_TYPES_H
#define _MACH_LOTTO_TYPES_H

/* id types */
typedef unsigned int lotto_id_t;
typedef lotto_id_t lotto_currency_id_t;
typedef lotto_id_t lotto_ticket_id_t;
typedef lotto_id_t lotto_ipc_id_t;

/* id array types */
#define LOTTO_CURRENCY_ID_ARRAY_MAX	(1024)
typedef lotto_currency_id_t *lotto_currency_id_array_t;

/* name types */
#define LOTTO_CURRENCY_NAME_MAX		(64)
typedef char lotto_currency_name_t[LOTTO_CURRENCY_NAME_MAX];

/* funds type */
typedef unsigned int lotto_funds_t;

/* metrics type */
typedef struct {
  unsigned select_count;
  unsigned attempt_count;
  unsigned search_count;
  unsigned idle_count;
  unsigned depress_count;
  unsigned undepress_count;
  unsigned min_value_count;
} lotto_metrics_info;
typedef lotto_metrics_info *lotto_metrics_info_t;

/* ipc metrics type */
typedef struct {
  /* basic operation counts */
  unsigned post_count;
  unsigned unpost_count;
  unsigned take_count;
  unsigned return_count;

  /* final state counts */
  unsigned unpost_after_post_count;
  unsigned unpost_after_take_count;
  unsigned unpost_after_return_count;
  unsigned unpost_after_other_count;

  /* search counts */
  unsigned search_count;
  unsigned search_fail_count;

  /* unusual condition counts */
  unsigned complex_request_count;
  unsigned complex_reply_count;
  unsigned post_after_post_count;
  unsigned take_after_post_count;
  unsigned take_after_take_count;
} lotto_ipc_metrics_info;
typedef lotto_ipc_metrics_info *lotto_ipc_metrics_t;

/* quantum metrics type */
#define LOTTO_QUANTUM_METRICS_MAX	(12)
#define LOTTO_QUANTUM_USED_MAX		(11)
typedef unsigned lotto_quantum_metrics_t[LOTTO_QUANTUM_METRICS_MAX];

/* ticket info types */
typedef unsigned int lotto_ticket_flavor_t;
#define LOTTO_TICKET_FLAVOR_OWNED	(0)
#define LOTTO_TICKET_FLAVOR_ISSUED	(1)

typedef struct {
  lotto_ticket_id_t id;
  lotto_ticket_flavor_t flavor;
  lotto_currency_id_t currency_id;
  lotto_funds_t amount;
} lotto_ticket_info;
typedef lotto_ticket_info *lotto_ticket_info_t;

#define LOTTO_TICKET_INFO_MAX		(1024)
typedef lotto_ticket_info *lotto_ticket_info_array;

/* id null constants */
#define LOTTO_CURRENCY_ID_NULL		(0)
#define LOTTO_TICKET_ID_NULL		(0)

#endif _MACH_LOTTO_TYPES_H
