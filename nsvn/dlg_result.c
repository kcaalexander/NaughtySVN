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
#include "svn/svn-nsvn-types.h" /* we need access to err */
#include "svn_client.h"
#include "svn_types.h"
#include "svn_wc.h"
#include "dlg_result.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"

struct dlg_result_context
{
  int cancelled;
  GtkListStore *store;
  GtkWidget *window;
  GtkWidget *cancel;
  GtkWidget *ok;
  GtkWidget *output_lbl;
  GladeXML *dlg_gui;
  gchar *error_message;
};

enum
{
  COLUMN_ACTION,
  COLUMN_PATH,
  COLUMN_MIME,
  NUM_COLUMNS
};

static const char *
svn_wc_notify_action_t_to_string(svn_wc_notify_action_t action)
{
  switch (action)
  {
    case svn_wc_notify_add:
      return "add";
    case svn_wc_notify_copy:
      return "copy";
    case svn_wc_notify_delete:
      return "delete";
    case svn_wc_notify_restore:
      return "restore";
    case svn_wc_notify_revert:
      return "revert";
    case svn_wc_notify_failed_revert:
      return "failed_revert";
    case svn_wc_notify_resolved:
      return "resolved";
    case svn_wc_notify_skip:
      return "skip";
    case svn_wc_notify_update_delete:
      return "update delete";
    case svn_wc_notify_update_add:
      return "update add";
    case svn_wc_notify_update_update:
      return "update update";
    case svn_wc_notify_update_completed:
      return "update complete";
    case svn_wc_notify_update_external:
      return "update external";
    case svn_wc_notify_status_completed:
      return "status completed";
    case svn_wc_notify_status_external:
      return "status external";
    case svn_wc_notify_commit_modified:
      return "commit modified";
    case svn_wc_notify_commit_added:
      return "commit added";
    case svn_wc_notify_commit_deleted:
      return "commit deleted";
    case svn_wc_notify_commit_replaced:
      return "commit replaced";
    case svn_wc_notify_commit_postfix_txdelta:
      return "commit postfix txdelta";
    case svn_wc_notify_blame_revision:
      return "blame revision";
    case svn_wc_notify_locked:
      return "locked";
    case svn_wc_notify_unlocked:
      return "unlocked";
    case svn_wc_notify_failed_lock:
      return "failed lock";
    case svn_wc_notify_failed_unlock:
      return "failed unlock";
    default:
      return "unknown";
  }
}

static const char *
svn_node_kind_t_to_string(svn_node_kind_t kind)
{
  switch (kind)
  {
    case svn_node_none:
      return "none";
    case svn_node_file:
      return "file";
    case svn_node_dir:
      return "dir";
    default:
    case svn_node_unknown:
      return "unknown";
  }
}

static const char *
svn_wc_notify_state_t_to_string(svn_wc_notify_state_t state)
{
  switch (state)
  {
    case svn_wc_notify_state_inapplicable:
      return "inapplicable";
    case svn_wc_notify_state_unknown:
    default:
      return "unknown";
    case svn_wc_notify_state_unchanged:
      return "unchanged";
    case svn_wc_notify_state_missing:
      return "missing";
    case svn_wc_notify_state_obstructed:
      return "obstructed";
    case svn_wc_notify_state_changed:
      return "changed";
    case svn_wc_notify_state_merged:
      return "merged";
    case svn_wc_notify_state_conflicted:
      return "conflicted";
  }
}

static const char *
svn_wc_notify_lock_state_t_to_string(svn_wc_notify_lock_state_t state)
{
  switch (state)
  {
    case svn_wc_notify_lock_state_inapplicable:
      return "inapplicable";
    case svn_wc_notify_lock_state_unknown:
    default:
      return "unknown";
    case svn_wc_notify_lock_state_unchanged:
      return "unchanged";
    case svn_wc_notify_lock_state_locked:
      return "locked";
    case svn_wc_notify_lock_state_unlocked:
      return "unlocked";
  }
}

