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

