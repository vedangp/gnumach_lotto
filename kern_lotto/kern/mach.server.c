/* Module mach */

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

/* Routine task_create */
mig_internal void _Xtask_create
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t inherit_memoryType;
		boolean_t inherit_memory;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t child_taskType;
		ipc_port_t child_task;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_create
		(task_t target_task, boolean_t inherit_memory, task_t *child_task);

	const mach_msg_type_t inherit_memoryCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t child_taskType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t target_task;
	task_t child_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->inherit_memoryType, &inherit_memoryCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_create(target_task, In0P->inherit_memory, &child_task);
	task_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 40;

	OutP->child_taskType = child_taskType;

	OutP->child_task = convert_task_to_port(child_task);
}

/* Routine task_terminate */
mig_internal void _Xtask_terminate
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_terminate
		(task_t target_task);

	task_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_terminate(target_task);
	task_deallocate(target_task);
}

/* Routine task_get_emulation_vector */
mig_internal void _Xtask_get_emulation_vector
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t vector_startType;
		int vector_start;
		mach_msg_type_long_t emulation_vectorType;
		emulation_vector_t emulation_vector;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_get_emulation_vector
		(task_t task, int *vector_start, emulation_vector_t *emulation_vector, mach_msg_type_number_t *emulation_vectorCnt);

	const mach_msg_type_t vector_startType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t emulation_vectorType = {
	{
		/* msgt_name = */		0,
		/* msgt_size = */		0,
		/* msgt_number = */		0,
		/* msgt_inline = */		FALSE,
		/* msgt_longform = */		TRUE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	},
		/* msgtl_name = */	2,
		/* msgtl_size = */	32,
		/* msgtl_number = */	0,
	};

	task_t task;
	mach_msg_type_number_t emulation_vectorCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_get_emulation_vector(task, &OutP->vector_start, &OutP->emulation_vector, &emulation_vectorCnt);
	task_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 56;

	OutP->vector_startType = vector_startType;

	OutP->emulation_vectorType = emulation_vectorType;

	OutP->emulation_vectorType.msgtl_number = emulation_vectorCnt;
}

/* Routine task_set_emulation_vector */
mig_internal void _Xtask_set_emulation_vector
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t vector_startType;
		int vector_start;
		mach_msg_type_long_t emulation_vectorType;
		emulation_vector_t emulation_vector;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_set_emulation_vector
		(task_t task, int vector_start, emulation_vector_t emulation_vector, mach_msg_type_number_t emulation_vectorCnt);

	const mach_msg_type_t vector_startCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->vector_startType, &vector_startCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->emulation_vectorType.msgtl_header.msgt_inline != FALSE) ||
	    (In0P->emulation_vectorType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->emulation_vectorType.msgtl_name != 2) ||
	    (In0P->emulation_vectorType.msgtl_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_set_emulation_vector(task, In0P->vector_start, In0P->emulation_vector, In0P->emulation_vectorType.msgtl_number);
	task_deallocate(task);
}

/* Routine task_threads */
mig_internal void _Xtask_threads
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_long_t thread_listType;
		thread_array_t thread_list;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_threads
		(task_t target_task, thread_array_t *thread_list, mach_msg_type_number_t *thread_listCnt);

	const mach_msg_type_long_t thread_listType = {
	{
		/* msgt_name = */		0,
		/* msgt_size = */		0,
		/* msgt_number = */		0,
		/* msgt_inline = */		FALSE,
		/* msgt_longform = */		TRUE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	},
		/* msgtl_name = */	17,
		/* msgtl_size = */	32,
		/* msgtl_number = */	0,
	};

	task_t target_task;
	mach_msg_type_number_t thread_listCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_threads(target_task, &OutP->thread_list, &thread_listCnt);
	task_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 48;

	OutP->thread_listType = thread_listType;

	OutP->thread_listType.msgtl_number = thread_listCnt;
}

/* Routine task_info */
mig_internal void _Xtask_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t flavorType;
		int flavor;
		mach_msg_type_t task_info_outCntType;
		mach_msg_type_number_t task_info_outCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t task_info_outType;
		integer_t task_info_out[1024];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_info
		(task_t target_task, int flavor, task_info_t task_info_out, mach_msg_type_number_t *task_info_outCnt);

	const mach_msg_type_t flavorCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t task_info_outCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t task_info_outType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1024,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t target_task;
	mach_msg_type_number_t task_info_outCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->flavorType, &flavorCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->task_info_outCntType, &task_info_outCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	task_info_outCnt = 1024;
	if (In0P->task_info_outCnt < task_info_outCnt)
		task_info_outCnt = In0P->task_info_outCnt;

	OutP->RetCode = task_info(target_task, In0P->flavor, OutP->task_info_out, &task_info_outCnt);
	task_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->task_info_outType = task_info_outType;

	OutP->task_info_outType.msgt_number = task_info_outCnt;
	OutP->Head.msgh_size = 36 + (4 * task_info_outCnt);
}