#include <stdio.h>
static void
notify_func (void *baton,
             const svn_wc_notify_t *notify,
             apr_pool_t *pool)
{
  struct dlg_result_context *context = baton;
  GtkTreeIter iter;

  fprintf(stderr, "path: %s\n", notify->path);
  fprintf(stderr, "action: %s\n", svn_wc_notify_action_t_to_string(notify->action));
  fprintf(stderr, "kind: %s\n", svn_node_kind_t_to_string(notify->kind));
  fprintf(stderr, "mime_type: %s\n", notify->mime_type?notify->mime_type:"");
  fprintf(stderr, "lock: TODO\n");//%s\n", svn_lock_t_to_string(notify->lock));
  fprintf(stderr, "err: %s\n", notify->err?notify->err->message:"");
  fprintf(stderr, "content_state: %s\n", svn_wc_notify_state_t_to_string(notify->content_state));
  fprintf(stderr, "prop_state: %s\n", svn_wc_notify_state_t_to_string(notify->prop_state));
  fprintf(stderr, "lock_state: %s\n", svn_wc_notify_lock_state_t_to_string(notify->lock_state));
  fprintf(stderr, "revision: %" SVN_REVNUM_T_FMT "\n", notify->revision);
  fprintf(stderr, "\n");

  gtk_list_store_append (context->store, &iter);
  gtk_list_store_set (context->store, &iter, COLUMN_ACTION, notify->action, COLUMN_PATH, notify->path, COLUMN_MIME, notify->mime_type, -1);

  while (g_main_context_iteration(NULL, FALSE));
}

static void
progress_func (apr_off_t progress,
               apr_off_t total,
               void *baton,
               apr_pool_t *pool)
{
  while (g_main_context_iteration(NULL, FALSE));
}

static svn_error_t *
cancel_func (void *baton)
{
  struct dlg_result_context *context = baton;

  while (g_main_context_iteration(NULL, FALSE));

  if (context->cancelled)
    return svn_error_create (SVN_ERR_CANCELLED, NULL, "User aborted");
  else
    return SVN_NO_ERROR;
}

static void
ok_clicked (GtkButton *button,
            gpointer user_data)
{
  gtk_main_quit();
}
 
static void
cancel_clicked (GtkButton *button,
                gpointer user_data)
{
  struct dlg_result_context *context = user_data;

  context->cancelled = 1;
}

static gboolean
window_delete_event (GtkWidget *widget,
                     GdkEvent  *event,
                     gpointer   user_data) 
{
  return TRUE;
}

void
nsvn_dlg_result_hide (gpointer user_data)
{
  struct dlg_result_context *context = user_data;

  if (!context)
    return;

  if (context->error_message)
  {
    gtk_label_set_text (GTK_LABEL(context->output_lbl), context->error_message);  
    g_free(context->error_message);
  }

  gtk_widget_set_sensitive (context->cancel, FALSE);

  gtk_widget_set_sensitive (context->ok, TRUE);

  gtk_main();

  gtk_widget_destroy (context->window);

  g_object_unref (context->dlg_gui);

  g_free (context);
}

static void
action_column_data (GtkTreeViewColumn *tree_column,
                    GtkCellRenderer *cell,
                    GtkTreeModel *tree_model,
                    GtkTreeIter *iter,
                    gpointer data)
{
  svn_wc_notify_action_t action;
  const char *text;

  gtk_tree_model_get(tree_model, iter, COLUMN_ACTION, &action, -1);

  switch (action)
  {
    case svn_wc_notify_add:
      text = "add";
      break;
    case svn_wc_notify_copy:
      text = "copy";
      break;
    case svn_wc_notify_delete:
      text = "delete";
      break;
    case svn_wc_notify_restore:
      text = "restore";
      break;
    case svn_wc_notify_revert:
      text = "revert";
      break;
    case svn_wc_notify_failed_revert:
      text = "failed_revert";
      break;
    case svn_wc_notify_resolved:
      text = "resolved";
      break;
    case svn_wc_notify_skip:
      text = "skip";
      break;
    case svn_wc_notify_update_delete:
      text = "update delete";
      break;
    case svn_wc_notify_update_add:
      text = "update add";
      break;
    case svn_wc_notify_update_update:
      text = "update update";
      break;
    case svn_wc_notify_update_completed:
      text = "update complete";
      break;
    case svn_wc_notify_update_external:
      text = "update external";
      break;
    case svn_wc_notify_status_completed:
      text = "status completed";
      break;
    case svn_wc_notify_status_external:
      text = "status external";
      break;
    case svn_wc_notify_commit_modified:
      text = "commit modified";
      break;
    case svn_wc_notify_commit_added:
      text = "commit added";
      break;
    case svn_wc_notify_commit_deleted:
      text = "commit deleted";
      break;
    case svn_wc_notify_commit_replaced:
      text = "commit replaced";
      break;
    case svn_wc_notify_commit_postfix_txdelta:
      text = "commit postfix txdelta";
      break;
    case svn_wc_notify_blame_revision:
      text = "blame revision";
      break;
    case svn_wc_notify_locked:
      text = "locked";
      break;
    case svn_wc_notify_unlocked:
      text = "unlocked";
      break;
    case svn_wc_notify_failed_lock:
      text = "failed lock";
      break;
    case svn_wc_notify_failed_unlock:
      text = "failed unlock";
      break;
    default:
      text = "unknown";
  }

  g_object_set(cell, "text", text, NULL);
}

