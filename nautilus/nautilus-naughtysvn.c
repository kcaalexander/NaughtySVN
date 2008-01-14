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
#include <libnautilus-extension/nautilus-info-provider.h>
#include <libnautilus-extension/nautilus-property-page-provider.h>
#include "nautilus-naughtysvn.h"
#include "svn/naughtysvn.h"
#include "svn/svn-nsvn-types.h"
#include "svn_wc.h"

#include <gtk/gtk.h>
#include <glade/glade.h>

#define DLG_GLADE_FILE  "naughtysvn.glade"


static GObjectClass *parent_class;

GtkWidget *
nsvn_properties_view_page (const char *location);


static void
nsvn_log (NautilusMenuItem *item,
          gpointer user_data)
{
  NautilusFileInfo *file;
  char *uri;
  char *path;

  file = g_object_get_data (G_OBJECT(item), "files");
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    GString *cmd;
    cmd = g_string_new ("naughtysvn");
    g_string_append_printf (cmd, " MID=NSVN CMD=log \"%s\"", path);
    g_spawn_command_line_async (cmd->str, NULL);
    g_string_free (cmd, TRUE);
    g_free (path);
  }
  g_free (uri);
}


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
      if (path)
      {
        g_string_append_printf (cmd, "%s", path);

        //Appending the unique delimiter to the argument if more args follows.
        if (file_ptr)
          g_string_append_printf (cmd, "%c", ARG_RECORD_SEPARATOR);

        g_free (path);
      }
      g_free (uri);
      file_ptr = g_list_next (file_ptr);
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
      if (path)
      {
        g_string_append_printf (cmd, "%s", path);

        //Appending the unique delimiter to the argument if more args follows.
        if (file_ptr)
          g_string_append_printf (cmd, "%c", ARG_RECORD_SEPARATOR);

        g_free (path);
      }
      g_free (uri);
      file_ptr = g_list_next (file_ptr);
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
      if (path)
      {
        g_string_append_printf (cmd, "%s", path);

        //Appending the unique delimiter to the argument if more args follows.
        if (file_ptr)
          g_string_append_printf (cmd, "%c", ARG_RECORD_SEPARATOR);

        g_free (path);
      }
      g_free (uri);
      file_ptr = g_list_next (file_ptr);
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

  file = g_object_get_data (G_OBJECT(item), "files");
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    GString *cmd;
    cmd = g_string_new ("naughtysvn");
    g_string_append_printf (cmd, " MID=NSVN CMD=checkout \"%s\"", path);
    g_spawn_command_line_async (cmd->str, NULL);
    g_string_free (cmd, TRUE);
    g_free (path);
  }
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

  file = g_object_get_data (G_OBJECT(item), "files");
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    GString *cmd;
    cmd = g_string_new ("naughtysvn");
    g_string_append_printf (cmd, " MID=NSVN CMD=create_repos \"%s\"", path);
    g_spawn_command_line_async (cmd->str, NULL);
    g_string_free (cmd, TRUE);
    g_free (path);
  }
  g_free (uri);
}

static void
nsvn_refresh (NautilusMenuItem *item,
              gpointer user_data)
{
  GList *files=NULL;
  GList *file_ptr=NULL;

  files =  g_object_get_data (G_OBJECT(item), "files");
  file_ptr = files;

  for (file_ptr = files; file_ptr != NULL; file_ptr = g_list_next (file_ptr))
    {
      NautilusFileInfo *file;

      file = NAUTILUS_FILE_INFO (file_ptr->data);
      nautilus_file_info_invalidate_extension_info (file);
    }
    
  g_list_free (files);
}

static GList*
nsvn_create_menuitem_log (NautilusMenuProvider *provider,
                          GtkWidget *widget,
                          GList *files,
                          GList *items)
{
  NautilusMenuItem *item = NULL;
  NautilusFileInfo *file;
  char *uri;
  char *path;
  char *wc_path;

  if ((!files) || (files && files->next != NULL))
    return items;

  file = NAUTILUS_FILE_INFO (files->data);
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    // If its a file, Getting parent directory to
    // find a entity is a working copy path or not.
    if (nautilus_file_info_is_directory (file))
      wc_path = path;
    else
      wc_path = g_path_get_dirname (path);

    if (nsvn_wc_check_is_wcpath (NULL, wc_path, NULL) != EXIT_SUCCESS)
    {
      if (wc_path!=path)
        g_free (wc_path);
      g_free (path);
      g_free (uri);
      return items;
    }

    item = nautilus_menu_item_new ("NautilusNSVN::FT_Log",
             _("NaughtySVN Show Log"),
             _("Show the log messages"),
             PIXDIR "/log.png");
    g_object_set_data (G_OBJECT (item), "files", file);
    g_signal_connect (item, "activate", G_CALLBACK (nsvn_log),
                      provider);

    items = g_list_append (items, item);

    if (wc_path!=path)
      g_free (wc_path);
    g_free (path);
  }
  g_free (uri);

  return items;
}