/* Routine thread_terminate */
mig_internal void _Xthread_terminate
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_terminate
		(thread_t target_thread);

	thread_t target_thread;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_terminate(target_thread);
	thread_deallocate(target_thread);
}

/* Routine thread_get_state */
mig_internal void _Xthread_get_state
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t flavorType;
		int flavor;
		mach_msg_type_t old_stateCntType;
		mach_msg_type_number_t old_stateCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t old_stateType;
		natural_t old_state[1024];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_get_state
		(thread_t target_thread, int flavor, thread_state_t old_state, mach_msg_type_number_t *old_stateCnt);

	const mach_msg_type_t flavorCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t old_stateCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t old_stateType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1024,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	thread_t target_thread;
	mach_msg_type_number_t old_stateCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->flavorType, &flavorCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->old_stateCntType, &old_stateCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	old_stateCnt = 1024;
	if (In0P->old_stateCnt < old_stateCnt)
		old_stateCnt = In0P->old_stateCnt;

	OutP->RetCode = thread_get_state(target_thread, In0P->flavor, OutP->old_state, &old_stateCnt);
	thread_deallocate(target_thread);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->old_stateType = old_stateType;

	OutP->old_stateType.msgt_number = old_stateCnt;
	OutP->Head.msgh_size = 36 + (4 * old_stateCnt);
}

/* Routine thread_set_state */
mig_internal void _Xthread_set_state
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t flavorType;
		int flavor;
		mach_msg_type_t new_stateType;
		natural_t new_state[1024];
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_set_state
		(thread_t target_thread, int flavor, thread_state_t new_state, mach_msg_type_number_t new_stateCnt);

	unsigned int msgh_size;

	const mach_msg_type_t flavorCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	thread_t target_thread;

#if	TypeCheck
	msgh_size = In0P->Head.msgh_size;
	if ((msgh_size < 36) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->flavorType, &flavorCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->new_stateType.msgt_inline != TRUE) ||
	    (In0P->new_stateType.msgt_longform != FALSE) ||
	    (In0P->new_stateType.msgt_name != 2) ||
	    (In0P->new_stateType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (msgh_size != 36 + (4 * In0P->new_stateType.msgt_number))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_set_state(target_thread, In0P->flavor, In0P->new_state, In0P->new_stateType.msgt_number);
	thread_deallocate(target_thread);
}

/* Routine thread_info */
mig_internal void _Xthread_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t flavorType;
		int flavor;
		mach_msg_type_t thread_info_outCntType;
		mach_msg_type_number_t thread_info_outCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t thread_info_outType;
		integer_t thread_info_out[1024];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_info
		(thread_t target_thread, int flavor, thread_info_t thread_info_out, mach_msg_type_number_t *thread_info_outCnt);

	const mach_msg_type_t flavorCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t thread_info_outCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t thread_info_outType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1024,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	thread_t target_thread;
	mach_msg_type_number_t thread_info_outCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->flavorType, &flavorCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->thread_info_outCntType, &thread_info_outCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	thread_info_outCnt = 1024;
	if (In0P->thread_info_outCnt < thread_info_outCnt)
		thread_info_outCnt = In0P->thread_info_outCnt;

	OutP->RetCode = thread_info(target_thread, In0P->flavor, OutP->thread_info_out, &thread_info_outCnt);
	thread_deallocate(target_thread);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->thread_info_outType = thread_info_outType;

	OutP->thread_info_outType.msgt_number = thread_info_outCnt;
	OutP->Head.msgh_size = 36 + (4 * thread_info_outCnt);
}

/* Routine vm_allocate */
mig_internal void _Xvm_allocate
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t anywhereType;
		boolean_t anywhere;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t addressType;
		vm_address_t address;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_allocate
		(vm_map_t target_task, vm_address_t *address, vm_size_t size, boolean_t anywhere);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t anywhereCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t addressType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->anywhereType, &anywhereCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_allocate(target_task, &In0P->address, In0P->size, In0P->anywhere);
	vm_map_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->addressType = addressType;

	OutP->address = In0P->address;
}