gpointer
nsvn_dlg_result_show (void)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *result_output_lst;
  GtkWidget *cancel, *ok;
  GtkWidget *output_lbl;
  struct dlg_result_context *context;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, "result_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return 0;
    }

  /* Getting widgets in result dialog. */
  window = glade_xml_get_widget (dlg_gui, "result_dialog");
  result_output_lst = glade_xml_get_widget (dlg_gui, "result_output_lst");
  cancel = glade_xml_get_widget (dlg_gui, "result_cancel_btn");
  ok = glade_xml_get_widget (dlg_gui, "result_ok_btn");
  output_lbl = glade_xml_get_widget (dlg_gui, "result_output_lbl");

  if ( ! ( window && result_output_lst && output_lbl) )
  {
    g_object_unref (dlg_gui);
    return 0;
  }

  gtk_widget_set_sensitive (ok, FALSE);
  g_signal_connect (ok, "clicked", G_CALLBACK (ok_clicked), context);
  g_signal_connect (cancel, "clicked", G_CALLBACK (cancel_clicked), context);
  g_signal_connect (window, "delete-event", G_CALLBACK (window_delete_event), context);

  context = g_malloc0 (sizeof (*context));
  context->window = window;
  context->ok = ok;
  context->cancel = cancel;
  context->dlg_gui = dlg_gui;
  context->output_lbl = output_lbl;

  context->store = gtk_list_store_new (NUM_COLUMNS,
                                       G_TYPE_INT,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING);
  gtk_tree_view_set_model (GTK_TREE_VIEW (result_output_lst),
                           GTK_TREE_MODEL (context->store));

  /* Setting widgets in list */
  {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Action"), renderer,
                                                       NULL);
    gtk_tree_view_column_set_cell_data_func (column, renderer, action_column_data, NULL, NULL);
    gtk_tree_view_column_set_sort_column_id (column, COLUMN_ACTION);
    gtk_tree_view_append_column (GTK_TREE_VIEW(result_output_lst), column);

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Path"), renderer,
                                                       "text",
                                                       COLUMN_PATH,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, COLUMN_PATH);
    gtk_tree_view_append_column (GTK_TREE_VIEW(result_output_lst), column);

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("MIME"), renderer,
                                                       "text",
                                                       COLUMN_MIME,
                                                       NULL);
    gtk_tree_view_column_set_sort_column_id (column, COLUMN_MIME);
    gtk_tree_view_append_column (GTK_TREE_VIEW(result_output_lst), column);


  }

  gtk_widget_show (window);

  return context;
}

void
nsvn_dlg_result_attach(gpointer user_data,
                       nsvn_t *nsvn)
{
  struct dlg_result_context *context = user_data;

  if (context)
  {
    GtkWidget *result_output_lst;
    GtkTreeModel *model;

    result_output_lst = glade_xml_get_widget (context->dlg_gui, "result_output_lst");

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (result_output_lst) );

    nsvn_base_setup_notify (nsvn, notify_func, context);

    nsvn_base_setup_cancel (nsvn, cancel_func, context);

    nsvn_base_setup_progress (nsvn, progress_func, context);
  }
}

void
nsvn_dlg_result_deattach(gpointer user_data,
                         nsvn_t *nsvn)
{
  struct dlg_result_context *context = user_data;

  if (context)
  {
    if (nsvn->err)
    {
      GString *string = g_string_new(0);
      svn_error_t *err = nsvn->err;
      while (err)
      {
        g_string_append_printf(string, "%s%s", string->len?"\n":"", err->message);
	err = err->child;
      }
      context->error_message = string->str;
      g_string_free( string, FALSE);
    }

    nsvn_base_setup_notify (nsvn, 0, 0);

    nsvn_base_setup_cancel (nsvn, 0, 0);

    nsvn_base_setup_progress (nsvn, 0, 0);
  }
}
