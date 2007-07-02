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
#include "dlg_update.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


static char*
nsvn__get_revstr (GladeXML *window);

static int
nsvn__toggle_revtype (GtkWidget *widget,
                      gpointer *user_data);

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


static int
nsvn__toggle_revtype (GtkWidget *widget,
                      gpointer *user_data)
{
  gtk_widget_set_sensitive (GTK_WIDGET (user_data),
                            gtk_toggle_button_get_active (
                              GTK_TOGGLE_BUTTON (widget)));
  return 0;
}


static char*
nsvn__get_revstr (GladeXML *window)
{
  GtkWidget *widget;

  widget = glade_xml_get_widget (window, "update_revnone_rad");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    return (g_strdup ("HEAD"));

  widget = glade_xml_get_widget (window, "update_revnum_rad");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      double rev;

      widget = glade_xml_get_widget (window, "update_revnum_ent");
      rev = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
      return (g_strdup_printf ("%lu", (long unsigned) rev));
    }

  widget = glade_xml_get_widget (window, "update_revdate_rad");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      time_t rev;

      widget = glade_xml_get_widget (window, "update_revdate_ent");
      rev = gnome_date_edit_get_time (GNOME_DATE_EDIT (widget));
      /* Return date in a format acceptable to Subversion. */
      return (g_strdup_printf ("{%s}", ctime (&rev)));
    }

  widget = glade_xml_get_widget (window, "update_revkword_rad");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      char *rev;

      widget = glade_xml_get_widget (window, "update_revkword_cmb");
      rev = gtk_combo_box_get_active_text (GTK_COMBO_BOX (widget));
      rev = g_strstrip (rev);
      if (rev[0])
        return (g_strdup ("HEAD"));
      else
        return (g_strdup (rev));
    }

  return NULL;
}


static int
nsvn__update_wc (GtkWidget *widget,
                 GladeXML *user_data)
{
  GtkWidget *window;
  GtkWidget *wid;
  gboolean nrecurse = FALSE;
  gboolean ignextn = FALSE;
  nsvn_t *nsvn;
  GList *files = NULL;
  const char **target_list;
  unsigned int nitems, i;
  char *revstr;

  /* Updated widgets. */
  window = glade_xml_get_widget (user_data, "update_dialog");

  /* Get revision from revision widget group. */
  if ((revstr = nsvn__get_revstr (user_data)) == NULL)
    revstr = g_strdup ("HEAD");

  /* Check non recurive. */
  wid = glade_xml_get_widget (user_data, "update_nrecurse_chk");
  nrecurse = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wid));

  /* Check Ignore externals. */
  wid = glade_xml_get_widget (user_data, "update_ignextn_chk");
  ignextn = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wid));

  files = g_object_get_data (G_OBJECT(window), "files");
  nitems = g_list_length (files);

  target_list = malloc(nitems*(sizeof(char*)));
  if (!target_list)
    return 0;

  for (i=0; i<nitems; i++)
      target_list[i] = g_list_nth_data (files, i);
  target_list[nitems] = NULL;

  nsvn = nsvn_base_init (NULL);

  if (nsvn_wc_update (nsvn, target_list, revstr,
                      nrecurse, ignextn) == EXIT_SUCCESS)
    {
      MSG_DEBUG("success\n");
      /* do stuff to display the output of the command */
    }
  else
    {
      MSG_DEBUG("failed\n");
    };

  g_free(target_list);

  nsvn = nsvn_base_uninit (nsvn);
  gtk_widget_destroy (window);

  return EXIT_SUCCESS;
}


gboolean
nsvn_dlg_update (GtkWidget *widget,
                 gpointer args)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cancel_btn;
  GtkWidget *update_btn;
  GtkWidget *rad_wid;
  GtkWidget *rev_wid;
  GList *files = NULL;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, "update_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "update_dialog");
  cancel_btn = glade_xml_get_widget (dlg_gui, "update_cancel_btn");
  update_btn = glade_xml_get_widget (dlg_gui, "update_upd_btn");

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (update_btn), "clicked",
                    G_CALLBACK (nsvn__update_wc),
                    dlg_gui);

  /* Connecting signal from revision types. */
  rad_wid = glade_xml_get_widget (dlg_gui, "update_revnone_rad");
  g_signal_connect (G_OBJECT (rad_wid), "toggled",
                    G_CALLBACK (nsvn__toggle_revtype),
                    NULL);
  rad_wid = glade_xml_get_widget (dlg_gui, "update_revnum_rad");
  rev_wid = glade_xml_get_widget (dlg_gui,"update_revnum_ent");
  /* TODO: Need to set the spinner MAX property with head revision. */
  g_signal_connect (G_OBJECT (rad_wid), "toggled",
                    G_CALLBACK (nsvn__toggle_revtype),
                    rev_wid);
  rad_wid = glade_xml_get_widget (dlg_gui, "update_revdate_rad");
  rev_wid = glade_xml_get_widget (dlg_gui, "update_revdate_ent");
  g_signal_connect (G_OBJECT (rad_wid), "toggled",
                    G_CALLBACK (nsvn__toggle_revtype),
                    rev_wid);
  rad_wid = glade_xml_get_widget (dlg_gui, "update_revkword_rad");
  rev_wid = glade_xml_get_widget (dlg_gui,"update_revkword_cmb");
  g_signal_connect (G_OBJECT (rad_wid), "toggled",
                    G_CALLBACK (nsvn__toggle_revtype),
                    rev_wid);

  Split_Arg ((char*) args, &files);
  g_object_set_data (G_OBJECT(window), "files", files);

  /* Activating dialog box. */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
