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
#include <glib.h>

#include "gconf.h"
#include "global.h"


int
nsvn_gconf_create_config (nsvn_config_t *config)
{
  if (config == NULL)
    return 0;

  config->config_dir = NULL;
  config->def_username = NULL;
  config->def_passwd = NULL;
  config->no_cache_auth = FALSE;
  config->no_prompt_auth = FALSE;
  config->no_save_passwd = FALSE;

  return 1;
}


int
nsvn_gconf_clean_config (nsvn_config_t *config)
{
  g_free((void*)config->config_dir);
  g_free((void*)config->def_username);
  g_free((void*)config->def_passwd);

  return 1;
}


int
nsvn_gconf_read_config (nsvn_config_t *config,
                        GConfClient *gcc)
{
  if (config == NULL)
    return 0;

  nsvn_gconf_clean_config(config);

  /* TODO: read config_dir string. */ 
  config->config_dir = (char*)nsvn_gconf_read_config_entry(
                          NSVN_SVN_CONFIG_DIR_GCONF_KEY,
                          NSVN_SVN_CONFIG_DIR_GCONF_SCHEMAS_KEY, gcc);
                         
  /* TODO: read def_username string. */ 
  config->def_username = (char*)nsvn_gconf_read_config_entry(
                                     NSVN_SVN_USERNAME_GCONF_KEY,
                                     NSVN_SVN_USERNAME_GCONF_SCHEMAS_KEY, gcc);
  /* TODO: read def_passwd string. */ 
  config->def_passwd = (char*)nsvn_gconf_read_config_entry(
                                     NSVN_SVN_PASSWORD_GCONF_KEY,
                                     NSVN_SVN_PASSWORD_GCONF_SCHEMAS_KEY, gcc);
  /* TODO: read no_cache_auth boolean. */
  config->no_cache_auth = *((gboolean*)nsvn_gconf_read_config_entry(
                              NSVN_NO_CACHE_AUTH_TOKEN_GCONF_KEY,
                              NSVN_NO_CACHE_AUTH_TOKEN_GCONF_SCHEMAS_KEY, gcc));
  /* TODO: read no_prompt_auth boolean. */
  config->no_prompt_auth = *((gboolean*)nsvn_gconf_read_config_entry(
                              NSVN_NO_PROMPT_USER_GCONF_KEY,
                              NSVN_NO_PROMPT_USER_GCONF_SCHEMAS_KEY, gcc));
  /* TODO: read no_save_passwd boolean. */
  config->no_save_passwd = *((gboolean*)nsvn_gconf_read_config_entry(
                              NSVN_NO_STORE_PASSWD_GCONF_KEY,
                              NSVN_NO_STORE_PASSWD_GCONF_SCHEMAS_KEY, gcc));

  return 1;
}


