/* Module mach_lotto */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#define EXPORT_BOOLEAN
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/message.h>
#include <mach/mig_errors.h>
#include <mach/mig_support.h>
#include <ipc/ipc_port.h>

#ifndef	mig_internal
#define	mig_internal	static
#endif

#ifndef	mig_external
#define mig_external
#endif

#ifndef	TypeCheck
#define	TypeCheck 1
#endif

#ifndef	UseExternRCSId
#define	UseExternRCSId		1
#endif

#define BAD_TYPECHECK(type, check) ({\
  union { mach_msg_type_t t; unsigned32_t w; } _t, _c;\
  _t.t = *(type); _c.t = *(check); _t.w != _c.w; })
#define msgh_request_port	msgh_remote_port
#define MACH_MSGH_BITS_REQUEST(bits)	MACH_MSGH_BITS_REMOTE(bits)
#define msgh_reply_port		msgh_local_port
#define MACH_MSGH_BITS_REPLY(bits)	MACH_MSGH_BITS_LOCAL(bits)

#include <mach/mach_lotto_types.h>
#include <mach/std_types.h>
#include <kern/ipc_kobject.h>
#include <kern/ipc_tt.h>
#include <kern/ipc_host.h>
#include <kern/task.h>
#include <kern/thread.h>
#include <kern/host.h>
#include <kern/processor.h>
#include <vm/vm_object.h>
#include <vm/vm_map.h>
#include <ipc/ipc_space.h>
#include <mach/mach_types.h>

/* Routine lotto_create_ticket */
mig_internal void _Xlotto_create_ticket
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
		mach_msg_type_t amountType;
		lotto_funds_t amount;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t ticket_idType;
		lotto_ticket_id_t ticket_id;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_create_ticket
		(processor_set_t pset, lotto_currency_id_t currency_id, lotto_funds_t amount, lotto_ticket_id_t *ticket_id);

	const mach_msg_type_t currency_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t amountCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t ticket_idType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->currency_idType, &currency_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->amountType, &amountCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_create_ticket(pset, In0P->currency_id, In0P->amount, &OutP->ticket_id);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->ticket_idType = ticket_idType;
}

/* Routine lotto_create_currency */
mig_internal void _Xlotto_create_currency
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_long_t nameType;
		lotto_currency_name_t name;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_create_currency
		(processor_set_t pset, lotto_currency_name_t name, lotto_currency_id_t *currency_id);

	const mach_msg_type_t currency_idType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 100) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->nameType.msgtl_header.msgt_inline != TRUE) ||
	    (In0P->nameType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->nameType.msgtl_name != 12) ||
	    (In0P->nameType.msgtl_number != 1) ||
	    (In0P->nameType.msgtl_size != 512))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_create_currency(pset, In0P->name, &OutP->currency_id);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->currency_idType = currency_idType;
}

/* Routine lotto_destroy_ticket */
mig_internal void _Xlotto_destroy_ticket
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t ticket_idType;
		lotto_ticket_id_t ticket_id;
		mach_msg_type_t issuer_hintType;
		lotto_currency_id_t issuer_hint;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_destroy_ticket
		(processor_set_t pset, lotto_ticket_id_t ticket_id, lotto_currency_id_t issuer_hint);

	const mach_msg_type_t ticket_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t issuer_hintCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->ticket_idType, &ticket_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->issuer_hintType, &issuer_hintCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_destroy_ticket(pset, In0P->ticket_id, In0P->issuer_hint);
	pset_deallocate(pset);
}

/* Routine lotto_destroy_currency */
mig_internal void _Xlotto_destroy_currency
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
		mach_msg_type_t destroy_ticketsType;
		boolean_t destroy_tickets;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_destroy_currency
		(processor_set_t pset, lotto_currency_id_t currency_id, boolean_t destroy_tickets);

	const mach_msg_type_t currency_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t destroy_ticketsCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->currency_idType, &currency_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->destroy_ticketsType, &destroy_ticketsCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_destroy_currency(pset, In0P->currency_id, In0P->destroy_tickets);
	pset_deallocate(pset);
}

