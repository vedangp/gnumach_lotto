/* Module mach4 */

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

/* Routine task_enable_pc_sampling */
mig_internal void _Xtask_enable_pc_sampling
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t flavorType;
		sampled_pc_flavor_t flavor;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t tickType;
		int tick;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_enable_pc_sampling
		(task_t host, int *tick, sampled_pc_flavor_t flavor);

	const mach_msg_type_t flavorCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t tickType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t host;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->flavorType, &flavorCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	host = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_enable_pc_sampling(host, &OutP->tick, In0P->flavor);
	task_deallocate(host);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->tickType = tickType;
}

/* Routine task_disable_pc_sampling */
mig_internal void _Xtask_disable_pc_sampling
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t samplecntType;
		int samplecnt;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_disable_pc_sampling
		(task_t host, int *samplecnt);

	const mach_msg_type_t samplecntType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t host;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	host = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = task_disable_pc_sampling(host, &OutP->samplecnt);
	task_deallocate(host);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->samplecntType = samplecntType;
}

/* Routine task_get_sampled_pcs */
mig_internal void _Xtask_get_sampled_pcs
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t seqnoType;
		sampled_pc_seqno_t seqno;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t seqnoType;
		sampled_pc_seqno_t seqno;
		mach_msg_type_t sampled_pcsType;
		sampled_pc_t sampled_pcs[512];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t task_get_sampled_pcs
		(task_t host, sampled_pc_seqno_t *seqno, sampled_pc_array_t sampled_pcs, mach_msg_type_number_t *sampled_pcsCnt);

	const mach_msg_type_t seqnoCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t seqnoType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sampled_pcsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1536,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	task_t host;
	mach_msg_type_number_t sampled_pcsCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->seqnoType, &seqnoCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	host = convert_port_to_task((ipc_port_t) In0P->Head.msgh_request_port);

	sampled_pcsCnt = 512;

	OutP->RetCode = task_get_sampled_pcs(host, &In0P->seqno, OutP->sampled_pcs, &sampled_pcsCnt);
	task_deallocate(host);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->seqnoType = seqnoType;

	OutP->seqno = In0P->seqno;

	OutP->sampled_pcsType = sampled_pcsType;

	OutP->sampled_pcsType.msgt_number = 3 * sampled_pcsCnt;
	OutP->Head.msgh_size = 44 + (12 * sampled_pcsCnt);
}

/* Routine thread_enable_pc_sampling */
mig_internal void _Xthread_enable_pc_sampling
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t flavorType;
		sampled_pc_flavor_t flavor;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t tickType;
		int tick;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_enable_pc_sampling
		(thread_t host, int *tick, sampled_pc_flavor_t flavor);

	const mach_msg_type_t flavorCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t tickType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	thread_t host;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->flavorType, &flavorCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	host = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_enable_pc_sampling(host, &OutP->tick, In0P->flavor);
	thread_deallocate(host);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->tickType = tickType;
}

/* Routine thread_disable_pc_sampling */
mig_internal void _Xthread_disable_pc_sampling
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t samplecntType;
		int samplecnt;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_disable_pc_sampling
		(thread_t host, int *samplecnt);

	const mach_msg_type_t samplecntType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	thread_t host;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	host = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = thread_disable_pc_sampling(host, &OutP->samplecnt);
	thread_deallocate(host);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->samplecntType = samplecntType;
}

/* Routine thread_get_sampled_pcs */
mig_internal void _Xthread_get_sampled_pcs
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t seqnoType;
		sampled_pc_seqno_t seqno;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t seqnoType;
		sampled_pc_seqno_t seqno;
		mach_msg_type_t sampled_pcsType;
		sampled_pc_t sampled_pcs[512];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t thread_get_sampled_pcs
		(thread_t host, sampled_pc_seqno_t *seqno, sampled_pc_array_t sampled_pcs, mach_msg_type_number_t *sampled_pcsCnt);

	const mach_msg_type_t seqnoCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t seqnoType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t sampled_pcsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1536,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	thread_t host;
	mach_msg_type_number_t sampled_pcsCnt;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->seqnoType, &seqnoCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	host = convert_port_to_thread((ipc_port_t) In0P->Head.msgh_request_port);

	sampled_pcsCnt = 512;

	OutP->RetCode = thread_get_sampled_pcs(host, &In0P->seqno, OutP->sampled_pcs, &sampled_pcsCnt);
	thread_deallocate(host);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->seqnoType = seqnoType;

	OutP->seqno = In0P->seqno;

	OutP->sampled_pcsType = sampled_pcsType;

	OutP->sampled_pcsType.msgt_number = 3 * sampled_pcsCnt;
	OutP->Head.msgh_size = 44 + (12 * sampled_pcsCnt);
}

