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

#ifndef SVN_NSVN_PRIVATE_H
#define SVN_NSVN_PRIVATE_H
#include "apr_pools.h"
#include "svn_error.h"
#include "svn_client.h"
#include "svn_config.h"

typedef struct naughtysvn_t {
  svn_error_t *err;
  apr_status_t apr_err;
  apr_allocator_t *allocator;
  apr_pool_t *pool;
  svn_client_ctx_t *ctx;

  const char *config_dir;
  svn_config_t *cfg;

} naughtysvn_t;

#define NSVN_ERR (err) SVN_ERR (err)


#endif /* SVN_NSVN_PRIVATE_H */
