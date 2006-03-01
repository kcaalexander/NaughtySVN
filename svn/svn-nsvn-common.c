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

#include <stdlib.h>

int
nsvn_common_parse_revision (nsvn_t *n,
                            void *start_rev,
                            void *end_rev,
                            const char *rev_range)
{
  nsvn_t *nsvn;
  apr_pool_t *subpool;

  /* ### Replace this block with a macro.  Something like:
     nsvn = NSVN_ASSURE_INSTANCE(NULL);
  */
  if (n == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) n;

  if (nsvn == NULL || rev_range == NULL)
    return EXIT_FAILURE;

  subpool = svn_pool_create (nsvn->pool);

  if (svn_opt_parse_revision ((svn_opt_revision_t*) start_rev,
                              (svn_opt_revision_t*) end_rev,
                              rev_range, subpool) != 0)
    {
      if (start_rev)
        ((svn_opt_revision_t*) start_rev)->kind = svn_opt_revision_unspecified;

      if (end_rev)
        ((svn_opt_revision_t*) end_rev)->kind = svn_opt_revision_unspecified;
    }

  svn_pool_destroy (subpool);

  if (n == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}

