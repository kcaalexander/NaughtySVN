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
#include "dlg_log.h"
#include "global.h"

#include <svn_client.h>
#include <svn_time.h>
#include <apr_pools.h>

#define DLG_GLADE_FILE  "naughtysvn.glade"


enum
{
  LOG_COLUMN_REV,
  LOG_COLUMN_AUTHOR,
  LOG_COLUMN_DATE,
  LOG_COLUMN_MSG,
  LOG_COLUMN_PATH,
  NUM_LOG_COLUMNS
};


enum
{
  CLOG_COLUMN_ACTION,
  CLOG_COLUMN_PATH,
  CLOG_COLUMN_COPY_PATH,
  CLOG_COLUMN_COPY_REV,
  NUM_CLOG_COLUMNS
};


static void
nsvn__destroy_window (GtkWidget *widget,
                      GladeXML *user_data)
{
  GtkWidget *window;
  nsvn_t *nsvn;

  window = glade_xml_get_widget (user_data, "log_dialog");
  nsvn = g_object_get_data(G_OBJECT(window), "nsvn");
  nsvn = nsvn_base_uninit(nsvn);
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
}


static void
nsvn__close (GtkWidget *widget,
             GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "log_dialog");
  gtk_widget_destroy (window);
}


static void*
nsvn__populate_logmsgs (void *data, apr_hash_t *changed_path,
                        svn_revnum_t rev, const char *author,
                        const char *date, const char *msg,
                        void *pool)
{
  GtkListStore *store;
  GtkTreeIter iter;
  GtkWidget *window;
  apr_time_t temp;
  const char *hdate;
  apr_hash_t *duplicate;
  nsvn_t *nsvn;
  apr_pool_t *nsvn_pool;


  if (!data || !SVN_IS_VALID_REVNUM(rev))
    return NULL;

  window = glade_xml_get_widget ((GladeXML*)data, "log_dialog");
  store = g_object_get_data (G_OBJECT(window), "store");
  nsvn = g_object_get_data (G_OBJECT(window), "nsvn");
  
  /* Convert time to human readable format. */
  svn_time_from_cstring(&temp, date, pool);
  hdate = svn_time_to_human_cstring(temp, pool);

  nsvn_pool = (apr_pool_t*)nsvn_base_get_aprpool(nsvn);
  duplicate = apr_hash_copy(nsvn_pool, changed_path);

  /* Allocating a new row in store. */
  gtk_list_store_append (store, &iter);
  /* Setting the log in the newely created store row. */
  gtk_list_store_set (store, &iter,
                      LOG_COLUMN_REV, rev, LOG_COLUMN_AUTHOR, author,
                      LOG_COLUMN_DATE, hdate, LOG_COLUMN_MSG, msg,
                      LOG_COLUMN_PATH, (void*)duplicate, -1);

  return NULL;
}


