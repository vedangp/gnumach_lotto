# Project:	Mach Lottery Scheduler (Kernel Modifications)
# Author:	Carl Waldspurger
# History:
#
#	06-Jun-94	Added rules for fundrx.
#	03-Jun-94	Added libmachid and libnetname to LIBS.
#	05-May-94	Renamed fx => fundx.  Added rules for unfund.
#	22-Apr-94	Added rules for fx.
#	19-Jan-94	Added rules for lscur.
#	18-Jan-94	Added rules for lstkt.
#	13-Jan-94	Added rules for lottostat.
#	09-Jan-94	Consolidated from various lotto Makefiles.
#	13-Dec-93	Added rules for rmcur.
#	05-Nov-93	First version written.

DEPENDENCIES	=
LIBS 		= ${LIBMACHID} ${LIBNETNAME} ${LIBMACH} ${LIBCMUCS}

PROGRAMS 	= mkcur rmcur mktkt rmtkt fund lottostat lstkt lscur \
		  fundx unfund fundrx 
ILIST		= mkcur rmcur mktkt rmtkt fund lottostat lstkt lscur \
		  fundx unfund fundrx 
IDIR 		= /bin/

mkcur_OFILES		= mkcur.o lotto_util.o
rmcur_OFILES		= rmcur.o lotto_util.o
mktkt_OFILES		= mktkt.o lotto_util.o
rmtkt_OFILES		= rmtkt.o lotto_util.o
fund_OFILES		= fund.o  lotto_util.o
lottostat_OFILES	= lottostat.o lotto_util.o
lstkt_OFILES		= lstkt.o lotto_util.o
lscur_OFILES		= lscur.o lotto_util.o
fundx_OFILES		= fundx.o lotto_util.o
unfund_OFILES		= unfund.o lotto_util.o
fundrx_OFILES		= fundrx.o lotto_util.o

.include	<${RULES_MK}>
