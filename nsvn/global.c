/*
* ====================================================================
* File will contain global functions, which can be 
* used in nsvn module.
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
#include <libgnomevfs/gnome-vfs-directory.h>
#include <libgnomevfs/gnome-vfs-utils.h>

#include "global.h"


gboolean
check_directory_isempty (char *uri)
{
  GnomeVFSResult result;
  gboolean isempty = FALSE;
  GnomeVFSDirectoryHandle *handle;
  GnomeVFSFileInfo *file_info = NULL;

  if (uri == NULL)
    return FALSE;

  result = gnome_vfs_directory_open (&handle, uri,
                                     GNOME_VFS_FILE_INFO_DEFAULT);

  file_info = gnome_vfs_file_info_new ();
  gnome_vfs_file_info_ref (file_info);

  while (result == GNOME_VFS_ERROR_NOT_FOUND ||
         result == GNOME_VFS_OK)
    {
      result = gnome_vfs_directory_read_next (handle, file_info);

      if ((result != GNOME_VFS_ERROR_EOF) ||
          (g_ascii_strcasecmp (file_info->name, ".") != 0) ||
          (g_ascii_strcasecmp (file_info->name, "..") != 0))
        {
          isempty = FALSE;
          break;
        }
      else
        isempty = TRUE;

      gnome_vfs_file_info_clear (file_info);
    }

  /* Cleaning up. */
  gnome_vfs_file_info_unref (file_info);
  result = gnome_vfs_directory_close (handle);

  return isempty;
}


GtkWidget*
Show_Msgbox (GtkWidget *parent,
             gboolean noshow,
             const char *msg_type,
             const char *msg,
             const char *icon,
             const char **buttons)
{
  GtkWidget *msg_box;
  char *smsg;

  smsg = g_strdup_printf ("%s: %s", msg_type, msg);
  msg_box = gnome_message_box_newv (smsg, icon, buttons);

  gtk_window_set_modal (GTK_WINDOW (msg_box), TRUE);

  if (parent)
    gtk_window_set_transient_for (GTK_WINDOW (msg_box),
                                  GTK_WINDOW (parent));

  if (!noshow)
    gtk_widget_show (msg_box);

  return msg_box;
}