/* Routine memory_object_create_proxy */
mig_internal void _Xmemory_object_create_proxy
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t max_protectionType;
		vm_prot_t max_protection;
		mach_msg_type_t objectType;
		ipc_port_t object[1024];
		mach_msg_type_t offsetType;
		vm_offset_t offset[1024];
		mach_msg_type_t startType;
		vm_offset_t start[1024];
		mach_msg_type_t lenType;
		vm_offset_t len[1024];
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t proxyType;
		ipc_port_t proxy;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Request *In1P;
	Request *In2P;
	Request *In3P;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t memory_object_create_proxy
		(ipc_space_t task, vm_prot_t max_protection, memory_object_array_t object, mach_msg_type_number_t objectCnt, vm_offset_array_t offset, mach_msg_type_number_t offsetCnt, vm_offset_array_t start, mach_msg_type_number_t startCnt, vm_offset_array_t len, mach_msg_type_number_t lenCnt, ipc_port_t *proxy);

	unsigned int msgh_size;
	unsigned int msgh_size_delta;

	const mach_msg_type_t max_protectionCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t proxyType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	ipc_space_t task;

#if	TypeCheck
	msgh_size = In0P->Head.msgh_size;
	if ((msgh_size < 48) ||
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->max_protectionType, &max_protectionCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->objectType.msgt_inline != TRUE) ||
	    (In0P->objectType.msgt_longform != FALSE) ||
	    (In0P->objectType.msgt_name != 17) ||
	    (In0P->objectType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	msgh_size_delta = 4 * In0P->objectType.msgt_number;
#if	TypeCheck
	if (msgh_size < 48 + msgh_size_delta)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
	msgh_size -= msgh_size_delta;
#endif	/* TypeCheck */

	In1P = (Request *) ((char *) In0P + msgh_size_delta - 4096);

#if	TypeCheck
	if ((In1P->offsetType.msgt_inline != TRUE) ||
	    (In1P->offsetType.msgt_longform != FALSE) ||
	    (In1P->offsetType.msgt_name != 2) ||
	    (In1P->offsetType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	msgh_size_delta = 4 * In1P->offsetType.msgt_number;
#if	TypeCheck
	if (msgh_size < 48 + msgh_size_delta)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
	msgh_size -= msgh_size_delta;
#endif	/* TypeCheck */

	In2P = (Request *) ((char *) In1P + msgh_size_delta - 4096);

#if	TypeCheck
	if ((In2P->startType.msgt_inline != TRUE) ||
	    (In2P->startType.msgt_longform != FALSE) ||
	    (In2P->startType.msgt_name != 2) ||
	    (In2P->startType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	msgh_size_delta = 4 * In2P->startType.msgt_number;
#if	TypeCheck
	if (msgh_size < 48 + msgh_size_delta)
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
	msgh_size -= msgh_size_delta;
#endif	/* TypeCheck */

	In3P = (Request *) ((char *) In2P + msgh_size_delta - 4096);

#if	TypeCheck
	if ((In3P->lenType.msgt_inline != TRUE) ||
	    (In3P->lenType.msgt_longform != FALSE) ||
	    (In3P->lenType.msgt_name != 2) ||
	    (In3P->lenType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (msgh_size != 48 + (4 * In3P->lenType.msgt_number))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_space((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = memory_object_create_proxy(task, In0P->max_protection, In0P->object, In0P->objectType.msgt_number, In1P->offset, In1P->offsetType.msgt_number, In2P->start, In2P->startType.msgt_number, In3P->len, In3P->lenType.msgt_number, &OutP->proxy);
	space_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 40;

	OutP->proxyType = proxyType;
}

mig_routine_t mach4_server_routines[] = {
		_Xtask_enable_pc_sampling,
		_Xtask_disable_pc_sampling,
		_Xtask_get_sampled_pcs,
		_Xthread_enable_pc_sampling,
		_Xthread_disable_pc_sampling,
		_Xthread_get_sampled_pcs,
		0,
		0,
		0,
		0,
		_Xmemory_object_create_proxy,
};

mig_external boolean_t mach4_server
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

	if ((InP->msgh_id > 4010) || (InP->msgh_id < 4000) ||
	    ((routine = mach4_server_routines[InP->msgh_id - 4000]) == 0)) {
		OutP->RetCode = MIG_BAD_ID;
		return FALSE;
	}
	(*routine) (InP, &OutP->Head);
	return TRUE;
}

mig_external mig_routine_t mach4_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 4000;

	if ((msgh_id > 10) || (msgh_id < 0))
		return 0;

	return mach4_server_routines[msgh_id];
}

