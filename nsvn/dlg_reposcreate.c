/*
* ====================================================================
* Repository creation dialog program. 
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
#include "dlg_reposcreate.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


static void
nsvn__choose_fsfs_options  (GtkWidget *widget,
                            GladeXML *user_data)
{
  GtkWidget *fsync_chk;
  GtkWidget *log_chk;

  fsync_chk = glade_xml_get_widget (user_data,
                                    "repocreate_tcommit_chk");
  log_chk = glade_xml_get_widget (user_data,
                                  "repocreate_logrm_chk");

  gtk_widget_set_sensitive (fsync_chk, FALSE);
  gtk_widget_set_sensitive (log_chk, FALSE);
}


static void
nsvn__choose_bdb_options  (GtkWidget *widget,
                           GladeXML *user_data)
{
  GtkWidget *fsync_chk;
  GtkWidget *log_chk;

  fsync_chk = glade_xml_get_widget (user_data,
                                    "repocreate_tcommit_chk");
  log_chk = glade_xml_get_widget (user_data,
                                  "repocreate_logrm_chk");

  gtk_widget_set_sensitive (fsync_chk, TRUE);
  gtk_widget_set_sensitive (log_chk, TRUE);
}


static void
nsvn__create_repository  (GtkWidget *widget,
                          GladeXML *user_data)
{
  nsvn_t *nsvn;
  GtkWidget *entry;
  GtkWidget *window;
  GtkWidget *fsfs;
  GtkWidget *bdb;
  GtkWidget *commit_sync;
  GtkWidget *logrm;
  const char *txt;
  char *path, *newpath;
  char *fs_type = NULL;
  gboolean is_sync = FALSE;
  gboolean is_logrm = FALSE;
  gchar *basename;

  /* Getting widgets */
  entry = glade_xml_get_widget (user_data, "repocreate_nrepo_ent");
  window = glade_xml_get_widget (user_data, "repo_create_dialog");
  fsfs = glade_xml_get_widget (user_data, "repocreate_fsfs_rad");
  bdb = glade_xml_get_widget (user_data, "repocreate_bdb_rad");
  commit_sync = glade_xml_get_widget (user_data, "repocreate_tcommit_chk");
  logrm = glade_xml_get_widget (user_data, "repocreate_logrm_chk");

  path = g_object_get_data (G_OBJECT(window), "path");
  basename = g_path_get_basename(path);
  txt = gtk_entry_get_text (GTK_ENTRY(entry));

  if (g_strcasecmp (txt, "") == 0)
    {
      const char *buttons[] = {"OK", NULL};
      Show_Msgbox (window, FALSE, "Error",
                   _("Repository name invalid ..."),
                   GNOME_MESSAGE_BOX_ERROR,
                   buttons);
      gtk_entry_set_text (GTK_ENTRY(entry), basename);
      return;
    }
  
  if (g_strcasecmp (basename, txt) != 0)
    newpath = g_build_path (G_DIR_SEPARATOR_S,
                            path, txt, NULL);
  else
    newpath = g_build_path (G_DIR_SEPARATOR_S,
                            path, NULL);

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(fsfs)))
    fs_type = g_strdup (_("fsfs"));

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(bdb)))
    { 
      fs_type = g_strdup (_("bdb"));
      is_sync = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(commit_sync));
      is_logrm = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(logrm));
    }
    
 
  /* TODO: Following NULL will be replace later by a valid config_dir,
    which will be taken from the NaughtySVN preference or configuration */
  nsvn = nsvn_base_init (NULL);
  if (nsvn_admin_create_repos (nsvn, is_sync, is_logrm,
                               fs_type, NULL, newpath) == EXIT_FAILURE)
    {
      const char *buttons[] = {_("OK"), NULL};
      Show_Msgbox (window, FALSE, _("Error"),
                   _("Repository creation failed ..."),
                   GNOME_MESSAGE_BOX_ERROR,
                   buttons);
    }
  else
      gtk_widget_destroy (window);

  nsvn = nsvn_base_uninit (nsvn);
  
  g_free (newpath);
  g_free (fs_type);
  g_free (basename);
  
  return;
}


static void
nsvn__destroy_window  (GtkWidget *widget,
                       GladeXML *user_data)
{
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
}


static void
nsvn__cancel  (GtkWidget *widget,
               GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "repo_create_dialog");
  if (window)
    gtk_widget_destroy (window);
}


gboolean
nsvn_dlg_reposcreate    (GtkWidget *widget,
                         gpointer user_data)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *fsfs_rad;
  GtkWidget *bdb_rad;
  GtkWidget *nrepo_ent;
  GtkWidget *create_btn;
  GtkWidget *cancel_btn;
  char *uri;
  char *basename;

  uri = gnome_vfs_get_uri_from_local_path ((const char*) user_data);

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE,
                           "repo_create_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return EXIT_FAILURE;
    }
  g_warning (_("Found Glade file at [%s]\n"), GLADEDIR "/" DLG_GLADE_FILE);

  /* Get Widgets in repository creation dialog */
  window = glade_xml_get_widget (dlg_gui, "repo_create_dialog");
  fsfs_rad = glade_xml_get_widget (dlg_gui, "repocreate_fsfs_rad");
  bdb_rad = glade_xml_get_widget (dlg_gui, "repocreate_bdb_rad");
  nrepo_ent = glade_xml_get_widget (dlg_gui, "repocreate_nrepo_ent");
  create_btn = glade_xml_get_widget (dlg_gui, "repocreate_create_btn");
  cancel_btn = glade_xml_get_widget (dlg_gui, "repocreate_cancel_btn");

  g_object_set_data (G_OBJECT(window), "path", user_data);
 
  basename = g_path_get_basename ((char*)user_data);
  gtk_entry_set_text (GTK_ENTRY(nrepo_ent), basename);
  
  /* Connecting widgets to callbacks */
  g_signal_connect (G_OBJECT (fsfs_rad), "clicked",
                    G_CALLBACK (nsvn__choose_fsfs_options),
                    dlg_gui);
  g_signal_connect (G_OBJECT (bdb_rad), "clicked",
                    G_CALLBACK (nsvn__choose_bdb_options),
                    dlg_gui);
  g_signal_connect (G_OBJECT (create_btn), "clicked",
                    G_CALLBACK (nsvn__create_repository),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__cancel),
                    dlg_gui);
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
  
  /* Activating dialog box */
  gtk_widget_grab_focus (nrepo_ent);
  gtk_widget_show (window);

  g_free(basename);
  return EXIT_SUCCESS;
}
