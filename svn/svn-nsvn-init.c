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

#include "naughtysvn.h"
#include "svn-nsvn-types.h"


static svn_error_t*
nsvn_base__check_svnlib_ver (void)
{
  static const svn_version_checklist_t checklist[] =
    {
      { "svn_subr",   svn_subr_version },
      { "svn_client", svn_client_version },
      { "svn_repos",  svn_repos_version },
      { "svn_fs",     svn_fs_version },
      { "svn_wc",     svn_wc_version },
      { "svn_ra",     svn_ra_version },
      { NULL, NULL }
    };

    NSVN_SVN_VER_REQUIRED(req_ver);
    return svn_ver_check_list(&req_ver, checklist);
}


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


nsvn_t*
nsvn_base_init (const char *config_dir)
{
  nsvn_t *nsvn;

  /* Initializing apr module. */
  if (nsvn_base__init_apr() == EXIT_FAILURE)
    return NULL;

  /* Allocating memory for NSVN internal data structure. */
  nsvn = (nsvn_t*) calloc (1, sizeof(nsvn_t));
  if (nsvn == NULL)
    return NULL;
    
  if (apr_allocator_create (&nsvn->allocator))
    return nsvn_base_uninit(nsvn);

  apr_allocator_max_free_set (nsvn->allocator,
                              SVN_ALLOCATOR_RECOMMENDED_MAX_FREE);
  nsvn->pool = svn_pool_create_ex (NULL,
                                   nsvn->allocator);
  apr_allocator_owner_set (nsvn->allocator, nsvn->pool);

  /* Check svn library version, which are used here */
  #ifdef NSVN_DEBUG
  nsvn->err = nsvn_base__check_svnlib_ver();
  if (nsvn->err)
    return nsvn_base_uninit (nsvn);
  #endif

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

  nsvn->cfg = apr_hash_get(nsvn->ctx->config, SVN_CONFIG_CATEGORY_CONFIG,
                           APR_HASH_KEY_STRING);

  return nsvn;
}


nsvn_t*
nsvn_base_uninit (nsvn_t *nsvn)
{
  if (nsvn)
    {
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

nsvn_t*
nsvn_base_reinit (nsvn_t *nsvn, const char *config_dir)
{
  nsvn_base_uninit (nsvn);
   
  return (nsvn_base_init (config_dir));
}

nsvn_t*
nsvn_base_clear (nsvn_t *nsvn)
{
  if (nsvn)
    {
      svn_error_clear (nsvn->err);
      apr_pool_clear (nsvn->pool);
    }

  return nsvn;
}

nsvn_t*
nsvn_base_setup_auth (nsvn_t *instance,
                      const char *username,
                      const char *password,
                      int non_interactive,
                      int no_auth_cache)
{
  if (!instance)
    return NULL;

  instance->err = svn_cmdline_setup_auth_baton(
                     &instance->ctx->auth_baton,
                     non_interactive, username,
                     password, instance->config_dir,
                     no_auth_cache, instance->cfg,
                     instance->ctx->cancel_func,
                     instance->ctx->cancel_baton,
                     instance->pool);
  if (instance->err)
    instance->ctx->auth_baton = NULL;

  return instance;
}

nsvn_t*
nsvn_base_setup_cancel (nsvn_t *instance,
                        //svn_cancel_func_t cancel_func,
                        void *cancel_func,
                        void *cancel_baton)
{
  if (!instance)
    return NULL;
  instance->ctx->cancel_func = cancel_func;
  instance->ctx->cancel_baton = cancel_baton;

  return instance;
}

nsvn_t*
nsvn_base_setup_notify (nsvn_t *instance,
                        //svn_wc_notify_func2_t notify_func,
                        void *notify_func,
                        void *notify_baton)
{
  if (!instance)
    return NULL;
  instance->ctx->notify_func2 = notify_func;
  instance->ctx->notify_baton2 = notify_baton;

  return instance;
}

nsvn_t*
nsvn_base_setup_log (nsvn_t *instance,
                     //svn_client_get_commit_log2_t log_func,
                     void *log_func,
                     void *log_baton)
{
  if (!instance)
    return NULL;
  instance->ctx->log_msg_func2 = log_func;
  instance->ctx->log_msg_baton2 = log_baton;

  return instance;
}

nsvn_t*
nsvn_base_setup_progress (nsvn_t *instance,
                          //svn_ra_progress_notify_func_t progress_func,
                          void *progress_func,
                          void *progress_baton)
{
  if (!instance)
    return NULL;
  instance->ctx->progress_func = progress_func;
  instance->ctx->progress_baton = progress_baton;

  return instance;
}

#if 0
nsvn_t*
nsvn_base_setup (nsvn_t *instance,
                 const char *user_name,
                 const char *password,
                 int non_interactive,
                 int no_auth_cache,
                 svn_cancel_func_t cancel_func,
                 void *cancel_baton,
                 svn_wc_notify_func2_t notify_func,
                 void *notify_baton,
                 svn_client_get_commit_log2_t log_func,
                 void *log_baton,
                 svn_ra_progress_notify_func_t progress_func,
                 void *progress_baton)
{

  return instance;
}
#endif
