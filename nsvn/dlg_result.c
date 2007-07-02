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
#include "dlg_result.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


gboolean
nsvn_dlg_result_hide (GtkWidget *parent_window,
                      gpointer user_data)
{
  GladeXML *dlg_gui = (GladeXML*) user_data;
  GtkWidget *window;

  if (!dlg_gui)
    return EXIT_FAILURE;

  /* Getting widgets in result dialog. */
  window = glade_xml_get_widget (dlg_gui, "result_dialog");

  gtk_widget_hide (window);

  return EXIT_SUCCESS; 
}


gboolean
nsvn_dlg_result_show (GtkWidget *parent_window,
                      gpointer user_data)
{
  GladeXML *dlg_gui = (GladeXML*) user_data;
  GtkWidget *window;

  if (!dlg_gui)
    return EXIT_FAILURE;

  /* Getting widgets in result dialog. */
  window = glade_xml_get_widget (dlg_gui, "result_dialog");

  gtk_widget_show (window);

  return EXIT_SUCCESS; 
}

enum
{
  COLUMN_CHECK,
  COLUMN_STOCK,
  COLUMN_BASENAME,
  COLUMN_PATH,
  NUM_COLUMNS
};
