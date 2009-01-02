/*
* ====================================================================
* NaughtySVN auth dialog box.
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
#include <gtk/gtk.h>
#include <glade/glade.h>

#include <svn_error.h>
#include <svn_auth.h>
#include <svn_error_codes.h>

#include "dlg_auth.h"

#define DLG_GLADE_FILE  "naughtysvn.glade"

#define _RESPONSE_REJECT 0
#define _RESPONSE_ACCEPTTEMP 1
#define _RESPONSE_ACCEPTPERM 2

static void
nsvn__destroy_window (GtkWidget *widget,
                      GladeXML *user_data)
{
  g_object_unref (G_OBJECT(user_data));
}


svn_error_t *
nsvn_auth_simple_prompt (svn_auth_cred_simple_t **cred_p,
                         void *baton,
                         const char *realm,
                         const char *defaultusername,
                         svn_boolean_t may_save,
                         apr_pool_t *pool)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *url;
  GtkWidget *username;
  GtkWidget *password;
  GtkWidget *saveauth;
  GtkWidget *ok_btn;
  GtkWidget *cancel_btn;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE,
                           "auth_dialog", NULL);

  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return svn_error_create (SVN_ERR_AUTHN_CREDS_UNAVAILABLE, NULL, _("Error loading GUI"));
    }

  window = glade_xml_get_widget (dlg_gui, "auth_dialog" );
  url = glade_xml_get_widget (dlg_gui, "auth_url_lbl" );
  username = glade_xml_get_widget (dlg_gui, "auth_user_ent" );
  password = glade_xml_get_widget (dlg_gui, "auth_password_ent" );
  saveauth = glade_xml_get_widget (dlg_gui, "auth_save_chk" );
  ok_btn = glade_xml_get_widget (dlg_gui, "auth_ok_btn" );
  cancel_btn = glade_xml_get_widget (dlg_gui, "auth_cancel_btn" );

  gtk_label_set_text (GTK_LABEL(url), realm);

  if (defaultusername)
    {
      gtk_entry_set_text (GTK_ENTRY(username), defaultusername);
      gtk_widget_grab_focus (password);
    }
  else
    gtk_widget_grab_focus (username);

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(saveauth), may_save);

  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);

  switch (gtk_dialog_run (GTK_DIALOG(window)))
    {
       default:
         gtk_widget_destroy (window);
         while (gtk_events_pending())
           gtk_main_iteration();
         return svn_error_create (SVN_ERR_CANCELLED, NULL, _("User aborted"));
       case GTK_RESPONSE_OK:
         {
           svn_auth_cred_simple_t *ret = apr_pcalloc (pool, sizeof(*ret));

           ret->username = apr_pstrdup (pool, gtk_entry_get_text (GTK_ENTRY(username)));
           ret->password = apr_pstrdup (pool, gtk_entry_get_text (GTK_ENTRY(password)));
           ret->may_save = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(saveauth));
           *cred_p = ret;

           gtk_widget_destroy (window);
           while (gtk_events_pending())
             gtk_main_iteration();
           return SVN_NO_ERROR;
         }
    }
}


svn_error_t *
nsvn_auth_username_prompt (svn_auth_cred_username_t **cred_p,
                           void *baton,
                           const char *realm,
                           svn_boolean_t may_save,
                           apr_pool_t *pool)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *url;
  GtkWidget *username;
  GtkWidget *password, *password_label;
  GtkWidget *saveauth;
  GtkWidget *ok_btn;
  GtkWidget *cancel_btn;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE,
                           "auth_dialog", NULL);

  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return svn_error_create (SVN_ERR_AUTHN_CREDS_UNAVAILABLE, NULL, _("Error loading GUI"));
    }

  window = glade_xml_get_widget (dlg_gui, "auth_dialog" );
  url = glade_xml_get_widget (dlg_gui, "auth_url_lbl" );
  username = glade_xml_get_widget (dlg_gui, "auth_user_ent" );
  password = glade_xml_get_widget (dlg_gui, "auth_password_ent" );
  password_label = glade_xml_get_widget (dlg_gui, "auth_password_lbl" );
  saveauth = glade_xml_get_widget (dlg_gui, "auth_save_chk" );
  ok_btn = glade_xml_get_widget (dlg_gui, "auth_ok_btn" );
  cancel_btn = glade_xml_get_widget (dlg_gui, "auth_cancel_btn" );

  gtk_label_set_text (GTK_LABEL(url), realm);

  /* disable the password entry */
  gtk_widget_set_sensitive (password, FALSE);
  gtk_widget_set_sensitive (password_label, FALSE);

  gtk_widget_grab_focus (username);

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(saveauth), may_save);

  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);

  switch (gtk_dialog_run (GTK_DIALOG(window)))
    {
      default:
        gtk_widget_destroy (window);
        while (gtk_events_pending())
          gtk_main_iteration();
        return svn_error_create (SVN_ERR_CANCELLED, NULL, _("User aborted"));
      case GTK_RESPONSE_OK:
        {
          svn_auth_cred_username_t *ret = apr_pcalloc (pool, sizeof(*ret));

          ret->username = apr_pstrdup (pool, gtk_entry_get_text (GTK_ENTRY(username)));
          ret->may_save = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(saveauth));
          *cred_p = ret;

          gtk_widget_destroy (window);
          while (gtk_events_pending())
            gtk_main_iteration();
          return SVN_NO_ERROR;
        }
    }
}


