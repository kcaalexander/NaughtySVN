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

typedef struct naughtysvn_t nsvn_t;

#define NSVN_FS_TYPE_BDB   SVN_FS_TYPE_BDB
#define NSVN_FS_TYPE_FSFS  SVN_FS_TYPE_FSFS

/* Initilization and uninitization functions. */
nsvn_t*
nsvn_base_init (const char *config_path);

nsvn_t*
nsvn_base_uninit (nsvn_t *instance);

nsvn_t*
nsvn_base_reinit (nsvn_t *instance,
                  const char *config_dir);

nsvn_t*
nsvn_base_clear (nsvn_t *instance);


/* Repository admin functions. */

int 
nsvn_admin_create_repos (nsvn_t *instance,
                         int bdb_nosync,
                         int bdb_autorm,
                         const char *fs_type,
                         const char *config_dir,
                         const char *repos_path);


/* Working copy functions. */

int
nsvn_wc_check_is_wcpath (nsvn_t *instance,
                         const char *wc_path,
                         int *wc_format);

int
nsvn_wc_add (nsvn_t *instance,
             const char *path,
             int non_recurse,
             int force,
             int no_ignore,
             int autoprops,
             int no_autoprops);


int
nsvn_wc_propset (nsvn_t *instance,
                 const char *wc_path,
                 const char *propname,
                 const char *propval,
                 int recurse,
                 int skip_check);


/* Repository functions. */

int
nsvn_repos_checkout (nsvn_t *instance, const char *repos,
                     const char *path,
                     const char *rev,
                     int non_recurse,
                     int ign_external);


/* General NaughtySVN functions. */

int
nsvn_common_parse_revision (nsvn_t *instance,
                            void *start_rev,
                            void *end_rev,
                            const char *rev_range);

#endif /* SVN_NAUGHTYSVN_H */
