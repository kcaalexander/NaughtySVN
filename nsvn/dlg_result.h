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

void
nsvn_dlg_result_hide (gpointer);

gpointer
nsvn_dlg_result_show (void);

void
nsvn_dlg_result_attach(gpointer,
                       nsvn_t *nsvn);

void
nsvn_dlg_result_deattach(gpointer,
                         nsvn_t *nsvn);

#if 0
/* Function to handle PROGRESS BAR. */
gboolean
nsvn_dlg_result_show_pbar (GtkWidget *parent_window,
                           gpointer user_data);


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
#endif

#endif /* NSVN_DLG_RESULT_H */
