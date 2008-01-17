/*
* ====================================================================
* File contains exportable functions to handle and subversion
* working copy.
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

#include "svn_auth.h"
#include "svn_config.h"
#include "svn_error.h"
#include "svn_pools.h"
#include "svn_types.h"

#define NEED_svn_gtk_setup_auth_baton
#include "naughtysvn.h"
#include "dlg_auth.h"

svn_error_t *
svn_gtk_setup_auth_baton (svn_auth_baton_t **ab,
                          const char *auth_username,
                          const char *auth_password,
                          const char *config_dir,
                          svn_boolean_t no_auth_cache,
                          svn_config_t *cfg,
                          apr_pool_t *pool)
{
  svn_boolean_t store_password_val = TRUE;
  svn_auth_provider_object_t *provider;

  apr_array_header_t *providers = apr_array_make(pool, 12, sizeof(svn_auth_provider_object_t *));

#ifdef WIN32
  svn_auth_get_windows_simple_provider(&provider, pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;
#endif
#ifdef SVN_HAVE_KEYCHAIN_SERVICES
  svn_auth_get_keychain_simple_provider(&provider, pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;
#endif
  svn_auth_get_simple_provider(&provider, pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;
  svn_auth_get_username_provider(&provider, pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;

  /* The server-cert, client-cert, and client-cert-password providers. */
  svn_auth_get_ssl_server_trust_file_provider(&provider, pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;
  svn_auth_get_ssl_client_cert_file_provider(&provider, pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;
  svn_auth_get_ssl_client_cert_pw_file_provider(&provider, pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;

  /* Two basic prompt providers: username/password, and just username. */
  svn_auth_get_simple_prompt_provider(&provider,
                                      svn_gtk_auth_simple_prompt,
                                      NULL /*pb*/,
                                      2, /* retry limit */
                                      pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;

  svn_auth_get_username_prompt_provider(&provider,
                                        svn_gtk_auth_username_prompt,
                                        NULL /*pb*/,
                                        2, /* retry limit */
                                        pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;

  /* Three ssl prompt providers, for server-certs, client-certs,
     and client-cert-passphrases.  */

  svn_auth_get_ssl_server_trust_prompt_provider(&provider,
                                                svn_gtk_auth_ssl_server_trust_prompt,
                                                NULL /*pb*/,
                                                pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;

  svn_auth_get_ssl_client_cert_prompt_provider(&provider,
                                               svn_gtk_auth_ssl_client_cert_prompt,
                                               NULL /*pb*/,
                                               2,
                                               pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;

  svn_auth_get_ssl_client_cert_pw_prompt_provider(&provider,
                                                  svn_gtk_auth_ssl_client_cert_pw_prompt,
                                                  NULL /*pb*/,
                                                  2,
                                                  pool);
  APR_ARRAY_PUSH(providers, svn_auth_provider_object_t *) = provider;

  /* Build an authentication baton to give to libsvn_client. */
  svn_auth_open(ab, providers, pool);

  /* Place any default --username or --password credentials into the 
     auth_baton's run-time parameter hash. */

  if (auth_username)
    svn_auth_set_parameter(*ab,
                           SVN_AUTH_PARAM_DEFAULT_USERNAME,
                           auth_username);
                           
  if (auth_password)
    svn_auth_set_parameter(*ab,
                           SVN_AUTH_PARAM_DEFAULT_PASSWORD,
                           auth_password);

  /* Same with the --non-interactive option. */

  if (config_dir)
    svn_auth_set_parameter(*ab,
                           SVN_AUTH_PARAM_CONFIG_DIR,
                           config_dir);

  SVN_ERR(svn_config_get_bool(cfg,
                              &store_password_val,
                              SVN_CONFIG_SECTION_AUTH,
                              SVN_CONFIG_OPTION_STORE_PASSWORDS,
                              TRUE));

  if (! store_password_val)
    svn_auth_set_parameter(*ab, SVN_AUTH_PARAM_DONT_STORE_PASSWORDS, "");

  /* There are two different ways the user can disable disk caching
     of credentials:  either via --no-auth-cache, or in the config
     file ('store-auth-creds = no'). */

  SVN_ERR(svn_config_get_bool(cfg,
                              &store_password_val,
                              SVN_CONFIG_SECTION_AUTH,
                              SVN_CONFIG_OPTION_STORE_AUTH_CREDS,
                              TRUE));

  if (no_auth_cache || ! store_password_val)
      svn_auth_set_parameter(*ab, SVN_AUTH_PARAM_NO_AUTH_CACHE, "");

  return SVN_NO_ERROR;
}