static GList*
nsvn_create_menuitem_update (NautilusMenuProvider *provider,
                             GtkWidget *widget,
                             GList *files,
                             GList *items)
{
  NautilusMenuItem *item = NULL;

  if (!files)// && files->next != NULL)
    return items;

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

          if (path)
          {
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

                if (wc_path != path)
                  g_free (wc_path);
                g_free (path);
                g_free (uri);
                break;
            }

            if (wc_path != path)
              g_free (wc_path);
            g_free (path);
          }
          g_free (uri);
          file_ptr = g_list_next (file_ptr);
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
    return items;

#if 1
  file = NAUTILUS_FILE_INFO (files->data);
  scheme = nautilus_file_info_get_uri_scheme (file);
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    if (strcmp (scheme, "file") != 0 ||
        !nautilus_file_info_is_directory (file) ||
        nsvn_wc_check_is_wcpath (NULL, path, NULL) == EXIT_SUCCESS)
      {
        g_free (scheme);
        g_free (path);
        g_free (uri);
        return items;
      }
    g_free (path);
  }
  g_free (uri);
  g_free (scheme);
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

      if (path)
      {
        if (strcmp (scheme, "file") != 0 ||
            !nautilus_file_info_is_directory (file) ||
            nsvn_wc_check_is_wcpath (NULL, path, NULL) == EXIT_SUCCESS)
          {
            g_free (scheme);
            g_free (path);
            g_free (uri);
            return items;
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

        g_free (path);
      }
      g_free (scheme);
      g_free (uri);
    }

  return items;
}

