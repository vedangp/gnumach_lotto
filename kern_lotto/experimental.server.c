/* Module experimental */

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

/* Routine device_intr_register */
mig_internal void _Xdevice_intr_register
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t lineType;
		int line;
		mach_msg_type_t idType;
		int id;
		mach_msg_type_t flagsType;
		int flags;
		mach_msg_type_t receive_portType;
		ipc_port_t receive_port;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t experimental_device_intr_register
		(ipc_port_t master_port, int line, int id, int flags, ipc_port_t receive_port);

	const mach_msg_type_t lineCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t idCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t flagsCheck = {
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
	if (BAD_TYPECHECK(&In0P->lineType, &lineCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->idType, &idCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->flagsType, &flagsCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->receive_portType.msgt_inline != TRUE) ||
	    (In0P->receive_portType.msgt_longform != FALSE) ||
	    (In0P->receive_portType.msgt_name != 17) ||
	    (In0P->receive_portType.msgt_number != 1) ||
	    (In0P->receive_portType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = experimental_device_intr_register((ipc_port_t) In0P->Head.msgh_request_port, In0P->line, In0P->id, In0P->flags, In0P->receive_port);
}

/* Routine device_intr_enable */
mig_internal void _Xdevice_intr_enable
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t lineType;
		int line;
		mach_msg_type_t statusType;
		char status;
		char statusPad[3];
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t experimental_device_intr_enable
		(ipc_port_t master_port, int line, char status);

	const mach_msg_type_t lineCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t statusCheck = {
		/* msgt_name = */		8,
		/* msgt_size = */		8,
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
	if (BAD_TYPECHECK(&In0P->lineType, &lineCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->statusType, &statusCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = experimental_device_intr_enable((ipc_port_t) In0P->Head.msgh_request_port, In0P->line, In0P->status);
}

/* Routine vm_allocate_contiguous */
mig_internal void _Xvm_allocate_contiguous
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t target_taskType;
		ipc_port_t target_task;
		mach_msg_type_t sizeType;
		vm_size_t size;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t vaddrType;
		vm_address_t vaddr;
		mach_msg_type_t paddrType;
		vm_address_t paddr;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t experimental_vm_allocate_contiguous
		(host_t host_priv, vm_map_t target_task, vm_address_t *vaddr, vm_address_t *paddr, vm_size_t size);

	const mach_msg_type_t sizeCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t vaddrType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t paddrType = {
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
	    !(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->target_taskType.msgt_inline != TRUE) ||
	    (In0P->target_taskType.msgt_longform != FALSE) ||
	    (In0P->target_taskType.msgt_name != 17) ||
	    (In0P->target_taskType.msgt_number != 1) ||
	    (In0P->target_taskType.msgt_size != 32))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->sizeType, &sizeCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	target_task = convert_port_to_map(In0P->target_task);

	OutP->RetCode = experimental_vm_allocate_contiguous(convert_port_to_host_priv((ipc_port_t) In0P->Head.msgh_request_port), target_task, &OutP->vaddr, &OutP->paddr, In0P->size);
	vm_map_deallocate(target_task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	if (IP_VALID(In0P->target_task))
		ipc_port_release_send(In0P->target_task);

	OutP->Head.msgh_size = 48;

	OutP->vaddrType = vaddrType;

	OutP->paddrType = paddrType;
}

mig_routine_t experimental_server_routines[] = {
		0,
		_Xdevice_intr_register,
		_Xdevice_intr_enable,
		_Xvm_allocate_contiguous,
};

mig_external boolean_t experimental_server
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

	if ((InP->msgh_id > 424245) || (InP->msgh_id < 424242) ||
	    ((routine = experimental_server_routines[InP->msgh_id - 424242]) == 0)) {
		OutP->RetCode = MIG_BAD_ID;
		return FALSE;
	}
	(*routine) (InP, &OutP->Head);
	return TRUE;
}

mig_external mig_routine_t experimental_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 424242;

	if ((msgh_id > 3) || (msgh_id < 0))
		return 0;

	return experimental_server_routines[msgh_id];
}

