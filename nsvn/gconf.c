/*
* ====================================================================
* File will contain global gconf manipulation functions, which can be 
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

#include "gconf.h"


int
nsvn_gconf_write_entry (const char *key,
                        void *value,
                        GConfClient *gcc)
{
  GConfSchema *gcs;
  GError *gerr;
  GConfValueType gct;

  if (gcc == NULL || key == NULL)
    return 0;

  /* Check for the presense of schema, if not return fail. */
  /* Get key details from schema. */
  /* Check for the presense of dir, if not create it. */
  /* if already present update it with new value. */
  gcs = gconf_client_get_schema (gcc, key, &gerr);
  if (!gcs) return TRUE;

  gct = gconf_schema_get_type (gcs);
  switch (gct)
    {
      case GCONF_VALUE_STRING:
        gconf_client_set_string (gcc, key, value, &gerr);
        break;
      case GCONF_VALUE_INT:
        break;
      case GCONF_VALUE_FLOAT:
        break;
      case GCONF_VALUE_BOOL:
        gconf_client_set_bool (gcc, key, atoi (value), &gerr);
        break;
      case GCONF_VALUE_LIST:
        break;
      case GCONF_VALUE_PAIR:
        break;
      default:
        break;
    }
  return 1;
}