/* Routine vm_deallocate */
mig_internal void _Xvm_deallocate
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_t sizeType;
		vm_size_t size;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_deallocate
		(vm_map_t target_task, vm_address_t address, vm_size_t size);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_deallocate(target_task, In0P->address, In0P->size);
	vm_map_deallocate(target_task);
}

/* Routine vm_protect */
mig_internal void _Xvm_protect
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t set_maximumType;
		boolean_t set_maximum;
		mach_msg_type_t new_protectionType;
		vm_prot_t new_protection;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_protect
		(vm_map_t target_task, vm_address_t address, vm_size_t size, boolean_t set_maximum, vm_prot_t new_protection);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t set_maximumCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t new_protectionCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 56) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->set_maximumType, &set_maximumCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->new_protectionType, &new_protectionCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_protect(target_task, In0P->address, In0P->size, In0P->set_maximum, In0P->new_protection);
	vm_map_deallocate(target_task);
}

/* Routine vm_inherit */
mig_internal void _Xvm_inherit
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t new_inheritanceType;
		vm_inherit_t new_inheritance;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_inherit
		(vm_map_t target_task, vm_address_t address, vm_size_t size, vm_inherit_t new_inheritance);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t new_inheritanceCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->new_inheritanceType, &new_inheritanceCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_inherit(target_task, In0P->address, In0P->size, In0P->new_inheritance);
	vm_map_deallocate(target_task);
}

/* Routine vm_read */
mig_internal void _Xvm_read
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_t sizeType;
		vm_size_t size;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_long_t dataType;
		vm_offset_t data;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_read
		(vm_map_t target_task, vm_address_t address, vm_size_t size, vm_offset_t *data, mach_msg_type_number_t *dataCnt);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t dataType = {
	{
		/* msgt_name = */		0,
		/* msgt_size = */		0,
		/* msgt_number = */		0,
		/* msgt_inline = */		FALSE,
		/* msgt_longform = */		TRUE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	},
		/* msgtl_name = */	9,
		/* msgtl_size = */	8,
		/* msgtl_number = */	0,
	};

	vm_map_t target_task;
	mach_msg_type_number_t dataCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_read(target_task, In0P->address, In0P->size, &OutP->data, &dataCnt);
	vm_map_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 48;

	OutP->dataType = dataType;

	OutP->dataType.msgtl_number = dataCnt;
}

/* Routine vm_write */
mig_internal void _Xvm_write
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_long_t dataType;
		vm_offset_t data;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_write
		(vm_map_t target_task, vm_address_t address, vm_offset_t data, mach_msg_type_number_t dataCnt);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->dataType.msgtl_header.msgt_inline != FALSE) ||
	    (In0P->dataType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->dataType.msgtl_name != 9) ||
	    (In0P->dataType.msgtl_size != 8))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_write(target_task, In0P->address, In0P->data, In0P->dataType.msgtl_number);
	vm_map_deallocate(target_task);
}

/* Routine vm_copy */
mig_internal void _Xvm_copy
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t source_addressType;
		vm_address_t source_address;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t dest_addressType;
		vm_address_t dest_address;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_copy
		(vm_map_t target_task, vm_address_t source_address, vm_size_t size, vm_address_t dest_address);

	const mach_msg_type_t source_addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t dest_addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->source_addressType, &source_addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->dest_addressType, &dest_addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_copy(target_task, In0P->source_address, In0P->size, In0P->dest_address);
	vm_map_deallocate(target_task);
}

/* Routine vm_region */
mig_internal void _Xvm_region
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t protectionType;
		vm_prot_t protection;
		mach_msg_type_t max_protectionType;
		vm_prot_t max_protection;
		mach_msg_type_t inheritanceType;
		vm_inherit_t inheritance;
		mach_msg_type_t is_sharedType;
		boolean_t is_shared;
		mach_msg_type_t object_nameType;
		ipc_port_t object_name;
		mach_msg_type_t offsetType;
		vm_offset_t offset;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_region
		(vm_map_t target_task, vm_address_t *address, vm_size_t *size, vm_prot_t *protection, vm_prot_t *max_protection, vm_inherit_t *inheritance, boolean_t *is_shared, ipc_port_t *object_name, vm_offset_t *offset);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t addressType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t protectionType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t max_protectionType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t inheritanceType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t is_sharedType = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t object_nameType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t offsetType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_region(target_task, &In0P->address, &OutP->size, &OutP->protection, &OutP->max_protection, &OutP->inheritance, &OutP->is_shared, &OutP->object_name, &OutP->offset);
	vm_map_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 96;

	OutP->addressType = addressType;

	OutP->address = In0P->address;

	OutP->sizeType = sizeType;

	OutP->protectionType = protectionType;

	OutP->max_protectionType = max_protectionType;

	OutP->inheritanceType = inheritanceType;

	OutP->is_sharedType = is_sharedType;

	OutP->object_nameType = object_nameType;

	OutP->offsetType = offsetType;
}

