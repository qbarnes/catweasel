/****************************************************************************
 ****************************************************************************
 *
 * format/fill.c
 *
 ****************************************************************************
 ****************************************************************************/





/*
 * UGLY: cosmetical: to consolidate naming it would be better to prepend
 *       format_ to each function and FORMAT_ for defines for all files in
 *       this directory
 */



#include <stdio.h>
#include <string.h>

#include "../error.h"
#include "../debug.h"
#include "../verbose.h"
#include "../global.h"
#include "../options.h"
#include "../disk.h"
#include "../fifo.h"
#include "../format.h"
#include "container.h"
#include "histogram.h"
#include "setvalue.h"




/****************************************************************************
 *
 * functions for sector and track handling
 *
 ****************************************************************************/




/****************************************************************************
 * fill_write_pattern2
 ****************************************************************************/
static int
fill_write_pattern2(
	struct fill			*fll,
	unsigned char			*data,
	int				*len,
	int				*values)

	{
	int				b, i, j, l, v;

	for (i = l = 0; (values[i] != -1) && (*len > 0); )
		{
		b = values[i++];
		v = values[i++];
		if (*len < b) b = *len;
		*len -= b;
		for (j = 0; j < b; j++) data[l++] = v;
		}
	return (l);
	}



/****************************************************************************
 * fill_write_pattern
 ****************************************************************************/
