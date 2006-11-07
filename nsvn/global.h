/*
* ====================================================================
* Header file for global functions.
* Copyright (c) 2005 Alexander Thomas (AT) - NaughtySVN.
*
* 
* Leaving space here for Microsoft, to add license details.
*
*
* ====================================================================
*/

#include <gnome.h>

gboolean
check_directory_isempty (char *uri);


GtkWidget*
Show_Msgbox (
             GtkWidget *parent,
             gboolean noshow,
             const char *msg_type,
             const char *msg,
             const char *icon,
             const char **buttons);


GList*
Split_Arg (const char *arg, GList **args_list);