/* Routine vm_statistics */
mig_internal void _Xvm_statistics
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t vm_statsType;
		vm_statistics_data_t vm_stats;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_statistics
		(vm_map_t target_task, vm_statistics_data_t *vm_stats);

	const mach_msg_type_t vm_statsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		13,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_statistics(target_task, &OutP->vm_stats);
	vm_map_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 88;

	OutP->vm_statsType = vm_statsType;
}

/* Routine mach_ports_register */
mig_internal void _Xmach_ports_register
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_long_t init_port_setType;
		mach_port_array_t init_port_set;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_ports_register
		(task_t target_task, mach_port_array_t init_port_set, mach_msg_type_number_t init_port_setCnt);

	task_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->init_port_setType.msgtl_header.msgt_inline != FALSE) ||
	    (In0P->init_port_setType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->init_port_setType.msgtl_name != 17) ||
	    (In0P->init_port_setType.msgtl_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = mach_ports_register(target_task, In0P->init_port_set, In0P->init_port_setType.msgtl_number);
	task_deallocate(target_task);
}

/* Routine mach_ports_lookup */
mig_internal void _Xmach_ports_lookup
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_long_t init_port_setType;
		mach_port_array_t init_port_set;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_ports_lookup
		(task_t target_task, mach_port_array_t *init_port_set, mach_msg_type_number_t *init_port_setCnt);

	const mach_msg_type_long_t init_port_setType = {
	{
		/* msgt_name = */		0,
		/* msgt_size = */		0,
		/* msgt_number = */		0,
		/* msgt_inline = */		FALSE,
		/* msgt_longform = */		TRUE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	},
		/* msgtl_name = */	17,
		/* msgtl_size = */	32,
		/* msgtl_number = */	0,
	};

	task_t target_task;
	mach_msg_type_number_t init_port_setCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = mach_ports_lookup(target_task, &OutP->init_port_set, &init_port_setCnt);
	task_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 48;

	OutP->init_port_setType = init_port_setType;

	OutP->init_port_setType.msgtl_number = init_port_setCnt;
}

/* SimpleRoutine memory_object_data_provided */
mig_internal void _Xmemory_object_data_provided
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t offsetType;
		vm_offset_t offset;
		mach_msg_type_long_t dataType;
		vm_offset_t data;
		mach_msg_type_t lock_valueType;
		vm_prot_t lock_value;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_data_provided
		(vm_object_t memory_control, vm_offset_t offset, vm_offset_t data, mach_msg_type_number_t dataCnt, vm_prot_t lock_value);

	const mach_msg_type_t offsetCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t lock_valueCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 56) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->offsetType, &offsetCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->dataType.msgtl_header.msgt_inline != FALSE) ||
	    (In0P->dataType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->dataType.msgtl_name != 9) ||
	    (In0P->dataType.msgtl_size != 8))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->lock_valueType, &lock_valueCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_data_provided(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->offset, In0P->data, In0P->dataType.msgtl_number, In0P->lock_value);
}

/* SimpleRoutine memory_object_data_unavailable */
mig_internal void _Xmemory_object_data_unavailable
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t offsetType;
		vm_offset_t offset;
		mach_msg_type_t sizeType;
		vm_size_t size;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_data_unavailable
		(vm_object_t memory_control, vm_offset_t offset, vm_size_t size);

	const mach_msg_type_t offsetCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->offsetType, &offsetCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_data_unavailable(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->offset, In0P->size);
}

/* Routine memory_object_get_attributes */
mig_internal void _Xmemory_object_get_attributes
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t object_readyType;
		boolean_t object_ready;
		mach_msg_type_t may_cacheType;
		boolean_t may_cache;
		mach_msg_type_t copy_strategyType;
		memory_object_copy_strategy_t copy_strategy;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_get_attributes
		(vm_object_t memory_control, boolean_t *object_ready, boolean_t *may_cache, memory_object_copy_strategy_t *copy_strategy);

	const mach_msg_type_t object_readyType = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t may_cacheType = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t copy_strategyType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_get_attributes(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), &OutP->object_ready, &OutP->may_cache, &OutP->copy_strategy);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 56;

	OutP->object_readyType = object_readyType;

	OutP->may_cacheType = may_cacheType;

	OutP->copy_strategyType = copy_strategyType;
}

