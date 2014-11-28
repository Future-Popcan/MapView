#ifndef DBCREADER_P_H
#define DBCREADER_P_H

#include "abstractdbcformat.h"

#include <QSharedData>
#include <QIODevice>

class DBCReaderPrivate : public QSharedData {
   public:
      QIODevice*        Device;
      AbstractDBCFormat Format;

      quint32           Records;
      quint32           RecordSize;
      quint32           FieldCount;
      quint32           CurrentIndex;
      QByteArray        StringBlock;

      DBCReaderPrivate();
      QString readString(qint64 offset);
};

#endif // DBCREADER_P_H
