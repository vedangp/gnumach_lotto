/* Module mach_debug */

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
#include <mach_debug/mach_debug_types.h>

/* Routine mach_port_get_srights */
mig_internal void _Xmach_port_get_srights
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t nameType;
		mach_port_t name;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t srightsType;
		mach_port_rights_t srights;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_port_get_srights
		(ipc_space_t task, mach_port_t name, mach_port_rights_t *srights);

	const mach_msg_type_t nameCheck = {
		/* msgt_name = */		15,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t srightsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	ipc_space_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->nameType, &nameCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_space((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = mach_port_get_srights(task, In0P->name, &OutP->srights);
	space_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 40;

	OutP->srightsType = srightsType;
}

/* Routine host_ipc_hash_info */
mig_internal void _Xhost_ipc_hash_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t infoCntType;
		mach_msg_type_number_t infoCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_long_t infoType;
		hash_info_bucket_t info[512];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t host_ipc_hash_info
		(host_t host, hash_info_bucket_array_t *info, mach_msg_type_number_t *infoCnt);

	boolean_t msgh_simple;
	const mach_msg_type_t infoCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t infoType = {
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
		/* msgtl_number = */	512,
	};

	mach_msg_type_number_t infoCnt;

	hash_info_bucket_t *infoP;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->infoCntType, &infoCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	infoP = OutP->info;
	infoCnt = 512;
	if (In0P->infoCnt < infoCnt)
		infoCnt = In0P->infoCnt;

	OutP->RetCode = host_ipc_hash_info(convert_port_to_host((ipc_port_t) In0P->Head.msgh_request_port), &infoP, &infoCnt);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msgh_simple = TRUE;

	OutP->infoType = infoType;
	if (infoP != OutP->info) {
		OutP->infoType.msgtl_header.msgt_inline = FALSE;
		OutP->infoType.msgtl_header.msgt_deallocate = TRUE;
		*((hash_info_bucket_t **)OutP->info) = infoP;
		msgh_simple = FALSE;
	}

	OutP->infoType.msgtl_number = infoCnt;
	OutP->Head.msgh_size = 44 + ((OutP->infoType.msgtl_header.msgt_inline) ? 4 * infoCnt : sizeof(hash_info_bucket_t *));

	if (!msgh_simple)
		OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
}

/* Routine host_ipc_marequest_info */
mig_internal void _Xhost_ipc_marequest_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t infoCntType;
		mach_msg_type_number_t infoCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t max_requestsType;
		unsigned max_requests;
		mach_msg_type_long_t infoType;
		hash_info_bucket_t info[512];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t host_ipc_marequest_info
		(host_t host, unsigned *max_requests, hash_info_bucket_array_t *info, mach_msg_type_number_t *infoCnt);

	boolean_t msgh_simple;
	const mach_msg_type_t infoCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t max_requestsType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t infoType = {
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
		/* msgtl_number = */	512,
	};

	mach_msg_type_number_t infoCnt;

	hash_info_bucket_t *infoP;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->infoCntType, &infoCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	infoP = OutP->info;
	infoCnt = 512;
	if (In0P->infoCnt < infoCnt)
		infoCnt = In0P->infoCnt;

	OutP->RetCode = host_ipc_marequest_info(convert_port_to_host((ipc_port_t) In0P->Head.msgh_request_port), &OutP->max_requests, &infoP, &infoCnt);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msgh_simple = TRUE;

	OutP->max_requestsType = max_requestsType;

	OutP->infoType = infoType;
	if (infoP != OutP->info) {
		OutP->infoType.msgtl_header.msgt_inline = FALSE;
		OutP->infoType.msgtl_header.msgt_deallocate = TRUE;
		*((hash_info_bucket_t **)OutP->info) = infoP;
		msgh_simple = FALSE;
	}

	OutP->infoType.msgtl_number = infoCnt;
	OutP->Head.msgh_size = 52 + ((OutP->infoType.msgtl_header.msgt_inline) ? 4 * infoCnt : sizeof(hash_info_bucket_t *));

	if (!msgh_simple)
		OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
}

