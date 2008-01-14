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

#if HAVE_GTKSPELL
#include <gtkspell/gtkspell.h>
#endif

#include "svn/naughtysvn.h"
#include "dlg_commit.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


void*
nsvn__get_logmessage (const char **log_msg,
                      const char **tmp_file,
                      const void *commit_items,
                      void *baton, void *pool);


static int
nsvn__destroy_window (GtkWidget *widget,
                      GladeXML *user_data)
{
  GtkWidget *window;
  GtkWidget *logmsg_txt;

  window = glade_xml_get_widget (user_data, "commit_dialog");
  logmsg_txt = glade_xml_get_widget (user_data, "commit_logmsg_txt");
  if (window)
    {
      #if HAVE_GTKSPELL
      GtkSpell *spell;
      spell = gtkspell_get_from_text_view (GTK_TEXT_VIEW(logmsg_txt));
      if (spell)
        gtkspell_detach (spell);
      #endif
      gtk_widget_destroy (window);
    }
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
  return 0;
}


void*
nsvn__get_logmessage (const char **log_msg,
                      const char **tmp_file,
                      const void *commit_items,
                      void *baton, void *pool)
{
  GtkWidget *window;
  GtkWidget *textview;
  GtkTextBuffer *textbuf;
  GladeXML *data = (GladeXML*) baton;
  GtkTextIter start;
  GtkTextIter end;

  window = glade_xml_get_widget (data, "commit_dialog");
  textview = glade_xml_get_widget (data, "commit_logmsg_txt");
  textbuf = gtk_text_view_get_buffer (GTK_TEXT_VIEW(textview));
  gtk_text_buffer_get_start_iter (textbuf, &start);
  gtk_text_buffer_get_end_iter (textbuf, &end);

  *log_msg = gtk_text_buffer_get_text(textbuf, &start, &end, FALSE);

  return 0;
}


static void
nsvn__commit (GtkWidget *widget,
              GladeXML *user_data)
{
  GtkWidget *window;
  nsvn_t *nsvn;
  GList *files = NULL;
  const char **target_list;
  unsigned int nitems, i;

  window = glade_xml_get_widget (user_data, "commit_dialog");

  files = g_object_get_data (G_OBJECT(window), "files");
  nitems = g_list_length (files);

  target_list = g_malloc((nitems+1)*(sizeof(char*)));
  if (!target_list)
    return;

  for (i=0; i<nitems; i++)
      target_list[i] = g_list_nth_data (files, i);
  target_list[nitems] = NULL;

  nsvn = nsvn_base_init (NULL);

  nsvn_base_setup_log (nsvn, nsvn__get_logmessage, (void*)user_data);

  nsvn_wc_commit (nsvn, target_list, 1, 1);

  g_free(target_list);

  nsvn = nsvn_base_uninit (nsvn);
  gtk_widget_destroy (window);

  return;
}


gboolean
nsvn_dlg_commit (GtkWidget *widget,
                 gpointer args)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cancel_btn;
  GtkWidget *ok_btn;
  GtkWidget *logmsg_txt;
  GtkWidget *repospath_lbl;
  GList *files = NULL;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, "commit_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "commit_dialog");
  cancel_btn = glade_xml_get_widget (dlg_gui, "commit_cancel_btn");
  ok_btn = glade_xml_get_widget (dlg_gui, "commit_ok_btn");
  logmsg_txt = glade_xml_get_widget (dlg_gui, "commit_logmsg_txt");
  repospath_lbl = glade_xml_get_widget (dlg_gui, "commit_repospath_lbl");

  #if HAVE_GTKSPELL
  {
    GtkSpell *spell;
    GError *error;
    spell = gtkspell_new_attach (GTK_TEXT_VIEW(logmsg_txt), "en_US",
                                 &error);
  }      
  #endif

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (ok_btn), "clicked",
                    G_CALLBACK (nsvn__commit),
                    dlg_gui);
  Split_Arg ((char*) args, &files);
  //TODO: Only one working copy commit should support.
  //      if commit entities from more than one wc choosen,
  //      then either commit should not happen or else
  //      remove all entity that don't match from the list.
  g_object_set_data (G_OBJECT(window), "files", files);

  /* Activating dialog box. */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
