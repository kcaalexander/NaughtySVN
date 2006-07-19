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

#include <glade/glade.h>


/* Nautilus extension interface. */
void
nautilus_module_initialize (GTypeModule *module)
{
  nautilus_nsvn_register_type (module);
  nautilus_nsvn_prop_register_type (module);
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
  static GType type_list[2];

  type_list[0] = NAUTILUS_NSVN_TYPE;
  type_list[1] = NAUTILUS_NSVN_PROP_TYPE;

  *types = type_list;
  *num_types = G_N_ELEMENTS (type_list);
}
