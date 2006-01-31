/*
* ====================================================================
* File contain exportable functions to initialize and deinitialize 
* svn module.
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

#include <apr_file_io.h>
#include <apr_signal.h>

#include "svn_pools.h"
#include "svn_cmdline.h"
#include "svn_error.h"
#include "svn_opt.h"
#include "svn_utf.h"
#include "svn_subst.h"
#include "svn_path.h"
#include "svn_config.h"
#include "svn_repos.h"
#include "svn_fs.h"
#include "svn_version.h"
#include "svn_props.h"
#include "svn_time.h"

#include "svn_naughtysvn.h"
#include "svn-nsvn-private.h"

static int
nsvn_base__init_apr (void)
{
  apr_status_t status;

  /* Setting up APR internal data structures. */
  status = apr_initialize ();
  if (status)
    return EXIT_FAILURE;

  /* Registering to uninitialize APR internal data structure. */
  if (atexit (apr_terminate) != 0)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}


void*
nsvn_base_init (const char *config_dir)
{
  naughtysvn_t *nsvn;

  /* Initializing apr module. */
  if (nsvn_base__init_apr() == EXIT_FAILURE)
    return NULL;

  /* Allocating memory for NSVN internal data structure. */
  nsvn = (naughtysvn_t*) calloc (1, sizeof(naughtysvn_t));
  if (nsvn == NULL)
    return NULL;
    
  if (apr_allocator_create (&nsvn->allocator))
    return nsvn_base_uninit(nsvn);

  apr_allocator_max_free_set (nsvn->allocator,
                              SVN_ALLOCATOR_RECOMMENDED_MAX_FREE);
  nsvn->pool = svn_pool_create_ex (NULL,
                                   nsvn->allocator);
  apr_allocator_owner_set (nsvn->allocator, nsvn->pool);

  /* TODO: Check svn library version, which are used here */

  /* Initializing the SVN FS library. */
  nsvn->err = svn_fs_initialize (nsvn->pool);
  if (nsvn->err)
    return nsvn_base_uninit (nsvn); 

  /* TODO: Need to ignore SIGPIPE and hook SIGINT, SIGHUP, SIGTERM, and
     SIGUSR1 signals to a handler which will set a global volatile varaible
     to TRUE. So by looking at the variable we can decide to cancel a on
     going process. Also we need to write a exportable function which is
     same as the handler. */

  /* TODO: Need to decide, whether to check config_dir is a valid subversion
     config directory or not, if check needed add it here. */

  if (config_dir)
    nsvn->config_dir = strdup (config_dir);
  else
    nsvn->config_dir = NULL;

  /* Creating Subversion client context object. */
  nsvn->err = svn_client_create_context (&(nsvn->ctx), nsvn->pool);
  if (nsvn->err)
    return nsvn_base_uninit (nsvn);

  nsvn->err = svn_config_get_config (&(nsvn->ctx->config),
                                     nsvn->config_dir, nsvn->pool);
  if (nsvn->err)
    return nsvn_base_uninit (nsvn);

  return (void*) nsvn;
}


void*
nsvn_base_uninit (void *n)
{
  naughtysvn_t *nsvn;
  if (n)
    {
      nsvn = (naughtysvn_t*) n;
      if (nsvn->config_dir)
        free ((void*)nsvn->config_dir);
       
      if (nsvn->err)
        svn_error_clear (nsvn->err);

      apr_pool_destroy (nsvn->pool);
      nsvn->pool = NULL;
      
//    apr_allocator_destroy (nsvn->allocator);
//    nsvn->allocator=NULL;
     
      free (nsvn);
    }

  return NULL;
}

void*
nsvn_base_reinit (void *n, const char *config_dir)
{
  nsvn_base_uninit (n);
   
  return (nsvn_base_init (config_dir));
}

void*
nsvn_base_clear (void *n)
{
  naughtysvn_t *nsvn = (naughtysvn_t*) n;

  if (nsvn)
    {
      svn_error_clear (nsvn->err);
      apr_pool_clear (nsvn->pool);
    }

  return n;
}