svn_error_t *
nsvn_auth_ssl_server_trust_prompt (svn_auth_cred_ssl_server_trust_t **cred_p,
                                   void *baton,
                                   const char *realm,
                                   apr_uint32_t failures,
                                   const svn_auth_ssl_server_cert_info_t *cert_info,
                                   svn_boolean_t may_save,
                                   apr_pool_t *pool)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *message;
  GtkWidget *permanent;
  GString *tempmessage;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE,
                           "auth_ssl_server_trust_dialog", NULL);

  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return svn_error_create (SVN_ERR_AUTHN_CREDS_UNAVAILABLE, NULL, _("Error loading GUI"));
    }

  window = glade_xml_get_widget (dlg_gui, "auth_ssl_server_trust_dialog");
  message = glade_xml_get_widget (dlg_gui, "auth_ssl_server_trust_message_lbl");
  permanent = glade_xml_get_widget (dlg_gui, "auth_ssl_server_trust_perm");

  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);
 
  tempmessage = g_string_new (0);
  g_string_printf (tempmessage,
                   _("Error validating server certificate for '%s'\n\n\n"),
                   realm);
  
  if (failures & SVN_AUTH_SSL_UNKNOWNCA)
    g_string_append (tempmessage,
          _("The certificate is not issued by a trusted authority. Use the\n"
            "fingerprint to validate the certificate manually!\n\n"));
    
  if (failures & SVN_AUTH_SSL_CNMISMATCH)
    g_string_append (tempmessage,
                     _("The certificate hostname does not match.\n\n"));
    
  if (failures & SVN_AUTH_SSL_NOTYETVALID)
    g_string_append (tempmessage, _("The certificate is not yet valid.\n\n"));

  if (failures & SVN_AUTH_SSL_EXPIRED)
    g_string_append (tempmessage, _("The certificate has expired.\n\n"));
    
  if (failures & SVN_AUTH_SSL_OTHER)
    g_string_append (tempmessage, _("The certificate has an unknown error.\n\n"));
    
  g_string_append_printf (tempmessage,
                          _("Certificate information:\n"
                          " - Hostname: %s\n"
                          " - Valid: from %s until %s\n"
                          " - Issuer: %s\n"
                          " - Fingerprint:\n%s"),
                          g_markup_escape_text (cert_info->hostname, -1),
                          g_markup_escape_text (cert_info->valid_from, -1),
                          g_markup_escape_text (cert_info->valid_until, -1),
                          g_markup_escape_text (cert_info->issuer_dname, -1),
                          g_markup_escape_text (cert_info->fingerprint, -1));
  
  gtk_label_set_markup (GTK_LABEL(message), tempmessage->str);
  
  g_string_free (tempmessage, TRUE);
  
  if (!may_save)
    gtk_widget_set_sensitive (permanent, FALSE);

  switch (gtk_dialog_run (GTK_DIALOG(window)))
    {
      default:
      case _RESPONSE_REJECT:
        *cred_p = NULL;
        break;
      case _RESPONSE_ACCEPTTEMP:
        *cred_p = apr_pcalloc (pool, sizeof(**cred_p));
        (*cred_p)->may_save = FALSE;
        (*cred_p)->accepted_failures = failures;
        break;
      case _RESPONSE_ACCEPTPERM:
        *cred_p = apr_pcalloc (pool, sizeof(**cred_p));
        (*cred_p)->may_save = TRUE;
        (*cred_p)->accepted_failures = failures;
        break;
    }
    
  gtk_widget_destroy ( window );
  while (gtk_events_pending())
    gtk_main_iteration();

  return SVN_NO_ERROR;
}