/* Routine vm_set_default_memory_manager */
mig_internal void _Xvm_set_default_memory_manager
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t default_managerType;
		ipc_port_t default_manager;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t default_managerType;
		ipc_port_t default_manager;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_set_default_memory_manager
		(host_t host_priv, ipc_port_t *default_manager);

	const mach_msg_type_t default_managerType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->default_managerType.msgt_inline != TRUE) ||
	    (In0P->default_managerType.msgt_longform != FALSE) ||
	    (In0P->default_managerType.msgt_name != 17) ||
	    (In0P->default_managerType.msgt_number != 1) ||
	    (In0P->default_managerType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = vm_set_default_memory_manager(convert_port_to_host_priv((ipc_port_t) In0P->Head.msgh_request_port), &In0P->default_manager);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 40;

	OutP->default_managerType = default_managerType;

	OutP->default_manager = In0P->default_manager;
}

/* SimpleRoutine memory_object_lock_request */
mig_internal void _Xmemory_object_lock_request
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t offsetType;
		vm_offset_t offset;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t should_returnType;
		memory_object_return_t should_return;
		mach_msg_type_t should_flushType;
		boolean_t should_flush;
		mach_msg_type_t lock_valueType;
		vm_prot_t lock_value;
		mach_msg_type_t reply_toType;
		ipc_port_t reply_to;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_lock_request
		(vm_object_t memory_control, vm_offset_t offset, vm_size_t size, memory_object_return_t should_return, boolean_t should_flush, vm_prot_t lock_value, ipc_port_t reply_to, mach_msg_type_name_t reply_toPoly);

#if	TypeCheck
	boolean_t msgh_simple;
#endif	/* TypeCheck */

	const mach_msg_type_t offsetCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t should_returnCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t should_flushCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t lock_valueCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	msgh_simple = !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX);
	if ((In0P->Head.msgh_size != 72))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->offsetType, &offsetCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->should_returnType, &should_returnCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->should_flushType, &should_flushCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->lock_valueType, &lock_valueCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->reply_toType.msgt_inline != TRUE) ||
	    (In0P->reply_toType.msgt_longform != FALSE) ||
	    (MACH_MSG_TYPE_PORT_ANY(In0P->reply_toType.msgt_name) && msgh_simple) ||
	    (In0P->reply_toType.msgt_number != 1) ||
	    (In0P->reply_toType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_lock_request(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->offset, In0P->size, In0P->should_return, In0P->should_flush, In0P->lock_value, In0P->reply_to, In0P->reply_toType.msgt_name);
}

/* Routine task_suspend */
mig_internal void _Xtask_suspend
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_suspend
		(task_t target_task);

	task_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_suspend(target_task);
	task_deallocate(target_task);
}

/* Routine task_resume */
mig_internal void _Xtask_resume
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_resume
		(task_t target_task);

	task_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_resume(target_task);
	task_deallocate(target_task);
}

/* Routine task_get_special_port */
mig_internal void _Xtask_get_special_port
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t which_portType;
		int which_port;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t special_portType;
		ipc_port_t special_port;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_get_special_port
		(task_t task, int which_port, ipc_port_t *special_port);

	const mach_msg_type_t which_portCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t special_portType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->which_portType, &which_portCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_get_special_port(task, In0P->which_port, &OutP->special_port);
	task_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 40;

	OutP->special_portType = special_portType;
}

/* Routine task_set_special_port */
mig_internal void _Xtask_set_special_port
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t which_portType;
		int which_port;
		mach_msg_type_t special_portType;
		ipc_port_t special_port;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_set_special_port
		(task_t task, int which_port, ipc_port_t special_port);

	const mach_msg_type_t which_portCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->which_portType, &which_portCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->special_portType.msgt_inline != TRUE) ||
	    (In0P->special_portType.msgt_longform != FALSE) ||
	    (In0P->special_portType.msgt_name != 17) ||
	    (In0P->special_portType.msgt_number != 1) ||
	    (In0P->special_portType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_set_special_port(task, In0P->which_port, In0P->special_port);
	task_deallocate(task);
}

/* Routine thread_create */
mig_internal void _Xthread_create
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t child_threadType;
		ipc_port_t child_thread;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_create
		(task_t parent_task, thread_t *child_thread);

	const mach_msg_type_t child_threadType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t parent_task;
	thread_t child_thread;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	parent_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_create(parent_task, &child_thread);
	task_deallocate(parent_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 40;

	OutP->child_threadType = child_threadType;

	OutP->child_thread = convert_thread_to_port(child_thread);
}

