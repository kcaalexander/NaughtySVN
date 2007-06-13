/*
* ====================================================================
* main program for nsvn module.
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

#include "window_list.h"

static const nsvn_command commands_list[] = {
  {"CMD=create_repos", nsvn_dlg_reposcreate},
  {"CMD=about_nsvn", nsvn_dlg_aboutnsvn},
  {"CMD=preferences", nsvn_dlg_preferences},
  {"CMD=checkout", nsvn_dlg_checkout},
  {"CMD=add", nsvn_dlg_add},
  {"CMD=commit", nsvn_dlg_commit},
  {"CMD=update", nsvn_dlg_update},
  {NULL, NULL}
};

int
main (int argc, char *argv[])
{
  int i = 0;
  gboolean found=FALSE;

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif
  gnome_program_init ("naughtysvn", VERSION, LIBGNOMEUI_MODULE,
                      argc, argv,
                      GNOME_PARAM_APP_DATADIR, PACKAGE_DATA_DIR,
                      GNOME_PARAM_NONE);

  if (argc <= 1 || argv[1] == NULL ||
      strcasecmp (argv[1], "MID=NSVN") != 0)
    {
      fprintf (stderr, _("\nError: This program is an integral part of NaughtySVN \
and is not expected to be called from command line\n"));
      return 1;
    }

  while (commands_list[i].cmd != NULL)
    {
      if (argv[2] != NULL &&
          strcasecmp(commands_list[i].cmd, argv[2]) == 0 &&
          commands_list[i].cmd_func != NULL)
        {
          if (commands_list[i].cmd_func (NULL, argv[3]) == EXIT_SUCCESS)
            found = TRUE;
        }
      i = i + 1;
    }

  if (found)
    gtk_main ();

  return 0;
}

