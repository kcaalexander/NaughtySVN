/*
* ====================================================================
* program description here. 
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
//#include <glib/gi18n-lib.h>
#include <libgnomevfs/gnome-vfs-utils.h>
#include <libgnomevfs/gnome-vfs-file-info.h>
#include <libgnomevfs/gnome-vfs-ops.h>
#include <libgnomevfs/gnome-vfs-directory.h>
#include <libgnomevfs/gnome-vfs-result.h>

#include <libnautilus-extension/nautilus-extension-types.h>
#include <libnautilus-extension/nautilus-file-info.h>
#include <libnautilus-extension/nautilus-menu-provider.h>
#include <libnautilus-extension/nautilus-property-page-provider.h>
#include "nautilus-naughtysvn.h"
#include "svn/naughtysvn.h"


static GObjectClass *parent_class;


static void
nsvn_checkout (NautilusMenuItem *item,
               gpointer user_data)
{
  NautilusFileInfo *file;
  char *uri;
  char *path;
  GString *cmd;

  file = g_object_get_data (G_OBJECT(item), "files");
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  cmd = g_string_new ("naughtysvn");
  g_string_append_printf (cmd, " MID=NSVN CMD=checkout \"%s\"", path);
  g_spawn_command_line_async (cmd->str, NULL);
  g_string_free (cmd, TRUE);
  g_free (path);
  g_free (uri);
}


static void
nsvn_preferences (NautilusMenuItem *item,
                  gpointer user_data)
{
  GString *cmd;

  cmd = g_string_new ("naughtysvn MID=NSVN CMD=preferences");
  g_spawn_command_line_async (cmd->str, NULL);
  g_string_free (cmd, TRUE);
}


static void
nsvn_about_nsvn (NautilusMenuItem *item,
                 gpointer user_data)
{
  GString *cmd;

  cmd = g_string_new ("naughtysvn MID=NSVN CMD=about_nsvn");
  g_spawn_command_line_async (cmd->str, NULL);
  g_string_free (cmd, TRUE);
}


static void
nsvn_repos_create (NautilusMenuItem *item,
                   gpointer user_data)
{
  NautilusFileInfo *file;
  char *uri;
  char *path;
  GString *cmd;

  file = g_object_get_data (G_OBJECT(item), "files");
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  cmd = g_string_new ("naughtysvn");
  g_string_append_printf (cmd, " MID=NSVN CMD=create_repos \"%s\"", path);
  g_spawn_command_line_async (cmd->str, NULL);
  g_string_free (cmd, TRUE);
  g_free (path);
  g_free (uri);
}


static GList*
nsvn_create_menuitem_checkout (NautilusMenuProvider *provider,
                               GtkWidget *widget,
                               GList *files,
                               GList *items)
{
  NautilusMenuItem *item = NULL;
  NautilusFileInfo *file;
  char *scheme;
  char *uri;
  char *path;

  if (!files)// && files->next != NULL)
    return NULL;
#if 1
  file = NAUTILUS_FILE_INFO (files->data);
  scheme = nautilus_file_info_get_uri_scheme (file);
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (strcmp (scheme, "file") != 0 ||
      !nautilus_file_info_is_directory (file) ||
      nsvn_wc_check_is_wcpath (NULL, path, NULL) == EXIT_SUCCESS)
    {
      g_free (scheme);
      return NULL;
    }
#endif
  item = nautilus_menu_item_new ("NautilusNSVN::FT_Checkout",
                                 _("NaughtySVN Checkout"),
                                 _("Checkout out a Subversion repository"),
                                 NULL);
  g_object_set_data (G_OBJECT (item), "files", file);
  g_signal_connect (item, "activate", G_CALLBACK (nsvn_checkout),
                    provider);

  items = g_list_append (items, item);

  return items;
}


static GList*
nsvn_create_menuitem_preferences (NautilusMenuProvider *provider,
                                  GtkWidget *widget,
                                  GList *files,
                                  GList *items)
{
  NautilusMenuItem *item = NULL;

  item = nautilus_menu_item_new ("NautilusNSVN::FT_Preferences",
                                 _("NaughtySVN Preferences"),
                                 _("Global  NaughtySVN with preferences"),
                                 NULL);
  g_signal_connect (item, "activate", G_CALLBACK (nsvn_preferences),
                    provider);

  items = g_list_append (items, item);

  return items;
}


static GList*
nsvn_create_menuitem_aboutnsvn (NautilusMenuProvider *provider,
                                GtkWidget *widget,
                                GList *files,
                                GList *items)
{
  NautilusMenuItem *item = NULL;

  item = nautilus_menu_item_new ("NautilusNSVN::FT_About_Nsvn",
                                 _("About NaughtySVN"),
                                 _("All about NaughtySVN"),
                                 NULL);
  g_signal_connect (item, "activate", G_CALLBACK (nsvn_about_nsvn),
                    provider);

  items = g_list_append (items, item);

  return items;
}


static GList*
nsvn_create_menuitem_reposcreate (NautilusMenuProvider *provider,
                                  GtkWidget *widget,
                                  GList *files,
                                  GList *items)
{
  NautilusMenuItem *item = NULL;

  if (files && files->next == NULL)
    {
      NautilusFileInfo *file;
      char *scheme;
      char *uri;
      char *path;

      file = NAUTILUS_FILE_INFO (files->data);
      scheme = nautilus_file_info_get_uri_scheme (file);
      uri = nautilus_file_info_get_uri (file);
      path = gnome_vfs_get_local_path_from_uri (uri);

      if (strcmp (scheme, "file") != 0 ||
          !nautilus_file_info_is_directory (file) ||
          nsvn_wc_check_is_wcpath (NULL, path, NULL) == EXIT_SUCCESS)
        {
          g_free (scheme);
          return NULL;
        }
      item = nautilus_menu_item_new ("NautilusNSVN::FT_Create_Repos",
                                     _("Create Repository"),
                                     _("Create FSFS/BDB subversion repository"),
                                     NULL);
      g_object_set_data (G_OBJECT (item), "files", file);
      g_signal_connect (item, "activate",
                        G_CALLBACK (nsvn_repos_create),
                        provider);

      items = g_list_append (items, item);
    }

  return items;
}

#if 0
static GList*
nautilus_nsvn_get_toolbar_items (NautilusMenuProvider *provider,
                                 GtkWidget *widget,
                                 NautilusFileInfo *current_folder)
{
  GList *items = NULL;
  NautilusMenuItem *item;

  items = NULL;

  item = nautilus_menu_item_new ("NautilusNSVN::TB_Create_Repos",
                                 _("Create Repository"),
                                 "Create FSFS/BDB subversion repository",
                                 NULL);

  items = g_list_append (items, item);

  return items;
}
#endif

static GList*
nautilus_nsvn_get_background_items (NautilusMenuProvider *provider,
                                    GtkWidget *widget,
                                    NautilusFileInfo *current_folder)
{
  GList *items = NULL;
  GList *files = NULL;

  files = g_list_append (files, current_folder);

  items = nsvn_create_menuitem_reposcreate (provider, widget,
                                            files, items);

  return items;
}

static GList*
nautilus_nsvn_get_file_items (NautilusMenuProvider *provider,
                              GtkWidget *widget,
                              GList *files)
{
  GList *items = NULL;

  items = nsvn_create_menuitem_reposcreate (provider, widget,
                                            files, items);
  items = nsvn_create_menuitem_aboutnsvn (provider, widget,
                                          files, items);
  items = nsvn_create_menuitem_preferences (provider, widget,
                                            files, items);
  items = nsvn_create_menuitem_checkout (provider, widget,
                                         files, items);
  return items;
}


static void
nautilus_nsvn_menu_provider_iface_init (NautilusMenuProviderIface *iface)
{
  iface->get_file_items = nautilus_nsvn_get_file_items;
//  iface->get_toolbar_items = nautilus_nsvn_get_toolbar_items;
  iface->get_background_items = nautilus_nsvn_get_background_items;
}


static void
nautilus_nsvn_instance_init (NautilusNSVN *nsvn)
{
}


static void
nautilus_nsvn_class_init (NautilusNSVNClass *class)
{ 
  parent_class = g_type_class_peek_parent (class);
}


static GType nsvn_type = 0;

GType
nautilus_nsvn_get_type (void)
{
  return nsvn_type;
}


void
nautilus_nsvn_register_type (GTypeModule *module)
{
  static const GTypeInfo info = {
      sizeof (NautilusNSVNClass),
      (GBaseInitFunc) NULL,
      (GBaseFinalizeFunc) NULL,
      (GClassInitFunc) nautilus_nsvn_class_init,
      NULL,
      NULL,
      sizeof (NautilusNSVN),
      0,
      (GInstanceInitFunc) nautilus_nsvn_instance_init,
  };

  static const GInterfaceInfo menu_provider_iface_info = {
      (GInterfaceInitFunc) nautilus_nsvn_menu_provider_iface_init,
      NULL,
      NULL
  };
  
  nsvn_type = g_type_module_register_type (module, 
                                           G_TYPE_OBJECT,
                                           "NautilusNaughtySVN",
                                           &info, 0);

  g_type_module_add_interface (module,
                               nsvn_type,
                               NAUTILUS_TYPE_MENU_PROVIDER,
                               &menu_provider_iface_info);
}

#if NSVN_NEW_FEATURE
static GType nsvn_prop_type = 0;
df
GtkWidget *
nsvn_properties_view_page (const char *location)
{
  printf ("Property page location [%s]\n", location);
  return (gtk_label_new ("This a test"));
}


static GList *
nsvn_properties_get_pages (NautilusPropertyPageProvider *provider,
                           GList *files)
{
  GList *pages = NULL;
  NautilusFileInfo *file;
  char *uri = NULL;
  GtkWidget *page, *label;
  NautilusPropertyPage *property_page;
  guint i;
  gboolean found = FALSE;

  /* okay, make the page */
  file = NAUTILUS_FILE_INFO (files->data);
  uri = nautilus_file_info_get_uri (file);
  label = gtk_label_new (_("NaughtySVN"));
  page = nsvn_properties_view_page (uri);
  property_page = nautilus_property_page_new ("NaughtySVNProperties",
                                              label, page);

  pages = g_list_prepend (pages, property_page);

  g_free (uri);
  return pages;
}


static void
property_page_provider_iface_init (NautilusPropertyPageProviderIface *iface)
{
  iface->get_pages = nsvn_properties_get_pages;
}


void
nautilus_nsvn_prop_register_type (GTypeModule *module)
{
  static const GTypeInfo info = {
              sizeof (GObjectClass),
              (GBaseInitFunc) NULL,
              (GBaseFinalizeFunc) NULL,
              (GClassInitFunc) NULL,
              NULL,
              NULL,
              sizeof (GObject),
              0,
              (GInstanceInitFunc) NULL
  };
  
  static const GInterfaceInfo property_page_provider_iface_info = {
              (GInterfaceInitFunc)property_page_provider_iface_init,
              NULL,
              NULL
  };

  nsvn_prop_type = g_type_module_register_type (module,
                                                G_TYPE_OBJECT,
                                                "NaughtySVNProperties",
                                                &info, 0);
  g_type_module_add_interface (module, nsvn_prop_type,
                               NAUTILUS_TYPE_PROPERTY_PAGE_PROVIDER,
                               &property_page_provider_iface_info);
}
#endif