/* Routine lotto_move_ticket_to_currency */
mig_internal void _Xlotto_move_ticket_to_currency
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t ticket_idType;
		lotto_ticket_id_t ticket_id;
		mach_msg_type_t issuer_hintType;
		lotto_currency_id_t issuer_hint;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_move_ticket_to_currency
		(processor_set_t pset, lotto_ticket_id_t ticket_id, lotto_currency_id_t issuer_hint, lotto_currency_id_t currency_id);

	const mach_msg_type_t ticket_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t issuer_hintCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t currency_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->ticket_idType, &ticket_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->issuer_hintType, &issuer_hintCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->currency_idType, &currency_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_move_ticket_to_currency(pset, In0P->ticket_id, In0P->issuer_hint, In0P->currency_id);
	pset_deallocate(pset);
}

/* Routine lotto_remove_ticket_from_currency */
mig_internal void _Xlotto_remove_ticket_from_currency
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t ticket_idType;
		lotto_ticket_id_t ticket_id;
		mach_msg_type_t issuer_hintType;
		lotto_currency_id_t issuer_hint;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_remove_ticket_from_currency
		(processor_set_t pset, lotto_ticket_id_t ticket_id, lotto_currency_id_t issuer_hint, lotto_currency_id_t currency_id);

	const mach_msg_type_t ticket_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t issuer_hintCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t currency_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->ticket_idType, &ticket_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->issuer_hintType, &issuer_hintCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->currency_idType, &currency_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_remove_ticket_from_currency(pset, In0P->ticket_id, In0P->issuer_hint, In0P->currency_id);
	pset_deallocate(pset);
}

/* Routine lotto_currency_by_name */
mig_internal void _Xlotto_currency_by_name
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_long_t nameType;
		lotto_currency_name_t name;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_currency_by_name
		(processor_set_t pset, lotto_currency_name_t name, lotto_currency_id_t *currency_id);

	const mach_msg_type_t currency_idType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 100) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->nameType.msgtl_header.msgt_inline != TRUE) ||
	    (In0P->nameType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->nameType.msgtl_name != 12) ||
	    (In0P->nameType.msgtl_number != 1) ||
	    (In0P->nameType.msgtl_size != 512))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_currency_by_name(pset, In0P->name, &OutP->currency_id);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->currency_idType = currency_idType;
}

/* Routine lotto_currency_by_task */
mig_internal void _Xlotto_currency_by_task
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t taskType;
		ipc_port_t task;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_currency_by_task
		(processor_set_t pset, task_t task, lotto_currency_id_t *currency_id);

	const mach_msg_type_t currency_idType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;
	task_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->taskType.msgt_inline != TRUE) ||
	    (In0P->taskType.msgt_longform != FALSE) ||
	    (In0P->taskType.msgt_name != 17) ||
	    (In0P->taskType.msgt_number != 1) ||
	    (In0P->taskType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	task = convert_port_to_task(In0P->task);

	OutP->RetCode = lotto_currency_by_task(pset, task, &OutP->currency_id);
	task_deallocate(task);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	if (IP_VALID(In0P->task))
		ipc_port_release_send(In0P->task);

	OutP->Head.msgh_size = 40;

	OutP->currency_idType = currency_idType;
}

/* Routine lotto_currency_by_thread */
mig_internal void _Xlotto_currency_by_thread
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t taskType;
		ipc_port_t task;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_currency_by_thread
		(processor_set_t pset, thread_t task, lotto_currency_id_t *currency_id);

	const mach_msg_type_t currency_idType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;
	thread_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->taskType.msgt_inline != TRUE) ||
	    (In0P->taskType.msgt_longform != FALSE) ||
	    (In0P->taskType.msgt_name != 17) ||
	    (In0P->taskType.msgt_number != 1) ||
	    (In0P->taskType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	task = convert_port_to_thread(In0P->task);

	OutP->RetCode = lotto_currency_by_thread(pset, task, &OutP->currency_id);
	thread_deallocate(task);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	if (IP_VALID(In0P->task))
		ipc_port_release_send(In0P->task);

	OutP->Head.msgh_size = 40;

	OutP->currency_idType = currency_idType;
}

