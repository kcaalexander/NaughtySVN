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

#include "naughtysvn.h"
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


int
nsvn_wc_add (nsvn_t *instance,
             const char *path,
             int nonrecurse,
             int force,
             int no_ignore,
             int autoprops,
             int no_autoprops)
{
  nsvn_t *nsvn;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) instance;

  if (nsvn == NULL)
    return EXIT_FAILURE;

  nsvn->err = svn_client_add3 (path, (! nonrecurse),
                               force, no_ignore,
                               nsvn->ctx, nsvn->pool);

  if (nsvn->err)
    {
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}


int
nsvn_wc_propset (nsvn_t *instance,
                 const char *wc_path,
                 const char *propname,
                 const char *propval,
                 int recurse,
                 int skip_check)
{
  nsvn_t *nsvn;
  const svn_string_t *svn_propval = NULL;
  const char *propval_utf8;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = instance;

  if (nsvn == NULL)
    return EXIT_FAILURE;

  nsvn->err = svn_utf_cstring_to_utf8 (&propval_utf8, propval,
                                       nsvn->pool);
  if (nsvn->err)
    {
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (propval)
    svn_propval = svn_string_create (propval_utf8, nsvn->pool);

  nsvn->err = svn_client_propset2 (propname, svn_propval, wc_path,
                                   recurse, skip_check, nsvn->ctx,
                                   nsvn->pool);

  if (nsvn->err)
    {
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}


int
nsvn_wc_commit (nsvn_t *instance,
                const char **target_list,
                int recurse,
                int keep_locks)
{
  nsvn_t *nsvn;
  unsigned int idx = 0;
  apr_array_header_t *targets = NULL;
  svn_commit_info_t *commit_info = NULL;
  const char *path;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = instance;

  if (nsvn == NULL)
    return EXIT_FAILURE;

  targets = apr_array_make(nsvn->pool, 0, sizeof(const char*));
  path = target_list[idx];

  while (path != NULL)
    {
      *(const char**)apr_array_push(targets) = apr_pstrdup(nsvn->pool, path);
      path = target_list[++idx];
    }

  nsvn->err = svn_client_commit3 (&commit_info,
                                  targets, recurse,
                                  keep_locks,
                                  nsvn->ctx,
                                  nsvn->pool);

  if (nsvn->err)
    {
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int
nsvn_wc_info (nsvn_t *instance,
              const char *path,
              const char *rev,
              void *callback,
              void *callback_data,
              int recurse)
{
  nsvn_t *nsvn;
  svn_opt_revision_t peg_rev;
  svn_opt_revision_t revision;
  const char *truepath;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = instance;

  nsvn_common_parse_revision (nsvn, &revision, NULL, rev ? rev : "HEAD");
  nsvn->err = svn_opt_parse_path (&peg_rev, &truepath, path, nsvn->pool);

  if (!nsvn->err)
    {
      if ((svn_path_is_url(path))
          && (peg_rev.kind == svn_opt_revision_unspecified))
        peg_rev.kind = svn_opt_revision_head;

      nsvn->err = svn_client_info (path, &peg_rev, &revision,
                                   callback, callback_data,
                                   recurse, nsvn->ctx, nsvn->pool);
    }

  if (nsvn->err)
    {
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}


int
nsvn_wc_status (nsvn_t *instance,
                const char *path,
                void *callback,
                void *callback_data,
                int recurse,
                int verbose,
                int update,
                int no_ignore,
                int ignore_externals)
{
  nsvn_t *nsvn;
  svn_opt_revision_t rev;
  svn_revnum_t repos_rev = SVN_INVALID_REVNUM;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = instance;

  rev.kind = svn_opt_revision_head;

  nsvn->err = svn_client_status2 (&repos_rev, path, &rev,
                                  callback, callback_data,
                                  recurse, verbose, update,
                                  no_ignore, ignore_externals,
                                  nsvn->ctx, nsvn->pool);
  if (nsvn->err)
    {
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}

/* Makes the update operation */
/* FIXME: currently doesn't update to version
 * and uses some assumptions, see below */
int
nsvn_wc_update (nsvn_t *instance,
                const char **paths,
                const char *rev,
                void *callback,
                void *callback_data,
                int recurse,
                int ignore_externals,
                const char *mergetool,
                char *username,
                char *password)
{
  nsvn_t *nsvn;
  apr_array_header_t **result_revs;
  svn_opt_revision_t svn_opt_head_rev; /* FIXME: drop this */

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = instance;

  if (nsvn->err)
    {
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  /* FIXME: paths needs to be converted to apr_array_header_t type */

  /* FIXME: the revision information must be passed to the client
   * for now we always update to head revision */
  /* for now we don't care about the revisions we updated to */
  result_revs = NULL;
  svn_opt_head_rev.kind = svn_opt_revision_head ;
  nsvn->err = svn_client_update2 (result_revs,
      apr_paths,
      &svn_opt_head_rev,
      (svn_boolean_t) recurse,
      (svn_boolean_t) ignore_externals,
      nsvn->ctx, nsvn->pool);

  if (nsvn->err != SVN_NO_ERROR )
    {
      MSG_DEBUG("update: %d", nsvn->err);
      return EXIT_FAILURE;
    };

  return EXIT_SUCCESS;

}

/*
 * vim: ts=2 : sw=2
 */
