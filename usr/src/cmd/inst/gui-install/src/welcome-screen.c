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
#pragma ident	"@(#)welcome-screen.c	1.2	07/08/08 SMI"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include "installation-profile.h"
#include "interface-globals.h"
#include "window-graphics.h"
#include "welcome-screen.h"
#include "help-dialog.h"

void
on_installradio_toggled(GtkWidget *widget, gpointer user_data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) == FALSE)
		return;

	InstallationProfile.installationtype = INSTALLATION_TYPE_INITIAL_INSTALL;
	/* Show the possibly hidden stage progress labels */
	gtk_widget_show(MainWindow.timezonelabel);
	gtk_widget_show(MainWindow.languagelabel);
	gtk_widget_show(MainWindow.userlabel);

	/* "Upgrade" label becomes "Installation" */
	g_free(MainWindow.ActiveStageTitles[INSTALLATION_SCREEN]);
	g_free(MainWindow.InactiveStageTitles[INSTALLATION_SCREEN]);
	MainWindow.ActiveStageTitles[INSTALLATION_SCREEN] =
		g_strdup_printf(ActiveStageTitleMarkup, _("Installation"));
	MainWindow.InactiveStageTitles[INSTALLATION_SCREEN] =
		g_strdup_printf(InactiveStageTitleMarkup, _("Installation"));
	gtk_label_set_label(GTK_LABEL(MainWindow.installationlabel),
		MainWindow.InactiveStageTitles[INSTALLATION_SCREEN]);

	/* Reset the confirmation screen title */
	g_free(MainWindow.ScreenTitles[CONFIRMATION_SCREEN]);
	g_free(MainWindow.ScreenTitles[INSTALLATION_SCREEN]);
	g_free(MainWindow.ScreenTitles[FAILURE_SCREEN]);
	MainWindow.ScreenTitles[CONFIRMATION_SCREEN] =
		g_strdup_printf(ScreenTitleMarkup, _("Install"));
	MainWindow.ScreenTitles[INSTALLATION_SCREEN] =
		g_strdup_printf(ScreenTitleMarkup, _("Installing"));
	MainWindow.ScreenTitles[FAILURE_SCREEN] =
		g_strdup_printf(ScreenTitleMarkup, _("Installation Failed"));
}

void
on_upgraderadio_toggled(GtkWidget *widget, gpointer user_data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) == FALSE)
		return;

	InstallationProfile.installationtype = INSTALLATION_TYPE_INPLACE_UPGRADE;
	/* Hide the unusged stage progress labels */
	gtk_widget_hide(MainWindow.timezonelabel);
	gtk_widget_hide(MainWindow.languagelabel);
	gtk_widget_hide(MainWindow.userlabel);

	/* "Install" label becomes "Upgrade" */
	g_free(MainWindow.ActiveStageTitles[INSTALLATION_SCREEN]);
	g_free(MainWindow.InactiveStageTitles[INSTALLATION_SCREEN]);
	MainWindow.ActiveStageTitles[INSTALLATION_SCREEN] =
		g_strdup_printf(ActiveStageTitleMarkup, _("Upgrade"));
	MainWindow.InactiveStageTitles[INSTALLATION_SCREEN] =
		g_strdup_printf(InactiveStageTitleMarkup, _("Upgrade"));
	gtk_label_set_label(GTK_LABEL(MainWindow.installationlabel),
		MainWindow.InactiveStageTitles[INSTALLATION_SCREEN]);

	/* Reset the confirmation screen title */
	g_free(MainWindow.ScreenTitles[CONFIRMATION_SCREEN]);
	g_free(MainWindow.ScreenTitles[INSTALLATION_SCREEN]);
	g_free(MainWindow.ScreenTitles[FAILURE_SCREEN]);
	MainWindow.ScreenTitles[CONFIRMATION_SCREEN] =
		g_strdup_printf(ScreenTitleMarkup, _("Upgrade"));
	MainWindow.ScreenTitles[INSTALLATION_SCREEN] =
		g_strdup_printf(ScreenTitleMarkup, _("Upgrading"));
	MainWindow.ScreenTitles[FAILURE_SCREEN] =
		g_strdup_printf(ScreenTitleMarkup, _("Upgrade Failed"));
}

static void
release_notes_hide(GtkWidget *widget, gpointer *dialog)
{
	gtk_widget_hide(GTK_WIDGET(dialog));
}

static void
release_notes_delete_event(GtkWidget *widget, gpointer *user_data)
{
	gtk_widget_hide(widget);
}