/* Routine mach_port_space_info */
mig_internal void _Xmach_port_space_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t table_infoCntType;
		mach_msg_type_number_t table_infoCnt;
		mach_msg_type_t tree_infoCntType;
		mach_msg_type_number_t tree_infoCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t infoType;
		ipc_info_space_t info;
		mach_msg_type_long_t table_infoType;
		ipc_info_name_t table_info[56];
		mach_msg_type_long_t tree_infoType;
		ipc_info_tree_name_t tree_info[46];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_port_space_info
		(ipc_space_t task, ipc_info_space_t *info, ipc_info_name_array_t *table_info, mach_msg_type_number_t *table_infoCnt, ipc_info_tree_name_array_t *tree_info, mach_msg_type_number_t *tree_infoCnt);

	boolean_t msgh_simple;
	unsigned int msgh_size;
	unsigned int msgh_size_delta;

	const mach_msg_type_t table_infoCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t tree_infoCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t infoType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		6,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t table_infoType = {
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
		/* msgtl_number = */	504,
	};

	const mach_msg_type_long_t tree_infoType = {
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
		/* msgtl_number = */	506,
	};

	ipc_space_t task;
	mach_msg_type_number_t table_infoCnt;
	ipc_info_tree_name_t tree_info[46];
	mach_msg_type_number_t tree_infoCnt;

	ipc_info_name_t *table_infoP;
	ipc_info_tree_name_t *tree_infoP;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 40) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->table_infoCntType, &table_infoCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->tree_infoCntType, &tree_infoCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_space((ipc_port_t) In0P->Head.msgh_request_port);

	table_infoP = OutP->table_info;
	table_infoCnt = 56;
	if (In0P->table_infoCnt < table_infoCnt)
		table_infoCnt = In0P->table_infoCnt;

	tree_infoP = tree_info;
	tree_infoCnt = 46;
	if (In0P->tree_infoCnt < tree_infoCnt)
		tree_infoCnt = In0P->tree_infoCnt;

	OutP->RetCode = mach_port_space_info(task, &OutP->info, &table_infoP, &table_infoCnt, &tree_infoP, &tree_infoCnt);
	space_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msgh_simple = TRUE;

	OutP->infoType = infoType;

	OutP->table_infoType = table_infoType;
	if (table_infoP != OutP->table_info) {
		OutP->table_infoType.msgtl_header.msgt_inline = FALSE;
		OutP->table_infoType.msgtl_header.msgt_deallocate = TRUE;
		*((ipc_info_name_t **)OutP->table_info) = table_infoP;
		msgh_simple = FALSE;
	}

	OutP->table_infoType.msgtl_number = 9 * table_infoCnt;
	msgh_size_delta = (OutP->table_infoType.msgtl_header.msgt_inline) ? 36 * table_infoCnt : sizeof(ipc_info_name_t *);
	msgh_size = 84 + msgh_size_delta;
	OutP = (Reply *) ((char *) OutP + msgh_size_delta - 2016);

	OutP->tree_infoType = tree_infoType;

	if (tree_infoP != tree_info) {
		OutP->tree_infoType.msgtl_header.msgt_inline = FALSE;
		OutP->tree_infoType.msgtl_header.msgt_deallocate = TRUE;
		*((ipc_info_tree_name_t **)OutP->tree_info) = tree_infoP;
		msgh_simple = FALSE;
	}
	else {
		memcpy(OutP->tree_info, tree_info, 44 * tree_infoCnt);
	}

	OutP->tree_infoType.msgtl_number = 11 * tree_infoCnt;
	msgh_size += (OutP->tree_infoType.msgtl_header.msgt_inline) ? 44 * tree_infoCnt : sizeof(ipc_info_tree_name_t *);

	OutP = (Reply *) OutHeadP;
	if (!msgh_simple)
		OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = msgh_size;
}

