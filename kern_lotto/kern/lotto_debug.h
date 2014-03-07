/*
 * Project:	Mach Lottery Scheduler (Kernel Modifications)
 * Module:	Lotto Debugging Header File
 * Author:	Carl Waldspurger
 * History:
 *
 *	13-Jun-94	Defined MACH_LOTTO_CREDIT_WIN for conditional
 *			compilation of (relatively expensive) feature.
 *	29-Mar-94	Moved MACH_LOTTO_UNUSED to lotto.h.
 *	28-Jan-94	Defined MACH_LOTTO_UNUSED for commenting out code.
 *	24-Jan-94	Defined MACH_LOTTO_HACK for nasty temporary hacks.
 *	21-Jan-94	First version, based on lotto.h compilation flags.
 *
 */

#ifndef _LOTTO_DEBUG_H
#define _LOTTO_DEBUG_H

/*
 * compilation flags
 *
 */

#define MACH_LOTTO_DEBUG		(1)
#define	MACH_LOTTO_DEBUG_VERBOSE	(1)

#define MACH_LOTTO_CREDIT_WIN		(1)
#define	MACH_LOTTO_HACK			(0)


/*
 * operations
 *
 */

#if	MACH_LOTTO_DEBUG
extern void
  lotto_currency_print
  (lotto_currency_t currency);

extern void
  lotto_ticket_print
  (lotto_ticket_t ticket);
#endif	MACH_LOTTO_DEBUG

#endif _LOTTO_DEBUG_H