static GList*
nsvn_create_menuitem_refresh (NautilusMenuProvider *provider,
                             GtkWidget *widget,
                             GList *files,
                             GList *items)
{
  NautilusMenuItem *item = NULL;

  if (!files)// && files->next != NULL)
    return items;

  item = nautilus_menu_item_new ("NautilusNSVN::FT_Refresh",
           _("NaughtySVN Refresh"),
           _("Refreshes the emblems on the icons"),
           PIXDIR "/refresh.png");
  g_object_set_data (G_OBJECT (item), "files",
                     (void*)g_list_copy (files));
  g_signal_connect (item, "activate", G_CALLBACK (nsvn_refresh),
                    provider);

  items = g_list_append (items, item);

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
  items = nsvn_create_menuitem_log (provider, widget,
                                    files, items);
  items = nsvn_create_menuitem_refresh (provider, widget,
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

/* We will have one worker thread that is common for all instances. So when the
 * first instance is created, we start the helper thread, and when the last
 * instance exists, we destroy the thread again. This way, the plugin can be
 * unloaded again in a safe maner.
 *
 * Per time of testing, only one instance of the interface is created, and  it
 * is never destroyed. This might however change in the future. and there is
 * nothing wrong with making it possible to exit cleanly.
 *
 * Currently, it isn't possible to cancel a call to nsvn_wc_status() when it
 * already is in progress.
 */


/* We need a lock to keep the interface-counter (and the thread variable)
 * threadsafe
 */
static GStaticMutex emblem_async_refcount_mutex = G_STATIC_MUTEX_INIT;
static gint emblem_async_refcount = 0;
static GThread *emblem_async_thread;

/* And we need a communcation channel agains the thread, and protection.
 * The only reason for not using GAsyncQueue, is that GAsyncQueue doesn't have
 * a peek (just a bit handy) and find (used in order to see if a cancel signal
 * from nautilus is safe to distribute) interface.
 */
static GQueue *emblem_async_queue = 0;
static GMutex *emblem_async_queue_mutex = 0;
static GCond  *emblem_async_queue_cond = 0;

/* And we also need a way to tell the thread that it should shutdown. This is
 * protected with a mutex, so that we force memory to flush.
 * */
static gint emblem_async_shutdown = 0;
static GStaticMutex emblem_async_shutdown_mutex = G_STATIC_MUTEX_INIT;

typedef struct async_work_t
{
  NautilusInfoProvider *provider;
  NautilusFileInfo     *file;
  GClosure             *update_complete;
  gint                  cancelled;
  gchar                *path;
} async_work_t;

typedef struct sync_work_t
{
  const char *path;
  enum svn_wc_status_kind result;
} sync_work_t;

static GType nsvn_info_type = 0;

GType
nautilus_nsvn_get_info_type (void)
{
  return nsvn_info_type;
}

static int
single_entry_info_callback (void *data,
                            const char *path,
                            svn_wc_status2_t *status)
{
  sync_work_t *work;

  if (!data)
    return EXIT_FAILURE;

  work = data;

  if (strcmp (work->path, path))
    return EXIT_SUCCESS;
 
  work->result = status->text_status;

  return EXIT_SUCCESS;
}

static svn_error_t *
multi_entry_cancelled (void *cancel_baton)
{
  struct async_work_t *work = cancel_baton;
  gint cancelled;

  /* sync memory before checking cancelled */
  g_mutex_lock (emblem_async_queue_mutex);

  cancelled = work->cancelled;

  g_mutex_unlock (emblem_async_queue_mutex);

  if (cancelled)
    return svn_error_create (SVN_ERR_CANCELLED, 0, "nautilus aborted the call");
  else
    return SVN_NO_ERROR;
}

static int
multi_entry_info_callback (void *data,
                           const char *path,
                           svn_wc_status2_t *status)
{
  enum svn_wc_status_kind *result;

  if (!data)
    return EXIT_FAILURE;

  result = data;
  switch (status->text_status)
  {
    case svn_wc_status_none:
    case svn_wc_status_unversioned:
    case svn_wc_status_missing:
    case svn_wc_status_ignored:
    case svn_wc_status_obstructed:
    case svn_wc_status_external:
    case svn_wc_status_incomplete:
    case svn_wc_status_normal:
      break;
    case svn_wc_status_added:
    case svn_wc_status_deleted:
    case svn_wc_status_replaced:
    case svn_wc_status_modified:
    case svn_wc_status_merged:
      if (*result == svn_wc_status_normal)
        *result = svn_wc_status_modified;
      break;
    case svn_wc_status_conflicted:
      *result = svn_wc_status_conflicted;
      break;
  }

  return EXIT_SUCCESS;
}

static gpointer
emblem_async_thread_worker (gpointer userdata)
{
  nsvn_t *nsvn;

  nsvn = nsvn_base_init (NULL);

  while (1)
  {
    async_work_t *work;
    sync_work_t swork;

    g_static_mutex_lock (&emblem_async_shutdown_mutex);

    if (emblem_async_shutdown)
      break;

    g_mutex_lock (emblem_async_queue_mutex);

    work = g_queue_peek_head (emblem_async_queue);

    /* if we got no work here, we have no work, since we checked emblem_async_shutdown before peeking */
    if (!work)
    {
      g_static_mutex_unlock (&emblem_async_shutdown_mutex);
      /* sleep until we get a job */
      g_cond_wait (emblem_async_queue_cond, emblem_async_queue_mutex);
      g_mutex_unlock (emblem_async_queue_mutex);
      continue;
    }

    g_mutex_unlock (emblem_async_queue_mutex);
  
    g_static_mutex_unlock (&emblem_async_shutdown_mutex);

    swork.result = svn_wc_status_none;
    swork.path = work->path;
    nsvn_wc_status (nsvn, work->path,
                    single_entry_info_callback,
                    &swork, FALSE,
                    TRUE, FALSE, TRUE, FALSE);
    switch (swork.result)
    {
      case svn_wc_status_none:
      case svn_wc_status_unversioned:
      case svn_wc_status_missing:
      case svn_wc_status_ignored:
      case svn_wc_status_obstructed:
      case svn_wc_status_external:
      case svn_wc_status_incomplete:
        break;
      case svn_wc_status_normal:
        {
          enum svn_wc_status_kind result = svn_wc_status_normal;

          nsvn->ctx->cancel_func = multi_entry_cancelled;
          nsvn->ctx->cancel_baton = work;

          nsvn_wc_status (nsvn, work->path,
                          multi_entry_info_callback,
                          &result, TRUE,
                          FALSE, FALSE, TRUE, FALSE);

          nsvn->ctx->cancel_func = NULL;
          nsvn->ctx->cancel_baton = NULL;

          switch (result)
          {
            default:
              nautilus_file_info_add_emblem (work->file, "svn-normal");
              break;
            case svn_wc_status_modified:
              nautilus_file_info_add_emblem (work->file, "svn-modified");
              break;
            case svn_wc_status_conflicted:
              nautilus_file_info_add_emblem (work->file, "svn-conflict");
              break;
          }
          break;
	}
      case svn_wc_status_added:
        nautilus_file_info_add_emblem (work->file, "svn-added");
        break;
      case svn_wc_status_deleted:
        nautilus_file_info_add_emblem (work->file, "svn-deleted");
        break;
      case svn_wc_status_merged:
      case svn_wc_status_replaced:
      case svn_wc_status_modified:
        nautilus_file_info_add_emblem (work->file, "svn-modified");
        break;
      case svn_wc_status_conflicted:
        nautilus_file_info_add_emblem (work->file, "svn-conflict");
        break;
    }

    g_mutex_lock (emblem_async_queue_mutex);
    work = g_queue_pop_head (emblem_async_queue);
    if (!work->cancelled)
      nautilus_info_provider_update_complete_invoke (work->update_complete,
                                                     work->provider,
                                                     (NautilusOperationHandle*)work,
                                                     NAUTILUS_OPERATION_COMPLETE);
    g_mutex_unlock (emblem_async_queue_mutex);
    g_free (work->path);
    g_closure_unref (work->update_complete);
    g_object_unref (work->file);
    g_free (work);
  }
  emblem_async_shutdown = 0;
  {
    /* We flush the queue here. This should not be needed, since when we are
     * destroyed, it should not be any pending jobs here, but we still check,
     * just for the sanity.
     */
    async_work_t *work;

    g_mutex_lock (emblem_async_queue_mutex);

    while ((work = g_queue_pop_head (emblem_async_queue))!=NULL)
    {
      if (!work->cancelled)
        nautilus_info_provider_update_complete_invoke (work->update_complete,
                                                       work->provider,
                                                       (NautilusOperationHandle*)work,
                                                       NAUTILUS_OPERATION_COMPLETE);
      g_free (work->path);
      g_closure_unref (work->update_complete);
      g_object_unref (work->file);
      g_free (work);
    }

    g_mutex_unlock (emblem_async_queue_mutex);
  }

  g_static_mutex_unlock (&emblem_async_shutdown_mutex);

  nsvn = nsvn_base_uninit (nsvn);

  return NULL;
}

static void
nsvn_info_cancel (NautilusInfoProvider *provider,
                  NautilusOperationHandle *handle)
{
  async_work_t *whandle = (async_work_t *)handle;
  g_mutex_lock (emblem_async_queue_mutex);

  if (g_queue_find (emblem_async_queue, whandle))
    whandle->cancelled = TRUE;

  g_mutex_unlock (emblem_async_queue_mutex);
}

static NautilusOperationResult
nsvn_info_update_file_info (NautilusInfoProvider     *provider,
                            NautilusFileInfo         *file,
                            GClosure                 *update_complete,
                            NautilusOperationHandle **handle)
{
  int wc_for;
  gchar *uri;
  gchar *path;

  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  /* if the uri is not a local uri, path will be NULL, so then we don't care about it, just bail out. */
  if (path == NULL)
  {
    g_free (uri);
    return NAUTILUS_OPERATION_COMPLETE;
  }

  if (nautilus_file_info_is_directory (file))
  {
    if (nsvn_wc_check_is_wcpath (NULL, path, &wc_for) == EXIT_SUCCESS)
    {
      async_work_t *whandle = g_new0(async_work_t, 1);
      whandle->provider = provider;
      whandle->file = g_object_ref (file);
      whandle->update_complete = g_closure_ref (update_complete);
      whandle->path = g_strdup (path);
      *handle = (NautilusOperationHandle *)whandle;
      g_mutex_lock (emblem_async_queue_mutex);
      g_queue_push_tail (emblem_async_queue, whandle);
      g_cond_signal (emblem_async_queue_cond);
      g_mutex_unlock (emblem_async_queue_mutex);
      return NAUTILUS_OPERATION_IN_PROGRESS;
    }
  } else { /* !nautilus_file_info_is_directory (file) */
    sync_work_t swork;
    nsvn_t *nsvn;

    nsvn = nsvn_base_init (NULL);

    swork.result = svn_wc_status_none;
    swork.path = path;
    nsvn_wc_status (nsvn, path,
                    single_entry_info_callback,
                    &swork, FALSE,
                    TRUE, FALSE, TRUE, FALSE);

    nsvn = nsvn_base_uninit (nsvn);

    switch (swork.result)
    {
      case svn_wc_status_none:
      case svn_wc_status_unversioned:
      case svn_wc_status_missing:
      case svn_wc_status_ignored:
      case svn_wc_status_obstructed:
      case svn_wc_status_external:
      case svn_wc_status_incomplete:
        break;
      case svn_wc_status_normal:
        nautilus_file_info_add_emblem (file, "svn-normal");
        break;
      case svn_wc_status_added:
        nautilus_file_info_add_emblem (file, "svn-added");
        break;
      case svn_wc_status_deleted:
        nautilus_file_info_add_emblem (file, "svn-deleted");
        break;
      case svn_wc_status_merged:
      case svn_wc_status_replaced:
      case svn_wc_status_modified:
        nautilus_file_info_add_emblem (file, "svn-modified");
        break;
      case svn_wc_status_conflicted:
        nautilus_file_info_add_emblem (file, "svn-conflict");
        break;
    }
  }

  g_free (uri);
  g_free (path);

  return NAUTILUS_OPERATION_COMPLETE;
}

static void
info_provider_iface_init (NautilusInfoProviderIface *iface, gpointer iface_data)
{
  iface->update_file_info = nsvn_info_update_file_info;
  iface->cancel_update = nsvn_info_cancel;

  g_static_mutex_lock (&emblem_async_refcount_mutex);

  /* check the state of the queue and worker thread */
  if (!emblem_async_refcount)
  {
    GError *err = 0;
    emblem_async_queue = g_queue_new ();
    emblem_async_queue_mutex = g_mutex_new ();
    emblem_async_queue_cond = g_cond_new ();
    emblem_async_thread = g_thread_create (emblem_async_thread_worker, NULL, TRUE, &err);
    if (!emblem_async_thread)
      g_error ("naughtysvn: unable to start helper thread: %s\n", err->message);
  }
  emblem_async_refcount++;

  g_static_mutex_unlock (&emblem_async_refcount_mutex);
}

static void
info_provider_iface_finalize (NautilusInfoProviderIface *iface, gpointer iface_data)
{
  g_static_mutex_lock (&emblem_async_refcount_mutex);

  emblem_async_refcount--;
  if (!emblem_async_refcount)
  {
    g_static_mutex_lock (&emblem_async_shutdown_mutex);
    emblem_async_shutdown = 1;
    g_mutex_lock (emblem_async_queue_mutex);
    g_queue_push_head (emblem_async_queue, NULL); /* we insert an empty job, just to wake the thread up */
    g_cond_signal (emblem_async_queue_cond);
    g_mutex_unlock (emblem_async_queue_mutex);
    g_static_mutex_unlock (&emblem_async_shutdown_mutex);
    g_thread_join (emblem_async_thread); /* we don't want to destroy interface until memory is safe */
    g_queue_free (emblem_async_queue);
    g_cond_free (emblem_async_queue_cond);
    g_mutex_free (emblem_async_queue_mutex);
    emblem_async_queue_cond = NULL;
    emblem_async_queue_mutex = NULL;
    emblem_async_queue = NULL;
  }

  g_static_mutex_unlock (&emblem_async_refcount_mutex);
}

void
nautilus_nsvn_info_register_type (GTypeModule *module)
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

  static const GInterfaceInfo info_provider_iface_info = {
              (GInterfaceInitFunc) info_provider_iface_init,
              (GInterfaceFinalizeFunc) info_provider_iface_finalize,
              NULL
  };

  nsvn_info_type = g_type_module_register_type (module,
                                                G_TYPE_OBJECT,
                                                "NaughtySVNInfo",
                                                &info, 0);
  g_type_module_add_interface (module, nsvn_info_type,
                               NAUTILUS_TYPE_INFO_PROVIDER,
                               &info_provider_iface_info);
}
