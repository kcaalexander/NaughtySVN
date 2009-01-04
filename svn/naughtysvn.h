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

#include <svn_auth.h>
#include <svn_client.h>

typedef struct naughtysvn_t nsvn_t;

#define NSVN_FS_TYPE_BDB   SVN_FS_TYPE_BDB
#define NSVN_FS_TYPE_FSFS  SVN_FS_TYPE_FSFS

#ifndef NSVN_DEBUG
#define MSG_DEBUG(ARGS...) {;}
#define INFO(FORMAT, ARGS...) MSG_DEBUG("I:"FORMAT, ##ARGS)
#define WARN(FORMAT, ARGS...) MSG_DEBUG("W:"FORMAT, ##ARGS)
#define ERR(FORMAT, ARGS...) MSG_DEBUG("E:"FORMAT, ##ARGS)
#else
#define MSG_DEBUG(ARGS...) {fprintf(stderr, ##ARGS);}
#define INFO(FORMAT, ARGS...) MSG_DEBUG("I:"FORMAT, ##ARGS)
#define WARN(FORMAT, ARGS...) MSG_DEBUG("W:"FORMAT, ##ARGS)
#define ERR(FORMAT, ARGS...) MSG_DEBUG("E:"FORMAT, ##ARGS)
#endif /* NSVN_DEBUG */


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
nsvn_base_setup_cancel (nsvn_t *instance,
                        void *cancel_func,
                        void *cancel_baton);

nsvn_t*
nsvn_base_setup_notify (nsvn_t *instance,
                        void *notify_func,
                        void *notify_baton);

nsvn_t*
nsvn_base_setup_progress (nsvn_t *instance,
                          void *progress_func,
                          void *progress_baton);

nsvn_t*
nsvn_base_setup_log (nsvn_t *instance,
                     void *log_func,
                     void *log_baton);
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

void*
nsvn_base_get_aprpool (nsvn_t *instance);


/* Auth functions. */
int
nsvn_auth_register (nsvn_t *n,
                    const char *def_username,
                    const char *def_password,
                    int non_interactive,
                    int store_passwd,
                    int auth_cache,
                //  int store_passwd_as_plaintext,
                //  int store_ssl_cert_passphrase,
                //  int store_ssl_cert_passphrase_as_plaintext,
                    apr_array_header_t *providers);

int
nsvn_auth_get_simple_provider (nsvn_t *n,
                               apr_array_header_t **providers);

int
nsvn_auth_get_username_provider (nsvn_t *n,
                                 apr_array_header_t **providers);

int
nsvn_auth_get_ssl_server_trust_file_provider (nsvn_t *n,
                                              apr_array_header_t **providers);

int
nsvn_auth_get_ssl_client_cert_file_provider (nsvn_t *n,
                                             apr_array_header_t **providers);

int
nsvn_auth_get_ssl_client_cert_pw_file_provider (nsvn_t *n,
                                                apr_array_header_t **providers);

int
nsvn_auth_get_simple_prompt_provider (nsvn_t *n,
                                      svn_auth_simple_prompt_func_t prompt_func,
                                      int retry_limit,
                                      apr_array_header_t **providers);

int
nsvn_auth_get_username_prompt_provider (
                                    nsvn_t *n,
                                    svn_auth_username_prompt_func_t prompt_func,
                                    int retry_limit,
                                    apr_array_header_t **providers);

int
nsvn_auth_get_ssl_server_trust_prompt_provider (
                      nsvn_t *n,
                      svn_auth_ssl_server_trust_prompt_func_t prompt_func,
                      apr_array_header_t **providers);

int
nsvn_auth_get_ssl_client_cert_prompt_provider (
                     nsvn_t *n,
                     svn_auth_ssl_client_cert_prompt_func_t prompt_func,
                     int retry_limit,
                     apr_array_header_t **providers);

int
nsvn_auth_get_ssl_client_cert_pw_prompt_provider (
                  nsvn_t *n,
                  svn_auth_ssl_client_cert_pw_prompt_func_t prompt_func,
                  int retry_limit,
                  apr_array_header_t **providers);



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
nsvn_wc_log (nsvn_t *instance,
             const char **target_list,
             const char *start_rev,
             const char *end_rev,
             int limit,
             int discover_changed_paths,
             int strict_node_history,
             void *callback,
             void *callback_data);

int
nsvn_wc_info (nsvn_t *instance,
              const char *path,
              const char *rev,
              int recurse,
              void *callback,
              void *callback_data);

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


int
nsvn_wc_update (nsvn_t *instance,
                const char **paths,
                const char *rev_str,
                int recurse,
                int ignore_externals);


int
nsvn_wc_cleanup (nsvn_t *instance,
                 const char *dir);


int
nsvn_wc_switch (nsvn_t *instance,
                const char *paths,
                const char *url,
                const char *rev_str,
                int recurse,
                long int *result_rev);

int
nsvn_wc_mkdir (nsvn_t *instance,
               const char **paths);

int
nsvn_wc_blame (nsvn_t *instance,
               const char *path,
               const char *peg_rev,
               const char *start_rev,
               const char *end_rev,
               void *callback,
               void *callback_data);

int
nsvn_wc_copy (nsvn_t *instance,
              const char *src_path,
              const char *dst_path,
              const char *rev);

int
nsvn_wc_delete (nsvn_t *instance,
                const char **paths,
                int force);

int
nsvn_wc_move (nsvn_t *instance,
              const char *src_path,
              const char *dst_path,
              int force);


/* Repository functions. */

int
nsvn_repos_check_is_repospath (nsvn_t *n,
                               const char *repos_path);


const char*
nsvn_repos_get_repospath (nsvn_t *n,
                          const char *repos_path);


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
