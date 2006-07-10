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
#include "global.h"
#include "gconf.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"


gboolean
nsvn__verify_log_tab (GladeXML *dlg_gui,
                      GtkWidget *window,
                      GConfClient *client)
{
  GtkWidget *widget;
  void *value;
  const char *buttons[] = {"OK", NULL};

  return TRUE;
}


gboolean
nsvn__verify_general_tab (GladeXML *dlg_gui,
                          GtkWidget *window,
                          GConfClient *client)
{
  GtkWidget *widget;
  void *value;
  const char *buttons[] = {"OK", NULL};

  /* Configuration directory. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_configdir_ent");
  value = (void*) gtk_entry_get_text (GTK_ENTRY(widget));
  if (g_path_is_absolute ((const char*) value) == FALSE)
    {
      Show_Msgbox (window, FALSE, "Error",
                   "Needs an absolute path ...",
                   GNOME_MESSAGE_BOX_ERROR,
                   buttons);
      gtk_widget_grab_focus (widget);
      return FALSE;
    }
  if (nsvn_gconf_write_entry (NSVN_SVN_CONFIG_DIR_KEY, value,
                          client))
    return FALSE;

  /* User Name. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_uname_ent");
  value = (void*) gtk_entry_get_text (GTK_ENTRY(widget));
  if (nsvn_gconf_write_entry (NSVN_SVN_USERNAME_KEY, value,
                              client))
    return FALSE;

  /* User Password. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_pword_ent");
  value = (void*) gtk_entry_get_text (GTK_ENTRY(widget));
  if (nsvn_gconf_write_entry (NSVN_SVN_PASSWORD_KEY, value,
                              client))
    return FALSE;

  /* Cache Authentication token. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_cacheauth_chk");
  if (gtk_toggle_button_get_active ((GTK_TOGGLE_BUTTON(widget))))
    value = "0";
  else
    value = "1";
  if (nsvn_gconf_write_entry (NSVN_CACHE_AUTH_TOKEN_KEY, value,
                              client))
    return FALSE;

  /* Prompt for user name and password. */
  widget = glade_xml_get_widget (dlg_gui, "preferences_promptuser_chk");
  if (gtk_toggle_button_get_active ((GTK_TOGGLE_BUTTON(widget))))
    value = "0";
  else
    value = "1";
  if (nsvn_gconf_write_entry (NSVN_PROMPT_USER_KEY, value, client))
    return FALSE;

  return TRUE;
}


gboolean
nsvn__verify_save_preferences (GtkWidget *window,
                               gpointer user_data)
{
  GladeXML *dlg_gui = (GladeXML*) user_data;
  GConfClient *client;

  client = gconf_client_get_default ();

  if (!nsvn__verify_general_tab (dlg_gui, window, client))
    return FALSE;

  if (!nsvn__verify_log_tab (dlg_gui, window, client))
    return FALSE;

  return TRUE;
}


static int
nsvn__destory_window  (GtkWidget *widget,
                       GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "preferences_dialog");

  gtk_widget_destroy (window);
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
  return 0;
}


static int
nsvn__save_settings  (GtkWidget *widget,
                      GladeXML *user_data)
{
  GtkWidget *window;

  window = glade_xml_get_widget (user_data, "preferences_dialog");
  //TODO: Write user preferences into gconf.
  nsvn__verify_save_preferences (window, user_data);

  gtk_widget_destroy (window);
  g_object_unref (G_OBJECT(user_data));
  gtk_main_quit ();
  return 0;
}


gboolean
nsvn_dlg_preferences (GtkWidget *widget,
                      gpointer user_data)
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
      g_warning ("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n");
      return EXIT_FAILURE;
    }

  /* Getting Widgets in repository creation dialog. */
  window = glade_xml_get_widget (dlg_gui, "preferences_dialog");
  cfgdir_ent = glade_xml_get_widget (dlg_gui, "preferences_configdir_ent");
  ok_btn = glade_xml_get_widget (dlg_gui, "preferences_ok_btn");
  cancel_btn = glade_xml_get_widget (dlg_gui, "preferences_cancel_btn");

  /* Connecting widgets to callbacks. */
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);
  g_signal_connect (G_OBJECT (cancel_btn), "clicked",
                    G_CALLBACK (nsvn__destory_window),
                    dlg_gui);

  g_signal_connect (G_OBJECT (ok_btn), "clicked",
                    G_CALLBACK (nsvn__save_settings),
                    dlg_gui);
  
  /* Activating dialog box */
  gtk_widget_grab_focus (cfgdir_ent);
  gtk_widget_show (window);

  return EXIT_SUCCESS;
}