static int
nsvn__showlogmsg (GtkWidget *widget,
                  GladeXML *user_data)
{
  GtkTreeSelection *sel_row;
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkWidget *window;
  GtkWidget *logmsg_txt;
  GtkWidget *clog_lst;
  nsvn_t *nsvn;

  window = glade_xml_get_widget (user_data, "log_dialog");
  logmsg_txt = glade_xml_get_widget (user_data, "log_msg_txt");
  clog_lst = glade_xml_get_widget (user_data, "log_chnglist_lst");

  sel_row = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));

  if (gtk_tree_selection_get_selected (sel_row, &model, &iter))
    {
      GtkTextBuffer *txt_buf;
      gchar *logmsg;
      apr_hash_t *changed_path;

      gtk_tree_model_get (model, &iter, LOG_COLUMN_MSG, &logmsg,
                          LOG_COLUMN_PATH, &changed_path, -1);

      txt_buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (logmsg_txt));
      gtk_text_buffer_set_text(txt_buf, logmsg, -1);
      g_free (logmsg);

      if (changed_path)
        {
          apr_hash_index_t *hi;
          char *path;
          apr_pool_t *pool;
          GtkListStore *cstore;
          GtkTreeIter iter;

          nsvn = g_object_get_data (G_OBJECT(window), "nsvn");
          pool = (apr_pool_t*)nsvn_base_get_aprpool(nsvn);
          cstore = g_object_get_data (G_OBJECT(window), "cstore");
          gtk_list_store_clear(GTK_LIST_STORE(cstore));

          for (hi = apr_hash_first (pool, changed_path);
               hi != NULL;
               hi = apr_hash_next (hi))
            {
              void *val;
              char action[10];
              char revstr[20];
              svn_log_changed_path_t *log_item;

              apr_hash_this(hi, (void *) &path, NULL, &val);
              printf ("\n****** Path [%s]\n", path);
              log_item = val;

              bzero(revstr, 20);
              if (log_item->copyfrom_rev >=0)
                sprintf (revstr, "%ld", log_item->copyfrom_rev);

              bzero(action, 10);
              if (log_item->action == 'A')
                strcpy (action, "Added");
              else if (log_item->action == 'D')
                strcpy (action, "Deleted");
              else if (log_item->action == 'R')
                strcpy (action, "Replaced");
              else if (log_item->action == 'M')
                strcpy (action, "Modified");
              else 
                action[0] = 0; 

              /* Allocating a new row in store. */
              gtk_list_store_append (cstore, &iter);
              gtk_list_store_set (cstore, &iter,
                                  CLOG_COLUMN_ACTION, action,
                                  CLOG_COLUMN_PATH, path,
                                  CLOG_COLUMN_COPY_PATH, log_item->copyfrom_path,
                                  CLOG_COLUMN_COPY_REV, revstr, -1);
            }
        }
    }

  return EXIT_SUCCESS;
}


static int
nsvn__showlog (GtkWidget *widget,
               GladeXML *user_data)
{
  GtkWidget *window;
  GtkWidget *fromrev_ent;
  GtkWidget *torev_ent;
  GtkWidget *limit_ent;
  GtkWidget *nocopyhist_chk;
  GtkListStore *store;
  const char *target[]={NULL, NULL};
  const char *fromrev;
  const char *torev;
  int limit=0;
  nsvn_t *nsvn=NULL;

  window = glade_xml_get_widget (user_data, "log_dialog");
  fromrev_ent = glade_xml_get_widget (user_data, "log_fromrev_ent");
  torev_ent = glade_xml_get_widget (user_data, "log_torev_ent");
  limit_ent = glade_xml_get_widget (user_data, "log_limit_ent");
  nocopyhist_chk = glade_xml_get_widget (user_data, "log_nocopyhist_chk");

  target[0] = g_object_get_data (G_OBJECT(window), "path");
  fromrev = gtk_entry_get_text (GTK_ENTRY(fromrev_ent)); 
  torev = gtk_entry_get_text (GTK_ENTRY(torev_ent)); 
  limit = atoi(gtk_entry_get_text (GTK_ENTRY(limit_ent))); 
  
  store = g_object_get_data (G_OBJECT(window), "store");
  gtk_list_store_clear(GTK_LIST_STORE(store));

  nsvn = g_object_get_data (G_OBJECT(window), "nsvn");
  if (nsvn != NULL)
    nsvn = nsvn_base_clear(nsvn);

  nsvn = nsvn_base_init(NULL);
  g_object_set_data(G_OBJECT(window), "nsvn", (void*)nsvn);

  nsvn_wc_log (nsvn, target, fromrev, torev, limit, TRUE,
               gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(nocopyhist_chk)),
               nsvn__populate_logmsgs, (void*) user_data);

  return EXIT_SUCCESS;
}