/*
 * Signal handler connected up by Glade XML signal autoconnect
 * for the release notes button clicked event.
 */
gboolean
on_releasenotesbutton_clicked(GtkWidget *widget,
		gpointer user_data)
{
	static gboolean initialised = FALSE;

	if (initialised == FALSE && \
	    MainWindow.TextFileLocations[RELEASE_NOTES]) {
		show_file_in_textview(WelcomeWindow.releasenotestextview,
				MainWindow.TextFileLocations[RELEASE_NOTES], TRUE, FALSE, TRUE);
		initialised = TRUE;
	}
	window_graphics_dialog_set_properties(
			WelcomeWindow.releasenotesdialog);
	gtk_widget_show(WelcomeWindow.releasenotesdialog);
	return (TRUE);
}

static void
welcome_screen_vbox_size_allocate_event(GtkWidget *widget,
		GtkAllocation *allocation,
		gpointer 	user_data)
{
	gtk_widget_set_size_request(WelcomeWindow.releasenoteslabel,
				allocation->width, -1);
	gtk_widget_set_size_request(WelcomeWindow.welcomesummarylabel,
				allocation->width, -1);
	g_signal_handlers_disconnect_by_func(G_OBJECT(widget),
				(gpointer)welcome_screen_vbox_size_allocate_event, NULL);
}

static void
release_notes_init(void)
{

	WelcomeWindow.releasenotesxml =
			glade_xml_new(GLADEDIR "/" FILENAME, RELEASENOTESNODE, NULL);

	WelcomeWindow.releasenotesclosebutton =
		glade_xml_get_widget(WelcomeWindow.releasenotesxml,
				"textviewclosebutton");
	WelcomeWindow.releasenotesdialog =
		glade_xml_get_widget(WelcomeWindow.releasenotesxml,
				"textviewdialog");
	WelcomeWindow.releasenotestextview =
		glade_xml_get_widget(WelcomeWindow.releasenotesxml,
				"textview");

	gtk_window_set_title(GTK_WINDOW(
	    WelcomeWindow.releasenotesdialog),
	    _("Release Notes"));
	g_signal_connect(G_OBJECT(WelcomeWindow.releasenotesclosebutton), "clicked",
			G_CALLBACK(release_notes_hide),
			WelcomeWindow.releasenotesdialog);
	g_signal_connect(G_OBJECT(WelcomeWindow.releasenotesdialog),
			"delete-event",
			G_CALLBACK(release_notes_delete_event),
			WelcomeWindow.releasenotesdialog);
}

void
welcome_screen_init(void)
{
	/*
	 * Welcome screen specific initialisation code.
	 */
	glade_xml_signal_autoconnect(MainWindow.welcomewindowxml);

	InstallationProfile.installationtype = INSTALLATION_TYPE_INITIAL_INSTALL;

	WelcomeWindow.installradio =
			glade_xml_get_widget(MainWindow.welcomewindowxml, "installradio");
	WelcomeWindow.upgraderadio =
			glade_xml_get_widget(MainWindow.welcomewindowxml, "upgraderadio");

	WelcomeWindow.releasenoteslabel =
			glade_xml_get_widget(MainWindow.welcomewindowxml,
					"releasenoteslabel");
	WelcomeWindow.welcomesummarylabel =
			glade_xml_get_widget(MainWindow.welcomewindowxml,
					"welcomesummarylabel");
	gtk_box_pack_start(GTK_BOX(MainWindow.screencontentvbox),
			MainWindow.welcomescreenvbox, TRUE, TRUE, 0);
	/*
	 * GtkLabel widgets don't respond to resizing so in order make
	 * the text labels wrap correctly they have to be explicitly
	 * set to the same width as their parent container's allocation
	 * in the main window which is "screencontentvbox"
	 */
	g_signal_connect(G_OBJECT(MainWindow.welcomescreenvbox), "size-allocate",
			G_CALLBACK(welcome_screen_vbox_size_allocate_event),
			NULL);

	release_notes_init();

	/* Initialise radio buttons */
	/* Installation by default */
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(WelcomeWindow.installradio),
			TRUE);
	g_signal_connect(G_OBJECT(WelcomeWindow.installradio), "toggled",
			G_CALLBACK(on_installradio_toggled), NULL);
	g_signal_connect(G_OBJECT(WelcomeWindow.upgraderadio), "toggled",
			G_CALLBACK(on_upgraderadio_toggled), NULL);
}
