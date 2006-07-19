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

#define NAUTILUS_NSVN_TYPE  (nautilus_nsvn_get_type ())
#define NAUTILUS_NSVN(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), NAUTILUS_NSVN_TYPE, NautilusNSVN))
#define NAUTILUS_IS_NSVN(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), NAUTILUS_NSVN_TYPE))

#define NAUTILUS_NSVN_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), NAUTILUS_NSVN_TYPE, NautilusActionsClass))
#define NAUTILUS_IS_NSVN_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), NAUTILUS_NSVN_TYPE))
#define NAUTILUS_NSVN_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((obj), NAUTILUS_NSVN_TYPE, NautilusActionsClass))

#define NAUTILUS_NSVN_PROP_TYPE  (nautilus_nsvn_get_prop_type ())

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

GType nautilus_nsvn_get_prop_type (void);
void nautilus_nsvn_prop_register_type (GTypeModule *module);

G_END_DECLS

#endif /* NAUTILUS_NAUGHTYSVN_H */

