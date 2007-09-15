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

#include <gtk/gtk.h>
#include <glade/glade.h>

#define DLG_GLADE_FILE  "naughtysvn.glade"


static GObjectClass *parent_class;

GtkWidget *
nsvn_properties_view_page (const char *location);


static void
nsvn_update (NautilusMenuItem *item,
             gpointer user_data)
{
  GList *files=NULL;
  GList *file_ptr=NULL;
  GString *cmd;

  cmd = g_string_new ("naughtysvn MID=NSVN CMD=update \"");
  files =  g_object_get_data (G_OBJECT(item), "files");
  file_ptr = files;

  while (file_ptr != NULL)
    {
      NautilusFileInfo *file;
      char *uri, *path;

      file = NAUTILUS_FILE_INFO (file_ptr->data);
      uri = nautilus_file_info_get_uri (file);
      path = gnome_vfs_get_local_path_from_uri (uri);
      g_string_append_printf (cmd, "%s", path);

      file_ptr = g_list_next (file_ptr);
      //Appending the unique delimiter to the argument if more args follows.
      if (file_ptr)
        g_string_append_printf (cmd, "%c", ARG_RECORD_SEPARATOR);
    }
    
  g_string_append (cmd, "\"");
  
  g_spawn_command_line_async (cmd->str, NULL);
  g_string_free (cmd, TRUE);
  file_ptr = NULL;
  g_list_free (files);
}


static void
nsvn_commit (NautilusMenuItem *item,
             gpointer user_data)
{
  GList *files=NULL;
  GList *file_ptr=NULL;
  GString *cmd;

  cmd = g_string_new ("naughtysvn MID=NSVN CMD=commit \"");
  files =  g_object_get_data (G_OBJECT(item), "files");
  file_ptr = files;

  while (file_ptr != NULL)
    {
      NautilusFileInfo *file;
      char *uri, *path;

      file = NAUTILUS_FILE_INFO (file_ptr->data);
      uri = nautilus_file_info_get_uri (file);
      path = gnome_vfs_get_local_path_from_uri (uri);
      g_string_append_printf (cmd, "%s", path);

      file_ptr = g_list_next (file_ptr);
      //Appending the unique delimiter to the argument if more args follows.
      if (file_ptr)
        g_string_append_printf (cmd, "%c", ARG_RECORD_SEPARATOR);
    }
    
  g_string_append (cmd, "\"");
  
  g_spawn_command_line_async (cmd->str, NULL);
  g_string_free (cmd, TRUE);
  file_ptr = NULL;
  g_list_free (files);
}


static void
nsvn_add (NautilusMenuItem *item,
          gpointer user_data)
{
  GList *files=NULL;
  GList *file_ptr=NULL;
  GString *cmd;

  cmd = g_string_new ("naughtysvn MID=NSVN CMD=add \"");
  files =  g_object_get_data (G_OBJECT(item), "files");
  file_ptr = files;

  while (file_ptr != NULL)
    {
      NautilusFileInfo *file;
      char *uri, *path;

      file = NAUTILUS_FILE_INFO (file_ptr->data);
      uri = nautilus_file_info_get_uri (file);
      path = gnome_vfs_get_local_path_from_uri (uri);
      g_string_append_printf (cmd, "%s", path);

      file_ptr = g_list_next (file_ptr);
      //Appending the unique delimiter to the argument if more args follows.
      if (file_ptr)
        g_string_append_printf (cmd, "%c", ARG_RECORD_SEPARATOR);
    }
    
  g_string_append (cmd, "\"");
  
  g_spawn_command_line_async (cmd->str, NULL);
  g_string_free (cmd, TRUE);
  file_ptr = NULL;
  g_list_free (files);
}


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
nsvn_create_menuitem_update (NautilusMenuProvider *provider,
                             GtkWidget *widget,
                             GList *files,
                             GList *items)
{
  NautilusMenuItem *item = NULL;

  if (!files)// && files->next != NULL)
    return NULL;

  item = nautilus_menu_item_new ("NautilusNSVN::FT_Update",
           _("NaughtySVN Update"),
           _("Bring changes from the repository into the working copy"),
           PIXDIR "/update.png");
  g_object_set_data (G_OBJECT (item), "files",
                     (void*)g_list_copy (files));
  g_signal_connect (item, "activate", G_CALLBACK (nsvn_update),
                    provider);

  items = g_list_append (items, item);

  return items;
}