static void
fill_write_pattern(
	struct fill			*fll,
	unsigned char			*data,
	int				len)

	{
	int				values1[] =
		{
		0x80, 0x78,   0x80, 0x70,   0x80, 0x68,   0x80, 0x60,
		0x80, 0x58,   0x80, 0x50,   0x80, 0x48,   0x80, 0x40,
		0x80, 0x38,   0x80, 0x30,   0x80, 0x2c,   0x80, 0x28,
		0x80, 0x24,   0x80, 0x20,   0x80, 0x1e,   0x80, 0x1c,
		0x80, 0x1a,   0x80, 0x18,   0x80, 0x16,   0x80, 0x14,
		0x80, 0x12,   0x80, 0x10,   0x80, 0x0e,   0x80, 0x0c,
		0x80, 0x0a,  0x100, 0x40,   -1, -1
		};
	int				values2[] =
		{
		0x80, 0x60,   0x10, 0x12,   0x10, 0x24,   0x10, 0x12,
		0x80, 0x60,   0x10, 0x24,   0x10, 0x36,   0x10, 0x24,
		0x80, 0x60,   0x10, 0x12,   0x10, 0x36,   0x10, 0x12,
		0x80, 0x60,   0x10, 0x1c,   0x10, 0x38,   0x10, 0x1c,
		0x80, 0x60,   0x10, 0x38,   0x10, 0x54,   0x10, 0x38,
		0x80, 0x60,   0x10, 0x1c,   0x10, 0x54,   0x10, 0x1c,
		-1, -1
		};
	int				values3[] =
		{
		0x80, 0x60,   0x01, 0x38,   0x01, 0x1c,   0x01, 0x38,
		0x01, 0x1c,   0x01, 0x38,   0x01, 0x1c,   0x01, 0x38,
		0x02, 0x1c,   0x02, 0x38,   0x02, 0x1c,   0x02, 0x38,
		0x03, 0x1c,   0x03, 0x38,   0x03, 0x1c,   0x03, 0x38,
		0x04, 0x1c,   0x04, 0x38,   0x04, 0x1c,   0x04, 0x38,

		0x80, 0x60,   0x01, 0x34,   0x01, 0x1a,   0x01, 0x34,
		0x01, 0x1a,   0x01, 0x34,   0x01, 0x1a,   0x01, 0x34,
		0x02, 0x1a,   0x02, 0x34,   0x02, 0x1a,   0x02, 0x34,
		0x03, 0x1a,   0x03, 0x34,   0x03, 0x1a,   0x03, 0x34,
		0x04, 0x1a,   0x04, 0x34,   0x04, 0x1a,   0x04, 0x34,

		0x80, 0x60,   0x01, 0x30,   0x01, 0x18,   0x01, 0x30,
		0x01, 0x18,   0x01, 0x30,   0x01, 0x18,   0x01, 0x30,
		0x02, 0x18,   0x02, 0x30,   0x02, 0x18,   0x02, 0x30,
		0x03, 0x18,   0x03, 0x30,   0x03, 0x18,   0x03, 0x30,
		0x04, 0x18,   0x04, 0x30,   0x04, 0x18,   0x04, 0x30,

		0x80, 0x60,   0x01, 0x2c,   0x01, 0x16,   0x01, 0x2c,
		0x01, 0x16,   0x01, 0x2c,   0x01, 0x16,   0x01, 0x2c,
		0x02, 0x16,   0x02, 0x2c,   0x02, 0x16,   0x02, 0x2c,
		0x03, 0x16,   0x03, 0x2c,   0x03, 0x16,   0x03, 0x2c,
		0x04, 0x16,   0x04, 0x2c,   0x04, 0x16,   0x04, 0x2c,

		0x80, 0x60,   0x01, 0x28,   0x01, 0x14,   0x01, 0x28,
		0x01, 0x14,   0x01, 0x28,   0x01, 0x14,   0x01, 0x28,
		0x02, 0x14,   0x02, 0x28,   0x02, 0x14,   0x02, 0x28,
		0x03, 0x14,   0x03, 0x28,   0x03, 0x14,   0x03, 0x28,
		0x04, 0x14,   0x04, 0x28,   0x04, 0x14,   0x04, 0x28,

		0x80, 0x60,   0x01, 0x24,   0x01, 0x12,   0x01, 0x24,
		0x01, 0x12,   0x01, 0x24,   0x01, 0x12,   0x01, 0x24,
		0x02, 0x12,   0x02, 0x24,   0x02, 0x12,   0x02, 0x24,
		0x03, 0x12,   0x03, 0x24,   0x03, 0x12,   0x03, 0x24,
		0x04, 0x12,   0x04, 0x24,   0x04, 0x12,   0x04, 0x24,

		0x80, 0x60,   0x01, 0x24,   0x01, 0x12,   0x01, 0x24,
		0x01, 0x12,   0x01, 0x24,   0x01, 0x12,   0x01, 0x24,
		0x01, 0x12,   0x01, 0x24,   0x01, 0x12,   0x01, 0x24,
		0x01, 0x12,   0x01, 0x24,   0x01, 0x12,   0x01, 0x24,

		0x80, 0x60,   0x01, 0x2e,   0x01, 0x08,   0x01, 0x2e,
		0x01, 0x08,   0x01, 0x2e,   0x01, 0x08,   0x01, 0x2e,
		0x01, 0x08,   0x01, 0x2e,   0x01, 0x08,   0x01, 0x2e,
		0x01, 0x08,   0x01, 0x2e,   0x01, 0x08,   0x01, 0x2e,

		0x80, 0x60,   0x01, 0x32,   0x01, 0x04,   0x01, 0x32,
		0x01, 0x04,   0x01, 0x32,   0x01, 0x04,   0x01, 0x32,
		0x01, 0x04,   0x01, 0x32,   0x01, 0x04,   0x01, 0x32,
		0x01, 0x04,   0x01, 0x32,   0x01, 0x04,   0x01, 0x32,

		0x80, 0x60,   0x01, 0x36,   0x01, 0x12,   0x01, 0x36,
		0x01, 0x12,   0x01, 0x36,   0x01, 0x12,   0x01, 0x36,
		0x01, 0x12,   0x01, 0x36,   0x01, 0x12,   0x01, 0x36,
		0x01, 0x12,   0x01, 0x36,   0x01, 0x12,   0x01, 0x36,

		0x80, 0x60,   0x01, 0x12,   0x01, 0x24,   0x01, 0x36,
		0x01, 0x12,   0x01, 0x24,   0x01, 0x36,   0x01, 0x12,
		0x01, 0x24,   0x01, 0x36,   0x01, 0x12,   0x01, 0x24,
		0x01, 0x36,   0x01, 0x12,   0x01, 0x24,   0x01, 0x36,

		0x80, 0x60,   0x01, 0x36,   0x01, 0x24,   0x01, 0x12,
		0x01, 0x36,   0x01, 0x24,   0x01, 0x12,   0x01, 0x36,
		0x01, 0x24,   0x01, 0x12,   0x01, 0x36,   0x01, 0x24,
		0x01, 0x12,   0x01, 0x36,   0x01, 0x24,   0x01, 0x12,

		-1, -1
		};
	int				values4[] =
		{
		0x80, 0x60,

		0x01, 0x60,   0x01, 0x5e,   0x01, 0x5c,   0x01, 0x5a,
		0x01, 0x58,   0x01, 0x56,   0x01, 0x54,   0x01, 0x52,
		0x01, 0x50,   0x01, 0x4e,   0x01, 0x4c,   0x01, 0x4a,
		0x01, 0x48,   0x01, 0x46,   0x01, 0x44,   0x01, 0x42,
		0x01, 0x40,   0x01, 0x3e,   0x01, 0x3c,   0x01, 0x3a,
		0x01, 0x38,   0x01, 0x36,   0x01, 0x34,   0x01, 0x32,
		0x01, 0x30,   0x01, 0x2e,   0x01, 0x2c,   0x01, 0x2a,
		0x01, 0x28,   0x01, 0x26,   0x01, 0x24,   0x01, 0x22,
		0x01, 0x20,   0x01, 0x1e,   0x01, 0x1c,   0x01, 0x1a,
		0x01, 0x18,   0x01, 0x16,   0x01, 0x14,   0x01, 0x12,

		0x80, 0x60,

		0x01, 0x12,   0x01, 0x14,   0x01, 0x12,   0x01, 0x14,
		0x01, 0x12,   0x01, 0x14,   0x01, 0x12,   0x01, 0x14,
		0x01, 0x12,   0x01, 0x14,   0x01, 0x12,   0x01, 0x14,
		0x01, 0x12,   0x01, 0x14,   0x01, 0x12,   0x01, 0x14,

		0x02, 0x12,   0x02, 0x14,   0x02, 0x12,   0x02, 0x14,
		0x02, 0x12,   0x02, 0x14,   0x02, 0x12,   0x02, 0x14,
		0x02, 0x12,   0x02, 0x14,   0x02, 0x12,   0x02, 0x14,
		0x02, 0x12,   0x02, 0x14,   0x02, 0x12,   0x02, 0x14,

		0x01, 0x12,   0x01, 0x16,   0x01, 0x12,   0x01, 0x16,
		0x01, 0x12,   0x01, 0x16,   0x01, 0x12,   0x01, 0x16,
		0x01, 0x12,   0x01, 0x16,   0x01, 0x12,   0x01, 0x16,
		0x01, 0x12,   0x01, 0x16,   0x01, 0x12,   0x01, 0x16,

		0x02, 0x12,   0x02, 0x16,   0x02, 0x12,   0x02, 0x16,
		0x02, 0x12,   0x02, 0x16,   0x02, 0x12,   0x02, 0x16,
		0x02, 0x12,   0x02, 0x16,   0x02, 0x12,   0x02, 0x16,
		0x02, 0x12,   0x02, 0x16,   0x02, 0x12,   0x02, 0x16,

		-1, -1
		};
	int				l = 0;

	/* write out long blocks with varying values */

	l += fill_write_pattern2(fll, &data[l], &len, values1);

	/* write out shorter blocks with varying values */

	l += fill_write_pattern2(fll, &data[l], &len, values2);

	/* specific pattern for pre/postcompensation */

	l += fill_write_pattern2(fll, &data[l], &len, values3);

	/* specific pattern for testing resolution */

	l += fill_write_pattern2(fll, &data[l], &len, values4);

	/* fill the rest with fill_value */

	if (len > 0) memset(&data[l], fll->wr.fill_value, len);
	}



