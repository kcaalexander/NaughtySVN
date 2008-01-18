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
#include <libgnomevfs/gnome-vfs-utils.h>

#include "svn/naughtysvn.h"
#include "dlg_checkout.h"
#include "dlg_result.h"
#include "global.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"
#define CO_MAX_URL 10
#define CO_URLLBL_NAME "checkout_url%d_lbl"
#define CO_URLENT_NAME "checkout_url%d_ent"
#define CO_DIRBTN_NAME "checkout_dir%d_btn"
#define CO_URLADD_NAME "checkout_add_btn"
#define CO_URLDEL_NAME "checkout_del_btn"
#define URLADD_EXTRA_DATA "URLADD_EXTRA_DATA"


static int
nsvn__add_url_item (GtkWidget *widget,
                    GladeXML *user_data);

static int
nsvn__remove_url_item (GtkWidget *widget,
                       gpointer user_data);

static char*
nsvn__get_revstr (GladeXML *window);

static void
nsvn__destroy_window (GtkWidget *widget,
                      GladeXML *user_data)
{
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
}

static void
nsvn__cancel (GtkWidget *widget,
              GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "checkout_dialog");
  if (window)
    gtk_widget_destroy (window);
}

static char*
nsvn__get_revstr (GladeXML *window)
{
  GtkWidget *widget;

  widget = glade_xml_get_widget (window, "checkout_revnone_rad");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    return (g_strdup ("HEAD"));

  widget = glade_xml_get_widget (window, "checkout_revnum_rad");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      double rev;

      widget = glade_xml_get_widget (window, "checkout_revnum_ent");
      rev = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
      return (g_strdup_printf ("%lu", (long unsigned) rev));
    }

  widget = glade_xml_get_widget (window, "checkout_revdate_rad");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      time_t rev;

      widget = glade_xml_get_widget (window, "checkout_revdate_ent");
      rev = gnome_date_edit_get_time (GNOME_DATE_EDIT (widget));
      /* Return date in a format acceptable to Subversion. */
      return (g_strdup_printf ("{%s}", ctime (&rev)));
    }

  widget = glade_xml_get_widget (window, "checkout_revkword_rad");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
    {
      char *rev;

      widget = glade_xml_get_widget (window, "checkout_revkword_cmb");
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
nsvn__destroy_filechooser_window (GtkWidget *widget,
                                  gpointer user_data)
{
  gtk_widget_hide (GTK_WIDGET(user_data));
  return 0;
}


static int
nsvn__select_filechooser_window (GtkWidget *widget,
                                 gpointer user_data)
{
  GSList *lst, *item;
  GtkWidget *window;
  GtkWidget *parent;
  GtkWidget *url_tbl;
  unsigned int nrows;
  char wid_name[25];
  char *extra_data;

  window = glade_xml_get_widget (user_data, "filechooser_dialog");
  parent = glade_xml_get_widget (user_data, "checkout_dialog");
  url_tbl = glade_xml_get_widget (user_data, "checkout_url_tbl");

  extra_data = g_object_get_data (G_OBJECT(window), URLADD_EXTRA_DATA);
  
  lst = gtk_file_chooser_get_uris (GTK_FILE_CHOOSER(window));
  item = lst;

  nrows = atoi (extra_data);
  while (item != NULL)
    {
      GtkWidget *wid;

      sprintf (wid_name, CO_URLENT_NAME, nrows);
      wid = g_object_get_data (G_OBJECT (parent), wid_name);
      if (wid)
        gtk_entry_set_text (GTK_ENTRY (wid), (char*)item->data);
      item = item->next;
      if (item != NULL && GTK_TABLE (url_tbl)->nrows == nrows)
        {
          nsvn__add_url_item (parent, (GladeXML*) user_data);
          nrows = GTK_TABLE (url_tbl)->nrows;
        }
      else
        nrows ++;
    }
  g_slist_free (lst);

  gtk_widget_hide (GTK_WIDGET(window));
  return 0;
}


static int
nsvn__choose_dir_item (GtkWidget *widget,
                       GladeXML *user_data)
{
  GtkWidget *window;
  GtkWidget *parent;
  GtkWidget *cancel;
  GtkWidget *choose;
  GtkWidget *urlwid;
  char *extra_data;
  char *url;
  char widname[25];

  window = glade_xml_get_widget (user_data, "filechooser_dialog");
  parent = glade_xml_get_widget (user_data, "checkout_dialog");
  cancel = glade_xml_get_widget (user_data, "filechooser_cancel_btn");
  choose = glade_xml_get_widget (user_data, "filechooser_select_btn");

  extra_data = g_object_get_data (G_OBJECT(widget), URLADD_EXTRA_DATA); 
  g_object_set_data (G_OBJECT(window), URLADD_EXTRA_DATA, extra_data);

  /* Reading given URL and making it the default top-level directory. */
  sprintf (widname, CO_URLENT_NAME, atoi(extra_data));
  urlwid = g_object_get_data (G_OBJECT (parent), widname);
  url = (char*) gtk_entry_get_text (GTK_ENTRY (urlwid));
  url = g_strstrip (url);
  if (url[0] !='\0')
    gtk_file_chooser_set_uri (GTK_FILE_CHOOSER(window),
                              url);

  gtk_window_set_modal (GTK_WINDOW (window), TRUE);
  gtk_window_set_transient_for (GTK_WINDOW (window),
                                GTK_WINDOW (parent));

  g_signal_connect (G_OBJECT (window), "delete_event",
                    G_CALLBACK (gtk_true),
                    window);
  g_signal_connect (G_OBJECT (cancel), "clicked",
                    G_CALLBACK (nsvn__destroy_filechooser_window),
                    window);
  g_signal_connect (G_OBJECT (choose), "clicked",
                    G_CALLBACK (nsvn__select_filechooser_window),
                    user_data);

  gtk_widget_show (window);
  return 0;
}


static int
nsvn__add_url_item (GtkWidget *widget,
                    GladeXML *user_data)
{
  GtkWidget *window;
  GtkWidget *url_tbl;
  GtkWidget *lbl_wid;
  char url_lbl[10];
  char wid_name[25];
  GtkWidget *wid;
  char *val = NULL;
  unsigned int nrows;
  char *extra_data;

  window = glade_xml_get_widget (user_data, "checkout_dialog");
  url_tbl = glade_xml_get_widget (user_data, "checkout_url_tbl");

  nrows = GTK_TABLE (url_tbl)->nrows;
  sprintf (wid_name, CO_URLENT_NAME, nrows);
  wid = g_object_get_data (G_OBJECT(window), wid_name);
  if (wid != NULL)
    {
      val = (char*) gtk_entry_get_text (GTK_ENTRY (wid));
      val = g_strstrip (val);
    }
  else
    return 0;

  /* Create a new url controls only if previous one is empty. */
  if (!val || (val && strcmp (val, "") == 0) || nrows >= CO_MAX_URL)
    return 0;

  gtk_table_resize (GTK_TABLE(url_tbl), nrows+1, 5);

  /* URL label. */
  sprintf (url_lbl, _("URL %u"), nrows+1);
  wid = gtk_label_new (url_lbl);
  sprintf (wid_name, CO_URLLBL_NAME, nrows+1);
  g_object_set_data (G_OBJECT (window), wid_name, wid);
  gtk_widget_show (wid);
  gtk_table_attach (GTK_TABLE (url_tbl), wid, 0, 1,
                    nrows, nrows+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  /* URL text entry. */
  wid = gtk_entry_new ();
  sprintf (wid_name, CO_URLENT_NAME, nrows+1);
  g_object_set_data (G_OBJECT (window), wid_name, wid);
  gtk_widget_show (wid);
  gtk_table_attach (GTK_TABLE (url_tbl), wid, 1, 2,
                    nrows, nrows+1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  /* File chooser button. */
  wid = gtk_button_new ();
  sprintf (wid_name, CO_DIRBTN_NAME, nrows+1);
  g_object_set_data (G_OBJECT (window), wid_name, wid);
  extra_data = g_strdup_printf ("%u", nrows+1);
  g_object_set_data (G_OBJECT (wid), URLADD_EXTRA_DATA, extra_data);
  gtk_widget_show (wid);
  gtk_table_attach (GTK_TABLE (url_tbl), wid, 2, 3,
                    nrows, nrows+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  g_signal_connect (G_OBJECT (wid), "clicked",
                    G_CALLBACK (nsvn__choose_dir_item),
                    user_data);
  lbl_wid = gtk_label_new (_("<b>...</b>"));
  gtk_label_set_use_markup (GTK_LABEL (lbl_wid), TRUE);
  gtk_widget_show (lbl_wid);
  gtk_container_add (GTK_CONTAINER (wid), lbl_wid);

  /* URL remove button. */
  if (nrows == 1)
    {
      GtkWidget *img_wid;

      wid = gtk_button_new ();
      sprintf (wid_name, CO_URLDEL_NAME);
      g_object_set_data (G_OBJECT (window), wid_name, wid);
      gtk_widget_show (wid);
      gtk_table_attach (GTK_TABLE (url_tbl), wid, 3, 4,
                        nrows, nrows+1,
                        (GtkAttachOptions) (GTK_FILL),
                        (GtkAttachOptions) (0), 0, 0);
      g_signal_connect (G_OBJECT (wid), "clicked",
                        G_CALLBACK (nsvn__remove_url_item),
                        user_data);
      img_wid = gtk_image_new_from_stock ("gtk-close", GTK_ICON_SIZE_BUTTON);
      gtk_widget_show (img_wid);
      gtk_container_add (GTK_CONTAINER (wid), img_wid);
    }

  return 0;
}


static int
nsvn__checkout_url (GtkWidget *widget,
                    GladeXML *user_data)
{
  GtkWidget *window;
  GtkWidget *wcpath_wid;
  GtkWidget *url_tbl;
  GtkWidget *wid;
  unsigned int nrows, i;
  char *wcpath_name;
  gboolean nrecurse = FALSE;
  gboolean ignextn = FALSE;
  void *nsvn;
  char *revstr;

  /* Checkout widgets. */
  window = glade_xml_get_widget (user_data, "checkout_dialog");
  wcpath_wid = glade_xml_get_widget (user_data, "checkout_wcpath_ent");
  url_tbl = glade_xml_get_widget (user_data, "checkout_url_tbl");

  /* Check wc_path. */
  wcpath_name = (char*) gtk_entry_get_text (GTK_ENTRY (wcpath_wid));
  wcpath_name = g_strstrip (wcpath_name);

  if ((revstr = nsvn__get_revstr (user_data)) == NULL)
    revstr = g_strdup ("HEAD");

  /* Check non recurive. */
  wid = glade_xml_get_widget (user_data, "checkout_nrecurse_chk");
  nrecurse = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wid));

  /* Check Ignore externals. */
  wid = glade_xml_get_widget (user_data, "checkout_ignextn_chk");
  ignextn = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wid));

  /* Hiding checkout window and loads result window. */
  gtk_widget_hide (window);
  nsvn_dlg_result_show (window, user_data);

  nrows = GTK_TABLE (url_tbl)->nrows;
  for (i=1; i<=nrows; i++)
    {
      GtkWidget *url_wid;
      char widname [25];
      char *url;
      char *wcpath;
      char *basename;

      //TODO: Not a good idea to reinit and uninit nsvn library while in loop
      nsvn = nsvn_base_init (NULL);

      /* Getting URL. */
      sprintf (widname, CO_URLENT_NAME, i);
      url_wid = g_object_get_data (G_OBJECT (window), widname);
      url = (char*) gtk_entry_get_text (GTK_ENTRY (url_wid));
      url = g_strstrip (url);
      /* Do nothing if no urls are supplied. */
      if (!url[0] && i == 1)
        return 1;

      basename = g_path_get_basename(url);
      if (nrows > 1)
        wcpath = g_strdup_printf ("%s/%s", wcpath_name, basename);
      else
        wcpath = g_strdup (wcpath_name);

      if (nsvn_repos_checkout (nsvn, url, wcpath,
                               revstr, nrecurse,
                               ignextn) == EXIT_FAILURE)
        {
          const char *buttons[] = {"OK", NULL};
          Show_Msgbox (window, FALSE, "Error",
                       _("Repository creation failed ..."),
                       GNOME_MESSAGE_BOX_ERROR,
                       buttons);
        }
      
      g_free (wcpath);
      g_free (basename);

      nsvn = nsvn_base_uninit (nsvn);
    }

  g_free (revstr);
  gtk_widget_destroy (window);
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
  return 0;
}


