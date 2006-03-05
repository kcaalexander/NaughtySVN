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
#include "svn_wc.h"
#include "svn_version.h"
#include "svn_props.h"
#include "svn_time.h"

#include "svn_naughtysvn.h"
#include "svn-nsvn-types.h"

int
nsvn_wc_check_is_wcpath (nsvn_t *n,
                         const char *wc_path,
                         int *wc_format)
{
  nsvn_t *nsvn;
  int format;

  if (wc_path == NULL)
    return EXIT_SUCCESS;
 
  if (n == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) n;

  if (nsvn == NULL)
    return EXIT_FAILURE;

  nsvn->err = svn_wc_check_wc (svn_path_internal_style (wc_path,
                                 nsvn->pool),
                               &format, nsvn->pool);

  if (nsvn->err || format == 0)
    {
      if (n == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (wc_format != NULL)
    *wc_format = format;

  if (n == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}