static void
nsvn__auth_ssl_client_cert_open (GtkWidget *widget,
                                 GladeXML *dlg_gui)
{
  GtkWidget *fc;
  GtkWidget *window;

  window = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_dialog");
  fc = gtk_file_chooser_dialog_new (_("Open client certificate"),
                                    GTK_WINDOW(window),
                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                    NULL);
  if (gtk_dialog_run (GTK_DIALOG(fc)) == GTK_RESPONSE_ACCEPT)
  {
    gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(fc));
    GtkWidget *file;
    GtkWidget *ok;
    
    ok = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_ok_btn");
    file = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_file_lbl");

    if (file)
      gtk_label_set_text (GTK_LABEL(file), filename?filename:"");
    gtk_widget_set_sensitive (ok, !!filename);
    if (filename)
      g_free (filename);
  }
  gtk_widget_destroy (fc);
}


svn_error_t *
nsvn_auth_ssl_client_cert_prompt (svn_auth_cred_ssl_client_cert_t **cred_p,
                                  void *baton,
                                  const char *realm,
                                  svn_boolean_t may_save,
                                  apr_pool_t *pool)
{
  svn_auth_cred_ssl_client_cert_t *cred = NULL;
  gchar *string;
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *message;
  GtkWidget *open;
  GtkWidget *ok;
  GtkWidget *saveauth;
  GtkWidget *file;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE,
                           "auth_ssl_client_cert_dialog", NULL);

  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return svn_error_create (SVN_ERR_AUTHN_CREDS_UNAVAILABLE, NULL, _("Error loading GUI"));
    }

  window = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_dialog");
  message = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_message_lbl");
  open = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_open_btn");
  ok = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_ok_btn");
  file = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_file_lbl");
  saveauth = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_save_chk");

  string = g_strdup_printf (_("Need a client certificate for '%s'"), realm);
  gtk_label_set_text (GTK_LABEL(message), string);
  g_free (string);

  g_signal_connect (G_OBJECT(open), "clicked",
                    G_CALLBACK(nsvn__auth_ssl_client_cert_open),
                    dlg_gui);
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);

  gtk_widget_set_sensitive (ok, FALSE);

  if (!may_save)
    gtk_widget_set_sensitive (saveauth, FALSE);

  switch (gtk_dialog_run (GTK_DIALOG(window)))
    {
      case GTK_RESPONSE_OK:
        cred = apr_palloc (pool, sizeof(*cred));
        cred->cert_file = apr_pstrdup (pool, gtk_label_get_text (GTK_LABEL(file)));
        cred->may_save = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(saveauth));
        *cred_p = cred;
        gtk_widget_destroy (window);
        while (gtk_events_pending())
          gtk_main_iteration();
        return SVN_NO_ERROR;
      default:
        {
          gtk_widget_destroy (window);
          while (gtk_events_pending())
            gtk_main_iteration();
          *cred_p = 0;
          return svn_error_create ( SVN_ERR_CANCELLED, NULL, _("User aborted"));
        }
    }
}


