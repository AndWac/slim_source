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
#pragma ident	"@(#)installation-screen.c	1.7	07/10/29 SMI"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <gnome.h>
#include <glade/glade-build.h>
#include <sys/nvpair.h>
#include <ctype.h>
#include <locale.h>
#include "installation-profile.h"
#include "interface-globals.h"
#include "installation-screen.h"
#include "callbacks.h"
#include "help-dialog.h"
#include "language-screen.h"
#include "window-graphics.h"

gchar *InstallationInfoLabelMarkup = "<span font_desc=\"Arial Bold\">%s</span>";

void
installation_window_init(void)
{
	if (!MainWindow.installationwindowxml) {
		g_warning("Failed to access Install Progress Window.");
		exit(-1);
	}

	glade_xml_signal_autoconnect(MainWindow.installationwindowxml);

	MainWindow.InstallationWindow.installationwindowtable = NULL;
	MainWindow.InstallationWindow.installationframe = NULL;
	MainWindow.InstallationWindow.installationalignment = NULL;
	MainWindow.InstallationWindow.installationeventbox = NULL;
	MainWindow.InstallationWindow.installationimage = NULL;
	MainWindow.InstallationWindow.installationinfolabel = NULL;
	MainWindow.InstallationWindow.installationprogressbar = NULL;
	MainWindow.InstallationWindow.install_files = NULL;
	MainWindow.InstallationWindow.current_install_file = NULL;
	MainWindow.InstallationWindow.current_install_message = NULL;
	MainWindow.InstallationWindow.progress_bar_fraction = 0.0;
	MainWindow.InstallationWindow.current_fraction = 0.0;
	MainWindow.InstallationWindow.marketing_timer = NULL;
	MainWindow.InstallationWindow.marketing_entered = FALSE;
	MainWindow.InstallationWindow.tools_install_started = FALSE;
}

static void
installation_free_list(gpointer data, gpointer user_data)
{
	InstallationFileData *install_files = (InstallationFileData *)data;

	if (install_files->file_name != NULL) {
		g_free(install_files->file_name);
	}
	install_files->color.pixel = 0;
	install_files->color.red = 0;
	install_files->color.green = 0;
	install_files->color.blue = 0;
}

static void
installation_get_install_files()
{
	gchar *install_files[6] =
		{ "install-01.png", "install-02.png", "install-03.png",
			"install-04.png", "install-05.png", "install-06.png" };
	int red_install_colors[8] = { 63488, 49664, 50432, 60672,
								48384, 41728, 63232, 58624 };
	int green_install_colors[8] = { 54528, 55040, 54528, 39680,
								48640, 47104, 47360, 59392 };
	int blue_install_colors[8] = { 33536, 54528, 43264, 20224,
								49152, 51968, 33280, 60672 };
	InstallationFileData *tmpFileData;
	gint i = 0;
	gchar *locale_id = setlocale(LC_MESSAGES, NULL);

	if (MainWindow.InstallationWindow.install_files != NULL) {
		/* Clear down the list and reload the new file names */
		g_list_foreach(MainWindow.InstallationWindow.install_files,
						installation_free_list,
						NULL);
		MainWindow.InstallationWindow.install_files = NULL;
	}

	/* These should be on the install media, however until then */
	/* They will be hard coded as install-0[123] */
	for (i = 0; i < 6; i++) {
		tmpFileData = g_new0(InstallationFileData, 1);
		tmpFileData->file_name =
			help_generate_file_path(
				INSTALL_PROGRESS_PATH,
				locale_id,
				install_files[i]);
		tmpFileData->color.pixel = 0;
		tmpFileData->color.red = red_install_colors[i];
		tmpFileData->color.green = green_install_colors[i];
		tmpFileData->color.blue = blue_install_colors[i];

		MainWindow.InstallationWindow.install_files =
				g_list_append(MainWindow.InstallationWindow.install_files,
						tmpFileData);
	}
}

