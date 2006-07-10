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

/*
* TODO: Documentation for each function prototype need to be added
*       along with the the doxygen documentation system tags.
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

nsvn_t*
nsvn_base_setup_auth (nsvn_t *instance,
                      const char *username,
                      const char *password,
                      int non_interactive,
                      int no_auth_cache);

nsvn_t*
nsvn_base_setup_cancel (nsvn_t *instance,
                        void *cancel_func,
                        void *cancel_baton);

nsvn_t*
nsvn_base_setup_notify (nsvn_t *instance,
                        void *notify_func,
                        void *notify_baton);

nsvn_t*
nsvn_base_setup_log (nsvn_t *instance,
                     void *log_func,
                     void *log_baton);

nsvn_t*
nsvn_base_setup_progress (nsvn_t *instance,
                          void *progress_func,
                          void *progress_baton);
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
                 void *progress_baton);
#endif

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

int
nsvn_wc_commit (nsvn_t *instance,
                const char **target_list,
                int recurse,
                int keep_locks);

int
nsvn_wc_info (nsvn_t *instance,
              const char *path,
              const char *rev,
              void *callback,
              void *callback_data,
              int recurse);

int
nsvn_wc_status (nsvn_t *instance,
                const char *path,
                void *callback,
                void *callback_data,
                int recurse,
                int verbose,
                int update,
                int no_ignore,
                int ignore_externals);


/* Repository functions. */

int
nsvn_repos_checkout (nsvn_t *instance, const char *repos,
                     const char *path,
                     const char *rev,
                     int non_recurse,
                     int ign_external);

int
nsvn_repos_propset (nsvn_t *instance,
                    const char *url,
                    const char *propname,
                    const char *propval,
                    const char *rev,
                    int recurse);


/* General NaughtySVN functions. */

int
nsvn_common_parse_revision (nsvn_t *instance,
                            void *start_rev,
                            void *end_rev,
                            const char *rev_range);

#endif /* SVN_NAUGHTYSVN_H */
