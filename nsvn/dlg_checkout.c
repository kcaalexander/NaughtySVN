/*
* ====================================================================
* NaughtySVN checkout dialog box.
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

#include "svn/svn_naughtysvn.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"
#define MAX_URL 10


static int
nsvn__destory_window (GtkWidget *widget,
                      GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "checkout_dialog");

  gtk_widget_destroy (window);
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
  return 0;
}


static int
nsvn__add_url_item (GtkWidget *widget,
                    GladeXML *user_data)
{
  GtkWidget *window;
  GtkWidget *url_tbl;
  GtkWidget *wid;
  GtkWidget *img_wid;
  unsigned short nrows;
  const char *url;
  char widget_name[20];
  char url_lbl[7];

  window = glade_xml_get_widget (user_data, "checkout_dialog");
  url_tbl = glade_xml_get_widget (user_data, "checkout_url_tbl");
  nrows = GTK_TABLE(url_tbl)->nrows;

  /* Add new rows only if last is empty and not used. */
  sprintf (widget_name, "checkout_url%u_ent", nrows);
  if (nrows == 1)
    wid = glade_xml_get_widget (user_data, widget_name);
  else
    wid = g_object_get_data (G_OBJECT(window), widget_name);
  url = gtk_entry_get_text (GTK_ENTRY (wid));
  //TODO: check for blank and valid url
  if (!url || (url && strcmp (url, "") == 0) || nrows >= MAX_URL)
    return 0;

  gtk_table_resize (GTK_TABLE(url_tbl), nrows+1, 5);

  sprintf (url_lbl, "URL %u", nrows+1);
  wid = gtk_label_new (url_lbl);
  gtk_widget_show (wid);
  gtk_table_attach (GTK_TABLE (url_tbl), wid, 0, 1,
                    nrows, nrows+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  sprintf (widget_name, "checkout_url%u_ent", nrows+1);
  wid = gtk_entry_new ();
  gtk_widget_show (wid);
  g_object_set_data (G_OBJECT(window), widget_name, wid);
  gtk_table_attach (GTK_TABLE (url_tbl), wid, 1, 2,
                    nrows, nrows+1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  wid = gtk_label_new (_("<b>@</b>"));
  gtk_widget_show (wid);
  gtk_table_attach (GTK_TABLE (url_tbl), wid, 2, 3,
                    nrows, nrows+1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (wid), TRUE);

  sprintf (widget_name, "checkout_peg%u_btn", nrows+1);
  wid = gtk_entry_new ();
  gtk_widget_show (wid);
  g_object_set_data (G_OBJECT(window), widget_name, wid);
  gtk_table_attach (GTK_TABLE (url_tbl), wid, 3, 4,
                    nrows, nrows+1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_size_request (wid, 50, -1);

  if ((nrows+1) == 2)
    {
      sprintf (widget_name, "checkout_del_btn");
      wid = gtk_button_new ();
      //TODO: connect the button with a callback
      gtk_widget_show (wid);
      g_object_set_data (G_OBJECT(window), widget_name, wid);
      gtk_table_attach (GTK_TABLE (url_tbl), wid, 4, 5,
                        nrows, nrows+1,
                        (GtkAttachOptions) (GTK_FILL),
                        (GtkAttachOptions) (0), 0, 0);
      img_wid = gtk_image_new_from_stock ("gtk-close", GTK_ICON_SIZE_BUTTON);
      gtk_widget_show (img_wid);
      gtk_container_add (GTK_CONTAINER (wid), img_wid);
    }

#if 0
  sprintf (str_url_ent, "checkout_url%u_ent", nrows+1);
  sprintf (str_peg_ent, "checkout_peg%u_ent", nrows+1);
  sprintf (str_del_btn, "checkout_del%u_btn", nrows+1);
  url_btn = glade_xml_get_widget (user_data, str_url_btn);
  url_ent = glade_xml_get_widget (user_data, str_url_ent);
  peg_ent = glade_xml_get_widget (user_data, str_peg_ent);
  del_btn = glade_xml_get_widget (user_data, str_del_btn);
#endif
  
//  gtk_widget_destroy (window);
//  g_object_unref (G_OBJECT(user_data));
//  gtk_main_quit ();
  return 0;
}


gboolean
nsvn_dlg_checkout (GtkWidget *widget,
                   gpointer user_data)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cout_btn;
  GtkWidget *cancel_btn;
  GtkWidget *urladd_btn;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, NULL, NULL);
  if (!dlg_gui)
    {
      g_warning ("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n");
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog */
  window = glade_xml_get_widget (dlg_gui, "checkout_dialog");
  cout_btn = glade_xml_get_widget (dlg_gui, "checkout_cout_btn");
  cancel_btn = glade_xml_get_widget (dlg_gui, "checkout_cancel_btn");
  urladd_btn = glade_xml_get_widget (dlg_gui, "checkout_add_btn");

  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (urladd_btn), "clicked",
                    G_CALLBACK (nsvn__add_url_item),
                    dlg_gui);

  /* Activating dialog box */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
