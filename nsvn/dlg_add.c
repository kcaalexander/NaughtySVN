/*
* ====================================================================
* NaughtySVN add dialog box.
* Copyright (c) 2005, All rights reserved for Alexander Thomas (AT)
*
* 
* Leaving space here for Microsoft, to add license details.
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
#include "dlg_add.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


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


gboolean
nsvn_dlg_add (GtkWidget *widget,
              gpointer user_data)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cancel_btn;
  GtkWidget *ok_btn;
  GtkWidget *select_chk;
  GtkWidget *file_lst;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, NULL, NULL);
  if (!dlg_gui)
    {
      g_warning ("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n");
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "add_dialog");
  cancel_btn = glade_xml_get_widget (dlg_gui, "add_cancel_btn");
  ok_btn = glade_xml_get_widget (dlg_gui, "add_ok_btn");
  select_chk = glade_xml_get_widget (dlg_gui, "add_select_chk");
  file_lst = glade_xml_get_widget (dlg_gui, "add_file_lst");

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
/*  g_signal_connect (G_OBJECT (ok_btn), "clicked",
                    G_CALLBACK (nsvn__add_files),
                    dlg_gui);
  g_signal_connect (G_OBJECT (select_chk), "clicked",
                    G_CALLBACK (nsvn__add_url_item),
                    dlg_gui);
*/

  /* Activating dialog box. */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
