/*
* ====================================================================
* Header file for NaughtySVN update dialog box.
* Copyright (c) 2007, All rights reserved for Eddy Petrișor
*
*
* Leaving space here to add license details.
*
*
* ====================================================================
*/

#ifndef NSVN_DLG_UPDATE_H
#define NSVN_DLG_UPDATE_H

gboolean
nsvn_dlg_update (GtkWidget *widget,
                 gpointer user_data);

void
on_externals_update_check_toggled (GtkWidget *widget,
                                    gpointer user_data);

#endif /* NSVN_DLG_UPDATE_H */
