/*
* ====================================================================
* File contains exportable functions to administor Subversion.
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

#include <stdlib.h>
int
nsvn_admin_create_repos (void *n, int bdb_nosync,
                         int bdb_autorm,
                         const char *fs_type,
                         const char *config_dir,
                         const char *repos_path)
{
   svn_repos_t *repos;
   apr_hash_t *config;
   naughtysvn_t *nsvn;

   if (n == NULL)
     nsvn = nsvn_base_init (NULL);
   else
     nsvn = (naughtysvn_t*) n;

   if (nsvn == NULL)
     return EXIT_FAILURE;
     
   apr_hash_t *fs_config = apr_hash_make (nsvn->pool);

   apr_hash_set (fs_config, SVN_FS_CONFIG_BDB_TXN_NOSYNC,
                 APR_HASH_KEY_STRING, (bdb_nosync ? "1" : "0"));

   apr_hash_set (fs_config, SVN_FS_CONFIG_BDB_LOG_AUTOREMOVE,
                 APR_HASH_KEY_STRING, (bdb_autorm ? "1" : "0"));

   apr_hash_set (fs_config, SVN_FS_CONFIG_FS_TYPE,
                 APR_HASH_KEY_STRING, fs_type);

   nsvn->err = svn_config_get_config (&config, nsvn->config_dir,
                                      nsvn->pool);
   if (nsvn->err)
     {
       if (n == NULL)
         nsvn = nsvn_base_uninit (nsvn);
       return EXIT_FAILURE;
     }


   nsvn->err = svn_repos_create (&repos,
                                 repos_path,
                                 NULL, NULL,
                                 config, fs_config, nsvn->pool);

   if (nsvn->err)
     {
       if (n == NULL)
         nsvn = nsvn_base_uninit (nsvn);
       return EXIT_FAILURE;
     }

   /* TODO: Not sure about hooking warning function to fs, like svn_client do. */

   if (n == NULL)
     nsvn = nsvn_base_uninit (nsvn);

   return EXIT_SUCCESS;
}

