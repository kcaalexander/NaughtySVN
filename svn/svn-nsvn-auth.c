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

#include "naughtysvn.h"
#include "svn-nsvn-types.h"


int
nsvn_auth_register (nsvn_t *n,
                    const char *def_username,
                    const char *def_password,
                    int non_interactive,
                    int store_passwd,
                    int auth_cache
//                  int store_passwd_as_plaintext,
//                  int store_ssl_cert_passphrase,
//                  int store_ssl_cert_passphrase_as_plaintext,
                   )
{
  if (n == NULL || n->auth_providers == NULL)
    return EXIT_FAILURE;

  svn_auth_open(&(n->ctx->auth_baton),
                (apr_array_header_t*) n->auth_providers,
                n->pool);

  if (def_username)
    svn_auth_set_parameter(n->ctx->auth_baton, SVN_AUTH_PARAM_DEFAULT_USERNAME,
                           def_username);

  if (def_password)
    svn_auth_set_parameter(n->ctx->auth_baton, SVN_AUTH_PARAM_DEFAULT_PASSWORD,
                           def_password);

  if (n->config_dir)
    svn_auth_set_parameter(n->ctx->auth_baton, SVN_AUTH_PARAM_CONFIG_DIR,
                           n->config_dir);

  if (non_interactive)
    svn_auth_set_parameter(n->ctx->auth_baton,
                           SVN_AUTH_PARAM_NON_INTERACTIVE, "");

  if (store_passwd)
    svn_auth_set_parameter(n->ctx->auth_baton,
                           SVN_AUTH_PARAM_DONT_STORE_PASSWORDS, "");

  if (!auth_cache || !store_passwd)
    svn_auth_set_parameter(n->ctx->auth_baton,
                           SVN_AUTH_PARAM_NO_AUTH_CACHE, "");

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_simple_provider (nsvn_t *n)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                        sizeof (svn_auth_provider_object_t *));

  svn_client_get_simple_provider(&provider, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_username_provider (nsvn_t *n)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                   sizeof (svn_auth_provider_object_t *));

  svn_client_get_username_provider (&provider, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_ssl_server_trust_file_provider (nsvn_t *n)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                   sizeof (svn_auth_provider_object_t *));

  svn_client_get_ssl_server_trust_file_provider (&provider, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_ssl_client_cert_file_provider (nsvn_t *n)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                        sizeof (svn_auth_provider_object_t *));

  svn_client_get_ssl_client_cert_file_provider (&provider, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_ssl_client_cert_pw_file_provider (nsvn_t *n)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                   sizeof (svn_auth_provider_object_t *));

  svn_client_get_ssl_client_cert_pw_file_provider (&provider, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_simple_prompt_provider (nsvn_t *n,
                                      svn_auth_simple_prompt_func_t prompt_func,
                                      int retry_limit)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                   sizeof (svn_auth_provider_object_t *));

  svn_client_get_simple_prompt_provider (&provider, prompt_func,
                                         n->ctx, retry_limit, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_username_prompt_provider (nsvn_t *n,
                                    svn_auth_username_prompt_func_t prompt_func,
                                    int retry_limit)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                   sizeof(svn_auth_provider_object_t *));

  svn_client_get_username_prompt_provider (&provider, prompt_func,
                                           n->ctx, retry_limit, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_ssl_server_trust_prompt_provider (nsvn_t *n,
                      svn_auth_ssl_server_trust_prompt_func_t prompt_func)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                   sizeof (svn_auth_provider_object_t *));

  svn_client_get_ssl_server_trust_prompt_provider (&provider, prompt_func,
                                                   n->ctx, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_ssl_client_cert_prompt_provider (nsvn_t *n,
                     svn_auth_ssl_client_cert_prompt_func_t prompt_func,
                     int retry_limit)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                   sizeof (svn_auth_provider_object_t *));

  svn_client_get_ssl_client_cert_prompt_provider (&provider, prompt_func,
                                                  n->ctx, retry_limit, n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}


int
nsvn_auth_get_ssl_client_cert_pw_prompt_provider (nsvn_t *n,
                  svn_auth_ssl_client_cert_pw_prompt_func_t prompt_func,
                  int retry_limit)
{
  svn_auth_provider_object_t *provider;

  if (n == NULL)
    return EXIT_FAILURE;

  if (n->auth_providers == NULL)
    n->auth_providers = apr_array_make (n->pool, 0,
                                   sizeof (svn_auth_provider_object_t *));

  svn_client_get_ssl_client_cert_pw_prompt_provider (&provider, prompt_func,
                                                     n->ctx, retry_limit,
                                                     n->pool);
  *(svn_auth_provider_object_t**)apr_array_push(n->auth_providers) = provider;

  return EXIT_SUCCESS;
}
