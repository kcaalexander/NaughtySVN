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
#include "dlg_preferences.h"
#include "global.h"
#include "gconf.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


static gboolean
nsvn__verify_log_tab (GladeXML *dlg_gui,
                      GtkWidget *window,
                      GConfClient *client);

static gboolean
nsvn__verify_general_tab (GladeXML *dlg_gui,
                          GtkWidget *window,
                          GConfClient *client);

static gboolean
nsvn__verify_save_preferences (GtkWidget *window,
                               gpointer user_data);


static int
nsvn__populate_settings (GtkWidget *window,
                         GladeXML *user_data);
static gboolean
nsvn__verify_log_tab (GladeXML *dlg_gui,
                      GtkWidget *window,
                      GConfClient *client)
{
  //GtkWidget *widget;
  //void *value;
  //const char *buttons[] = {"OK", NULL};

  return TRUE;
}


static gboolean
nsvn__verify_general_tab (GladeXML *dlg_gui,
                          GtkWidget *window,
                          GConfClient *client)
{
  GtkWidget *widget;
  char *value;
//  const char *buttons[] = {"OK", NULL};
  nsvn_config_t config;
  int ret;

  nsvn_gconf_create_config(&config);

  /* Configuration directory. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_configdir_ent");
  value = g_strdup(gtk_entry_get_text (GTK_ENTRY(widget)));
  config.config_dir = g_strstrip(value); 
#if 0
  if (!(strcmp(value, "") == 0)) //Not empty
    {
      if (g_path_is_absolute ((const char*) value) == FALSE)
        {
          Show_Msgbox (window, FALSE, "Error",
                       _("Needs an absolute path ..."),
                       GNOME_MESSAGE_BOX_ERROR, buttons);
          gtk_widget_grab_focus (widget);
          return FALSE;
        }
    }
#endif

  /* User Name. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_uname_ent");
  value = g_strdup(gtk_entry_get_text (GTK_ENTRY(widget)));
  config.def_username = g_strstrip(value);

  /* User Password. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_pword_ent");
  value = g_strdup(gtk_entry_get_text (GTK_ENTRY(widget)));
  config.def_passwd = g_strstrip(value);

  /* Cache Authentication token. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_cacheauth_chk");
  config.no_cache_auth = gtk_toggle_button_get_active ((GTK_TOGGLE_BUTTON(widget)));

  /* Prompt for user name and password. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_promptuser_chk");
  config.no_prompt_auth = gtk_toggle_button_get_active ((GTK_TOGGLE_BUTTON(widget)));

  /* Save password to disk. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_storepasswd_chk");
  config.no_save_passwd = gtk_toggle_button_get_active ((GTK_TOGGLE_BUTTON(widget)));

  ret = nsvn_gconf_write_config (&config, client);
  nsvn_gconf_clean_config(&config);
  
  return ret;
}


static gboolean
nsvn__verify_save_preferences (GtkWidget *window,
                               gpointer user_data)
{
  GladeXML *dlg_gui = (GladeXML*) user_data;
  GConfClient *client;

  client = gconf_client_get_default ();

  if (!nsvn__verify_general_tab (dlg_gui, window, client))
    {
      g_object_unref((void*)client);
      return FALSE;
    }

  if (!nsvn__verify_log_tab (dlg_gui, window, client))
    {
      g_object_unref((void*)client);
      return FALSE;
    }

  g_object_unref((void*)client);
  return TRUE;
}


static void
nsvn__destroy_window  (GtkWidget *widget,
                       GladeXML *user_data)
{
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
}


static void
nsvn__cancel  (GtkWidget *widget,
               GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "preferences_dialog");
  if (window)
    gtk_widget_destroy (window);
}


static int
nsvn__save_settings  (GtkWidget *widget,
                      GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "preferences_dialog");
  nsvn__verify_save_preferences (window, user_data);

  gtk_widget_destroy (window);
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
  return 0;
}


static int
nsvn__populate_settings (GtkWidget *window,
                         GladeXML *user_data)
{
  GtkWidget *widget;
  GConfClient *client;
  nsvn_config_t config;

  client = gconf_client_get_default ();
  nsvn_gconf_create_config(&config);
  nsvn_gconf_read_config(&config, client);
  g_object_unref((void*)client);

  widget = glade_xml_get_widget (user_data, "preferences_configdir_ent");
  gtk_entry_set_text(GTK_ENTRY(widget), config.config_dir);

  widget = glade_xml_get_widget (user_data, "preferences_uname_ent"); 
  gtk_entry_set_text(GTK_ENTRY(widget), config.def_username);

  widget = glade_xml_get_widget (user_data, "preferences_pword_ent");
  gtk_entry_set_text(GTK_ENTRY(widget), config.def_passwd);

  widget = glade_xml_get_widget (user_data, "preferences_cacheauth_chk");
  gtk_toggle_button_set_active ((GTK_TOGGLE_BUTTON(widget)),
                                config.no_cache_auth);

  widget = glade_xml_get_widget (user_data, "preferences_promptuser_chk");
  gtk_toggle_button_set_active ((GTK_TOGGLE_BUTTON(widget)),
                                config.no_prompt_auth);

  widget = glade_xml_get_widget (user_data, "preferences_storepasswd_chk");
  gtk_toggle_button_set_active ((GTK_TOGGLE_BUTTON(widget)),
                                config.no_save_passwd);
  return TRUE;
}


gboolean
nsvn_dlg_preferences (GtkWidget *widget,
                      const gchar **user_data)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *cfgdir_ent;
  GtkWidget *ok_btn;
  GtkWidget *cancel_btn;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE,
                           "preferences_dialog", NULL);
  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "preferences_dialog");
  cfgdir_ent = glade_xml_get_widget (dlg_gui, "preferences_configdir_ent");
  ok_btn = glade_xml_get_widget (dlg_gui, "preferences_ok_btn");
  cancel_btn = glade_xml_get_widget (dlg_gui, "preferences_cancel_btn");

  /* Connecting widgets to callbacks. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__cancel),
                    dlg_gui);

  g_signal_connect (G_OBJECT (ok_btn), "clicked",
                    G_CALLBACK (nsvn__save_settings),
                    dlg_gui);
 
  /*Read gconf and fill in the widgets. */
  nsvn__populate_settings(window, dlg_gui);

  /* Activating dialog box */
  gtk_widget_grab_focus (cfgdir_ent);
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