void
installation_window_load_widgets(void)
{
	MainWindow.InstallationWindow.installationwindowtable =
				glade_xml_get_widget(MainWindow.installationwindowxml,
							"installationwindowtable");
	MainWindow.InstallationWindow.installationframe =
				glade_xml_get_widget(MainWindow.installationwindowxml,
							"installationframe");
	MainWindow.InstallationWindow.installationalignment =
				glade_xml_get_widget(MainWindow.installationwindowxml,
							"installationalignment");
	MainWindow.InstallationWindow.installationeventbox =
				glade_xml_get_widget(MainWindow.installationwindowxml,
							"installationeventbox");
	MainWindow.InstallationWindow.installationimage =
				glade_xml_get_widget(MainWindow.installationwindowxml,
							"installationimage");
	MainWindow.InstallationWindow.installationinfolabel =
				glade_xml_get_widget(MainWindow.installationwindowxml,
							"installationinfolabel");
	MainWindow.InstallationWindow.installationprogressbar =
				glade_xml_get_widget(MainWindow.installationwindowxml,
							"installationprogressbar");

	/* Get list of install_files to be displayed */
	installation_get_install_files();
}

static void
display_progress_info(gpointer *data)
{
	InstallationFileData *install_file = (InstallationFileData *)data;

	gtk_image_set_from_file(
		GTK_IMAGE(MainWindow.InstallationWindow.installationimage),
		install_file->file_name);
}

void
installation_window_set_contents(void)
{
	GdkColor backcolour;
	/* Images/Files should be got from the install media somewhere */
	/* For the moment we'll hard code them as install-01 etc.. */

	switch (InstallationProfile.installationtype) {
		case INSTALLATION_TYPE_INITIAL_INSTALL:
			break;

		case INSTALLATION_TYPE_INPLACE_UPGRADE:
			break;
	}

	gdk_color_parse(WHITE_COLOR, &backcolour);
	gtk_widget_modify_bg(
		MainWindow.InstallationWindow.installationprogressbar,
		GTK_STATE_NORMAL,
		&backcolour);

	gtk_widget_modify_bg(
		MainWindow.InstallationWindow.installationeventbox,
		GTK_STATE_NORMAL,
		&backcolour);

	/* Initialize success/failure status */
	InstallationProfile.installfailed = FALSE;

	MainWindow.InstallationWindow.current_install_file =
						MainWindow.InstallationWindow.install_files;

	display_progress_info(
		(gpointer *)MainWindow.InstallationWindow.current_install_file->data);

	switch (InstallationProfile.installationtype) {
		case INSTALLATION_TYPE_INITIAL_INSTALL:
			MainWindow.InstallationWindow.current_install_message =
					g_strdup("Preparing for OpenSolaris Developer Preview installation");
			break;
		case INSTALLATION_TYPE_INPLACE_UPGRADE:
			MainWindow.InstallationWindow.current_install_message =
					g_strdup("Preparing for OpenSolaris Developer Preview upgrade");
			break;
	}

	gtk_label_set_label(
			GTK_LABEL(MainWindow.InstallationWindow.installationinfolabel),
			(gchar *)MainWindow.InstallationWindow.current_install_message);

	gtk_progress_bar_set_fraction(
		GTK_PROGRESS_BAR(MainWindow.InstallationWindow.installationprogressbar),
		MainWindow.MileStonePercentage[MainWindow.CurrentMileStone]/100.0);

	g_timeout_add(INSTALLATION_TIMEOUT_SECONDS, installation_next_step, NULL);

	if (MainWindow.InstallationWindow.marketing_timer != NULL) {
		g_timer_reset(MainWindow.InstallationWindow.marketing_timer);
	} else {
		MainWindow.InstallationWindow.marketing_timer = g_timer_new();
	}
}

static void
installation_next_file()
{
	/* Advance to the next file */
	if (MainWindow.InstallationWindow.current_install_file !=
		g_list_last(MainWindow.InstallationWindow.install_files)) {
		MainWindow.InstallationWindow.current_install_file =
			g_list_next(MainWindow.InstallationWindow.current_install_file);
	} else { /* Cycle to the first */
		MainWindow.InstallationWindow.current_install_file =
		    g_list_first(MainWindow.InstallationWindow.current_install_file);
	}

	display_progress_info(
		(gpointer *)MainWindow.InstallationWindow.current_install_file->data);
}

static void
installation_prev_file()
{
	/* Go to previous file */
	if (MainWindow.InstallationWindow.current_install_file !=
		g_list_first(MainWindow.InstallationWindow.install_files)) {
		MainWindow.InstallationWindow.current_install_file =
			g_list_previous(MainWindow.InstallationWindow.current_install_file);
	} else { /* Cycle backwards to the last file */
		MainWindow.InstallationWindow.current_install_file =
		    g_list_last(MainWindow.InstallationWindow.install_files);
	}
	display_progress_info(
		(gpointer *)MainWindow.InstallationWindow.current_install_file->data);
}

