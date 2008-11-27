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
#include <svn_pools.h>
#include <svn_wc.h>

#include <gtk/gtk.h>
#include <glade/glade.h>

#define DLG_GLADE_FILE  "naughtysvn.glade"


static GObjectClass *parent_class;

static GtkWidget *
nsvn_properties_view_page (NautilusFileInfo *file);

static gchar **
g_queue_to_gchar_array (GQueue *queue)
{
  gint index = 0;
  gchar **retval = g_malloc ((queue->length+1) * sizeof (gpointer));
  GList *current;

  for (current = queue->head; current; current = g_list_next (current) )
    retval[index++] = current->data;
  retval[index] = NULL;
  return retval;
}

static void
g_free_wrap ( gpointer data,
              gpointer user_data)
{
  g_free (data);
}

static void
nsvn_log (NautilusMenuItem *item,
          gpointer user_data)
{
  GList *files;
  NautilusFileInfo *file;
  char *uri;
  char *path;

  files = g_object_get_data (G_OBJECT(item), "files");
  file = files->data; /* only one file is given to log */
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    gchar *argv[5];
    argv[0] = "naughtysvn";
    argv[1] = "MID=NSVN";
    argv[2] = "CMD=log";
    argv[3] = path;
    argv[4] = NULL;
    g_spawn_async ( NULL, /* working_directory */
                    argv,
                    NULL, /* envp */
                    G_SPAWN_SEARCH_PATH,
                    NULL, /* child_setup */
		    NULL, /* user-data */
		    NULL, /* child_pid */
		    NULL /* error */
		  );
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
  GQueue *cmd = g_queue_new();
  gint file_count=0;

  g_queue_push_tail (cmd, g_strdup("naughtysvn"));
  g_queue_push_tail (cmd, g_strdup("MID=NSVN"));
  g_queue_push_tail (cmd, g_strdup("CMD=update"));

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
        g_queue_push_tail (cmd, path);
        file_count++;
      }
      g_free (uri);
      file_ptr = g_list_next (file_ptr);
    }

  if (file_count)
  {
    gchar **argv = g_queue_to_gchar_array (cmd);
    g_spawn_async ( NULL, /* working_directory */
                    argv,
                    NULL, /* envp */
                    G_SPAWN_SEARCH_PATH,
                    NULL, /* child_setup */
		    NULL, /* user-data */
		    NULL, /* child_pid */
		    NULL /* error */
		  );
    g_free (argv);
  }
  g_queue_foreach (cmd, g_free_wrap, NULL);
  g_queue_free (cmd);
}


static void
nsvn_commit (NautilusMenuItem *item,
             gpointer user_data)
{
  GList *files=NULL;
  GList *file_ptr=NULL;
  GQueue *cmd = g_queue_new();
  gint file_count=0;

  g_queue_push_tail (cmd, g_strdup("naughtysvn"));
  g_queue_push_tail (cmd, g_strdup("MID=NSVN"));
  g_queue_push_tail (cmd, g_strdup("CMD=commit"));
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
        g_queue_push_tail (cmd, path);
        file_count++;
      }
      g_free (uri);
      file_ptr = g_list_next (file_ptr);
    }

  if (file_count)
  {
    gchar **argv = g_queue_to_gchar_array (cmd);
    g_spawn_async ( NULL, /* working_directory */
                    argv,
                    NULL, /* envp */
                    G_SPAWN_SEARCH_PATH,
                    NULL, /* child_setup */
		    NULL, /* user-data */
		    NULL, /* child_pid */
		    NULL /* error */
		  );
    g_free (argv);
  }
  g_queue_foreach (cmd, g_free_wrap, NULL);
  g_queue_free (cmd);
}

static void
nsvn_add (NautilusMenuItem *item,
          gpointer user_data)
{
  GList *files=NULL;
  GList *file_ptr=NULL;
  GQueue *cmd = g_queue_new();
  gint file_count=0;

  g_queue_push_tail (cmd, g_strdup("naughtysvn"));
  g_queue_push_tail (cmd, g_strdup("MID=NSVN"));
  g_queue_push_tail (cmd, g_strdup("CMD=add"));
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
        g_queue_push_tail (cmd, path);
        file_count++;
      }
      g_free (uri);
      file_ptr = g_list_next (file_ptr);
    }

  if (file_count)
  {
    gchar **argv = g_queue_to_gchar_array (cmd);
    g_spawn_async ( NULL, /* working_directory */
                    argv,
                    NULL, /* envp */
                    G_SPAWN_SEARCH_PATH,
                    NULL, /* child_setup */
		    NULL, /* user-data */
		    NULL, /* child_pid */
		    NULL /* error */
		  );
    g_free (argv);
  }
  g_queue_foreach (cmd, g_free_wrap, NULL);
  g_queue_free (cmd);
}


