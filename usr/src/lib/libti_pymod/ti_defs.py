#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
"""TI defines"""

import re

H_FILE = open('/usr/include/admin/ti_api.h', 'r').read()

# Search ti_api.h for the #defines and place the name value
# pairs into a dictionary.
FINDER = re.compile(r'^#define\s+(\S+?)\s+(\S+?)$', re.M)
TI_DEFINES = dict(FINDER.findall(H_FILE))

TI_TARGET_TYPE_FDISK = int(TI_DEFINES['TI_TARGET_TYPE_FDISK'])
TI_TARGET_TYPE_VTOC = int(TI_DEFINES['TI_TARGET_TYPE_VTOC'])
TI_TARGET_TYPE_ZFS_RPOOL = int(TI_DEFINES['TI_TARGET_TYPE_ZFS_RPOOL'])
TI_TARGET_TYPE_ZFS_FS = int(TI_DEFINES['TI_TARGET_TYPE_ZFS_FS'])
TI_TARGET_TYPE_ZFS_VOLUME = int(TI_DEFINES['TI_TARGET_TYPE_ZFS_VOLUME'])
TI_TARGET_TYPE_BE = int(TI_DEFINES['TI_TARGET_TYPE_BE'])
TI_TARGET_TYPE_DC_UFS = int(TI_DEFINES['TI_TARGET_TYPE_DC_UFS'])
TI_TARGET_TYPE_DC_RAMDISK = int(TI_DEFINES['TI_TARGET_TYPE_DC_RAMDISK'])
TI_ATTR_TARGET_TYPE = TI_DEFINES['TI_ATTR_TARGET_TYPE'].strip('"')
TI_PROGRESS_MS_NUM = TI_DEFINES['TI_PROGRESS_MS_NUM'].strip('"')
TI_PROGRESS_MS_CURR = TI_DEFINES['TI_PROGRESS_MS_CURR'].strip('"')
TI_PROGRESS_MS_PERC = TI_DEFINES['TI_PROGRESS_MS_PERC'].strip('"')
TI_PROGRESS_MS_PERC_DONE = TI_DEFINES['TI_PROGRESS_MS_PERC_DONE'].strip('"')
TI_PROGRESS_TOTAL_TIME = TI_DEFINES['TI_PROGRESS_TOTAL_TIME'].strip('"')
TI_ATTR_FDISK_WDISK_FL = TI_DEFINES['TI_ATTR_FDISK_WDISK_FL'].strip('"')
TI_ATTR_FDISK_DISK_NAME = TI_DEFINES['TI_ATTR_FDISK_DISK_NAME'].strip('"')
TI_ATTR_FDISK_PART_NUM = TI_DEFINES['TI_ATTR_FDISK_PART_NUM'].strip('"')
TI_ATTR_FDISK_PART_IDS = TI_DEFINES['TI_ATTR_FDISK_PART_IDS'].strip('"')
TI_ATTR_FDISK_PART_ACTIVE = TI_DEFINES['TI_ATTR_FDISK_PART_ACTIVE'].strip('"')
TI_ATTR_FDISK_PART_BHEADS = TI_DEFINES['TI_ATTR_FDISK_PART_BHEADS'].strip('"')
TI_ATTR_FDISK_PART_BSECTS = TI_DEFINES['TI_ATTR_FDISK_PART_BSECTS'].strip('"')
TI_ATTR_FDISK_PART_BCYLS = TI_DEFINES['TI_ATTR_FDISK_PART_BCYLS'].strip('"')
TI_ATTR_FDISK_PART_EHEADS = TI_DEFINES['TI_ATTR_FDISK_PART_EHEADS'].strip('"')
TI_ATTR_FDISK_PART_ESECTS = TI_DEFINES['TI_ATTR_FDISK_PART_ESECTS'].strip('"')
TI_ATTR_FDISK_PART_ECYLS = TI_DEFINES['TI_ATTR_FDISK_PART_ECYLS'].strip('"')
TI_ATTR_FDISK_PART_RSECTS = TI_DEFINES['TI_ATTR_FDISK_PART_RSECTS'].strip('"')
TI_ATTR_FDISK_PART_NUMSECTS = \
    TI_DEFINES['TI_ATTR_FDISK_PART_NUMSECTS'].strip('"')
TI_ATTR_FDISK_PART_PRESERVE = \
    TI_DEFINES['TI_ATTR_FDISK_PART_PRESERVE'].strip('"')
TI_ATTR_SLICE_DEFAULT_LAYOUT = \
    TI_DEFINES['TI_ATTR_SLICE_DEFAULT_LAYOUT'].strip('"')