/* Routine mach_port_dnrequest_info */
mig_internal void _Xmach_port_dnrequest_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t nameType;
		mach_port_t name;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t totalType;
		unsigned total;
		mach_msg_type_t usedType;
		unsigned used;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_port_dnrequest_info
		(ipc_space_t task, mach_port_t name, unsigned *total, unsigned *used);

	const mach_msg_type_t nameCheck = {
		/* msgt_name = */		15,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t totalType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t usedType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	ipc_space_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->nameType, &nameCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_space((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = mach_port_dnrequest_info(task, In0P->name, &OutP->total, &OutP->used);
	space_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 48;

	OutP->totalType = totalType;

	OutP->usedType = usedType;
}

/* Routine host_stack_usage */
mig_internal void _Xhost_stack_usage
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t reservedType;
		vm_size_t reserved;
		mach_msg_type_t totalType;
		unsigned total;
		mach_msg_type_t spaceType;
		vm_size_t space;
		mach_msg_type_t residentType;
		vm_size_t resident;
		mach_msg_type_t maxusageType;
		vm_size_t maxusage;
		mach_msg_type_t maxstackType;
		vm_offset_t maxstack;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t host_stack_usage
		(host_t host, vm_size_t *reserved, unsigned *total, vm_size_t *space, vm_size_t *resident, vm_size_t *maxusage, vm_offset_t *maxstack);

	const mach_msg_type_t reservedType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t totalType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t spaceType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t residentType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t maxusageType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t maxstackType = {
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

	OutP->RetCode = host_stack_usage(convert_port_to_host((ipc_port_t) In0P->Head.msgh_request_port), &OutP->reserved, &OutP->total, &OutP->space, &OutP->resident, &OutP->maxusage, &OutP->maxstack);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 80;

	OutP->reservedType = reservedType;

	OutP->totalType = totalType;

	OutP->spaceType = spaceType;

	OutP->residentType = residentType;

	OutP->maxusageType = maxusageType;

	OutP->maxstackType = maxstackType;
}

/* Routine processor_set_stack_usage */
mig_internal void _Xprocessor_set_stack_usage
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t totalType;
		unsigned total;
		mach_msg_type_t spaceType;
		vm_size_t space;
		mach_msg_type_t residentType;
		vm_size_t resident;
		mach_msg_type_t maxusageType;
		vm_size_t maxusage;
		mach_msg_type_t maxstackType;
		vm_offset_t maxstack;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t processor_set_stack_usage
		(processor_set_t pset, unsigned *total, vm_size_t *space, vm_size_t *resident, vm_size_t *maxusage, vm_offset_t *maxstack);

	const mach_msg_type_t totalType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t spaceType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t residentType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t maxusageType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t maxstackType = {
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
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	pset = convert_port_to_pset_name((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = processor_set_stack_usage(pset, &OutP->total, &OutP->space, &OutP->resident, &OutP->maxusage, &OutP->maxstack);
	pset_deallocate(pset);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 72;

	OutP->totalType = totalType;

	OutP->spaceType = spaceType;

	OutP->residentType = residentType;

	OutP->maxusageType = maxusageType;

	OutP->maxstackType = maxstackType;
}

/* Routine host_virtual_physical_table_info */
mig_internal void _Xhost_virtual_physical_table_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t infoCntType;
		mach_msg_type_number_t infoCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_long_t infoType;
		hash_info_bucket_t info[512];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t host_virtual_physical_table_info
		(host_t host, hash_info_bucket_array_t *info, mach_msg_type_number_t *infoCnt);

	boolean_t msgh_simple;
	const mach_msg_type_t infoCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t infoType = {
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
		/* msgtl_number = */	512,
	};

	mach_msg_type_number_t infoCnt;

	hash_info_bucket_t *infoP;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->infoCntType, &infoCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	infoP = OutP->info;
	infoCnt = 512;
	if (In0P->infoCnt < infoCnt)
		infoCnt = In0P->infoCnt;

	OutP->RetCode = host_virtual_physical_table_info(convert_port_to_host((ipc_port_t) In0P->Head.msgh_request_port), &infoP, &infoCnt);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msgh_simple = TRUE;

	OutP->infoType = infoType;
	if (infoP != OutP->info) {
		OutP->infoType.msgtl_header.msgt_inline = FALSE;
		OutP->infoType.msgtl_header.msgt_deallocate = TRUE;
		*((hash_info_bucket_t **)OutP->info) = infoP;
		msgh_simple = FALSE;
	}

	OutP->infoType.msgtl_number = infoCnt;
	OutP->Head.msgh_size = 44 + ((OutP->infoType.msgtl_header.msgt_inline) ? 4 * infoCnt : sizeof(hash_info_bucket_t *));

	if (!msgh_simple)
		OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
}

/* Routine host_load_symbol_table */
mig_internal void _Xhost_load_symbol_table
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t taskType;
		ipc_port_t task;
		mach_msg_type_long_t nameType;
		symtab_name_t name;
		mach_msg_type_long_t symtabType;
		vm_offset_t symtab;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t host_load_symbol_table
		(host_t host, task_t task, symtab_name_t name, vm_offset_t symtab, mach_msg_type_number_t symtabCnt);

	task_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 92) ||
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

#if	TypeCheck
	if ((In0P->nameType.msgtl_header.msgt_inline != TRUE) ||
	    (In0P->nameType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->nameType.msgtl_name != 12) ||
	    (In0P->nameType.msgtl_number != 1) ||
	    (In0P->nameType.msgtl_size != 256))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if ((In0P->symtabType.msgtl_header.msgt_inline != FALSE) ||
	    (In0P->symtabType.msgtl_header.msgt_longform != TRUE) ||
	    (In0P->symtabType.msgtl_name != 9) ||
	    (In0P->symtabType.msgtl_size != 8))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_task(In0P->task);

	OutP->RetCode = host_load_symbol_table(convert_port_to_host_priv((ipc_port_t) In0P->Head.msgh_request_port), task, In0P->name, In0P->symtab, In0P->symtabType.msgtl_number);
	task_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	if (IP_VALID(In0P->task))
		ipc_port_release_send(In0P->task);
}

/* Routine mach_port_kernel_object */
mig_internal void _Xmach_port_kernel_object
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t nameType;
		mach_port_t name;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t object_typeType;
		unsigned object_type;
		mach_msg_type_t object_addrType;
		vm_offset_t object_addr;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_port_kernel_object
		(ipc_space_t task, mach_port_t name, unsigned *object_type, vm_offset_t *object_addr);

	const mach_msg_type_t nameCheck = {
		/* msgt_name = */		15,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t object_typeType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t object_addrType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	ipc_space_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->nameType, &nameCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_space((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = mach_port_kernel_object(task, In0P->name, &OutP->object_type, &OutP->object_addr);
	space_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 48;

	OutP->object_typeType = object_typeType;

	OutP->object_addrType = object_addrType;
}

/* Routine mach_vm_region_info */
mig_internal void _Xmach_vm_region_info
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
		mach_msg_type_t regionType;
		vm_region_info_t region;
		mach_msg_type_t objectType;
		ipc_port_t object;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_vm_region_info
		(vm_map_t task, vm_address_t address, vm_region_info_t *region, ipc_port_t *object);

	const mach_msg_type_t addressCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t regionType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		11,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t objectType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_map_t task;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->addressType, &addressCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	task = convert_port_to_map((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = mach_vm_region_info(task, In0P->address, &OutP->region, &OutP->object);
	vm_map_deallocate(task);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 88;

	OutP->regionType = regionType;

	OutP->objectType = objectType;
}

/* Routine mach_vm_object_info */
mig_internal void _Xmach_vm_object_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t infoType;
		vm_object_info_t info;
		mach_msg_type_t shadowType;
		ipc_port_t shadow;
		mach_msg_type_t copyType;
		ipc_port_t copy;
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_vm_object_info
		(vm_object_t object, vm_object_info_t *info, ipc_port_t *shadow, ipc_port_t *copy);

	const mach_msg_type_t infoType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		14,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t shadowType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_t copyType = {
		/* msgt_name = */		17,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	vm_object_t object;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	object = vm_object_lookup_name((ipc_port_t) In0P->Head.msgh_request_port);

	OutP->RetCode = mach_vm_object_info(object, &OutP->info, &OutP->shadow, &OutP->copy);
	vm_object_deallocate(object);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
	OutP->Head.msgh_size = 108;

	OutP->infoType = infoType;

	OutP->shadowType = shadowType;

	OutP->copyType = copyType;
}

/* Routine mach_vm_object_pages */
mig_internal void _Xmach_vm_object_pages
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t pagesCntType;
		mach_msg_type_number_t pagesCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_long_t pagesType;
		vm_page_info_t pages[85];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t mach_vm_object_pages
		(vm_object_t object, vm_page_info_array_t *pages, mach_msg_type_number_t *pagesCnt);

	boolean_t msgh_simple;
	const mach_msg_type_t pagesCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t pagesType = {
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
		/* msgtl_number = */	510,
	};

	vm_object_t object;
	mach_msg_type_number_t pagesCnt;

	vm_page_info_t *pagesP;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->pagesCntType, &pagesCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	object = vm_object_lookup_name((ipc_port_t) In0P->Head.msgh_request_port);

	pagesP = OutP->pages;
	pagesCnt = 85;
	if (In0P->pagesCnt < pagesCnt)
		pagesCnt = In0P->pagesCnt;

	OutP->RetCode = mach_vm_object_pages(object, &pagesP, &pagesCnt);
	vm_object_deallocate(object);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msgh_simple = TRUE;

	OutP->pagesType = pagesType;
	if (pagesP != OutP->pages) {
		OutP->pagesType.msgtl_header.msgt_inline = FALSE;
		OutP->pagesType.msgtl_header.msgt_deallocate = TRUE;
		*((vm_page_info_t **)OutP->pages) = pagesP;
		msgh_simple = FALSE;
	}

	OutP->pagesType.msgtl_number = 6 * pagesCnt;
	OutP->Head.msgh_size = 44 + ((OutP->pagesType.msgtl_header.msgt_inline) ? 24 * pagesCnt : sizeof(vm_page_info_t *));

	if (!msgh_simple)
		OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
}

/* Routine host_slab_info */
mig_internal void _Xhost_slab_info
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t infoCntType;
		mach_msg_type_number_t infoCnt;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_long_t infoType;
		cache_info_t info[26];
	} Reply;

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t host_slab_info
		(host_t host, cache_info_array_t *info, mach_msg_type_number_t *infoCnt);

	boolean_t msgh_simple;
	const mach_msg_type_t infoCntCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	const mach_msg_type_long_t infoType = {
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
		/* msgtl_number = */	494,
	};

	mach_msg_type_number_t infoCnt;

	cache_info_t *infoP;

#if	TypeCheck
	if ((In0P->Head.msgh_size != 32) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->infoCntType, &infoCntCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	infoP = OutP->info;
	infoCnt = 26;
	if (In0P->infoCnt < infoCnt)
		infoCnt = In0P->infoCnt;

	OutP->RetCode = host_slab_info(convert_port_to_host((ipc_port_t) In0P->Head.msgh_request_port), &infoP, &infoCnt);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	msgh_simple = TRUE;

	OutP->infoType = infoType;
	if (infoP != OutP->info) {
		OutP->infoType.msgtl_header.msgt_inline = FALSE;
		OutP->infoType.msgtl_header.msgt_deallocate = TRUE;
		*((cache_info_t **)OutP->info) = infoP;
		msgh_simple = FALSE;
	}

	OutP->infoType.msgtl_number = 19 * infoCnt;
	OutP->Head.msgh_size = 44 + ((OutP->infoType.msgtl_header.msgt_inline) ? 76 * infoCnt : sizeof(cache_info_t *));

	if (!msgh_simple)
		OutP->Head.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
}

mig_routine_t mach_debug_server_routines[] = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		_Xmach_port_get_srights,
		_Xhost_ipc_hash_info,
		_Xhost_ipc_marequest_info,
		_Xmach_port_space_info,
		_Xmach_port_dnrequest_info,
		0,
		0,
		_Xhost_stack_usage,
		_Xprocessor_set_stack_usage,
		_Xhost_virtual_physical_table_info,
		_Xhost_load_symbol_table,
		_Xmach_port_kernel_object,
		_Xmach_vm_region_info,
		_Xmach_vm_object_info,
		_Xmach_vm_object_pages,
		_Xhost_slab_info,
};

mig_external boolean_t mach_debug_server
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

	if ((InP->msgh_id > 3022) || (InP->msgh_id < 3000) ||
	    ((routine = mach_debug_server_routines[InP->msgh_id - 3000]) == 0)) {
		OutP->RetCode = MIG_BAD_ID;
		return FALSE;
	}
	(*routine) (InP, &OutP->Head);
	return TRUE;
}

mig_external mig_routine_t mach_debug_server_routine
	(const mach_msg_header_t *InHeadP)
{
	int msgh_id;

	msgh_id = InHeadP->msgh_id - 3000;

	if ((msgh_id > 22) || (msgh_id < 0))
		return 0;

	return mach_debug_server_routines[msgh_id];
}

