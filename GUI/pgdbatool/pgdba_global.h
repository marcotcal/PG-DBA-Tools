#ifndef PGDBA_GLOBAL_H
#define PGDBA_GLOBAL_H

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
#define ENDL Qt::endl
#else
#define ENDL endl
#endif

#endif // PGDBA_GLOBAL_H
