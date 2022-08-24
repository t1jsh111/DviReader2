#ifndef DVIINTERPRETER_GLOBAL_H
#define DVIINTERPRETER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DVIINTERPRETER_LIBRARY)
#  define DVIINTERPRETER_EXPORT Q_DECL_EXPORT
#else
#  define DVIINTERPRETER_EXPORT Q_DECL_IMPORT
#endif

#endif // DVIINTERPRETER_GLOBAL_H