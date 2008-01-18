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

#include "svn_wc.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"

enum
{
  COLUMN_CHECK,
  COLUMN_STOCK,
  COLUMN_BASENAME,
  COLUMN_PATH,
  NUM_COLUMNS
};

void*
nsvn__get_logmessage (const char **log_msg,
                      const char **tmp_file,
                      const void *commit_items,
                      void *baton, void *pool);


static void
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
    }
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
}

static void
nsvn__cancel (GtkWidget *widget,
              GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "commit_dialog");
  gtk_widget_destroy (window);
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
nsvn__toggle_check (GtkCellRendererToggle *cell,
                    char *path_str, void *data)
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreeIter  iter;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  gboolean check;

  /* get toggled iter. */
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, COLUMN_CHECK, &check, -1);
  /* toggling. */
  check ^= 1;
  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_CHECK, check, -1);

  gtk_tree_path_free (path);
}

static void
wrapped_g_free (gpointer data,
                gpointer user_data)
{
  g_free (data);
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
  GtkTreeModel *model;
  GtkTreeIter iter;
  gboolean found = FALSE;
  GtkWidget *file_lst;
 
  window = glade_xml_get_widget (user_data, "commit_dialog");

  file_lst = glade_xml_get_widget (user_data, "add_file_lst");
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (file_lst));

  /* Nothing to do is no model present. */
  if (!model) goto out1;

  found = gtk_tree_model_get_iter_first (model, &iter);
  while (found)
    { gboolean check;
      char *bname, *path, *full_path;

      gtk_tree_model_get (model, &iter, COLUMN_CHECK, &check,
                          COLUMN_BASENAME, &bname,
                          COLUMN_PATH, &path, -1);
      /* Commit only if its selected. */
      if (check)
        {
          full_path = g_build_filename (path, bname, NULL);
          files = g_list_append (files, full_path);
        }
      found = gtk_tree_model_iter_next (model, &iter);
    }

  nitems = g_list_length (files);

  target_list = g_malloc ((nitems+1)*(sizeof(char*)));
  if (!target_list)
    goto out2;

  for (i=0; i<nitems; i++)
      target_list[i] = g_list_nth_data (files, i);
  target_list[nitems] = NULL;

  nsvn = nsvn_base_init (NULL);

  nsvn_base_setup_log (nsvn, nsvn__get_logmessage, (void*)user_data);

  nsvn_wc_commit (nsvn, target_list, 1, 1);

  g_free (target_list);

  nsvn = nsvn_base_uninit (nsvn);

out2:
  g_list_foreach (files, wrapped_g_free, NULL);
  g_list_free (files);

out1:
  gtk_widget_destroy (window);

  return;
}

static int
nsvn__populate_files (void *data,
                           const char *path,
                           svn_wc_status2_t *status)
{
  GtkListStore *store;
  GtkTreeIter iter;

  if (!data)
    return EXIT_FAILURE;

  store = g_object_get_data (G_OBJECT(data), "store");

  if (!status->entry)
    return EXIT_SUCCESS;

  switch (status->text_status)
  {
    case svn_wc_status_none:
    case svn_wc_status_unversioned:
    case svn_wc_status_normal:
    case svn_wc_status_missing:
    case svn_wc_status_conflicted:
    case svn_wc_status_ignored:
    case svn_wc_status_obstructed:
    case svn_wc_status_external:
    case svn_wc_status_incomplete:
      break;
    case svn_wc_status_added:
    case svn_wc_status_deleted:
    case svn_wc_status_replaced:
    case svn_wc_status_modified:
    case svn_wc_status_merged:
    {
      GdkPixbuf *icon = 0;
      gchar *basename;
      gchar *dirname;

      /* Allocating a new row in store. */
      gtk_list_store_append (store, &iter);

      /* Setting the unrev in the newely created store row. */
      switch (status->entry->kind)
      {
        case svn_node_none:
        case svn_node_unknown:
          break;
          //TODO: Move out icon rendering code out of this function.
          //      multiple rendering need more resources, Just need to
          //      create only one copy of two versions (DIR, FILE) of
          //      icons and we can utilise the same copy for all the
          //      store elements.
        case svn_node_dir:
            icon = gtk_widget_render_icon (GTK_WIDGET (data),
                                           GTK_STOCK_DIRECTORY,
                                           GTK_ICON_SIZE_MENU,
                                           NULL);
        case svn_node_file:
            icon = gtk_widget_render_icon (GTK_WIDGET (data),
                                           GTK_STOCK_FILE,
                                           GTK_ICON_SIZE_MENU,
                                           NULL);
      }
      basename = g_path_get_basename (path);
      dirname = g_path_get_dirname (path);

      gtk_list_store_set (store, &iter,
                          COLUMN_CHECK, TRUE,
                          COLUMN_STOCK, icon,
                          COLUMN_BASENAME, basename,
                          COLUMN_PATH, dirname, -1);

      g_free(basename);
      g_free(dirname);
    }
  }

  return EXIT_SUCCESS;
}