static void
nsvn_checkout (NautilusMenuItem *item,
               gpointer user_data)
{
  GList *files;
  NautilusFileInfo *file;
  char *uri;
  char *path;

  files = g_object_get_data (G_OBJECT(item), "files");
  file = files->data; /* only one file is given to checkout */
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    gchar *argv[5];
    argv[0] = "naughtysvn";
    argv[1] = "MID=NSVN";
    argv[2] = "CMD=checkout";
    argv[3] = path;
    argv[4] = NULL;
    g_spawn_async ( NULL, /* working_directory */
                    argv,
                    NULL, /* envp */
                    G_SPAWN_SEARCH_PATH,
                    NULL, /* child_setup */
		    NULL, /* user-data */
		    NULL, /* child_pid */
		    NULL /* error */
		  );
    g_free (path);
  }
  g_free (uri);
}


static void
nsvn_preferences (NautilusMenuItem *item,
                  gpointer user_data)
{
  g_spawn_command_line_async ("naughtysvn MID=NSVN CMD=preferences", NULL);
}


static void
nsvn_about_nsvn (NautilusMenuItem *item,
                 gpointer user_data)
{
  g_spawn_command_line_async ("naughtysvn MID=NSVN CMD=about_nsvn", NULL);
}


static void
nsvn_repos_create (NautilusMenuItem *item,
                   gpointer user_data)
{
  GList *files;
  NautilusFileInfo *file;
  char *uri;
  char *path;

  files = g_object_get_data (G_OBJECT(item), "files");
  file = files->data; /* only one file is given to repos_create */
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    gchar *argv[5];
    argv[0] = "naughtysvn";
    argv[1] = "MID=NSVN";
    argv[2] = "CMD=create_repos";
    argv[3] = path;
    argv[4] = NULL;
    g_spawn_async ( NULL, /* working_directory */
                    argv,
                    NULL, /* envp */
                    G_SPAWN_SEARCH_PATH,
                    NULL, /* child_setup */
		    NULL, /* user-data */
		    NULL, /* child_pid */
		    NULL /* error */
		  );
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

  if (!files)
    return items;

  if (files->next)
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
    g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy(files), (GDestroyNotify) nautilus_file_info_list_free);
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
  GList *file_ptr = files;
  int versioneditems=0;

  /* If we have files, then check all entries until we find a entry that is versioned, else bail out
   *   entries that are directories are checked as they are
   *   entries that are files, we check if the parent directory is versioned.
   * If all above fails, bail out
   *
   * Append "Update" to the context menu
   */

  if (!files)
    return items;

  while (file_ptr)
  {
    NautilusFileInfo *file;
    gchar *uri;
    gchar *path;

    file = NAUTILUS_FILE_INFO (file_ptr->data);
    uri = nautilus_file_info_get_uri (file);
    path = gnome_vfs_get_local_path_from_uri (uri);

    if (path)
    {
      if (nautilus_file_info_is_directory (file))
      {
        int wc_for;
        if (nsvn_wc_check_is_wcpath (NULL, path, &wc_for) == EXIT_SUCCESS)
          versioneditems++;
      } else {
        gchar *wc_path;
        int wc_for;
        wc_path = g_path_get_dirname (path);
        if (nsvn_wc_check_is_wcpath (NULL, wc_path, &wc_for) == EXIT_SUCCESS)
          versioneditems++;
        g_free (wc_path);
      }
      g_free(path);
    }
    g_free(uri);
    if (versioneditems)
      break;
    else
      file_ptr = g_list_next (file_ptr);
  }
  if (!versioneditems)
    return items;

  item = nautilus_menu_item_new ("NautilusNSVN::FT_Update",
           _("NaughtySVN Update"),
           _("Bring changes from the repository into the working copy"),
           PIXDIR "/update.png");
  g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy(files), (GDestroyNotify) nautilus_file_info_list_free);
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
  GList *file_ptr = files;
  nsvn_t *nsvn;
  nsvn = nsvn_base_init (NULL);

  /* Walking through the selected list to find any
   * on the selected items is a part of the wc.
   */
  while (file_ptr)
    {
      NautilusFileInfo *file;
      gchar *uri;
      gchar *path;
      int wc_for;

      file = NAUTILUS_FILE_INFO (file_ptr->data);
      uri = nautilus_file_info_get_uri (file);
      path = gnome_vfs_get_local_path_from_uri (uri);

      if (path)
        {
          /* If its a file, getting parent directory to
           * find a entity is a working copy path or not.
           *
           *If its a dir, check that the dir is in  is a
           * working copy or not.
           */
          if (nautilus_file_info_is_directory (file))
            {
              if (nsvn_wc_check_is_wcpath (NULL, path, &wc_for) != EXIT_SUCCESS)
                {
                  g_free (path);
                  g_free (uri);
                  file_ptr = g_list_next (file_ptr);
                  continue;
                }
            } else {
              gchar *wc_path;
              wc_path = g_path_get_dirname (path);
              if (nsvn_wc_check_is_wcpath (NULL, wc_path, &wc_for) != EXIT_SUCCESS)
                {
                  g_free (wc_path);
                  g_free (path);
                  g_free (uri);
                  file_ptr = g_list_next (file_ptr);
                  continue;
                }
              g_free (wc_path);
            }

          item = nautilus_menu_item_new ("NautilusNSVN::FT_Commit",
                   _("NaughtySVN Commit"),
                   _("Make permanent changes in Subversion repository"),
                   PIXDIR "/commit.png");
          g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy(files), (GDestroyNotify) nautilus_file_info_list_free);
          g_signal_connect (item, "activate", G_CALLBACK (nsvn_commit),
                            provider);

          items = g_list_append (items, item);

          g_free (path);
          g_free (uri);
          break;
        }
      g_free (uri);
      file_ptr = g_list_next (file_ptr);
    }
  nsvn = nsvn_base_uninit (nsvn);
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
          //
          // In a dir, check that the dir is not in SVN,
          // if not, run the same checks as for files on
          // the parent directory.
          if (nautilus_file_info_is_directory (file))
            {
              if (nsvn_wc_check_is_wcpath (NULL, path, &wc_for) == EXIT_SUCCESS)
                {
                  /* path is already in SVN */
                  g_free(path);
                  g_free(uri);
                  file_ptr = g_list_next (file_ptr);
                  continue;
                }
            }
          wc_path = g_path_get_dirname (path);
          if (nsvn_wc_check_is_wcpath (NULL, wc_path, &wc_for) == EXIT_SUCCESS)
            {
              item = nautilus_menu_item_new ("NautilusNSVN::FT_Add",
                                   _("NaughtySVN Add"),
                                   _("Add a unversioned item to Subversion"),
                                   PIXDIR "/add.png");
              g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy(files), (GDestroyNotify) nautilus_file_info_list_free);
              g_signal_connect (item, "activate", G_CALLBACK (nsvn_add),
                                provider);

              items = g_list_append (items, item);

              g_free (wc_path);
              g_free (path);
              g_free (uri);
              break;
            }

          g_free (wc_path);
          g_free (path);
        }
      g_free (uri);
      file_ptr = g_list_next (file_ptr);
    }
  nsvn = nsvn_base_uninit (nsvn);
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
  char *uri;
  char *path;

  if (!files)
    return items;

  if (files->next)
    return items;

  file = NAUTILUS_FILE_INFO (files->data);
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
  {
    if (!nautilus_file_info_is_directory (file) ||
        nsvn_wc_check_is_wcpath (NULL, path, NULL) == EXIT_SUCCESS)
      {
        g_free (path);
        g_free (uri);
        return items;
      }
    g_free (path);
  }
  g_free (uri);

  item = nautilus_menu_item_new ("NautilusNSVN::FT_Checkout",
                                 _("NaughtySVN Checkout"),
                                 _("Checkout out a Subversion repository"),
                                 PIXDIR "/checkout.png");
  g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy(files), (GDestroyNotify) nautilus_file_info_list_free);

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
  NautilusFileInfo *file;
  char *uri;
  char *path;

  if (!files)
    return items;

  if (files->next)
    return items;

  file = NAUTILUS_FILE_INFO (files->data);
  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);

  if (path)
    {
      GList *files = NULL;
      if (!nautilus_file_info_is_directory (file) ||
          nsvn_wc_check_is_wcpath (NULL, path, NULL) == EXIT_SUCCESS)
        {
          g_free (path);
          g_free (uri);
          return items;
        }
      item = nautilus_menu_item_new ("NautilusNSVN::FT_Create_Repos",
                                     _("NaughtySVN Create Repository"),
                                     _("Create FSFS/BDB subversion repository"),
                                     PIXDIR "/create_repos.png");
      files = g_list_append (files, file);
      g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy(files), (GDestroyNotify) nautilus_file_info_list_free);
      g_list_free(files);
      g_signal_connect (item, "activate",
                        G_CALLBACK (nsvn_repos_create),
                        provider);

      items = g_list_append (items, item);

      g_free (path);
    }
  g_free (uri);

  return items;
}

