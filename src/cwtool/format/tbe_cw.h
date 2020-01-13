/****************************************************************************
 ****************************************************************************
 *
 * format/tbe_cw.h
 *
 ****************************************************************************
 ****************************************************************************/





#ifndef CWTOOL_TBE_CW_H
#define CWTOOL_TBE_CW_H

#include "bounds.h"

struct tbe_cw
	{
	struct
		{
		unsigned char		sync_length;
		unsigned char		flags;
		unsigned char		reserved[2];
		}			rd;
	struct
		{
		unsigned short		prolog_length;
		unsigned short		epilog_length;
		short			precomp[36];
		}			wr;
	struct
		{
		unsigned char		sectors;
		unsigned char		format_id;
		unsigned short		crc16_init_value;
		unsigned short		sector0_size;
		unsigned short		sector_size;
		struct bounds		bnd[6];
		}			rw;
	};

/*
 * do not put this include at the beginning of the file, because ../format.h
 * also includes this file to construct union format. so the above struct
 * has to be known at time of inclusion
 */

#include "../format.h"

extern struct format_desc		tbe_cw_format_desc;



#endif /* !CWTOOL_TBE_CW_H */
/******************************************************** Karsten Scheibler */