static GtkTreeModel*
nsvn__find_files (GtkWidget *widget,
                  GtkListStore *store,
                  gpointer args)
{
  nsvn_t *nsvn;
  GList *files = NULL;

  Split_Arg ((char*) args, &files);

  g_object_set_data (G_OBJECT(widget), "files", files);

  nsvn = nsvn_base_init (NULL);
  g_object_set_data (G_OBJECT(widget), "store", store);

  /* Iterating thorugh the file list. */
  while (files)
  {
    nsvn_wc_status (nsvn, files->data,
                    nsvn__populate_files,
                    (void*) widget, TRUE,
                    FALSE, FALSE, TRUE, FALSE);
    files = g_list_next (files);
  }

  nsvn = nsvn_base_uninit (nsvn);

  return GTK_TREE_MODEL (store);
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
  GtkWidget *file_lst;
  GtkTreeModel *model;
  GtkListStore *store;


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
  file_lst = glade_xml_get_widget (dlg_gui, "commit_file_lst");

  #if HAVE_GTKSPELL
  {
    gtkspell_new_attach (GTK_TEXT_VIEW(logmsg_txt), "en_US", NULL);
  }      
  #endif

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__cancel),
                    dlg_gui);
  g_signal_connect (G_OBJECT (ok_btn), "clicked",
                    G_CALLBACK (nsvn__commit),
                    dlg_gui);

  //TODO: Only one working copy commit should support.
  //      if commit entities from more than one wc choosen,
  //      then either commit should not happen or else
  //      remove all entity that don't match from the list.
  store = gtk_list_store_new (NUM_COLUMNS,
                              G_TYPE_BOOLEAN,
                              GDK_TYPE_PIXBUF,
                              G_TYPE_STRING,
                              G_TYPE_STRING);
  model = nsvn__find_files ( window, store, args );
  gtk_tree_view_set_model (GTK_TREE_VIEW (file_lst),
                           model);
  gtk_tree_view_set_search_column (GTK_TREE_VIEW(file_lst),
                                   COLUMN_BASENAME);

  /* Setting widgets in list. */
  {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    column = gtk_tree_view_column_new ();

    /* Adding toggle button as first col. */
    renderer = gtk_cell_renderer_toggle_new ();
    gtk_tree_view_column_pack_start (column, renderer, FALSE);
    g_signal_connect (renderer, "toggled",
                      G_CALLBACK (nsvn__toggle_check), model);
    gtk_tree_view_column_set_attributes (column, renderer,
                                         "active", COLUMN_CHECK,
                                         NULL);

    /* Adding pixbuf to the first col. */
    renderer = gtk_cell_renderer_pixbuf_new ();
    gtk_tree_view_column_pack_start (column, renderer, TRUE);
    gtk_tree_view_column_add_attribute (column, renderer,
                                        "pixbuf", COLUMN_STOCK);
    gtk_tree_view_append_column (GTK_TREE_VIEW (file_lst), column);
    
    /* Adding text boxes for second col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Items to commit"), renderer,
                                                       "text",
                                                       COLUMN_BASENAME,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, COLUMN_BASENAME);
    gtk_tree_view_append_column (GTK_TREE_VIEW(file_lst), column);

    /* Adding text boxes for third col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Working copy path"), renderer,
                                                       "text",
                                                       COLUMN_PATH,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, COLUMN_PATH);
    gtk_tree_view_append_column (GTK_TREE_VIEW(file_lst), column);
  }

  /* Activating dialog box. */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
