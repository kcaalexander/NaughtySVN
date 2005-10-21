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

void*
nsvn_base_init (const char *config_path);

void*
nsvn_base_uninit (void *instance);

void*
nsvn_base_reinit (void *instance,
                  const char *config_dir);

void*
nsvn_base_clear (void *instance);

int 
nsvn_admin_create_repos (void *n,
                         int bdb_nosync,
                         int bdb_autorm,
                         const char *fs_type,
                         const char *config_dir,
                         const char *repos_path);
int
nsvn_wc_check_is_wcpath (void *n,
                         const char *wc_path,
                         int *wc_format);

#endif /* SVN_NAUGHTYSVN_H */
