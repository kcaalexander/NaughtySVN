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

//#include <apr_file_io.h>
//#include <apr_signal.h>

#include "svn_path.h"
#include "svn_repos.h"

#include "naughtysvn.h"
#include "svn-nsvn-types.h"

//#include <stdlib.h>

int
nsvn_repos_check_is_repospath (nsvn_t *n,
                               const char *repos_path)
{
  nsvn_t *nsvn;
  const char *root;

  if (repos_path == NULL)
    return EXIT_SUCCESS;

  if (n == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) n;

  if (nsvn == NULL)
    return EXIT_FAILURE;

  root = svn_repos_find_root_path (repos_path, nsvn->pool);

  if (n == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  if (root)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}


const char*
nsvn_repos_get_repospath (nsvn_t *n,
                          const char *repos_path)
{
  nsvn_t *nsvn;
  const char *root;

  if (repos_path == NULL)
    return NULL;

  if (n == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) n;

  if (nsvn == NULL)
    return NULL;

  root = svn_repos_find_root_path (repos_path, nsvn->pool);

  if (n == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return root;
}


int
nsvn_repos_checkout (nsvn_t *n, const char *repos,
                     const char *path,
                     const char *rev_str,
                     int non_recurse,
                     int ign_external)
{
  nsvn_t *nsvn;
  svn_revnum_t result_rev;
  svn_opt_revision_t peg_rev;
  svn_opt_revision_t revision;
  const char *url;

  if (n == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) n;

  if (nsvn == NULL)
    return EXIT_FAILURE;

  nsvn_common_parse_revision (n, &revision, NULL, rev_str ? rev_str : "HEAD");

  svn_opt_parse_path (&peg_rev, &url, repos, nsvn->pool);
  url = svn_path_canonicalize (url, nsvn->pool);

  svn_client_checkout2 (&result_rev, url, path,
                        &peg_rev, &revision,
                        non_recurse ? FALSE : TRUE,
                        ign_external ? FALSE : TRUE,
                        nsvn->ctx, nsvn->pool);

  /* TODO: Not sure about hooking warning function to fs, like svn_client do. */

  if (n == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}


int
nsvn_repos_lock (nsvn_t *instance,
                 const char **paths,
                 const char *comment,
                 int steal_lock)
{
  nsvn_t *nsvn;
  apr_array_header_t *apr_paths = NULL;
  const char *path;
  unsigned int idx = 0;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) instance;

  /* Converting PATHS to apr_array_header_t type. */
  apr_paths = apr_array_make(nsvn->pool, 0, sizeof(const char*));
  path = paths[idx];

  while (path != NULL)
    {
      *(const char**)apr_array_push(apr_paths) = apr_pstrdup(nsvn->pool, path);
      path = paths[++idx];
    }

  nsvn->err = svn_client_lock (apr_paths, comment, steal_lock,
                               nsvn->ctx, nsvn->pool);

  if (nsvn->err != SVN_NO_ERROR )
    {
      MSG_DEBUG("Lock operation failed ...");
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}


int
nsvn_repos_unlock (nsvn_t *instance,
                   const char **paths,
                   int break_lock)
{
  nsvn_t *nsvn;
  apr_array_header_t *apr_paths = NULL;
  const char *path;
  unsigned int idx = 0;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) instance;

  /* Converting PATHS to apr_array_header_t type. */
  apr_paths = apr_array_make(nsvn->pool, 0, sizeof(const char*));
  path = paths[idx];

  while (path != NULL)
    {
      *(const char**)apr_array_push(apr_paths) = apr_pstrdup(nsvn->pool, path);
      path = paths[++idx];
    }

  nsvn->err = svn_client_unlock (apr_paths, break_lock,
                                 nsvn->ctx, nsvn->pool);

  if (nsvn->err != SVN_NO_ERROR )
    {
      MSG_DEBUG("Unlock operation failed ...");
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    }

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}


int
nsvn_repos_import (nsvn_t *instance,
                   const char *path,
                   const char *url,
                   int norecurse,
                   int noignore)
{
  nsvn_t *nsvn;
  svn_commit_info_t *commit_info = NULL;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) instance;

  if (path == NULL || url == NULL)
    return EXIT_SUCCESS;

  //FIXME: Should I look for commit message it a URL is given.
  //
  nsvn->err = svn_client_import2 (&commit_info, path, url,
                                  norecurse, noignore,
                                  nsvn->ctx, nsvn->pool);

  if (nsvn->err != SVN_NO_ERROR )
    {
      MSG_DEBUG("Import operation failed ...");
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    };

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}


int
nsvn_repos_export (nsvn_t *instance,
                   const char *from,
                   const char *to,
                   const char *peg_rev,
                   const char *export_rev,
                   int overwrite,
                   int ignore_externals,
                   int recurse,
                   const char *native_eol)
{
  nsvn_t *nsvn;
  svn_revnum_t repos_rev = SVN_INVALID_REVNUM;
  svn_opt_revision_t peg_revision;
  svn_opt_revision_t revision;

  if (instance == NULL)
    nsvn = nsvn_base_init (NULL);
  else
    nsvn = (nsvn_t*) instance;

  if (from == NULL || to == NULL)
    return EXIT_SUCCESS;

  nsvn_common_parse_revision (nsvn, &revision, NULL,
                              export_rev ? export_rev : "HEAD");
  nsvn_common_parse_revision (nsvn, &peg_revision, NULL,
                              peg_rev ? peg_rev : "HEAD");

  nsvn->err = svn_client_export3 (&repos_rev, from, to,
                                  &peg_revision, &revision,
                                  overwrite, ignore_externals,
                                  recurse, native_eol,
                                  nsvn->ctx, nsvn->pool);

  if (nsvn->err != SVN_NO_ERROR )
    {
      MSG_DEBUG("Export operation failed ...");
      if (instance == NULL)
        nsvn = nsvn_base_uninit (nsvn);
      return EXIT_FAILURE;
    };

  if (instance == NULL)
    nsvn = nsvn_base_uninit (nsvn);

  return EXIT_SUCCESS;
}
