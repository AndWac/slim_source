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
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

import os
import errno
import osol_install.install_utils
from subprocess import *
import fnmatch
import logging
import datetime
import time

execfile("/usr/lib/python2.4/vendor-packages/osol_install/distro_const/DC_defs.py")

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def get_manifest_value(manifest_obj, path, is_key=False):
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	""" Read the value of the path specified.
	This returns only the first value of the list.

	Args:
	   manifest_obj: Manifest object (could be ManifestServ or
		ManifestRead object)
	   path: path to read
	   is_key: Set to True if the path is to be interpreted as a key

	Returns:
	   the first value found (as a string) if there is at least
		one value to retrieve.
	   None: if no value found

	Raises:
	   None
	"""

	node_list = manifest_obj.get_values(path, is_key)
	if (len(node_list) > 0) and (len(node_list[0]) > 0):
		return str(node_list[0])
	return None

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def get_manifest_list(manifest_obj, path, is_key=False):
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	""" Create a list of the values of the path specified.

	Args:
	   manifest_obj: Manifest object (could be ManifestServ or
		ManifestRead object)
	   path: path to read from
	   is_key: Set to True if the path is to be interpreted as a key

	Returns:
	   A list of values.
	   An empty list is returned if no values are found.

	Raises:
	   None
	"""

	node_list = manifest_obj.get_values(path, is_key)

	for i in (range(len(node_list))):
		node_list[i] = str(node_list[i])
	return node_list 


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def cleanup_dir(mntpt):
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	"""Remove all files and directories underneath the mount point.
	Any errors are ignored since they aren't fatal.

	Input:
	   mntpt: mount point to remove files and directories from

	Returns:
	   None

	Raises:
	   None
	"""

	for root, dirs, files in os.walk(mntpt, topdown=False):
		for name in files:
			try:
				os.unlink(os.path.join(root,name))
			except:
				pass

		for dir in dirs:
			try:
				os.rmdir(os.path.join(root,dir))
			except:
				pass

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def setup_dc_logfile_names(logging_dir):
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	"""Computes the name of the simple and detail log based on time

	Input:
	   logging_dir: name of the log file directory

	Returns:
	   Name of the simple log and detail log

	Raises:
	   None
	"""

	timeformat_str="%Y-%m-%d-%H-%M-%S"
	now = datetime.datetime.now()
	str_timestamp = now.strftime(timeformat_str)
		
	simple_log_name = logging_dir + "/simple-log-" + str_timestamp
	detail_log_name = logging_dir + "/detail-log-" + str_timestamp

	return (simple_log_name, detail_log_name)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def setup_dc_logging(simple_log_name, detail_log_name):
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	"""Setup logging for the Distribution Constructor application 
	Log information will go to 3 locations.
	(1) The console, this will have the same info as the simple log
	(2) A log file specified in simple_log_name.  The simple log will
	have debug level of INFO
	(2) A log file specified in detail_log_name.  The detail log will
	have debug level of DEBUG

	Input:
	   simple_log_name: name of the log file for the simple log
	   detail_log_name: name of the log file for the detail log

	Returns:
	   The logger object

	Raises:
	   None
	"""

	dc_log = logging.getLogger(DC_LOGGER_NAME)
	try:
	
		#
		# Need to set the most top level one to the lowest log level, so,
		# all handlers added can also log at various levels
		#
		dc_log.setLevel(logging.DEBUG)

		console = logging.StreamHandler()
		console.setLevel(logging.INFO)
		dc_log.addHandler(console)

		simple_log = logging.FileHandler(simple_log_name, "a+")
		simple_log.setLevel(logging.INFO)
		dc_log.addHandler(simple_log)

		detail_log = logging.FileHandler(detail_log_name, "a+")
		detail_log.setLevel(logging.DEBUG)
		dc_log.addHandler(detail_log)

	except Exception, e:
		print "WARNING: There's a problem with logging setup."
		print e

	return (dc_log)