/* Routine lotto_metrics */
mig_internal void _Xlotto_metrics
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t metricsType;
		lotto_metrics_info metrics;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_metrics
		(processor_set_t pset, lotto_metrics_info *metrics);

	const mach_msg_type_t metricsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		7,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_metrics(pset, &OutP->metrics);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 64;

	OutP->metricsType = metricsType;
}

/* Routine lotto_currency_info */
mig_internal void _Xlotto_currency_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
		mach_msg_type_t ticketsCntType;
		mach_msg_type_number_t ticketsCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t winsType;
		unsigned wins;
		mach_msg_type_long_t nameType;
		lotto_currency_name_t name;
		mach_msg_type_long_t ticketsType;
		lotto_ticket_info tickets[1024];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_currency_info
		(processor_set_t pset, lotto_currency_id_t currency_id, unsigned *wins, lotto_currency_name_t name, lotto_ticket_info_array tickets, mach_msg_type_number_t *ticketsCnt);

	const mach_msg_type_t currency_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t ticketsCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t winsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t nameType = {
	{
		/* msgt_name = */		0,
		/* msgt_size = */		0,
		/* msgt_number = */		0,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		TRUE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	},
		/* msgtl_name = */	12,
		/* msgtl_size = */	512,
		/* msgtl_number = */	1,
	};

	const mach_msg_type_long_t ticketsType = {
	{
		/* msgt_name = */		0,
		/* msgt_size = */		0,
		/* msgt_number = */		0,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		TRUE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	},
		/* msgtl_name = */	2,
		/* msgtl_size = */	32,
		/* msgtl_number = */	4096,
	};

	processor_set_t pset;
	mach_msg_type_number_t ticketsCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->currency_idType, &currency_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->ticketsCntType, &ticketsCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	ticketsCnt = 1024;
	if (In0P->ticketsCnt < ticketsCnt)
		ticketsCnt = In0P->ticketsCnt;

	OutP->RetCode = lotto_currency_info(pset, In0P->currency_id, &OutP->wins, OutP->name, OutP->tickets, &ticketsCnt);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->winsType = winsType;

	OutP->nameType = nameType;

	OutP->ticketsType = ticketsType;

	OutP->ticketsType.msgtl_number = 4 * ticketsCnt;
	OutP->Head.msgh_size = 128 + (16 * ticketsCnt);
}

/* Routine lotto_currency_ids */
mig_internal void _Xlotto_currency_ids
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t currency_idsCntType;
		mach_msg_type_number_t currency_idsCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t currency_idsType;
		lotto_currency_id_t currency_ids[1024];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_currency_ids
		(processor_set_t pset, lotto_currency_id_array_t currency_ids, mach_msg_type_number_t *currency_idsCnt);

	const mach_msg_type_t currency_idsCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t currency_idsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1024,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;
	mach_msg_type_number_t currency_idsCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->currency_idsCntType, &currency_idsCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	currency_idsCnt = 1024;
	if (In0P->currency_idsCnt < currency_idsCnt)
		currency_idsCnt = In0P->currency_idsCnt;

	OutP->RetCode = lotto_currency_ids(pset, OutP->currency_ids, &currency_idsCnt);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->currency_idsType = currency_idsType;

	OutP->currency_idsType.msgt_number = currency_idsCnt;
	OutP->Head.msgh_size = 36 + (4 * currency_idsCnt);
}

/* Routine lotto_compute_ticket_value */
mig_internal void _Xlotto_compute_ticket_value
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t ticket_idType;
		lotto_ticket_id_t ticket_id;
		mach_msg_type_t issuer_hintType;
		lotto_currency_id_t issuer_hint;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t valueType;
		lotto_funds_t value;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_compute_ticket_value
		(processor_set_t pset, lotto_ticket_id_t ticket_id, lotto_currency_id_t issuer_hint, lotto_funds_t *value);

	const mach_msg_type_t ticket_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t issuer_hintCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t valueType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->ticket_idType, &ticket_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->issuer_hintType, &issuer_hintCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_compute_ticket_value(pset, In0P->ticket_id, In0P->issuer_hint, &OutP->value);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->valueType = valueType;
}