static GList*
nsvn_create_menuitem_commit (NautilusMenuProvider *provider,
                             GtkWidget *widget,
                             GList *files,
                             GList *items)
{
  NautilusMenuItem *item = NULL;

  item = nautilus_menu_item_new ("NautilusNSVN::FT_Commit",
           _("NaughtySVN Commit"),
           _("Make permanent changes in Subversion repository"),
           PIXDIR "/commit.png");
  g_object_set_data (G_OBJECT (item), "files",
                     (void*)g_list_copy (files));
  g_signal_connect (item, "activate", G_CALLBACK (nsvn_commit),
                    provider);

  items = g_list_append (items, item);

  return items;
}


static GList*
nsvn_create_menuitem_add (NautilusMenuProvider *provider,
                          GtkWidget *widget,
                          GList *files,
                          GList *items)
{
  NautilusMenuItem *item = NULL;
  GList *file_ptr = files;
  int wc_for;
  nsvn_t *nsvn;
  nsvn = nsvn_base_init (NULL);

  if (files)
    {
      // Walking through the selected list to find any
      // on the selected items is a part of the wc.
      while (file_ptr)
        {
          NautilusFileInfo *file;
          char *uri;
          char *path;
          char *wc_path;

          file = NAUTILUS_FILE_INFO (file_ptr->data);
          uri = nautilus_file_info_get_uri (file);
          path = gnome_vfs_get_local_path_from_uri (uri);

          // In its a file, Getting parent directory to
          // find a entity is a working copy path or not.
          if (nautilus_file_info_is_directory (file))
            wc_path = path;
          else
            wc_path = g_path_get_dirname (path);

          if (nsvn_wc_check_is_wcpath (NULL, wc_path, &wc_for) == EXIT_SUCCESS)
            {
              item = nautilus_menu_item_new ("NautilusNSVN::FT_Add",
                                   _("NaughtySVN Add"),
                                   _("Add a unversioned item to Subversion"),
                                   PIXDIR "/add.png");
              g_object_set_data (G_OBJECT (item), "files",
                                 (void*)g_list_copy (files));
              g_signal_connect (item, "activate", G_CALLBACK (nsvn_add),
                                provider);

              items = g_list_append (items, item);
              break;
            }

          file_ptr = g_list_next(file_ptr);

          if (wc_path != path)
            g_free(wc_path);

          g_free(path);
        }
    }
  return items;
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
                                 PIXDIR "/checkout.png");
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
                                 PIXDIR "/preferences-general.png");
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
                                 PIXDIR "/user_icon.png");
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
                                     _("NaughtySVN Create Repository"),
                                     _("Create FSFS/BDB subversion repository"),
                                     PIXDIR "/create_repos.png");
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
  items = nsvn_create_menuitem_add (provider, widget,
                                    files, items);
  items = nsvn_create_menuitem_commit (provider, widget,
                                       files, items);
  items = nsvn_create_menuitem_update (provider, widget,
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

static GType nsvn_prop_type = 0;

GType
nautilus_nsvn_get_prop_type (void)
{
  return nsvn_prop_type;
}


GtkWidget *
nsvn_properties_view_page (const char *location)
{
  GtkWidget *page;
  GladeXML *dlg_gui;

  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, "property_page", NULL);
  if (!dlg_gui)
    {
      g_warning ("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n");
      return NULL;
    }

  /* Getting Widgets in repository creation dialog. */
  page = glade_xml_get_widget (dlg_gui, "property_page");

  return (page);
}


static GList *
nsvn_properties_get_pages (NautilusPropertyPageProvider *provider,
                           GList *files)
{
  GList *pages = NULL;
  NautilusFileInfo *file;
  char *uri = NULL;
  GtkWidget *page = NULL, *label;
  NautilusPropertyPage *property_page;

  /* okay, make the page */
  file = NAUTILUS_FILE_INFO (files->data);
  uri = nautilus_file_info_get_uri (file);
  label = gtk_label_new (_("NaughtySVN"));
  page = nsvn_properties_view_page (uri);
  if (page == NULL)
    goto go;
  property_page = nautilus_property_page_new ("NaughtySVNProperties",
                                              label, page);

  pages = g_list_prepend (pages, property_page);
go:
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