TI_ATTR_SLICE_DISK_NAME = TI_DEFINES['TI_ATTR_SLICE_DISK_NAME'].strip('"')
TI_ATTR_SLICE_NUM = TI_DEFINES['TI_ATTR_SLICE_NUM'].strip('"')
TI_ATTR_SLICE_PARTS = TI_DEFINES['TI_ATTR_SLICE_PARTS'].strip('"')
TI_ATTR_SLICE_TAGS = TI_DEFINES['TI_ATTR_SLICE_TAGS'].strip('"')
TI_ATTR_SLICE_FLAGS = TI_DEFINES['TI_ATTR_SLICE_FLAGS'].strip('"')
TI_ATTR_SLICE_1STSECS = TI_DEFINES['TI_ATTR_SLICE_1STSECS'].strip('"')
TI_ATTR_SLICE_SIZES = TI_DEFINES['TI_ATTR_SLICE_SIZES'].strip('"')
TI_ATTR_ZFS_RPOOL_NAME = TI_DEFINES['TI_ATTR_ZFS_RPOOL_NAME'].strip('"')
TI_ATTR_ZFS_RPOOL_DEVICE = TI_DEFINES['TI_ATTR_ZFS_RPOOL_DEVICE'].strip('"')
TI_ATTR_ZFS_RPOOL_PRESERVE = \
    TI_DEFINES['TI_ATTR_ZFS_RPOOL_PRESERVE'].strip('"')
TI_ATTR_ZFS_FS_NUM = TI_DEFINES['TI_ATTR_ZFS_FS_NUM'].strip('"')
TI_ATTR_ZFS_FS_POOL_NAME = TI_DEFINES['TI_ATTR_ZFS_FS_POOL_NAME'].strip('"')
TI_ATTR_ZFS_FS_NAMES = TI_DEFINES['TI_ATTR_ZFS_FS_NAMES'].strip('"')
TI_ATTR_ZFS_PROPERTIES = TI_DEFINES['TI_ATTR_ZFS_PROPERTIES'].strip('"')
TI_ATTR_ZFS_PROP_NAMES = TI_DEFINES['TI_ATTR_ZFS_PROP_NAMES'].strip('"')
TI_ATTR_ZFS_PROP_VALUES = TI_DEFINES['TI_ATTR_ZFS_PROP_VALUES'].strip('"')
TI_ATTR_ZFS_VOL_NUM = TI_DEFINES['TI_ATTR_ZFS_VOL_NUM'].strip('"')
TI_ATTR_ZFS_VOL_NAMES = TI_DEFINES['TI_ATTR_ZFS_VOL_NAMES'].strip('"')
TI_ATTR_ZFS_VOL_MB_SIZES = TI_DEFINES['TI_ATTR_ZFS_VOL_MB_SIZES'].strip('"')
TI_ATTR_BE_RPOOL_NAME = TI_DEFINES['TI_ATTR_BE_RPOOL_NAME'].strip('"')
TI_ATTR_BE_NAME = TI_DEFINES['TI_ATTR_BE_NAME'].strip('"')
TI_ATTR_BE_FS_NUM = TI_DEFINES['TI_ATTR_BE_FS_NUM'].strip('"')
TI_ATTR_BE_FS_NAMES = TI_DEFINES['TI_ATTR_BE_FS_NAMES'].strip('"')
TI_ATTR_BE_SHARED_FS_NUM = TI_DEFINES['TI_ATTR_BE_SHARED_FS_NUM'].strip('"')
TI_ATTR_BE_SHARED_FS_NAMES = \
    TI_DEFINES['TI_ATTR_BE_SHARED_FS_NAMES'].strip('"')
TI_ATTR_BE_MOUNTPOINT = TI_DEFINES['TI_ATTR_BE_MOUNTPOINT'].strip('"')
TI_ATTR_DC_RAMDISK_FS_TYPE = \
    TI_DEFINES['TI_ATTR_DC_RAMDISK_FS_TYPE'].strip('"')
TI_ATTR_DC_RAMDISK_SIZE = TI_DEFINES['TI_ATTR_DC_RAMDISK_SIZE'].strip('"')
TI_ATTR_DC_RAMDISK_BOOTARCH_NAME = \
    TI_DEFINES['TI_ATTR_DC_RAMDISK_BOOTARCH_NAME'].strip('"')
TI_ATTR_DC_RAMDISK_DEST = TI_DEFINES['TI_ATTR_DC_RAMDISK_DEST'].strip('"')
TI_ATTR_DC_UFS_DEST = TI_DEFINES['TI_ATTR_DC_UFS_DEST'].strip('"')
TI_ATTR_DC_RAMDISK_BYTES_PER_INODE = TI_DEFINES['TI_ATTR_DC_RAMDISK_BYTES_PER_INODE'].strip('"')  

TMP = TI_DEFINES['TI_DC_RAMDISK_FS_TYPE_UFS']
TI_DC_RAMDISK_FS_TYPE_UFS = \
    int(TMP.replace('((uint16_t)', '').replace(')', ''))
