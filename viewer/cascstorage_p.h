#ifndef CASCSTORAGE_P_H
#define CASCSTORAGE_P_H

#include "../casclib/src/CascLib.h"
#include "../casclib/src/common/Common.h"

#include <QSharedData>
#include <QString>

class CascStoragePrivate : public QSharedData {
   public:
      HANDLE   hStorage;
      QString  lastError;
      QString  path;
      QMutex   Lock;

      CascStoragePrivate();
      ~CascStoragePrivate();

      bool openStorage(const QString& path);
      void closeStorage();
      QString translateError(int code);
};

#endif // CASCSTORAGE_P_H
