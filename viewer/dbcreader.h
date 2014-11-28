#ifndef DBCREADER_H
#define DBCREADER_H

#include <QIODevice>
#include <QExplicitlySharedDataPointer>

class AbstractDBCFormat;

class DBCReaderPrivate;
class DBCReader
{
      friend class AbstractDBCFormat;

      void  readHeader();

   public:
      explicit DBCReader(QIODevice *device = 0);
      ~DBCReader();

      void        setDevice(QIODevice* device);
      QIODevice*  device();

      bool        atEnd();
      bool        isValid();

      void        setFormat(const AbstractDBCFormat& format);

      QMap<QString, QVariant> nextRow();

   signals:

   public slots:

   private:
      QExplicitlySharedDataPointer<DBCReaderPrivate> d;

};

#endif // DBCREAD_H