/****************************************************************************
 * fill_statistics
 ****************************************************************************/
static int
fill_statistics(
	union format			*fmt,
	struct fifo			*ffo_l0,
	int				cwtool_track,
	int				format_track,
	int				format_side)

	{
	histogram_normal(ffo_l0, cwtool_track, -1, -1);
	return (1);
	}



/****************************************************************************
 * fill_read_track
 ****************************************************************************/
static int
fill_read_track(
	union format			*fmt,
	struct container		*con,
	struct fifo			*ffo_l0,
	struct fifo			*ffo_l3,
	struct disk_sector		*dsk_sct,
	int				cwtool_track,
	int				format_track,
	int				format_side)

	{
	debug_error();
	return (0);
	}



/****************************************************************************
 * fill_write_track
 ****************************************************************************/
static int
fill_write_track(
	union format			*fmt,
	struct fifo			*ffo_l3,
	struct disk_sector		*dsk_sct,
	struct fifo			*ffo_l0,
	unsigned char			*data,
	int				cwtool_track,
	int				format_track,
	int				format_side)

	{
	unsigned char			*data_l0 = fifo_get_data(ffo_l0);
	unsigned char			val = fmt->fll.wr.fill_value;
	int				len = fmt->fll.wr.fill_length;

	debug_error_condition(len > fifo_get_limit(ffo_l0));
	if (fmt->fll.wr.mode == 1) fill_write_pattern(&fmt->fll, data_l0, len);
	else memset(data_l0, val, len);
	fifo_set_wr_ofs(ffo_l0, len);
	fifo_set_flags(ffo_l0, FIFO_FLAG_WRITABLE);
	return (1);
	}