/* Routine lotto_compute_currency_value */
mig_internal void _Xlotto_compute_currency_value
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t currency_idType;
		lotto_currency_id_t currency_id;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t valueType;
		lotto_funds_t value;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_compute_currency_value
		(processor_set_t pset, lotto_currency_id_t currency_id, lotto_funds_t *value);

	const mach_msg_type_t currency_idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t valueType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->currency_idType, &currency_idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_compute_currency_value(pset, In0P->currency_id, &OutP->value);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->valueType = valueType;
}

/* Routine lotto_ipc_metrics */
mig_internal void _Xlotto_ipc_metrics
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t metricsType;
		lotto_ipc_metrics_info metrics;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_ipc_metrics
		(processor_set_t pset, lotto_ipc_metrics_info *metrics);

	const mach_msg_type_t metricsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		15,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_ipc_metrics(pset, &OutP->metrics);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 96;

	OutP->metricsType = metricsType;
}

/* Routine lotto_quantum_metrics */
mig_internal void _Xlotto_quantum_metrics
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t metricsType;
		lotto_quantum_metrics_t metrics;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_quantum_metrics
		(processor_set_t pset, lotto_quantum_metrics_t metrics);

	const mach_msg_type_t metricsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		12,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_quantum_metrics(pset, OutP->metrics);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 84;

	OutP->metricsType = metricsType;
}

/* Routine lotto_ipc_enable */
mig_internal void _Xlotto_ipc_enable
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t enableType;
		boolean_t enable;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_ipc_enable
		(processor_set_t pset, boolean_t enable);

	const mach_msg_type_t enableCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->enableType, &enableCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_ipc_enable(pset, In0P->enable);
	pset_deallocate(pset);
}

/* Routine lotto_quantum_enable */
mig_internal void _Xlotto_quantum_enable
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t enableType;
		boolean_t enable;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t lotto_quantum_enable
		(processor_set_t pset, boolean_t enable);

	const mach_msg_type_t enableCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	processor_set_t pset;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->enableType, &enableCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = lotto_quantum_enable(pset, In0P->enable);
	pset_deallocate(pset);
}

mig_routine_t mach_lotto_server_routines[] = {
		_Xlotto_create_ticket,
		_Xlotto_create_currency,
		_Xlotto_destroy_ticket,
		_Xlotto_destroy_currency,
		_Xlotto_move_ticket_to_currency,
		_Xlotto_remove_ticket_from_currency,
		_Xlotto_currency_by_name,
		_Xlotto_currency_by_task,
		_Xlotto_currency_by_thread,
		_Xlotto_metrics,
		_Xlotto_currency_info,
		_Xlotto_currency_ids,
		_Xlotto_compute_ticket_value,
		_Xlotto_compute_currency_value,
		_Xlotto_ipc_metrics,
		_Xlotto_quantum_metrics,
		_Xlotto_ipc_enable,
		_Xlotto_quantum_enable,
			0,
			0,
			0,
			0,
};

mig_external boolean_t mach_lotto_server
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	mach_msg_header_t *InP =  InHeadP;
	mig_reply_header_t *OutP = (mig_reply_header_t *) OutHeadP;

	const mach_msg_type_t RetCodeType = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	mig_routine_t routine;

	OutP->Head.msgh_bits = MACH_MSGH_BITS(MACH_MSGH_BITS_REPLY(InP->msgh_bits), 0);
	OutP->Head.msgh_size = sizeof *OutP;
	OutP->Head.msgh_remote_port = InP->msgh_reply_port;
	OutP->Head.msgh_local_port = MACH_PORT_NULL;
	OutP->Head.msgh_seqno = 0;
	OutP->Head.msgh_id = InP->msgh_id + 100;

	OutP->RetCodeType = RetCodeType;

	if ((InP->msgh_id > 711021) || (InP->msgh_id < 711000) ||
	    ((routine = mach_lotto_server_routines[InP->msgh_id - 711000]) == 0)) {
		OutP->RetCode = MIG_BAD_ID;
		return FALSE;
	}
	(*routine) (InP, &OutP->Head);
	return TRUE;
}

mig_external mig_routine_t mach_lotto_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 711000;

	if ((msgh_id > 21) || (msgh_id < 0))
		return 0;

	return mach_lotto_server_routines[msgh_id];
}

