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

#include <gconf/gconf-client.h>

#define NSVN_SVN_CONFIG_DIR_KEY "/apps/naughtysvn/svn_conf_dir"
#define NSVN_SVN_USERNAME_KEY "/apps/naughtysvn/svn_user_name"
#define NSVN_SVN_PASSWORD_KEY "/apps/naughtysvn/svn_user_password"
#define NSVN_CACHE_AUTH_TOKEN_KEY "/apps/naughtysvn/cache_auth_token"
#define NSVN_PROMPT_USER_KEY "/apps/naughtysvn/prompt_for_user"
#define NSVN_LOG_TEMPL_DIR_KEY "/apps/naughtysvn/log_templates"
#define NSVN_LOG_TEMPL_DEFAULT_DIR_KEY "/apps/naughtysvn/log_templates/Default"
#define NSVN_LOG_TEMPL_DEFAULT_NAME_KEY "/apps/naughtysvn/log_templates/Default/name"
#define NSVN_LOG_TEMPL_DEFAULT_MSG_KEY "/apps/naughtysvn/log_templates/Default/message"
#define NSVN_LOG_TEMPL_CUSTOME_DIR_KEY "/apps/naughtysvn/log_templates/%s"
#define NSVN_LOG_TEMPL_CUSTOME_NAME_KEY "/apps/naughtysvn/log_templates/%s/name"
#define NSVN_LOG_TEMPL_CUSTOME_MSG_KEY "/apps/naughtysvn/log_templates/%s/message"


int
nsvn_gconf_write_entry (const char *key,
                        void *value,
                        GConfClient *gcc);