gboolean
installation_next_step(gpointer data)
{
	/*
	 * returning FALSE destroys timeout.
	 * Called by g_timeout_add, every 2 Seconds
	 * If timer has reached 60 seconds then display new file
	 */

	if (InstallationProfile.installfailed == TRUE) {
		g_warning("Installation Failed\n");
		g_timer_destroy(MainWindow.InstallationWindow.marketing_timer);
		on_nextbutton_clicked(GTK_BUTTON(MainWindow.nextbutton), NULL);
		return (FALSE);
	}

	if (g_timer_elapsed(MainWindow.InstallationWindow.marketing_timer, 0)
			>= INSTALLATION_IMAGE_CYCLE) {
		installation_next_file();
		g_timer_start(MainWindow.InstallationWindow.marketing_timer);
	}

	/*
	 * om_perform_install() is deemed complete when the POSTINSTAL_TASK
	 * has completed. Time to initiate the tools installation.
	 */
	if (MainWindow.MileStoneComplete[OM_POSTINSTAL_TASKS] == TRUE &&
		MainWindow.InstallationWindow.tools_install_started == FALSE) {
		if (orchestrator_om_perform_tools_install(installation_update_progress)
					== OM_FAILURE) {
			/* Failed start tools install, go to failure screen straight away */
			g_timer_destroy(MainWindow.InstallationWindow.marketing_timer);
			g_warning("om_perform_tools_install failed %d\n", om_get_error());
			InstallationProfile.installfailed = TRUE;
			on_nextbutton_clicked(GTK_BUTTON(MainWindow.nextbutton), NULL);
			return (FALSE);
		}
		MainWindow.InstallationWindow.tools_install_started = TRUE;
	}

	/*
	 * Once the last tools install milestone has completed e.g.
	 * OM_TOOLS_JAVAAPPSVR, then installation has completed.
	 * Show the install success screen via on_nextbutton
	 */
	if (MainWindow.MileStoneComplete[OM_TOOLS_JAVAAPPSVR] == TRUE) {
		/*
		 * reached last message Call on_nextbutton pressed to move onto
		 * the finish screen
		 */
		g_timer_destroy(MainWindow.InstallationWindow.marketing_timer);
		/*
		 * The Setting of InstallationProfile.installfailed should be done
		 * here before calling on_nextbutton_clicked
		 */
		InstallationProfile.installfailed = FALSE;
		on_nextbutton_clicked(GTK_BUTTON(MainWindow.nextbutton), NULL);
		return (FALSE);
	}

	gtk_label_set_label(
			GTK_LABEL(MainWindow.InstallationWindow.installationinfolabel),
			(gchar *)MainWindow.InstallationWindow.current_install_message);
	gtk_progress_bar_set_fraction(
		GTK_PROGRESS_BAR(MainWindow.InstallationWindow.installationprogressbar),
		MainWindow.OverallPercentage/100.0);

	return (TRUE);
}

gboolean
installation_file_enter(GtkWidget *widget,
			GdkEventCrossing *event,
			gpointer user_data)
{
/*
 * Motion timer halts the cycling of images while the mouse is over
 * the message content area. To reenable this feature comment out the
 * #ifdefs
 */
#ifdef ENABLE_MOTION_TIMER
	if (MainWindow.InstallationWindow.marketing_timer != NULL) {
		g_timer_stop(MainWindow.InstallationWindow.marketing_timer);
	}
	MainWindow.InstallationWindow.marketing_entered = TRUE;
#endif
	return (FALSE);
}

gboolean
installation_file_leave(GtkWidget *widget,
			GdkEventCrossing *event,
			gpointer user_data)
{
#ifdef ENABLE_MOTION_TIMER
	if (MainWindow.InstallationWindow.marketing_timer != NULL) {
		g_timer_continue(MainWindow.InstallationWindow.marketing_timer);
	}
	MainWindow.InstallationWindow.marketing_entered = FALSE;
#endif
	return (FALSE);
}

