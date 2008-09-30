#!/usr/bin/python

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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.

# =============================================================================
# =============================================================================
# ManifestServ.py - ManifestServ XML data access interface module
#		    commandline interface
# =============================================================================
# =============================================================================

import sys
import errno
import getopt
from osol_install.ManifestServ import ManifestServ

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def query_local(mfest_obj):
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	"""Process local (non-socket) queries from the commandline.

	Accepts queries from the commandline, and prints results to the
	screen.  Uses mfest_obj.get_values() to get the data.

	Loops until user types ^D.

	Args:
	  mfest_obj: object to which queries are made for data.

	Returns: None

	Raises: None.
	"""
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	key_mode = False
	print ""
	while (True):

		if (key_mode):
			print "Please enter a key or "
			print ("\"-key\" to search an element or " +
			    "attribute path:")
		else:
			print ("Please enter an element or " +
			    "attribute path")
			print "or \"+key\" to search for keys:"

		try:
			path = sys.stdin.readline()
			if len(path) == 0: break
		except:
			break
		path = path.strip()

		if (path == "+key"):
			key_mode = True
			print "key mode set to " + str(key_mode)
			continue
		elif (path == "-key"):
			key_mode = False
			print "key mode set to " + str(key_mode)
			continue

		matches = []
		results = mfest_obj.get_values(path, key_mode)
		for i in range(len(results)):
			if (results[i].strip() == ""):
				print "(empty string / no value)"
			else:
				print results[i]


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def usage():
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	"""Display commandline options and arguments.

	Args: None

	Returns: None

	Raises: None
	"""
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	print >>sys.stderr, ("Usage: %s [-d] [-h|-?] [-s] [-t] [-v] " +
	    "[-f <validation_file_base> ]") % sys.argv[0]
	print >>sys.stderr, ("    [-o <out_manifest.xml file> ] " +
	    "<manifest.xml file>")
	print >>sys.stderr, "where:"
	print >>sys.stderr, ("  -d: turn on socket debug output (valid when " +
	    "-s also specified)")
	print >>sys.stderr, ("  -f <validation_file_base>: give basename " +
	    "for schema and defval files")
	print >>sys.stderr, ("      Defaults to basename of manifest " +
	    "(name less .xml suffix) when not provided")
	print >>sys.stderr, "  -h or -?: print this message"
	print >>sys.stderr, ("  -o <out_manifest.xml file>: write resulting " +
	    "XML after defaults and")
	print >>sys.stderr, "      validation processing"
	print >>sys.stderr, "  -t: save temporary file"
	print >>sys.stderr, ("      Temp file is \"/tmp/" +
	    "<manifest_basename>_temp_<pid>")
	print >>sys.stderr, "  -v: verbose defaults/validation output"
	print >>sys.stderr, ("  -s: start socket server for use by " +
	   "ManifestRead")


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Main
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if __name__ == "__main__":

	# Initialize option flags.
	d_flag = f_flag = o_flag = s_flag = t_flag = v_flag = False

	mfest_obj = None
	err = None
	ret = 0

	# Options come first in the commandline.
	# See usage method for option explanations.
	try:
		(opt_pairs, other_args) = getopt.getopt(sys.argv[1:],
		    "df:ho:stv?")
	except getopt.GetoptError, err:
		print "ManifestServ: " + str(err)
	except IndexError, err:
		print "ManifestRead: Insufficient arguments"
        if (err):
                usage()
                sys.exit (errno.EINVAL)

	valfile_root = None
	out_manifest = None
	for (opt, optarg) in opt_pairs:
		if (opt == "-d"):
			d_flag = True
		if (opt == "-f"):
			valfile_root = optarg
		if ((opt == "-h") or (opt == "-?")):
			usage()
			sys.exit (0)
		if (opt == "-o"):
			out_manifest = optarg
		if (opt == "-s"):
			s_flag = True
		elif (opt == "-t"):
			t_flag = True
		elif (opt == "-v"):
			v_flag = True

	# Must have the project data manifest.
	# Also check for mismatching options.
	if ((len(other_args) != 1) or (d_flag and not s_flag)):
		usage()
		sys.exit (errno.EINVAL)

        try:
		# Create the object used to extract the data.
                mfest_obj = ManifestServ(other_args[0], valfile_root,
		    out_manifest, v_flag, t_flag)

		# Start the socket server if requested.
		if (s_flag):
			mfest_obj.start_socket_server(d_flag)
			print ("Connect to socket with name " +
			    mfest_obj.get_sockname())

		# Enable querying from this process as well.  This method will
		# block to hold the socket server open for remote queries as
		# well (if enabled).
		query_local(mfest_obj)
        except (SystemExit, KeyboardInterrupt):
                print >>sys.stderr, "Caught SystemExit exception"
        except Exception, err:
		print >>sys.stderr, "Error running Manifest Server"

	# Time to shut down socket server.
	if ((mfest_obj != None) and (s_flag)):
		mfest_obj.stop_socket_server()

	if (err != None):
		ret = err.args[0]
	sys.exit (ret)