static GList*
nsvn_create_menuitem_refresh (NautilusMenuProvider *provider,
                             GtkWidget *widget,
                             GList *files,
                             GList *items)
{
  NautilusMenuItem *item = NULL;

  if (!files)
    return items;

  item = nautilus_menu_item_new ("NautilusNSVN::FT_Refresh",
           _("NaughtySVN Refresh"),
           _("Refreshes the emblems on the icons"),
           PIXDIR "/refresh.png");
  g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy(files), (GDestroyNotify) nautilus_file_info_list_free);
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
nautilus_nsvn_get_file_items (NautilusMenuProvider *provider,
                              GtkWidget *widget,
                              GList *files)
{
  GList *items = NULL;

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
  items = nsvn_create_menuitem_checkout (provider, widget,
                                         files, items);
  items = nsvn_create_menuitem_reposcreate (provider, widget,
                                            files, items);
  items = nsvn_create_menuitem_preferences (provider, widget,
                                            files, items);
  items = nsvn_create_menuitem_aboutnsvn (provider, widget,
                                          files, items);
  return items;
}

static GList*
nautilus_nsvn_get_background_items (NautilusMenuProvider *provider,
                                    GtkWidget *widget,
                                    NautilusFileInfo *current_folder)
{
  GList *files = NULL;

  files = g_list_append (files, current_folder);

  return nautilus_nsvn_get_file_items ( provider, widget, files );
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

#if 0
static const char *
svn_node_kind_t_to_str(svn_node_kind_t kind)
{
  switch (kind)
  {
    case svn_node_none:
      return "none";
    case svn_node_file:
      return "file";
    case svn_node_dir:
      return "dir";
    default:
    case svn_node_unknown:
      return "unknown";
  }
}
#endif

static const gchar *
svn_wc_status_kind_to_str(enum svn_wc_status_kind kind)
{
  switch (kind)
  {
    case svn_wc_status_none:
      return "none";
    case svn_wc_status_unversioned:
      return "unversioned";
    case svn_wc_status_normal:
      return "normal";
    case svn_wc_status_added:
      return "added";
    case svn_wc_status_missing:
      return "missing";
    case svn_wc_status_deleted:
      return "deleted";
    case svn_wc_status_replaced:
      return "replaced";
    case svn_wc_status_modified:
      return "modified";
    case svn_wc_status_merged:
      return "merged";
    case svn_wc_status_conflicted:
      return "conflicted";
    case svn_wc_status_ignored:
      return "ignored";
    case svn_wc_status_obstructed:
      return "obstructed";
    case svn_wc_status_external:
      return "external";
    case svn_wc_status_incomplete:
      return "incomplete";
    default:
      return "unknown";
  }
}

#if 0
static const gchar *
svn_wc_schedule_t_to_str(svn_wc_schedule_t schedule)
{
  switch (schedule)
  {
    case svn_wc_schedule_normal:
      return "normal";
    case svn_wc_schedule_add:
      return "add";
    case svn_wc_schedule_delete:
      return "delete";
    case svn_wc_schedule_replace:
      return "replace";
    default:
      return "unknown";
  }
}
#endif

static GtkWidget *
nsvn_properties_view_page_content (apr_pool_t *pool, const gchar *dir, const gchar *path)
{
  GtkWidget *page, *summary_txt;
  GladeXML *dlg_gui;

  svn_wc_status2_t *status = 0;
  svn_error_t *err;
  svn_wc_adm_access_t *adm_access;
#if 0
  apr_hash_t *prop_list;
#endif

  err = svn_wc_adm_open3 (&adm_access,
                          0, /* associated, */
                          dir,
                          FALSE, /* writelock */
                          0, /* depth */
                          0, /* cancel-func */
                          0, /* cancel-baton */
                          pool);
  if (err != SVN_NO_ERROR)
  {
    char buf[512];
    g_warning ("svn_wc_adm_open3() failed: %s\n", svn_err_best_message(err, buf, sizeof(buf)));
    svn_error_clear (err);
    return 0;
  }

  err = svn_wc_status2 (&status, path, adm_access, pool);
  if (err != SVN_NO_ERROR)
  {
    char buf[512];
    g_warning ("svn_wc_status2() failed: %s\n", svn_err_best_message(err, buf, sizeof(buf)));
    svn_error_clear (err);
    return 0;
  }

  if (!status->entry)
    return 0;

  dlg_gui = glade_xml_new (GLADEDIR "/" DLG_GLADE_FILE, "property_page", NULL);
  if (!dlg_gui)
  {
    g_warning ("Could not find " GLADEDIR "/" DLG_GLADE_FILE "\n");
    return 0;
  }

  /* Getting Widgets in repository creation dialog. */
  page = glade_xml_get_widget (dlg_gui, "property_page");
  summary_txt = glade_xml_get_widget (dlg_gui, "property_summary_txt");

  if (page && summary_txt)
  {
    GString *text = g_string_new("");
#if 0
    gchar datetime[APR_RFC822_DATE_LEN];
#endif
    GtkTextBuffer *buffer = gtk_text_view_get_buffer ( GTK_TEXT_VIEW ( summary_txt ) );

    if (status->entry->name && status->entry->name[0])
      g_string_append_printf (text, "Name: %s\n", status->entry->name);
    g_string_append_printf (text, "Revision: %" SVN_REVNUM_T_FMT "\n", status->entry->revision);
    g_string_append_printf (text, "URL: %s\n", status->entry->url);
    g_string_append_printf (text, "Repos: %s\n", status->entry->repos);
    g_string_append_printf (text, "Text status: %s\n" , svn_wc_status_kind_to_str(status->text_status));
    g_string_append_printf (text, "Prop status: %s\n" , svn_wc_status_kind_to_str(status->prop_status));
    gtk_text_buffer_set_text (buffer, text->str, text->len);
    /* buffer should not be g_unref()'ed.
     * "text" data however should be released.
     */
    g_string_free (text, TRUE);

#if 0
    /* nice for debug, can be removed later */
    fprintf(stderr, "status->entry->name = %s\n", status->entry->name);
    fprintf(stderr, "status->entry->revision = %" SVN_REVNUM_T_FMT "\n", status->entry->revision);
    fprintf(stderr, "status->entry->url = %s\n", status->entry->url);
    fprintf(stderr, "status->entry->repos = %s\n", status->entry->repos);
    fprintf(stderr, "status->entry->uuid = %s\n", status->entry->uuid);
    fprintf(stderr, "status->entry->kind = %s\n", svn_node_kind_t_to_str(status->entry->kind));
    fprintf(stderr, "status->entry->schedule = %s\n", svn_wc_schedule_t_to_str(status->entry->schedule));
    fprintf(stderr, "status->entry->copied = %d\n", status->entry->copied);
    fprintf(stderr, "status->entry->deleted = %d\n", status->entry->deleted);
    fprintf(stderr, "status->entry->absent = %d\n", status->entry->absent);
    fprintf(stderr, "status->entry->incomplete = %d\n", status->entry->incomplete);
    fprintf(stderr, "status->entry->copyfrom_url = %s\n", status->entry->copyfrom_url);
    fprintf(stderr, "status->entry->copyfrom_rev = %" SVN_REVNUM_T_FMT "\n", status->entry->copyfrom_rev);
    fprintf(stderr, "status->entry->conflict_old = %s\n", status->entry->conflict_old);
    fprintf(stderr, "status->entry->conflict_new = %s\n", status->entry->conflict_new);
    fprintf(stderr, "status->entry->conflict_wrk = %s\n", status->entry->conflict_wrk);
    fprintf(stderr, "status->entry->prejfile = %s\n", status->entry->prejfile);
    if ( !status->entry->text_time || (apr_rfc822_date (datetime, status->entry->text_time) != APR_SUCCESS) )
      strcpy(datetime, "N/A");
    fprintf(stderr, "status->entry->text_time = %s\n", datetime);
    if ( !status->entry->prop_time || (apr_rfc822_date (datetime, status->entry->prop_time) != APR_SUCCESS) )
      strcpy(datetime, "N/A");
    fprintf(stderr, "status->entry->prop_time = %s\n", datetime);
    fprintf(stderr, "status->entry->checksum = %s\n", status->entry->checksum);
    fprintf(stderr, "status->entry->cmt_rev = %" SVN_REVNUM_T_FMT "\n", status->entry->cmt_rev);
    if ( !status->entry->cmt_date || (apr_rfc822_date (datetime, status->entry->cmt_date) != APR_SUCCESS) )
      strcpy(datetime, "N/A");
    fprintf(stderr, "status->entry->cmt_date = %s\n", datetime);
    fprintf(stderr, "status->entry->cmt_author = %s\n", status->entry->cmt_author);
    fprintf(stderr, "status->entry->lock_token = %s\n", status->entry->lock_token);
    fprintf(stderr, "status->entry->lock_owner = %s\n", status->entry->lock_owner);
    fprintf(stderr, "status->entry->lock_comment = %s\n", status->entry->lock_comment);
    if ( !status->entry->lock_creation_date || (apr_rfc822_date (datetime, status->entry->lock_creation_date) != APR_SUCCESS) )
      strcpy(datetime, "N/A");
    fprintf(stderr, "status->entry->lock_creation_date = %s\n", datetime);
    fprintf(stderr, "status->entry->has_props = %d\n", status->entry->has_props);
    fprintf(stderr, "status->entry->has_prop_mods = %d\n", status->entry->has_prop_mods);
    fprintf(stderr, "status->entry->cachable_props = %s\n", status->entry->cachable_props);
    fprintf(stderr, "status->entry->present_props = %s\n", status->entry->present_props);
    fprintf(stderr, "status->text_status = %s\n" , svn_wc_status_kind_to_str(status->text_status));
    fprintf(stderr, "status->prop_status = %s\n", svn_wc_status_kind_to_str(status->prop_status));
    fprintf(stderr, "status->locked = %d\n", status->locked);
    fprintf(stderr, "status->copied = %d\n", status->copied);
    fprintf(stderr, "status->switched = %d\n", status->switched);
    fprintf(stderr, "status->repos_text_status = %s\n" , svn_wc_status_kind_to_str(status->repos_text_status));
    fprintf(stderr, "status->repos_prop_status = %s\n", svn_wc_status_kind_to_str(status->repos_prop_status));
    if (status->repos_lock)
    {
      fprintf(stderr, "status->repos_lock->path = %s\n", status->repos_lock->path);
      fprintf(stderr, "status->repos_lock->token = %s\n", status->repos_lock->token);
      fprintf(stderr, "status->repos_lock->owner = %s\n", status->repos_lock->owner);
      fprintf(stderr, "status->repos_lock->comment = %s\n", status->repos_lock->comment);
      fprintf(stderr, "status->repos_lock->is_dav_comment = %d\n", status->repos_lock->is_dav_comment);
       if ( !status->repos_lock->creation_date || (apr_rfc822_date (datetime, status->repos_lock->creation_date) != APR_SUCCESS) )
        strcpy(datetime, "N/A");
      fprintf(stderr, "status->repos_lock->creation_date = %s\n", datetime);
      if ( !status->repos_lock->expiration_date || (apr_rfc822_date (datetime, status->repos_lock->expiration_date) != APR_SUCCESS) )
        strcpy(datetime, "N/A");
      fprintf(stderr, "status->repos_lock->expiration_date = %s\n", datetime);
    }
    fprintf(stderr, "status->ood_kind = %s\n", svn_node_kind_t_to_str(status->ood_kind));
    fprintf(stderr, "status->ood_last_cmt_author = %s\n", status->ood_last_cmt_author);
#endif
  }
#if 0
  prop_list = 0;
  err = svn_wc_prop_list ( &prop_list, path, adm_access, pool);
  if (err != SVN_NO_ERROR)
  {
    char buf[512];
    g_warning ("svn_wc_adm_open3() failed: %s\n", svn_err_best_message(err, buf, sizeof(buf)));
    svn_error_clear (err);
  } else {
    apr_hash_index_t *index;
    index = apr_hash_first (pool, prop_list);
    while (index)
    {
      const void *key;
      apr_ssize_t klen;
      svn_prop_t *val;

      apr_hash_this (index, &key, &klen, (void **)&val);

      fprintf(stderr, "key = %s\n", key);
      fprintf(stderr, "prop.name = %s\n", val->name);
      /*fprintf(stderr, "prop.value.data = %d\n", val->value->data);*/

      index = apr_hash_next (index);
    }
  }
#endif

  return (page);
}

static GtkWidget *
nsvn_properties_view_page (NautilusFileInfo *file)
{
  GtkWidget *page;

  apr_allocator_t *allocator;
  apr_pool_t *pool;

  gchar *uri;
  gchar *path;
  gchar *wc_path;

  uri = nautilus_file_info_get_uri (file);
  path = gnome_vfs_get_local_path_from_uri (uri);
  if (!path)
  {
    g_free(uri);
    return NULL;
  }

  if (apr_allocator_create (&allocator))
  {
    g_free(path);
    g_free(uri); 
    return NULL;
  }

  apr_allocator_max_free_set (allocator,
                              SVN_ALLOCATOR_RECOMMENDED_MAX_FREE);
  pool = svn_pool_create_ex (NULL,
                             allocator);
  apr_allocator_owner_set (allocator, pool);

  if (!nautilus_file_info_is_directory (file))
    wc_path = g_path_get_dirname (path);
  else
    wc_path = path;

  page = nsvn_properties_view_page_content (pool, wc_path, path);

  apr_pool_destroy (pool);

  if (wc_path != path)
    g_free(wc_path);
  g_free(path);
  g_free(uri);
 
  return (page);
}


static GList *
nsvn_properties_get_pages (NautilusPropertyPageProvider *provider,
                           GList *files)
{
  GList *pages = NULL;
  GtkWidget *page = NULL, *label;
  NautilusPropertyPage *property_page;
  NautilusFileInfo *file;

  /* okay, make the page */
  file = NAUTILUS_FILE_INFO (files->data);
  label = gtk_label_new (_("NaughtySVN"));
  page = nsvn_properties_view_page (file);
  if (page == NULL)
    goto go;
  property_page = nautilus_property_page_new ("NaughtySVNProperties",
                                              label, page);

  pages = g_list_prepend (pages, property_page);
go:
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

  if (work->result == svn_wc_status_normal)
  {
    switch (status->prop_status)
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
        work->result = svn_wc_status_modified;
        break;
      case svn_wc_status_conflicted:
        work->result = svn_wc_status_conflicted;
        break;
    }
  }
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
  switch (status->prop_status)
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

    gdk_threads_enter();
    g_mutex_lock (emblem_async_queue_mutex);

    if (!work->cancelled)
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

            g_mutex_unlock (emblem_async_queue_mutex);
            gdk_threads_leave();

            nsvn->ctx->cancel_func = multi_entry_cancelled;
            nsvn->ctx->cancel_baton = work;

            nsvn_wc_status (nsvn, work->path,
                            multi_entry_info_callback,
                            &result, TRUE,
                            FALSE, FALSE, TRUE, FALSE);

            nsvn->ctx->cancel_func = NULL;
            nsvn->ctx->cancel_baton = NULL;

	    gdk_threads_enter();
            g_mutex_lock (emblem_async_queue_mutex);

            if (work->cancelled)
              break;

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

    work = g_queue_pop_head (emblem_async_queue);
    if (!work->cancelled)
      nautilus_info_provider_update_complete_invoke (work->update_complete,
                                                     work->provider,
                                                     (NautilusOperationHandle*)work,
                                                     NAUTILUS_OPERATION_COMPLETE);
    g_mutex_unlock (emblem_async_queue_mutex);
    gdk_threads_leave();
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