gboolean
installation_file_key_release(GtkWidget *widget,
			GdkEventKey *event,
			gpointer user_data)
{
	/* Check for arrow keys */
	switch (event->keyval) {
		case GDK_Left :
			installation_prev_file();
			if (MainWindow.InstallationWindow.marketing_entered) {
				g_timer_continue(MainWindow.InstallationWindow.marketing_timer);
				g_timer_start(MainWindow.InstallationWindow.marketing_timer);
				g_timer_stop(MainWindow.InstallationWindow.marketing_timer);
			} else {
				g_timer_start(MainWindow.InstallationWindow.marketing_timer);
			}
			break;
		case GDK_Right :
			installation_next_file();
			if (MainWindow.InstallationWindow.marketing_entered) {
				g_timer_continue(MainWindow.InstallationWindow.marketing_timer);
				g_timer_start(MainWindow.InstallationWindow.marketing_timer);
				g_timer_stop(MainWindow.InstallationWindow.marketing_timer);
			} else {
				g_timer_start(MainWindow.InstallationWindow.marketing_timer);
			}
			break;
	}
	return (FALSE);
}

static char *
get_data_type_str(data_type_t type)
{
	char *tmpStr;

	switch (type) {
		case DATA_TYPE_UNKNOWN:
			tmpStr = "DATA_TYPE_UNKNOWN";
			break;
		case DATA_TYPE_BOOLEAN:
			tmpStr = "DATA_TYPE_BOOLEAN";
			break;
		case DATA_TYPE_BYTE:
			tmpStr = "DATA_TYPE_BYTE";
			break;
		case DATA_TYPE_INT16:
			tmpStr = "DATA_TYPE_INT16";
			break;
		case DATA_TYPE_UINT16:
			tmpStr = "DATA_TYPE_UINT16";
			break;
		case DATA_TYPE_INT32:
			tmpStr = "DATA_TYPE_INT32";
			break;
		case DATA_TYPE_UINT32:
			tmpStr = "DATA_TYPE_UINT32";
			break;
		case DATA_TYPE_INT64:
			tmpStr = "DATA_TYPE_INT64";
			break;
		case DATA_TYPE_UINT64:
			tmpStr = "DATA_TYPE_UINT64";
			break;
		case DATA_TYPE_STRING:
			tmpStr = "DATA_TYPE_STRING";
			break;
		case DATA_TYPE_BYTE_ARRAY:
			tmpStr = "DATA_TYPE_BYTE_ARRAY";
			break;
		case DATA_TYPE_INT16_ARRAY:
			tmpStr = "DATA_TYPE_INT16_ARRAY";
			break;
		case DATA_TYPE_UINT16_ARRAY:
			tmpStr = "DATA_TYPE_UINT16_ARRAY";
			break;
		case DATA_TYPE_INT32_ARRAY:
			tmpStr = "DATA_TYPE_INT32_ARRAY";
			break;
		case DATA_TYPE_UINT32_ARRAY:
			tmpStr = "DATA_TYPE_UINT32_ARRAY";
			break;
		case DATA_TYPE_INT64_ARRAY:
			tmpStr = "DATA_TYPE_INT64_ARRAY";
			break;
		case DATA_TYPE_UINT64_ARRAY:
			tmpStr = "DATA_TYPE_UINT64_ARRAY";
			break;
		case DATA_TYPE_STRING_ARRAY:
			tmpStr = "DATA_TYPE_STRING_ARRAY";
			break;
		case DATA_TYPE_HRTIME:
			tmpStr = "DATA_TYPE_HRTIME";
			break;
		case DATA_TYPE_NVLIST:
			tmpStr = "DATA_TYPE_NVLIST";
			break;
		case DATA_TYPE_NVLIST_ARRAY:
			tmpStr = "DATA_TYPE_NVLIST_ARRAY";
			break;
		case DATA_TYPE_BOOLEAN_VALUE:
			tmpStr = "DATA_TYPE_BOOLEAN_VALUE";
			break;
		case DATA_TYPE_INT8:
			tmpStr = "DATA_TYPE_INT8";
			break;
		case DATA_TYPE_UINT8:
			tmpStr = "DATA_TYPE_UINT8";
			break;
		case DATA_TYPE_BOOLEAN_ARRAY:
			tmpStr = "DATA_TYPE_BOOLEAN_ARRAY";
			break;
		case DATA_TYPE_INT8_ARRAY:
			tmpStr = "DATA_TYPE_INT8_ARRAY";
			break;
		case DATA_TYPE_UINT8_ARRAY:
			tmpStr = "DATA_TYPE_UINT8_ARRAY";
			break;
	}

	return (tmpStr);
}

