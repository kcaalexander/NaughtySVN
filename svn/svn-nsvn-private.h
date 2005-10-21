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
#include <apr_pools.h>
#include <svn_pools.h>
#include <svn_error.h>

typedef struct naughtysvn_t {
  svn_error_t *err;
  apr_status_t apr_err;
  apr_allocator_t *allocator;
  apr_pool_t *pool;

  const char *config_dir;

} naughtysvn_t;


#endif /* SVN_NSVN_PRIVATE_H */