/****************************************************************************
 *
 * functions for configuration
 *
 ****************************************************************************/




#define MAGIC_MODE			1
#define MAGIC_FILL_VALUE		2
#define MAGIC_FILL_LENGTH		3



/****************************************************************************
 * fill_set_defaults
 ****************************************************************************/
static void
fill_set_defaults(
	union format			*fmt)

	{
	const static struct fill	fll =
		{
		.wr =
			{
			.mode        = 0,
			.fill_value  = 0x78,
			.fill_length = GLOBAL_MAX_TRACK_SIZE
			}
		};

	debug_message(GENERIC, 2, "setting defaults");
	fmt->fll = fll;
	}



/****************************************************************************
 * fill_set_write_option
 ****************************************************************************/
static int
fill_set_write_option(
	union format			*fmt,
	int				magic,
	int				val,
	int				ofs)

	{
	debug_message(GENERIC, 2, "setting write option magic = %d, val = %d, ofs = %d", magic, val, ofs);
	if (magic == MAGIC_MODE) return (setvalue_uchar(&fmt->fll.wr.mode, val, 0, 0xff));
	if (magic == MAGIC_FILL_VALUE) return (setvalue_uchar(&fmt->fll.wr.fill_value, val, 3, 0x7f));
	debug_error_condition(magic != MAGIC_FILL_LENGTH);
	return (setvalue_uint(&fmt->fll.wr.fill_length, val, GLOBAL_MIN_TRACK_SIZE, GLOBAL_MAX_TRACK_SIZE));
	}



/****************************************************************************
 * fill_set_dummy_option
 ****************************************************************************/
static int
fill_set_dummy_option(
	union format			*fmt,
	int				magic,
	int				val,
	int				ofs)

	{
	debug_error();
	return (0);
	}



/****************************************************************************
 * fill_get_sector_size
 ****************************************************************************/
static int
fill_get_sector_size(
	union format			*fmt,
	int				sector)

	{
	debug_error_condition(sector != -1);
	return (0);
	}



/****************************************************************************
 * fill_get_sectors
 ****************************************************************************/
static int
fill_get_sectors(
	union format			*fmt)

	{
	return (0);
	}



/****************************************************************************
 * fill_get_flags
 ****************************************************************************/
static int
fill_get_flags(
	union format			*fmt)

	{
	return (FORMAT_FLAG_NONE);
	}



/****************************************************************************
 * fill_get_data_offset
 ****************************************************************************/
static int
fill_get_data_offset(
	union format			*fmt)

	{
	return (-1);
	}



/****************************************************************************
 * fill_get_data_size
 ****************************************************************************/
static int
fill_get_data_size(
	union format			*fmt)

	{
	return (-1);
	}



/****************************************************************************
 * fill_write_options
 ****************************************************************************/
static struct format_option		fill_write_options[] =
	{
	FORMAT_OPTION_INTEGER("mode",        MAGIC_MODE,        1),
	FORMAT_OPTION_INTEGER("fill_value",  MAGIC_FILL_VALUE,  1),
	FORMAT_OPTION_INTEGER("fill_length", MAGIC_FILL_LENGTH, 1),
	FORMAT_OPTION_END
	};



/****************************************************************************
 * fill_dummy_options
 ****************************************************************************/
static struct format_option		fill_dummy_options[] =
	{
	FORMAT_OPTION_END
	};




/****************************************************************************
 *
 * global functions
 *
 ****************************************************************************/




/****************************************************************************
 * fill_format_desc
 ****************************************************************************/
struct format_desc			fill_format_desc =
	{
	.name             = "fill",
	.level            = -1,
	.set_defaults     = fill_set_defaults,
	.set_read_option  = fill_set_dummy_option,
	.set_write_option = fill_set_write_option,
	.set_rw_option    = fill_set_dummy_option,
	.get_sectors      = fill_get_sectors,
	.get_sector_size  = fill_get_sector_size,
	.get_flags        = fill_get_flags,
	.get_data_offset  = fill_get_data_offset,
	.get_data_size    = fill_get_data_size,
	.track_statistics = fill_statistics,
	.track_read       = fill_read_track,
	.track_write      = fill_write_track,
	.fmt_opt_rd       = fill_dummy_options,
	.fmt_opt_wr       = fill_write_options,
	.fmt_opt_rw       = fill_dummy_options
	};
/******************************************************** Karsten Scheibler */