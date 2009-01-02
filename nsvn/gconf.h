/*
* ====================================================================
* Header file for gconf manupulation.
* Copyright (c) 2005 Alexander Thomas (AT) - NaughtySVN.
*
* 
* Leaving space here for Microsoft, to add license details.
*
*
* ====================================================================
*/
#ifndef NSVN_GCONF_H
#define NSVN_GCONF_H

#include <gconf/gconf-client.h>

typedef struct {
  const char *config_dir;
  const char *def_username;
  const char *def_passwd;
  gboolean no_cache_auth;
  gboolean no_prompt_auth;
  gboolean no_save_passwd;
} nsvn_config_t;

#define NSVN_GCONF_ROOT_KEY "/apps/naughtysvn"
#define NSVN_GCONF_SCHEMAS_ROOT_KEY "/schemas"NSVN_GCONF_ROOT_KEY

#define NSVN_SVN_CONFIG_DIR_GCONF_KEY NSVN_GCONF_ROOT_KEY"/svn_conf_dir"
#define NSVN_SVN_USERNAME_GCONF_KEY NSVN_GCONF_ROOT_KEY"/svn_user_name"
#define NSVN_SVN_PASSWORD_GCONF_KEY NSVN_GCONF_ROOT_KEY"/svn_user_password"
#define NSVN_NO_CACHE_AUTH_TOKEN_GCONF_KEY NSVN_GCONF_ROOT_KEY"/no_cache_auth_token"
#define NSVN_NO_PROMPT_USER_GCONF_KEY NSVN_GCONF_ROOT_KEY"/no_prompt_for_user"
#define NSVN_NO_STORE_PASSWD_GCONF_KEY NSVN_GCONF_ROOT_KEY"/no_store_passwd"
#define NSVN_LOG_TEMPL_DIR_GCONF_KEY NSVN_GCONF_ROOT_KEY"/log_templates"
#define NSVN_LOG_TEMPL_DEFAULT_DIR_GCONF_KEY NSVN_GCONF_ROOT_KEY"/log_templates/Default"
#define NSVN_LOG_TEMPL_DEFAULT_NAME_GCONF_KEY NSVN_GCONF_ROOT_KEY"/log_templates/Default/name"
#define NSVN_LOG_TEMPL_DEFAULT_MSG_GCONF_KEY NSVN_GCONF_ROOT_KEY"/log_templates/Default/message"
#define NSVN_LOG_TEMPL_CUSTOME_DIR_GCONF_KEY NSVN_GCONF_ROOT_KEY"/log_templates/%s"
#define NSVN_LOG_TEMPL_CUSTOME_NAME_GCONF_KEY NSVN_GCONF_ROOT_KEY"/log_templates/%s/name"
#define NSVN_LOG_TEMPL_CUSTOME_MSG_GCONF_KEY NSVN_GCONF_ROOT_KEY"/log_templates/%s/message"

#define NSVN_SVN_CONFIG_DIR_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/svn_conf_dir"
#define NSVN_SVN_USERNAME_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/svn_user_name"
#define NSVN_SVN_PASSWORD_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/svn_user_password"
#define NSVN_NO_CACHE_AUTH_TOKEN_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/no_cache_auth_token"
#define NSVN_NO_PROMPT_USER_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/no_prompt_for_user"
#define NSVN_NO_STORE_PASSWD_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/no_store_passwd"
#define NSVN_LOG_TEMPL_DIR_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/log_templates"
#define NSVN_LOG_TEMPL_DEFAULT_DIR_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/log_templates/Default"
#define NSVN_LOG_TEMPL_DEFAULT_NAME_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/log_templates/Default/name"
#define NSVN_LOG_TEMPL_DEFAULT_MSG_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/log_templates/Default/message"
#define NSVN_LOG_TEMPL_CUSTOME_DIR_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/log_templates/%s"
#define NSVN_LOG_TEMPL_CUSTOME_NAME_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/log_templates/%s/name"
#define NSVN_LOG_TEMPL_CUSTOME_MSG_GCONF_SCHEMAS_KEY NSVN_GCONF_SCHEMAS_ROOT_KEY"/log_templates/%s/message"


int
nsvn_gconf_create_config (nsvn_config_t *config);

int
nsvn_gconf_clean_config (nsvn_config_t *config);

int
nsvn_gconf_read_config (nsvn_config_t *config,
                        GConfClient *gcc);

int
nsvn_gconf_write_config (nsvn_config_t *config,
                         GConfClient *gcc);

void*
nsvn_gconf_read_config_entry (const char *key,
                              const char *schema,
                              GConfClient *gcc);

int
nsvn_gconf_write_config_entry (const char *key,
                               const char *schema,
                               void *value,
                               GConfClient *gcc);

int
nsvn_gconf_get_config (nsvn_config_t *config);

int
nsvn_gconf_set_config (nsvn_config_t *config);

#endif /* NSVN_GCONF_H */
