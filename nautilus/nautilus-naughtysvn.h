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

#ifndef NAUTILUS_NAUGHTYSVN_H
#define NAUTILUS_NAUGHTYSVN_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NAUTILUS_TYPE_NSVN  (nautilus_nsvn_get_type ())
#define NAUTILUS_NSVN(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), NAUTILUS_TYPE_NSVN, NautilusNSVN))
#define NAUTILUS_IS_NSVN(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), NAUTILUS_TYPE_NSVN))

typedef struct _NautilusNSVN      NautilusNSVN;
typedef struct _NautilusNSVNClass NautilusNSVNClass;

struct _NautilusNSVN
  {
    GObject __parent;
  };

struct _NautilusNSVNClass
  {
    GObjectClass __parent;
  };

GType nautilus_nsvn_get_type      (void);
void  nautilus_nsvn_register_type (GTypeModule *module);

G_END_DECLS

#endif /* NAUTILUS_NAUGHTYSVN_H */