/* Routine thread_suspend */
mig_internal void _Xthread_suspend
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_suspend
		(thread_t target_thread);

	thread_t target_thread;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_suspend(target_thread);
	thread_deallocate(target_thread);
}

/* Routine thread_resume */
mig_internal void _Xthread_resume
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_resume
		(thread_t target_thread);

	thread_t target_thread;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_resume(target_thread);
	thread_deallocate(target_thread);
}

/* Routine thread_abort */
mig_internal void _Xthread_abort
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_abort
		(thread_t target_thread);

	thread_t target_thread;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_abort(target_thread);
	thread_deallocate(target_thread);
}

/* Routine thread_get_special_port */
mig_internal void _Xthread_get_special_port
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t which_portType;
		int which_port;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t special_portType;
		ipc_port_t special_port;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_get_special_port
		(thread_t thread, int which_port, ipc_port_t *special_port);

	const mach_msg_type_t which_portCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t special_portType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	thread_t thread;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->which_portType, &which_portCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_get_special_port(thread, In0P->which_port, &OutP->special_port);
	thread_deallocate(thread);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 40;

	OutP->special_portType = special_portType;
}

/* Routine thread_set_special_port */
mig_internal void _Xthread_set_special_port
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t which_portType;
		int which_port;
		mach_msg_type_t special_portType;
		ipc_port_t special_port;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_set_special_port
		(thread_t thread, int which_port, ipc_port_t special_port);

	const mach_msg_type_t which_portCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	thread_t thread;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->which_portType, &which_portCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->special_portType.msgt_inline != TRUE) ||
	    (In0P->special_portType.msgt_longform != FALSE) ||
	    (In0P->special_portType.msgt_name != 17) ||
	    (In0P->special_portType.msgt_number != 1) ||
	    (In0P->special_portType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	thread = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_set_special_port(thread, In0P->which_port, In0P->special_port);
	thread_deallocate(thread);
}

/* Routine task_set_emulation */
mig_internal void _Xtask_set_emulation
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t routine_entry_ptType;
		vm_address_t routine_entry_pt;
		mach_msg_type_t routine_numberType;
		int routine_number;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_set_emulation
		(task_t target_port, vm_address_t routine_entry_pt, int routine_number);

	const mach_msg_type_t routine_entry_ptCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t routine_numberCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t target_port;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->routine_entry_ptType, &routine_entry_ptCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->routine_numberType, &routine_numberCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_port = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_set_emulation(target_port, In0P->routine_entry_pt, In0P->routine_number);
	task_deallocate(target_port);
}

/* Routine task_ras_control */
mig_internal void _Xtask_ras_control
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t basepcType;
		vm_address_t basepc;
		mach_msg_type_t boundspcType;
		vm_address_t boundspc;
		mach_msg_type_t flavorType;
		int flavor;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_ras_control
		(task_t target_task, vm_address_t basepc, vm_address_t boundspc, int flavor);

	const mach_msg_type_t basepcCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t boundspcCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t flavorCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->basepcType, &basepcCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->boundspcType, &boundspcCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->flavorType, &flavorCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_ras_control(target_task, In0P->basepc, In0P->boundspc, In0P->flavor);
	task_deallocate(target_task);
}

/* Routine vm_map */
mig_internal void _Xvm_map
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t maskType;
		vm_address_t mask;
		mach_msg_type_t anywhereType;
		boolean_t anywhere;
		mach_msg_type_t memory_objectType;
		ipc_port_t memory_object;
		mach_msg_type_t offsetType;
		vm_offset_t offset;
		mach_msg_type_t copyType;
		boolean_t copy;
		mach_msg_type_t cur_protectionType;
		vm_prot_t cur_protection;
		mach_msg_type_t max_protectionType;
		vm_prot_t max_protection;
		mach_msg_type_t inheritanceType;
		vm_inherit_t inheritance;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t addressType;
		vm_address_t address;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_map
		(vm_map_t target_task, vm_address_t *address, vm_size_t size, vm_address_t mask, boolean_t anywhere, ipc_port_t memory_object, vm_offset_t offset, boolean_t copy, vm_prot_t cur_protection, vm_prot_t max_protection, vm_inherit_t inheritance);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t maskCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t anywhereCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t offsetCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t copyCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t cur_protectionCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t max_protectionCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t inheritanceCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t addressType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 104) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->maskType, &maskCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->anywhereType, &anywhereCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->memory_objectType.msgt_inline != TRUE) ||
	    (In0P->memory_objectType.msgt_longform != FALSE) ||
	    (In0P->memory_objectType.msgt_name != 17) ||
	    (In0P->memory_objectType.msgt_number != 1) ||
	    (In0P->memory_objectType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->offsetType, &offsetCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->copyType, &copyCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->cur_protectionType, &cur_protectionCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->max_protectionType, &max_protectionCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->inheritanceType, &inheritanceCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_map(target_task, &In0P->address, In0P->size, In0P->mask, In0P->anywhere, null_conversion(In0P->memory_object), In0P->offset, In0P->copy, In0P->cur_protection, In0P->max_protection, In0P->inheritance);
	vm_map_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	if (IP_VALID(In0P->memory_object))
		ipc_port_release_send(In0P->memory_object);

	OutP->Head.msgh_size = 40;

	OutP->addressType = addressType;

	OutP->address = In0P->address;
}