static int
nsvn__remove_url_item (GtkWidget *widget,
                       gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *url_tbl;
  GtkWidget *wid;
  unsigned int nrows;
  char wid_name [25];

  window = glade_xml_get_widget (user_data, "checkout_dialog");
  url_tbl = glade_xml_get_widget (user_data, "checkout_url_tbl");
  nrows = GTK_TABLE (url_tbl)->nrows;

  if (nrows >= 2)
    {
      sprintf (wid_name, CO_URLLBL_NAME, nrows);
      wid = g_object_get_data (G_OBJECT (window), wid_name);
      gtk_widget_destroy (wid);

      sprintf (wid_name, CO_URLENT_NAME, nrows);
      wid = g_object_get_data (G_OBJECT (window), wid_name);
      gtk_widget_destroy (wid);

      sprintf (wid_name, CO_DIRBTN_NAME, nrows);
      wid = g_object_get_data (G_OBJECT (window), wid_name);
      gtk_widget_destroy (wid);

      if (nrows == 2)
        {
          sprintf (wid_name, CO_URLDEL_NAME);
          wid = g_object_get_data (G_OBJECT (window), wid_name);
          gtk_widget_destroy (wid);
        }

      gtk_table_resize (GTK_TABLE(url_tbl), nrows-1, 5);
    }

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


gboolean
nsvn_dlg_checkout (GtkWidget *widget,
                   gpointer user_data)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cout_btn;
  GtkWidget *cancel_btn;
  GtkWidget *wcpath_ent;
  GtkWidget *url_lbl;
  GtkWidget *url_ent;
  GtkWidget *urladd_btn;
  GtkWidget *dir_btn;
  GtkWidget *rad_wid;
  GtkWidget *rel_wid;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, "checkout_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "checkout_dialog");
  cout_btn = glade_xml_get_widget (dlg_gui, "checkout_cout_btn");
  cancel_btn = glade_xml_get_widget (dlg_gui, "checkout_cancel_btn");
  wcpath_ent = glade_xml_get_widget (dlg_gui, "checkout_wcpath_ent");

  gtk_entry_set_text (GTK_ENTRY(wcpath_ent), (const char*) user_data);

  url_lbl = glade_xml_get_widget (dlg_gui, "checkout_url1_lbl");
  g_object_set_data (G_OBJECT(window), "checkout_url1_lbl", url_lbl);
  url_ent = glade_xml_get_widget (dlg_gui, "checkout_url1_ent");
  g_object_set_data (G_OBJECT(window), "checkout_url1_ent", url_ent);
  urladd_btn = glade_xml_get_widget (dlg_gui, "checkout_add_btn");
  g_object_set_data (G_OBJECT(window), "checkout_add_btn", urladd_btn);
  dir_btn = glade_xml_get_widget (dlg_gui, "checkout_dir1_btn");
  g_object_set_data (G_OBJECT(window), "checkout_dir1_btn", dir_btn);
  g_object_set_data (G_OBJECT (dir_btn), URLADD_EXTRA_DATA, g_strdup ("1"));

  /* Connecting callbacks to widget. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__cancel),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cout_btn), "clicked",
                    G_CALLBACK (nsvn__checkout_url),
                    dlg_gui);
  g_signal_connect (G_OBJECT (urladd_btn), "clicked",
                    G_CALLBACK (nsvn__add_url_item),
                    dlg_gui);
  g_signal_connect (G_OBJECT (dir_btn), "clicked",
                    G_CALLBACK (nsvn__choose_dir_item),
                    dlg_gui);

  /* Connecting signal from revision types. */
  rad_wid = glade_xml_get_widget (dlg_gui, "checkout_revnone_rad");
  g_signal_connect (G_OBJECT (rad_wid), "toggled",
                    G_CALLBACK (nsvn__toggle_revtype),
                    NULL);
  rad_wid = glade_xml_get_widget (dlg_gui, "checkout_revnum_rad");
  rel_wid = glade_xml_get_widget (dlg_gui,"checkout_revnum_ent");
  /* TODO: Need to set the spinner MAX property with head revision. */
  g_signal_connect (G_OBJECT (rad_wid), "toggled",
                    G_CALLBACK (nsvn__toggle_revtype),
                    rel_wid);
  rad_wid = glade_xml_get_widget (dlg_gui, "checkout_revdate_rad");
  rel_wid = glade_xml_get_widget (dlg_gui, "checkout_revdate_ent");
  g_signal_connect (G_OBJECT (rad_wid), "toggled",
                    G_CALLBACK (nsvn__toggle_revtype),
                    rel_wid);
  rad_wid = glade_xml_get_widget (dlg_gui, "checkout_revkword_rad");
  rel_wid = glade_xml_get_widget (dlg_gui,"checkout_revkword_cmb");
  g_signal_connect (G_OBJECT (rad_wid), "toggled",
                    G_CALLBACK (nsvn__toggle_revtype),
                    rel_wid);

  /* Setting focus to first url entry item. */
  gtk_widget_grab_focus (url_ent);

  /* Activating dialog box. */
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
