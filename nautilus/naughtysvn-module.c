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

#include <libnautilus-extension/nautilus-extension-types.h>
#include <libnautilus-extension/nautilus-column-provider.h>
#include <libnautilus-extension/nautilus-property-page-provider.h>
#include <glib/gi18n-lib.h>
#include "nautilus-naughtysvn.h"
#include <stdlib.h>


/* Nautilus extension interface. */
void
nautilus_module_initialize (GTypeModule *module)
{
  nautilus_nsvn_register_type (module);
  #if NSVN_NEW_FEATURE
  nautilus_nsvn_prop_register_type (module);
  #endif
  /* Set up translation. */
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
}


void
nautilus_module_shutdown (void)
{
}


void
nautilus_module_list_types (const GType **types,
                            int *num_types)
{
  static GType type_list[1];

  type_list[0] = NAUTILUS_TYPE_NSVN;
  *types = type_list;
  *num_types = 1;
}