/* SimpleRoutine memory_object_data_error */
mig_internal void _Xmemory_object_data_error
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t offsetType;
		vm_offset_t offset;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t error_valueType;
		kern_return_t error_value;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_data_error
		(vm_object_t memory_control, vm_offset_t offset, vm_size_t size, kern_return_t error_value);

	const mach_msg_type_t offsetCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t error_valueCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->offsetType, &offsetCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->error_valueType, &error_valueCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_data_error(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->offset, In0P->size, In0P->error_value);
}

/* SimpleRoutine memory_object_set_attributes */
mig_internal void _Xmemory_object_set_attributes
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t object_readyType;
		boolean_t object_ready;
		mach_msg_type_t may_cacheType;
		boolean_t may_cache;
		mach_msg_type_t copy_strategyType;
		memory_object_copy_strategy_t copy_strategy;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_set_attributes
		(vm_object_t memory_control, boolean_t object_ready, boolean_t may_cache, memory_object_copy_strategy_t copy_strategy);

	const mach_msg_type_t object_readyCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t may_cacheCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t copy_strategyCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 48) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->object_readyType, &object_readyCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->may_cacheType, &may_cacheCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->copy_strategyType, &copy_strategyCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_set_attributes(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->object_ready, In0P->may_cache, In0P->copy_strategy);
}

/* SimpleRoutine memory_object_destroy */
mig_internal void _Xmemory_object_destroy
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t reasonType;
		kern_return_t reason;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_destroy
		(vm_object_t memory_control, kern_return_t reason);

	const mach_msg_type_t reasonCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->reasonType, &reasonCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_destroy(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->reason);
}

/* SimpleRoutine memory_object_data_supply */
mig_internal void _Xmemory_object_data_supply
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t offsetType;
		vm_offset_t offset;
		mach_msg_type_long_t dataType;
		vm_offset_t data;
		mach_msg_type_t lock_valueType;
		vm_prot_t lock_value;
		mach_msg_type_t preciousType;
		boolean_t precious;
		mach_msg_type_t reply_toType;
		ipc_port_t reply_to;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_data_supply
		(vm_object_t memory_control, vm_offset_t offset, vm_offset_t data, mach_msg_type_number_t dataCnt, vm_prot_t lock_value, boolean_t precious, ipc_port_t reply_to, mach_msg_type_name_t reply_toPoly);

	const mach_msg_type_t offsetCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t lock_valueCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t preciousCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 72) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->offsetType, &offsetCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->dataType.msgtl_header.msgt_inline != FALSE) ||
	    (In0P->dataType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->dataType.msgtl_name != 9) ||
	    (In0P->dataType.msgtl_size != 8))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->lock_valueType, &lock_valueCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->preciousType, &preciousCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->reply_toType.msgt_inline != TRUE) ||
	    (In0P->reply_toType.msgt_longform != FALSE) ||
	    (In0P->reply_toType.msgt_number != 1) ||
	    (In0P->reply_toType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_data_supply(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->offset, In0P->data, In0P->dataType.msgtl_number, In0P->lock_value, In0P->precious, In0P->reply_to, In0P->reply_toType.msgt_name);
}

/* SimpleRoutine memory_object_ready */
mig_internal void _Xmemory_object_ready
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t may_cacheType;
		boolean_t may_cache;
		mach_msg_type_t copy_strategyType;
		memory_object_copy_strategy_t copy_strategy;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_ready
		(vm_object_t memory_control, boolean_t may_cache, memory_object_copy_strategy_t copy_strategy);

	const mach_msg_type_t may_cacheCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t copy_strategyCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->may_cacheType, &may_cacheCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->copy_strategyType, &copy_strategyCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_ready(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->may_cache, In0P->copy_strategy);
}