svn_error_t *
nsvn_auth_ssl_client_cert_pw_prompt (svn_auth_cred_ssl_client_cert_pw_t **cred_p,
                                     void *baton,
                                     const char *realm,
                                     svn_boolean_t may_save,
                                     apr_pool_t *pool)
{
  GladeXML *dlg_gui;
  GtkWidget *window;
  GtkWidget *message;
  GtkWidget *passphrase;
  GtkWidget *saveauth;
  char *string;
  svn_auth_cred_ssl_client_cert_pw_t *cred = NULL;

  /* Error-out if supporting glade file missing in default path. */
  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE,
                           "auth_ssl_client_cert_pw_dialog", NULL);

  if (!dlg_gui)
    {
      g_warning (_("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n"));
      return svn_error_create (SVN_ERR_AUTHN_CREDS_UNAVAILABLE, NULL, _("Error loading GUI"));
    }

  window = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_pw_dialog");
  message = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_pw_message_lbl");
  passphrase = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_pw_passphrase_ent");
  saveauth = glade_xml_get_widget (dlg_gui, "auth_ssl_client_cert_pw_save_chk");

  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (nsvn__destroy_window),
                    dlg_gui);

  if (!may_save)
    gtk_widget_set_sensitive (saveauth, FALSE);

  string = g_strdup_printf (_("Passphrase for the client certificate on '%s'"),
                            realm);
  gtk_label_set_text (GTK_LABEL(message), string);
  g_free (string);

  switch (gtk_dialog_run (GTK_DIALOG(window)))
    {
      case GTK_RESPONSE_OK:
        cred = apr_pcalloc (pool, sizeof(*cred));
        cred->password = apr_pstrdup (pool, gtk_entry_get_text (GTK_ENTRY(passphrase)));
        cred->may_save = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(saveauth));
        *cred_p = cred;
        gtk_widget_destroy (window);
        while (gtk_events_pending())
          gtk_main_iteration();

        return SVN_NO_ERROR;
      default:
        gtk_widget_destroy (window);
        while (gtk_events_pending())
          gtk_main_iteration();
        *cred_p = 0;
        return svn_error_create ( SVN_ERR_CANCELLED, NULL, _("User aborted"));
    }
}


gboolean
nsvn_setup_auth(nsvn_t *nsvn, nsvn_config_t *config)
{
  apr_array_header_t *providers = NULL;

  if (nsvn == NULL)
    return FALSE;

  /* Disk based auth providers. */
  nsvn_auth_get_simple_provider(nsvn, &providers);
  nsvn_auth_get_username_provider(nsvn, &providers);

  /* Disk based ssl auth providers. */
  nsvn_auth_get_ssl_server_trust_file_provider(nsvn, &providers);
  nsvn_auth_get_ssl_client_cert_file_provider(nsvn, &providers);
  nsvn_auth_get_ssl_client_cert_pw_file_provider(nsvn, &providers);

  /* Interactive username/password auth provider. */
  nsvn_auth_get_simple_prompt_provider(nsvn, nsvn_auth_simple_prompt,
                                       2, &providers);
  /* Interactive username auth provider. */
  nsvn_auth_get_username_prompt_provider(nsvn, nsvn_auth_username_prompt,
                                         2, &providers);

  /* Interactive ssl auth providers. */
  nsvn_auth_get_ssl_server_trust_prompt_provider(nsvn,
                    nsvn_auth_ssl_server_trust_prompt, &providers);
  nsvn_auth_get_ssl_client_cert_prompt_provider(nsvn,
                    nsvn_auth_ssl_client_cert_prompt, 2, &providers);
  nsvn_auth_get_ssl_client_cert_pw_prompt_provider(nsvn,
                    nsvn_auth_ssl_client_cert_pw_prompt, 2, &providers);

  // TODO: Should get the values of non_interactive, store_passwd,
  // auth_cache from gconf.
  nsvn_auth_register(nsvn, config->def_username, config->def_passwd,
                     config->no_prompt_auth, config->no_save_passwd,
                     config->no_cache_auth, providers);

  return TRUE;
}