static void
nv_list_print(nvlist_t *nv_list)
{
	char *pairName;
	data_type_t pairType;
	nvpair_t *pair;
	boolean_t bool_val;
	char *str_val;
	gchar *pairValue = NULL;
	uint8_t uint8_val;

	pair =  nvlist_next_nvpair(nv_list, NULL);
	while (pair != NULL) {
		pairName = nvpair_name(pair);
		pairType = nvpair_type(pair);

		pairValue = NULL;
		switch (pairType) {
			case DATA_TYPE_BOOLEAN_VALUE :
				nvlist_lookup_boolean_value(nv_list, pairName, &bool_val);
				pairValue = g_strdup_printf("%s",
							bool_val == B_TRUE ? "TRUE" : "FALSE");
				break;
			case DATA_TYPE_STRING :
				nvlist_lookup_string(nv_list, pairName, &str_val);
				pairValue = g_strdup(str_val);
				break;
			case DATA_TYPE_UINT8 :
				nvlist_lookup_uint8(nv_list, pairName, &uint8_val);
				pairValue = g_strdup_printf("%d", uint8_val);
				break;
			default :
				pairValue = g_strdup("Unknown value");
		}

		if (pairValue) {
			g_warning("Pair : %s, Type : %s, Value : %s\n", pairName,
				get_data_type_str(pairType), pairValue);
		} else {
			g_warning("Pair : %s, Type : %s\n", pairName,
				get_data_type_str(pairType));
		}

		g_free(pairValue);
		pair = nvlist_next_nvpair(nv_list, pair);
	}
}

const gchar *
lookup_callback_type(int callback)
{
	switch (callback) {
		case OM_TARGET_TARGET_DISCOVERY :
			return ("OM_TARGET_TARGET_DISCOVERY");
		case OM_SYSTEM_VALIDATION :
			return ("OM_SYSTEM_VALIDATION");
		case OM_INSTALL_TYPE :
			return ("OM_INSTALL_TYPE");
		case OM_UPGRADE_TYPE :
			return ("OM_UPGRADE_TYPE");
		case OM_TOOLS_INSTALL_TYPE :
			return ("OM_TOOLS_INSTALL_TYPE");
		default :
			return ("UNKNOWN");
	}
}

const gchar *
lookup_milestone_type(int milestone)
{
	switch (milestone) {
		case OM_DISK_DISCOVERY :
			return ("OM_DISK_DISCOVERY");
		case OM_PARTITION_DISCOVERY :
			return ("OM_PARTITION_DISCOVERY");
		case OM_SLICE_DISCOVERY :
			return ("OM_SLICE_DISCOVERY");
		case OM_UPGRADE_TARGET_DISCOVERY :
			return ("OM_UPGRADE_TARGET_DISCOVERY");
		case OM_INSTANCE_DISCOVERY :
			return ("OM_INSTANCE_DISCOVERY");
		case OM_TARGET_INSTANTIATION :
			return ("OM_TARGET_INSTANTIATION");
		case OM_UPGRADE_CHECK :
			return ("OM_UPGRADE_CHECK");
		case OM_SOFTWARE_UPDATE :
			return ("OM_SOFTWARE_UPDATE");
		case OM_POSTINSTAL_TASKS :
			return ("OM_POSTINSTAL_TASKS");
		case OM_TOOLS_SUNSTUDIO :
			return ("OM_TOOLS_SUNSTUDIO");
		case OM_TOOLS_NETBEANS :
			return ("OM_TOOLS_NETBEANS");
		case OM_TOOLS_JAVAAPPSVR :
			return ("OM_TOOLS_JAVAAPPSVR");
		default :
			return ("UNKNOWN");
	}
}