/* SimpleRoutine memory_object_change_attributes */
mig_internal void _Xmemory_object_change_attributes
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t may_cacheType;
		boolean_t may_cache;
		mach_msg_type_t copy_strategyType;
		memory_object_copy_strategy_t copy_strategy;
		mach_msg_type_t reply_toType;
		ipc_port_t reply_to;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_change_attributes
		(vm_object_t memory_control, boolean_t may_cache, memory_object_copy_strategy_t copy_strategy, ipc_port_t reply_to, mach_msg_type_name_t reply_toPoly);

#if	TypeCheck
	boolean_t msgh_simple;
#endif	/* TypeCheck */

	const mach_msg_type_t may_cacheCheck = {
		/* msgt_name = */		0,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t copy_strategyCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	msgh_simple = !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX);
	if ((In0P->Head.msgh_size != 48))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->may_cacheType, &may_cacheCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->copy_strategyType, &copy_strategyCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->reply_toType.msgt_inline != TRUE) ||
	    (In0P->reply_toType.msgt_longform != FALSE) ||
	    (MACH_MSG_TYPE_PORT_ANY(In0P->reply_toType.msgt_name) && msgh_simple) ||
	    (In0P->reply_toType.msgt_number != 1) ||
	    (In0P->reply_toType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = memory_object_change_attributes(vm_object_lookup((ipc_port_t) In0P->Head.msgh_request_port), In0P->may_cache, In0P->copy_strategy, In0P->reply_to, In0P->reply_toType.msgt_name);
}

/* Routine vm_machine_attribute */
mig_internal void _Xvm_machine_attribute
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t addressType;
		vm_address_t address;
		mach_msg_type_t sizeType;
		vm_size_t size;
		mach_msg_type_t attributeType;
		vm_machine_attribute_t attribute;
		mach_msg_type_t valueType;
		vm_machine_attribute_val_t value;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t valueType;
		vm_machine_attribute_val_t value;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t vm_machine_attribute
		(vm_map_t target_task, vm_address_t address, vm_size_t size, vm_machine_attribute_t attribute, vm_machine_attribute_val_t *value);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t attributeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t valueCheck = {
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

	vm_map_t target_task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 56) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->attributeType, &attributeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->valueType, &valueCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = vm_machine_attribute(target_task, In0P->address, In0P->size, In0P->attribute, &In0P->value);
	vm_map_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->valueType = valueType;

	OutP->value = In0P->value;
}

mig_routine_t mach_server_routines[] = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		_Xtask_create,
		_Xtask_terminate,
		_Xtask_get_emulation_vector,
		_Xtask_set_emulation_vector,
		_Xtask_threads,
		_Xtask_info,
		0,
		0,
		0,
		_Xthread_terminate,
		_Xthread_get_state,
		_Xthread_set_state,
		_Xthread_info,
		0,
		_Xvm_allocate,
		0,
		_Xvm_deallocate,
		_Xvm_protect,
		_Xvm_inherit,
		_Xvm_read,
		_Xvm_write,
		_Xvm_copy,
		_Xvm_region,
		_Xvm_statistics,
		0,
		0,
		_Xmach_ports_register,
		_Xmach_ports_lookup,
		0,
		0,
		0,
		_Xmemory_object_data_provided,
		_Xmemory_object_data_unavailable,
		_Xmemory_object_get_attributes,
		_Xvm_set_default_memory_manager,
		0,
		0,
		_Xmemory_object_lock_request,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		_Xtask_suspend,
		_Xtask_resume,
		_Xtask_get_special_port,
		_Xtask_set_special_port,
		0,
		_Xthread_create,
		_Xthread_suspend,
		_Xthread_resume,
		_Xthread_abort,
		0,
		0,
		_Xthread_get_special_port,
		_Xthread_set_special_port,
		0,
		_Xtask_set_emulation,
		_Xtask_ras_control,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		_Xvm_map,
		_Xmemory_object_data_error,
		_Xmemory_object_set_attributes,
		_Xmemory_object_destroy,
		_Xmemory_object_data_supply,
		_Xmemory_object_ready,
		_Xmemory_object_change_attributes,
		0,
		0,
		0,
		_Xvm_machine_attribute,
			0,
};

mig_external boolean_t mach_server
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

	if ((InP->msgh_id > 2100) || (InP->msgh_id < 2000) ||
	    ((routine = mach_server_routines[InP->msgh_id - 2000]) == 0)) {
		OutP->RetCode = MIG_BAD_ID;
		return FALSE;
	}
	(*routine) (InP, &OutP->Head);
	return TRUE;
}

mig_external mig_routine_t mach_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 2000;

	if ((msgh_id > 100) || (msgh_id < 0))
		return 0;

	return mach_server_routines[msgh_id];
}

