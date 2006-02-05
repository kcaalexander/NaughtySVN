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

#ifndef SVN_NAUGHTYSVN_H
#define SVN_NAUGHTYSVN_H

/* Initilization and Uninitization functions. */

void*
nsvn_base_init (const char *config_path);

void*
nsvn_base_uninit (void *instance);

void*
nsvn_base_reinit (void *instance,
                  const char *config_dir);

void*
nsvn_base_clear (void *instance);


/* Repository admin functions. */

int 
nsvn_admin_create_repos (void *instance,
                         int bdb_nosync,
                         int bdb_autorm,
                         const char *fs_type,
                         const char *config_dir,
                         const char *repos_path);


/* Working copy functions. */

int
nsvn_wc_check_is_wcpath (void *instance,
                         const char *wc_path,
                         int *wc_format);


/* Repository functions. */

int
nsvn_repos_checkout (void *n, const char *repos,
                     const char *path,
                     const char *pegrev,
                     const char *rev,
                     int non_recurse,
                     int ign_external);
/* General NaughtySVN functions. */

int
nsvn_common_parse_revision (void *n,
                            void *start_rev,
                            void *end_rev,
                            const char *rev_range);

#endif /* SVN_NAUGHTYSVN_H */
