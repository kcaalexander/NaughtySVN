/*
* ====================================================================
* NaughtySVN add dialog box.
* Copyright (c) 2007, All rights reserved Eddy Petrișor
*
* 
* Leaving space here to add license details.
*
*
* ====================================================================
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <libgnomevfs/gnome-vfs-utils.h>

#include "svn/naughtysvn.h"
#include "dlg_update.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"

static gboolean update_ignore_externals;
static gboolean update_recursive;

static int
nsvn__destory_window (GtkWidget *widget,
                      GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "add_dialog");

  gtk_widget_destroy (window);
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
  return 0;
}

void
on_recursive_update_check_toggled (GtkWidget *widget,
                    gpointer user_data)
{
  update_recursive = gtk_toggle_button_get_active((GtkToggleButton *)widget);
  MSG_DEBUG("recursive       = %d\n", update_recursive);
}

void
on_externals_update_check_toggled (GtkWidget *widget,
                    gpointer user_data)
{
  update_ignore_externals = !gtk_toggle_button_get_active((GtkToggleButton *)widget);
  MSG_DEBUG("ignore externals = %d\n", update_ignore_externals);
}


void
on_updateok_clicked (GtkWidget *widget,
                      gpointer user_data)
{
  MSG_DEBUG("update ok clicked\n");
  MSG_DEBUG("  ignore externals = %d\n", update_ignore_externals);
  MSG_DEBUG("  recursive        = %d\n", update_recursive);
}

gboolean
nsvn_dlg_update (GtkWidget *widget,
                 gpointer user_data)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cancel_btn;
  GtkWidget *ok_btn;
  GtkWidget *recursive_chk;
  GtkWidget *externals_chk;
  GtkWidget *revision_entr;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, "update_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return EXIT_FAILURE;
    }

  glade_xml_signal_autoconnect(dlg_gui);
  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "update_dialog");
  cancel_btn = glade_xml_get_widget (dlg_gui, "update_cancel_btn");
  ok_btn = glade_xml_get_widget (dlg_gui, "update_ok_btn");
  recursive_chk = glade_xml_get_widget (dlg_gui, "update_recursive_chk");
  externals_chk = glade_xml_get_widget (dlg_gui, "update_externals_chk");
  revision_entr = glade_xml_get_widget (dlg_gui, "update_revision_entry");

  update_ignore_externals = !gtk_toggle_button_get_active(externals_chk);
  update_recursive = gtk_toggle_button_get_active(recursive_chk);

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);


  /* Activating dialog box. */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
