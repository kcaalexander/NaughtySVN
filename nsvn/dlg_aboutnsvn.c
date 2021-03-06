/*
* ====================================================================
* NaughtySVN about dialog box.
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

#include "svn/naughtysvn.h"
#include "dlg_aboutnsvn.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


static void
nsvn__destroy_window  (GtkWidget *widget,
                       GladeXML *user_data)
{
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
}


gboolean
nsvn_dlg_aboutnsvn (GtkWidget *widget,
                    const gchar **user_data)
{
  GladeXML *dlg_gui;
  GtkWidget *window;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE,
                           "about_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return EXIT_FAILURE;
    }

  /* Get Widgets in repository creation dialog */
  window = glade_xml_get_widget (dlg_gui, "about_dialog");

  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
  
  /* Activating dialog box */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