void
installation_update_progress(om_callback_info_t *cb_data,
			uintptr_t app_data)
{
	g_return_if_fail(cb_data);

//	Uncomment this when finished testing so that only INSTALL/UPGRADE/TOOLS
//	callbacks get processed here....
//	if (cb_data->callback_type != OM_INSTALL_TYPE &&
//		cb_data->callback_type != OM_UPGRADE_TYPE &&
//		cb_data->callback_type != OM_TOOLS_INSTALL_TYPE) {
//		g_warning("Invalid Install/Upgrade callback type %d\n",
//			db_data->callback_type);
//		return;
//	}

g_message("installation_update_progress : milestones      = %d\n",
					cb_data->num_milestones);
g_message("                             : curr_milestone  = %d : %s\n",
					cb_data->curr_milestone,
					lookup_milestone_type(cb_data->curr_milestone));

g_message("                             : callback_type   = %d : %s\n",
					cb_data->callback_type,
					lookup_callback_type(cb_data->callback_type));
g_message("                             : percentage_done = %d\n",
					cb_data->percentage_done);

	/*
	 * Fields to process
	 * cb->num_milestones = 3;
	 * cb->curr_milestone = OM_TARGET_INSTANTIATION;
	 * cb->percent_done = 0;
	 * cb->curr_milestone = OM_SOFTWARE_UPDATE;
	 * cb(cb_info, NULL);
	 */
	g_free(MainWindow.InstallationWindow.current_install_message);

	MainWindow.CurrentMileStone = cb_data->curr_milestone;
	MainWindow.MileStonePercentage[MainWindow.CurrentMileStone] =
		cb_data->percentage_done;
	MainWindow.MileStoneComplete[MainWindow.CurrentMileStone] =
		cb_data->percentage_done == 100 ? TRUE : FALSE;

	/*
	 * Current Overall Time split for milestones is :
	 * For Install :
	 *	TARGET_INSTANTIATION = 5 = 0.05
	 *	SOFTWARE_UPDATE = 81 = 0.81
	 *	POSTINSTAL_TASKS = 1 = 0.01
	 *	TOOLS_SUNSTUDIO = 3 = 0.03
	 *	TOOLS_NETBEANS = 4 = 0.04
	 *	TOOLS_JAVAAPPSVR = 6 = 0.06
	 *
	 * For Upgrade :
	 *	UPGRADE_CHECK = 10 = 0.1
	 *	SOFTWARE_UPDATE = 76 = 0.76
	 *	POSTINSTAL_TASKS = 1 = 0.01
	 *	TOOLS_SUNSTUDIO = 3 = 0.03
	 *	TOOLS_NETBEANS = 4 = 0.04
	 *	TOOLS_JAVAAPPSVR = 6 = 0.06
	 *
	 */

	switch (InstallationProfile.installationtype) {
		case INSTALLATION_TYPE_INITIAL_INSTALL:
			switch (cb_data->curr_milestone) {
				case OM_TARGET_INSTANTIATION :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Preparing disk for OpenSolaris Developer Preview installation"));
					/*
					 * Wild, random, guess that target instantiation accounts
					 * for approx. 5% of total installation time
					 */
					MainWindow.OverallPercentage =
						(guint)(cb_data->percentage_done * 0.05);
					break;

				case OM_SOFTWARE_UPDATE :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Installing OpenSolaris Developer Preview software"));
					/*
					 * And software installation takes 81%
					 */
					MainWindow.OverallPercentage = 5 +
						(guint)(cb_data->percentage_done * 0.81);
					break;

				case OM_POSTINSTAL_TASKS :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Performing post-installation tasks"));
					MainWindow.OverallPercentage = 86 +
						(guint)(cb_data->percentage_done * 0.01);
					break;

				case OM_TOOLS_SUNSTUDIO :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Installing Sun Studio"));
					MainWindow.OverallPercentage = 87 +
						(guint)(cb_data->percentage_done * 0.03);
					break;

				case OM_TOOLS_NETBEANS :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Installing NetBeans IDE"));
					MainWindow.OverallPercentage = 90 +
						(guint)(cb_data->percentage_done * 0.04);
					break;
				case OM_TOOLS_JAVAAPPSVR :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Installing Java System Application Server"));
					MainWindow.OverallPercentage = 94 +
						(guint)(cb_data->percentage_done * 0.06);
					break;

				case -1: /* Indicates that installation failed */
					g_warning("Installation failed: %s",
						g_strerror(cb_data->percentage_done));
					InstallationProfile.installfailed = TRUE;
					break;
				default :
					g_warning("Invalid install curr_milestone : %d : %s\n",
						cb_data->curr_milestone,
						lookup_milestone_type(cb_data->curr_milestone));
					break;
			}
			break;
		case INSTALLATION_TYPE_INPLACE_UPGRADE:
			switch (cb_data->curr_milestone) {
				case OM_UPGRADE_CHECK :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Performing upgrade check"));
					/*
					 * And software update takes 10%
					 */
					MainWindow.OverallPercentage =
						(guint)(cb_data->percentage_done * 0.1);
					break;

				case OM_SOFTWARE_UPDATE :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Updating OpenSolaris Developer Preview software"));
					/*
					 * And software update takes 76%
					 */
					MainWindow.OverallPercentage = 10 +
						(guint)(cb_data->percentage_done * 0.76);
					break;

				case OM_POSTINSTAL_TASKS :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Performing post-installation tasks"));
					MainWindow.OverallPercentage = 86 +
						(guint)(cb_data->percentage_done * 0.01);
					break;

				case OM_TOOLS_SUNSTUDIO :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Updating Sun Studio"));
					MainWindow.OverallPercentage = 87 +
						(guint)(cb_data->percentage_done * 0.03);
					break;

				case OM_TOOLS_NETBEANS :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Updating NetBeans IDE"));
					MainWindow.OverallPercentage = 90 +
						(guint)(cb_data->percentage_done * 0.04);
					break;

				case OM_TOOLS_JAVAAPPSVR :
					MainWindow.InstallationWindow.current_install_message =
						g_strdup(_("Updating Java System Application Server"));
					MainWindow.OverallPercentage = 94 +
						(guint)(cb_data->percentage_done * 0.06);
					break;

				case -1: /* Indicates that update failed */
					g_warning("Update failed: %s",
						g_strerror(cb_data->percentage_done));
					InstallationProfile.installfailed = TRUE;
					break;

				default :
					g_warning("Invalid update curr_milestone : %d : %s\n",
						cb_data->curr_milestone,
						lookup_milestone_type(cb_data->curr_milestone));
					break;
			}
			break;
	}
}

