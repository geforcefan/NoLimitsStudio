#ifndef TRACKBUILDER_GLOBAL_H
#define TRACKBUILDER_GLOBAL_H

#if defined(TRACKBUILDER_LIBRARY)
#  define TRACKBUILDER_EXPORT Q_DECL_EXPORT
#else
#  define TRACKBUILDER_EXPORT Q_DECL_IMPORT
#endif

#endif // TRACKBUILDER_GLOBAL_H
