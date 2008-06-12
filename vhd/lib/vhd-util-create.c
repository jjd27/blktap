/* Copyright (c) 2007, XenSource Inc.
 * All rights reserved.
 *
 * XenSource proprietary code.
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "libvhd.h"

int
vhd_util_create(int argc, char **argv)
{
	char *name;
	uint64_t size;
	int c, sparse, err;
	vhd_flag_creat_t flags;

	err       = -EINVAL;
	size      = 0;
	sparse    = 1;
	name      = NULL;
	flags     = 0;

	if (!argc || !argv)
		goto usage;

	optind = 0;
	while ((c = getopt(argc, argv, "n:s:rbh")) != -1) {
		switch (c) {
		case 'n':
			name = optarg;
			break;
		case 's':
			err  = 0;
			size = strtoull(optarg, NULL, 10);
			break;
		case 'r':
			sparse = 0;
			break;
		case 'b':
			vhd_flag_set(flags, VHD_FLAG_CREAT_FILE_SIZE_FIXED);
			break;
		case 'h':
		default:
			goto usage;
		}
	}

	if (err || !name || optind != argc)
		goto usage;

	return vhd_create(name, size << 20,
				  (sparse ? HD_TYPE_DYNAMIC : HD_TYPE_FIXED),
				  flags);

usage:
	printf("options: <-n name> <-s size (MB)> [-r reserve] "
			"[-b file_is_fixed_size] [-h help]\n");
	return -EINVAL;
}