int
nsvn_gconf_write_config (nsvn_config_t *config,
                         GConfClient *gcc)
{
  char *dir=NULL;
  char *dirname = NULL;
  char *basename = NULL;
  char *config_dir = NULL;

  if (config == NULL)
    return 0;

  /* TODO: write config_dir string. */
  if (config->config_dir == NULL || strcmp(config->config_dir, "") == 0)
    dir = g_build_path(G_DIR_SEPARATOR_S, ".subversion", NULL);
  else
    dir = g_build_path(G_DIR_SEPARATOR_S, config->config_dir, NULL);

  dirname = g_path_get_dirname(dir);
  basename = g_path_get_basename(dir);
  //Assume HOME directory if nill parent directory.
  if (dirname == NULL || strcmp(dirname, ".") == 0 ||
      strcmp(basename, "/") == 0)
    dirname = (char*)Get_Homedir();
  //No basename given assume it as .subversion.
  if (basename == NULL || strcmp(basename, "") == 0 ||
      strcmp(basename, "/") == 0)
    basename = g_strdup(".subversion");
 
  config_dir = g_build_path(G_DIR_SEPARATOR_S, dirname, basename, NULL);

  nsvn_gconf_write_config_entry(NSVN_SVN_CONFIG_DIR_GCONF_KEY,
                                NSVN_SVN_CONFIG_DIR_GCONF_SCHEMAS_KEY,
                                (void*)config_dir, gcc);
  g_free(config_dir);
  g_free(dir);

  /* TODO: write def_username string. */ 
  nsvn_gconf_write_config_entry(NSVN_SVN_USERNAME_GCONF_KEY,
                                NSVN_SVN_USERNAME_GCONF_SCHEMAS_KEY,
                                (void*)config->def_username, gcc);

  /* TODO: write def_passwd string. */ 
  nsvn_gconf_write_config_entry(NSVN_SVN_PASSWORD_GCONF_KEY,
                                NSVN_SVN_PASSWORD_GCONF_SCHEMAS_KEY,
                                (void*)config->def_passwd, gcc);

  /* TODO: write no_cache_auth boolean. */
  nsvn_gconf_write_config_entry(NSVN_NO_CACHE_AUTH_TOKEN_GCONF_KEY,
                                NSVN_NO_CACHE_AUTH_TOKEN_GCONF_SCHEMAS_KEY,
                                (void*)&(config->no_cache_auth), gcc);

  /* TODO: write no_prompt_auth boolean. */
  nsvn_gconf_write_config_entry(NSVN_NO_PROMPT_USER_GCONF_KEY,
                                NSVN_NO_PROMPT_USER_GCONF_SCHEMAS_KEY,
                                (void*)&(config->no_prompt_auth), gcc);

  /* TODO: write no_save_passwd boolean. */
  nsvn_gconf_write_config_entry(NSVN_NO_STORE_PASSWD_GCONF_KEY,
                                NSVN_NO_STORE_PASSWD_GCONF_SCHEMAS_KEY,
                                (void*)&(config->no_save_passwd), gcc);

  return 1;
}


void*
nsvn_gconf_read_config_entry (const char *key,
                              const char *schema,
                              GConfClient *gcc)
{
  GConfSchema *gcs;
  GConfValueType gct;
  GError *err=NULL;
  void *ret=NULL;
  gboolean *bval = NULL;

  if (gcc == NULL || key == NULL)
    return NULL;

  /* Check for the presense of schema, if not return fail. */
  /* Get key details from schema. */
  /* Check for the presense of dir, if not create it. */
  /* if already present update it with new value. */
  gcs = gconf_client_get_schema (gcc, schema, &err);
  if (!gcs) return NULL;

  gct = gconf_schema_get_type (gcs);
  switch (gct)
    {
      case GCONF_VALUE_STRING:
        ret = (void*) gconf_client_get_string (gcc, key, &err);
        break;
      case GCONF_VALUE_INT:
        break;
      case GCONF_VALUE_FLOAT:
        break;
      case GCONF_VALUE_BOOL:
        bval = (gboolean*)g_malloc0(sizeof(gboolean));
        *bval = gconf_client_get_bool (gcc, key, &err);
        ret = (void*)bval;
        break;
      case GCONF_VALUE_LIST:
        break;
      case GCONF_VALUE_PAIR:
        break;
      default:
        break;
    }

  return ret;
}


int
nsvn_gconf_write_config_entry (const char *key,
                               const char *schema,
                               void *value,
                               GConfClient *gcc)
{
  GConfSchema *gcs;
  GConfValueType gct;

  if (gcc == NULL || key == NULL)
    return 0;

  /* Check for the presense of schema, if not return fail. */
  /* Get key details from schema. */
  /* Check for the presense of dir, if not create it. */
  /* if already present update it with new value. */
  gcs = gconf_client_get_schema (gcc, schema, NULL);
  if (!gcs) return TRUE;

  gct = gconf_schema_get_type (gcs);
  switch (gct)
    {
      case GCONF_VALUE_STRING:
        gconf_client_set_string (gcc, key, (const char*)value, NULL);
        break;
      case GCONF_VALUE_INT:
        break;
      case GCONF_VALUE_FLOAT:
        break;
      case GCONF_VALUE_BOOL:
        gconf_client_set_bool (gcc, key, *((gboolean*)value), NULL);
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
