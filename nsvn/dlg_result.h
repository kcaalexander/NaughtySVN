/*
* ====================================================================
* Header file for NaughtySVN result dialog box.
* Copyright (c) 2005, All rights reserved for Alexander Thomas (AT)
*
* 
* Leaving space here for Microsoft, to add license details.
*
*
* ====================================================================
*/

#ifndef NSVN_DLG_RESULT_H
#define NSVN_DLG_RESULT_H

/* Functions to handle result dialog. */

gboolean
nsvn_dlg_result_hide (GtkWidget *parent_window,
                      gpointer user_data);

gboolean
nsvn_dlg_result_show (GtkWidget *parent_window,
                      gpointer user_data);


/* Function to handle PROGRESS BAR. */
gboolean
nsvn_dlg_result_show_pbar (GtkWidget *parent_window,
                           gpointer user_data);

gboolean
nsvn_dlg_result_hide_pbar (GtkWidget *parent_window,
                           gpointer user_data);

gboolean
nsvn_dlg_result_set_pbar (GtkWidget *parent_window,
                          gpointer user_data);

gboolean
nsvn_dlg_result_get_pbar (GtkWidget *parent_window,
                          int *value,
                          gpointer user_data);

/* Function to set Summary details. */
gboolean
nsvn_dlg_result_set_summary (GtkWidget *parent_window,
                             char *value,
                             gpointer user_data);

#endif /* NSVN_DLG_RESULT_H */
