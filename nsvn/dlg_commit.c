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
#include "dlg_commit.h"
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
nsvn_dlg_commit (GtkWidget *widget,
                 gpointer user_data)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cancel_btn;
  GtkWidget *ok_btn;
  GtkWidget *select_chk;
  GtkWidget *showunver_chk;
  GtkWidget *file_lst;
  GtkWidget *loghist_cmb;
  GtkWidget *logmsg_txt;
  GtkWidget *repospath_lbl;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, NULL, NULL);
  if (!dlg_gui)
    {
      g_warning ("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n");
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "commit_dialog");
  cancel_btn = glade_xml_get_widget (dlg_gui, "commit_cancel_btn");
  ok_btn = glade_xml_get_widget (dlg_gui, "commit_ok_btn");
  select_chk = glade_xml_get_widget (dlg_gui, "commit_selectfiles_chk");
  showunver_chk = glade_xml_get_widget (dlg_gui, "commit_showunver_chk");
  file_lst = glade_xml_get_widget (dlg_gui, "commit_file_lst");
  loghist_cmb = glade_xml_get_widget (dlg_gui, "commit_loghist_cmb");
  logmsg_txt = glade_xml_get_widget (dlg_gui, "commit_logmsg_txt");
  repospath_lbl = glade_xml_get_widget (dlg_gui, "commit_repospath_lbl");

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
/*  g_signal_connect (G_OBJECT (ok_btn), "clicked",
                    G_CALLBACK (nsvn__commit_files),
                    dlg_gui);
  g_signal_connect (G_OBJECT (select_chk), "clicked",
                    G_CALLBACK (nsvn__add_url_item),
                    dlg_gui);
*/
  /* Activating dialog box. */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