gboolean
installation_get_dummy_install(void)
{
	gchar *tmpStr = NULL;
	gboolean dummy_install = FALSE;

	if ((tmpStr = (gchar *)g_getenv("CAIMAN_DUMMY_INSTALL")) != NULL) {
		if (strncmp(tmpStr, "1", 1) == 0) {
			dummy_install = TRUE;
		} else {
			dummy_install = FALSE;
		}
		tmpStr = NULL;
	} else {
		dummy_install = FALSE;
	}

	return (dummy_install);
}

void
installation_window_start_install(void)
{
	/*
	 * Set up the necessary nvlist pairs to be passed to orchestrator API
	 * om_perform_install();
	 * Will pass in callback when callback type has been defined
	 */
	nvlist_t *install_choices;
	int err = 0;
	gchar *tmpStr = NULL;
	gboolean dummy_install = FALSE;

	InstallationProfile.installfailed = FALSE;

	if ((err = nvlist_alloc(&install_choices, NV_UNIQUE_NAME, 0)) != 0) {
		/* Failed to allocate the nvlist so exit install */
		g_warning(_("Failed to allocate named pair list"));
		InstallationProfile.installfailed = TRUE;
		on_nextbutton_clicked(GTK_BUTTON(MainWindow.nextbutton), NULL);
		return;
	}

	/* Set up the nv pairs */
	err = 0;
	/* INSTALL_TEST setting is based on env variable CAIMAN_DUMMY_INSTALL */
	dummy_install = installation_get_dummy_install();
	if (dummy_install == FALSE) {
		g_message("Performing REAL install\n");
	} else {
		g_message("Performing DUMMY install\n");
	}

	if ((err = nvlist_add_boolean_value(
				install_choices,
				OM_ATTR_INSTALL_TEST,
				dummy_install)) != 0) {
		g_warning(_("Failed to add OM_ATTR_INSTALL_TEST to pair list"));
		InstallationProfile.installfailed = TRUE;
		on_nextbutton_clicked(GTK_BUTTON(MainWindow.nextbutton), NULL);
		return;
	}

	switch (InstallationProfile.installationtype) {
		case INSTALLATION_TYPE_INITIAL_INSTALL:
			/* 1 : OM_ATTR_INSTALL_TYPE */
			if ((err = nvlist_add_uint8(
							install_choices,
							OM_ATTR_INSTALL_TYPE,
							OM_INITIAL_INSTALL)) != 0) {
				g_warning(
					_("Failed to add %s to pair list"),
					"OM_ATTR_INSTALL_TYPE");
				break;
			}

			/* 2 : OM_ATTR_DISK_NAME */
			if (InstallationProfile.diskname) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_DISK_NAME,
							InstallationProfile.diskname)) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_DISK_NAME");
					break;
				}
			}

			/* 3 : OM_ATTR_ROOT_PASSWORD */
			if (InstallationProfile.rootpassword) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_ROOT_PASSWORD,
							g_strdup(
							    om_encrypt_passwd(
								    (char *)InstallationProfile.rootpassword,
								    "root")))) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_ROOT_PASSWORD");
					break;
				}
			}

			/* 4 : OM_ATTR_USER_NAME */
			if (InstallationProfile.username) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_USER_NAME,
							InstallationProfile.username)) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_USER_NAME");
					break;
				}
			}

			/* 5 : OM_ATTR_USER_PASSWORD */
			if (InstallationProfile.userpassword) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_USER_PASSWORD,
							g_strdup(
							    om_encrypt_passwd(
								    (char *)InstallationProfile.userpassword,
								    (char *)InstallationProfile.loginname))))
							!= 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_USER_PASSWORD");
					break;
				}
			}

			/* 6 : OM_ATTR_LOGIN_NAME */
			if (InstallationProfile.loginname) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_LOGIN_NAME,
							InstallationProfile.loginname)) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_LOGIN_NAME");
					break;
				}
			}

			/* 7 : OM_ATTR_HOST_NAME */
			if (InstallationProfile.hostname) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_HOST_NAME,
							InstallationProfile.hostname)) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_HOST_NAME");
					break;
				}
			}

			/* 8 : OM_ATTR_TIMEZONE_INFO */
			if (InstallationProfile.timezone) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_TIMEZONE_INFO,
							InstallationProfile.timezone->tz_name)) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_TIMEZONE_INFO");
					break;
				}
			}

			/* 9 : OM_ATTR_DEFAULT_LOCALE */
			if (InstallationProfile.def_locale) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_DEFAULT_LOCALE,
							orchestrator_om_locale_get_name(
								InstallationProfile.def_locale))) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_DEFAULT_LOCALE");
					break;
				}
			}
			/* 10 : OM_ATTR_LOCALES_LIST */
			construct_locale_string(&tmpStr, FALSE, ' ');
			if (tmpStr) {
				if ((err = nvlist_add_string(
							install_choices,
							OM_ATTR_LOCALES_LIST,
							tmpStr)) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_LOCALES_LIST");
					break;
				}
			}
			break;

		case INSTALLATION_TYPE_INPLACE_UPGRADE:
			/* 1 : OM_ATTR_INSTALL_TYPE */
			if ((err = nvlist_add_uint8(
							install_choices,
							OM_ATTR_INSTALL_TYPE,
							OM_UPGRADE)) != 0) {
				g_warning(_("Failed to add %s to pair list"),
							"OM_ATTR_INSTALL_TYPE");
				break;
			}

			/* 2 : OM_ATTR_UPGRADE_TARGET */
			if (InstallationProfile.slicename) {
				if ((err = nvlist_add_string(
								install_choices,
								OM_ATTR_UPGRADE_TARGET,
								InstallationProfile.slicename)) != 0) {
					g_warning(
						_("Failed to add %s to pair list"),
						"OM_ATTR_UPGRADE_TARGET");
					break;
				}
			}
			break;
	}

	if (err != 0) {
		/* One of the nvlist_add's failed */
		InstallationProfile.installfailed = TRUE;
		on_nextbutton_clicked(GTK_BUTTON(MainWindow.nextbutton), NULL);
	} else {
		nv_list_print(install_choices);
		if (orchestrator_om_perform_install(
				install_choices,
				installation_update_progress) == OM_FAILURE) {
			/* Failed to start install, go to failure screen straight away */
			g_warning("om_perform_install failed %d\n",
				om_get_error());
			InstallationProfile.installfailed = TRUE;
			on_nextbutton_clicked(
				GTK_BUTTON(MainWindow.nextbutton), NULL);
		}
	}
}
