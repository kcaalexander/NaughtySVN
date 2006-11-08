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
#include "svn_wc.h"
#include "dlg_add.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


enum
{
  COLUMN_CHECK,
  COLUMN_STOCK,
  COLUMN_BASENAME,
  COLUMN_PATH,
  NUM_COLUMNS
};


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


static void
nsvn__add_files (GtkWidget *widget,
                 GladeXML *user_data)
{
  GtkWidget *window;
  GtkWidget *file_lst;
  GtkWidget *no_recurse_chk;
  GtkTreeModel *model;
  GtkTreeIter iter;
  gboolean found = FALSE, is_recurse = FALSE;
  nsvn_t *nsvn;

  window = glade_xml_get_widget (user_data, "add_dialog");

  /* Getting state of the check button widget. */
  no_recurse_chk = glade_xml_get_widget (user_data, "add_select_chk");
  is_recurse = gtk_toggle_button_get_active (
                                  GTK_TOGGLE_BUTTON (no_recurse_chk));

  /* Getting model from tree_view widget. */
  file_lst = glade_xml_get_widget (user_data, "add_file_lst");
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (file_lst));

  /* Nothing to do is no model present. */
  if (! model) return;

  nsvn = nsvn_base_init (NULL);

  /* Walk through all the rows in store. */
  found = gtk_tree_model_get_iter_first (model, &iter);
  while (found)
    { gboolean check;
      char *bname, *path, *full_path;

      gtk_tree_model_get (model, &iter, COLUMN_CHECK, &check,
                          COLUMN_BASENAME, &bname,
                          COLUMN_PATH, &path, -1);
      /* Add only if its selected. */
      if (check)
        {
          full_path = g_build_filename (path, bname, NULL);
          if (nsvn_wc_add (nsvn, full_path, !is_recurse, TRUE,
                          FALSE, FALSE, FALSE) == EXIT_SUCCESS)
            {
              /* TODO: Here show result. */
            }
          g_free (full_path);
        }
      found = gtk_tree_model_iter_next (model, &iter);
    }

  nsvn = nsvn_base_uninit (nsvn);
  gtk_widget_destroy (window);
  return;
}


static void
nsvn__select_toggle (GtkWidget *widget,
                     GladeXML *user_data)
{
  GtkWidget *file_lst;
  GtkWidget *select_chk;
  GtkTreeModel *model;
  GtkTreeIter iter;
  gboolean check, found = FALSE, is_select = FALSE;

  /* Getting state of the check button widget. */
  select_chk = glade_xml_get_widget (user_data, "add_select_chk");
  is_select = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (select_chk));

  /* Getting model from tree_view widget. */
  file_lst = glade_xml_get_widget (user_data, "add_file_lst");
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (file_lst));

  /* Nothing to do is no model present. */
  if (! model) return;

  /* Walk through all the rows in store. */
  found = gtk_tree_model_get_iter_first (model, &iter);
  while (found)
    {
      gtk_tree_model_get (model, &iter, COLUMN_CHECK, &check, -1);
      check = is_select;
      gtk_list_store_set (GTK_LIST_STORE (model), &iter,
                          COLUMN_CHECK, check, -1);
      found = gtk_tree_model_iter_next (model, &iter);
    }
  return;
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


static int
nsvn__populate_unrevfiles (void *data,
                           const char *path,
                           svn_wc_status2_t *status)
{
  GtkListStore *store;
  GtkTreeIter iter;

  if (!data)
    return EXIT_FAILURE;

  store = g_object_get_data (G_OBJECT(data), "store");

  if (status->entry == NULL ||
      ! SVN_IS_VALID_REVNUM(status->entry->revision))
    {
      struct stat buf;

      /* Allocating a new row in store. */
      gtk_list_store_append (store, &iter);

      /* Setting the unrev in the newely created store row. */
      if (stat (path, &buf) == 0)
      
        {
          GdkPixbuf *icon;
          //TODO: Move out icon rendering code out of this function.
          //      multiple rendering need more resources, Just need to
          //      create only one copy of two versions (DIR, FILE) of
          //      icons and we can utilise the same copy for all the
          //      store elements.
          if (S_ISDIR (buf.st_mode))
            icon = gtk_widget_render_icon (GTK_WIDGET (data),
                                           GTK_STOCK_DIRECTORY,
                                           GTK_ICON_SIZE_MENU,
                                           NULL);
          else
            icon = gtk_widget_render_icon (GTK_WIDGET (data),
                                           GTK_STOCK_FILE,
                                           GTK_ICON_SIZE_MENU,
                                           NULL);

          gtk_list_store_set (store, &iter,
                              COLUMN_CHECK, TRUE,
                              COLUMN_STOCK, icon,
                              COLUMN_BASENAME, g_path_get_basename (path),
                              COLUMN_PATH, g_path_get_dirname (path),
                              -1);
        }
    }

  return EXIT_SUCCESS;
}


static GtkTreeModel*
nsvn__find_unrevfiles (GtkWidget *widget,
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
                      nsvn__populate_unrevfiles,
                      (void*) widget, TRUE,
                      FALSE, FALSE, TRUE, FALSE);
      files = g_list_next(files);
    }

  nsvn = nsvn_base_uninit (nsvn);

  return GTK_TREE_MODEL (store);
}


gboolean
nsvn_dlg_add (GtkWidget *widget,
              gpointer args)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cancel_btn;
  GtkWidget *ok_btn;
  GtkWidget *select_chk;
  GtkWidget *no_recurse_chk;
  GtkWidget *file_lst;
  GtkTreeModel *model;
  GtkListStore *store;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, NULL, NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find ") GLADEDIR "/" DLG_GLADE_FILE "\n");
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "add_dialog");
  cancel_btn = glade_xml_get_widget (dlg_gui, "add_cancel_btn");
  ok_btn = glade_xml_get_widget (dlg_gui, "add_ok_btn");
  select_chk = glade_xml_get_widget (dlg_gui, "add_select_chk");
  no_recurse_chk = glade_xml_get_widget (dlg_gui, "non_recurse_chk");
  file_lst = glade_xml_get_widget (dlg_gui, "add_file_lst");

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (select_chk), "toggled",
                    G_CALLBACK (nsvn__select_toggle),
                    dlg_gui);
  g_signal_connect (G_OBJECT (ok_btn), "clicked",
                    G_CALLBACK (nsvn__add_files),
                    dlg_gui);

  /* Creating store for storing the unrev files. */
  store = gtk_list_store_new (NUM_COLUMNS,
                              G_TYPE_BOOLEAN,
                              GDK_TYPE_PIXBUF,
                              G_TYPE_STRING,
                              G_TYPE_STRING);
  /* Populate the unver item to list. */
  model = nsvn__find_unrevfiles (window, store, args);
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
    column = gtk_tree_view_column_new_with_attributes (_("Items to add"), renderer,
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