gboolean
nsvn_dlg_log (GtkWidget *widget,
              gpointer args)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *close_btn;
  GtkWidget *showlog_btn;
  GtkWidget *log_lst;
  GtkWidget *clog_lst;
  GtkWidget *from_ent;
  GtkWidget *to_ent;
  GtkListStore *store;
  GtkListStore *cstore;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, "log_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "log_dialog");
  close_btn = glade_xml_get_widget (dlg_gui, "log_close_btn");
  showlog_btn = glade_xml_get_widget (dlg_gui, "log_showlog_btn");
  log_lst = glade_xml_get_widget (dlg_gui, "log_log_lst");
  clog_lst = glade_xml_get_widget (dlg_gui, "log_chnglist_lst");
  from_ent = glade_xml_get_widget (dlg_gui, "log_fromrev_ent");
  to_ent = glade_xml_get_widget (dlg_gui, "log_torev_ent");

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (close_btn), "clicked",
                    G_CALLBACK (nsvn__close),
                    dlg_gui);
  g_signal_connect (G_OBJECT (showlog_btn), "clicked",
                    G_CALLBACK (nsvn__showlog),
                    dlg_gui);
  g_signal_connect (G_OBJECT (log_lst), "cursor-changed",
                    G_CALLBACK (nsvn__showlogmsg),
                    dlg_gui);

  g_object_set_data (G_OBJECT(window), "path", args);

  /* Creating Model for the log list. */
  store = gtk_list_store_new (NUM_LOG_COLUMNS,
                              G_TYPE_LONG, G_TYPE_STRING,
                              G_TYPE_STRING, G_TYPE_STRING,
                              G_TYPE_POINTER);

  g_object_set_data (G_OBJECT(window), "store", store);

  nsvn__showlog(close_btn, dlg_gui);

  gtk_tree_view_set_model (GTK_TREE_VIEW (log_lst), GTK_TREE_MODEL(store));
  gtk_tree_view_set_search_column (GTK_TREE_VIEW(log_lst),
                                   LOG_COLUMN_REV);
  gtk_tree_view_set_search_column (GTK_TREE_VIEW(log_lst),
                                   LOG_COLUMN_AUTHOR);
  /* Setting widgets in log list. */
  {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    column = gtk_tree_view_column_new ();

    /* Adding text boxes for first REVISION col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Revision"), renderer,
                                                       "text", LOG_COLUMN_REV,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, LOG_COLUMN_REV);
    gtk_tree_view_append_column (GTK_TREE_VIEW(log_lst), column);

    /* Adding text boxes for second AUTHOR col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Author"), renderer,
                                                       "text", LOG_COLUMN_AUTHOR,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, LOG_COLUMN_AUTHOR);
    gtk_tree_view_append_column (GTK_TREE_VIEW(log_lst), column);

    /* Adding text boxes for third DATE col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Date"), renderer,
                                                       "text", LOG_COLUMN_DATE,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, LOG_COLUMN_DATE);
    gtk_tree_view_append_column (GTK_TREE_VIEW(log_lst), column);

    /* Adding text boxes for fourth MSG col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Message"), renderer,
                                                       "text", LOG_COLUMN_MSG,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(log_lst), column);
  }

  /* Creating Model for the change log list. */
  cstore = gtk_list_store_new (NUM_CLOG_COLUMNS,
                               G_TYPE_STRING, G_TYPE_STRING,
                               G_TYPE_STRING, G_TYPE_STRING);

  g_object_set_data (G_OBJECT(window), "cstore", cstore);

  gtk_tree_view_set_model (GTK_TREE_VIEW (clog_lst), GTK_TREE_MODEL(cstore));
  gtk_tree_view_set_search_column (GTK_TREE_VIEW(clog_lst),
                                   CLOG_COLUMN_ACTION);
  /* Setting widgets in change log list. */
  {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    column = gtk_tree_view_column_new ();

    /* Adding text boxes for first ACTION col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Action"), renderer,
                                                       "text", CLOG_COLUMN_ACTION,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, CLOG_COLUMN_ACTION);
    gtk_tree_view_append_column (GTK_TREE_VIEW(clog_lst), column);

    /* Adding text boxes for second PATH col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Path"), renderer,
                                                       "text", CLOG_COLUMN_PATH,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, CLOG_COLUMN_PATH);
    gtk_tree_view_append_column (GTK_TREE_VIEW(clog_lst), column);

    /* Adding text boxes for third COPY FROM PATH col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Copy from path"),
                                                       renderer, "text",
                                                       CLOG_COLUMN_COPY_PATH,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, CLOG_COLUMN_COPY_PATH);
    gtk_tree_view_append_column (GTK_TREE_VIEW(clog_lst), column);

    /* Adding text boxes for fourth COPY FROM REVISION col. */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Copy  from revision"),
                                                       renderer, "text",
                                                       CLOG_COLUMN_COPY_REV,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(clog_lst), column);
  }

  /* Activating dialog box. */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
