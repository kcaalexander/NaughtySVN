/*
* ====================================================================
* Complte list of windows/dialogs and internals structure used
* in nsvn modules.
* Copyright (c) 2005, All rights reserved for Alexander Thomas (AT)
*
* 
* Leaving space here for Microsoft, to add license details.
*
*
* ====================================================================
*/

#ifndef NSVN_WINDOW_LIST_H
#define NSVN_WINDOW_LIST_H

#include "dlg_reposcreate.h"
#include "dlg_aboutnsvn.h"
#include "dlg_preferences.h"
#include "dlg_checkout.h"
#include "dlg_add.h"
#include "dlg_commit.h"
#include "dlg_update.h"
#include "dlg_log.h"

typedef gboolean
(*nsvn_command_func_t) (GtkWidget *widget,
                        const gchar **);

typedef struct nsvn_command {
  const char *cmd;
  nsvn_command_func_t cmd_func;
} nsvn_command;

#endif /* NSVN_WINDOW_LIST_H */
