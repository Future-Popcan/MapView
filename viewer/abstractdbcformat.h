#ifndef ABSTRACTDBCFORMAT_H
#define ABSTRACTDBCFORMAT_H

#include <QVector>
#include <QVariant>
#include <QMap>
#include <QExplicitlySharedDataPointer>

#include "dbcreader.h"

class AbstractDBCFormatPrivate;
class AbstractDBCFormat
{
      QExplicitlySharedDataPointer<AbstractDBCFormatPrivate> d;

   public:
      enum LocVersion{
         Invalid = -1,
         Cataclysm = 0,
      };

      enum Type{
         Integer,
         Reference,
         Float,
         Double,
         Boolean,
         String,
         Localized
      };

      struct Field{
         QString  Id;
         Type     Type;
         qint32   Offset;
      };


   protected:
      AbstractDBCFormat(LocVersion version);
      void  addField(Type type, const QString& id);

   public:
      AbstractDBCFormat();
      AbstractDBCFormat(const AbstractDBCFormat& other);
      ~AbstractDBCFormat();

      bool                    isValid();
      qint64                  size();
      int                     fieldCount();

      void                    setReader(DBCReader* reader);
      QMap<QString, QVariant> readRecord(const QByteArray& data) const;

      AbstractDBCFormat& operator=(const AbstractDBCFormat& rh);
};

#endif // ABSTRACTDBCFORMAT_H
