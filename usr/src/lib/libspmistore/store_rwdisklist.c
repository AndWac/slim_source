/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)store_rwdisklist.c	1.3	07/10/09 SMI"


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "spmistore_api.h"

#define	DISK_FILE_PATH "/tmp/disk_list.bin"

/*
 * *********************************************************************
 * FUNCTION NAME: WriteDiskList
 *
 * DESCRIPTION:
 *   This function writes the global current disk list out to a binary
 *   that can later be read back.  This capability is used for failure
 *   recovery along with passing the disk information between a child
 *   process, where the disk list is generated, and the parent process,
 *   where the disk list is processed.
 *
 * RETURN:
 *  TYPE			DESCRIPTION
 *   int			D_OK = Success
 *				D_FAILED = Failure
 * PARAMETERS:
 *  TYPE			DESCRIPTION
 *
 * DESIGNER/PROGRAMMER: Craig Vosburgh/RMTC (719)528-3647
 * *********************************************************************
 */

int WriteDiskList(void)
{
	int NumDisks = 0;
	Disk_t	*dp;
	int DiskFileFD;

	/*
	 * Walk the current disk list and determine how many disks
	 * are in the list.
	 */

	WALK_DISK_LIST(dp) {
		NumDisks++;
	}

	/*
	 * Open the disk file
	 */

	if ((DiskFileFD = open(DISK_FILE_PATH, O_WRONLY | O_CREAT)) == -1) {
		return (D_FAILED);
	}

	/*
	 * Write out the number of disks in the disk list
	 */

	if (write(DiskFileFD, &NumDisks, sizeof (NumDisks)) == -1) {
		return (D_FAILED);
	}

	/*
	 * walk the list writing out the contents of the disk link
	 * into the file
	 */

	WALK_DISK_LIST(dp) {

		/*
		 * write out the disk link to file
		 */

		if (write(DiskFileFD, dp, sizeof (Disk_t)) == -1) {
			return (D_FAILED);
		}
	}
	return (D_OK);
}

/*
 * *********************************************************************
 * FUNCTION NAME: ReadDiskList
 *
 * DESCRIPTION:
 *  This function reads in the disk list file generated by WriteDiskList
 *  and stores the reconstructed disk list into the HeadDP variable.
 *
 * RETURN:
 *  TYPE			DESCRIPTION
 *   int			D_OK = Success
 *				D_FAILED = Failure
 *
 * PARAMETERS:
 *  TYPE			DESCRIPTION
 *  Disk_t **			The double pointer that is set to point
 *				at the disk list read in from file.
 *
 * DESIGNER/PROGRAMMER: Craig Vosburgh/RMTC (719)528-3647
 * *********************************************************************
 */

int ReadDiskList(Disk_t **HeadDP)
{
	int NumDisks = 0;
	Disk_t	*CurrentDP;
	Disk_t	*PrevDP = NULL;
	int DiskFileFD;
	int i;

	*HeadDP = NULL;

	/*
	 * Open the disk file that contains the disk list
	 */

	if ((DiskFileFD = open(DISK_FILE_PATH, O_RDONLY)) == -1) {
		return (D_FAILED);
	}

	/*
	 * Read how many disks are in the list
	 */

	if (read(DiskFileFD, &NumDisks, sizeof (NumDisks)) == -1) {
		return (D_FAILED);
	}

	/*
	 * Loop for all of the disks in the list
	 */

	for (i = 0; i < NumDisks; i++) {

		/*
		 * Malloc out space to hold the link just read from the disk
		 */

		if ((CurrentDP = (Disk_t *) malloc(sizeof (Disk_t))) == NULL) {
			return (D_FAILED);
		}

		/*
		 * Read in the information for the disk
		 */

		if (read(DiskFileFD, CurrentDP, sizeof (Disk_t)) == -1) {
			free(CurrentDP);
			return (D_FAILED);
		}

		/*
		 * If the head pointer is NULL then set it to the current disk
		 */

		if (*HeadDP == NULL)
			*HeadDP = CurrentDP;

		/*
		 * If the Previous pointer is not NULL then set the
		 * previous links next pointer to the current link
		 */

		if (PrevDP) {
			PrevDP->next = CurrentDP;
		}

		/*
		 * Point the Geom_t pointer in the Sdisk_t array at the
		 * Geom_t structure in the Disk_t structure.
		 */

		CurrentDP->sdisk[0].geom = &(CurrentDP->geom);
		CurrentDP->sdisk[1].geom = &(CurrentDP->geom);
		CurrentDP->sdisk[2].geom = &(CurrentDP->geom);

		PrevDP = CurrentDP;

	}
	return (D_OK);
}
