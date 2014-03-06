/* 
 * Copyright (c) 1994 The University of Utah and
 * the Computer Systems Laboratory (CSL).  All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software is hereby
 * granted provided that (1) source code retains these copyright, permission,
 * and disclaimer notices, and (2) redistributions including binaries
 * reproduce the notices in supporting documentation, and (3) all advertising
 * materials mentioning features or use of this software display the following
 * acknowledgement: ``This product includes software developed by the
 * Computer Systems Laboratory at the University of Utah.''
 *
 * THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
 * IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
 * ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * CSL requests users of this software to return to csl-dist@cs.utah.edu any
 * improvements that they make and grant CSL redistribution rights.
 *
 *      Author: Bryan Ford, University of Utah CSL
 */
/* This is a server presentation file.  */

#define KERNEL_SERVER 1

#ifdef MIGRATING_THREADS
#define thread_assign		act_thread_assign
#define thread_assign_default	act_thread_assign_default
#define thread_get_assignment	act_thread_get_assignment
#define thread_priority		act_thread_priority
#define thread_max_priority	act_thread_max_priority
#define thread_policy		act_thread_policy
#define thread_depress_abort	act_thread_depress_abort
#define	thread_wire		act_thread_wire
#endif /* MIGRATING_THREADS */

#include <mach/mach_host.defs>
