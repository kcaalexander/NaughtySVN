/*
* ====================================================================
* Header file for NaughtySVN add dialog box.
* Copyright (c) 2005, All rights reserved for Alexander Thomas (AT)
*
* 
* Leaving space here for Microsoft, to add license details.
*
*
* ====================================================================
*/

#ifndef NSVN_DLG_AUTH_H
#define NSVN_DLG_AUTH_H

#include <svn_error.h>
#include <svn_auth.h>
#include <svn_error_codes.h>

#include "gconf.h"
#include "svn/naughtysvn.h"

/* this should be compatible with svn_cmdline_auth_simple_prompt */
extern svn_error_t *
nsvn_auth_simple_prompt (svn_auth_cred_simple_t **cred_p,
                         void *baton,
                         const char *realm,
                         const char *username,
                         svn_boolean_t may_save,
                         apr_pool_t *pool);

svn_error_t *
nsvn_auth_username_prompt (svn_auth_cred_username_t **cred_p,
                           void *baton,
                           const char *realm,
                           svn_boolean_t may_save,
                           apr_pool_t *pool);

svn_error_t *
nsvn_auth_ssl_server_trust_prompt (svn_auth_cred_ssl_server_trust_t **cred_p,
                                   void *baton,
                                   const char *realm,
                                   apr_uint32_t failures,
                                   const svn_auth_ssl_server_cert_info_t *cert_info,
                                   svn_boolean_t may_save, apr_pool_t *pool);

svn_error_t *
nsvn_auth_ssl_client_cert_prompt (svn_auth_cred_ssl_client_cert_t **cred_p,
                                  void *baton,
                                  const char *realm,
                                  svn_boolean_t may_save,
                                  apr_pool_t *pool);

svn_error_t *
nsvn_auth_ssl_client_cert_pw_prompt (svn_auth_cred_ssl_client_cert_pw_t **cred_p,
                                     void *baton,
                                     const char *realm,
                                     svn_boolean_t may_save,
                                     apr_pool_t *pool);

gboolean
nsvn_setup_auth(nsvn_t *nsvn, nsvn_config_t *config);

#endif /* NSVN_DLG_ADD_H */
