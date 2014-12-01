#include "abstractdbcformat.h"
#include "dbcreader_p.h"

#include <QSharedData>
#include <QDataStream>
#include <QBuffer>
#include <QDebug>

class AbstractDBCFormatPrivate : public QSharedData{
   public:
      QVector<AbstractDBCFormat::Field>   Fields;
      AbstractDBCFormat::LocVersion       Version;
      quint32                             OffsetCount;
      DBCReader*                          Reader;

      AbstractDBCFormatPrivate() : QSharedData(){
         this->Reader      = 0;
         this->OffsetCount = 0;
         this->Version     = AbstractDBCFormat::Invalid;
      }
};

AbstractDBCFormat::AbstractDBCFormat(LocVersion version) :
   d(new AbstractDBCFormatPrivate)
{
   d->Version = version;
}

AbstractDBCFormat::AbstractDBCFormat() :
   d(new AbstractDBCFormatPrivate)
{

}

AbstractDBCFormat::AbstractDBCFormat(const AbstractDBCFormat& other) :
   d(other.d)
{

}

AbstractDBCFormat::~AbstractDBCFormat(){

}

void AbstractDBCFormat::addField(Type type, const QString& id){
   Field field = {id, type, 0};
   d->Fields.push_back(field);

   switch(type){
      case Reference:
      case Integer:  d->OffsetCount += sizeof(quint32);       break;
      case Boolean:  d->OffsetCount += sizeof(bool);          break;
      case Float:    d->OffsetCount += sizeof(float);         break;
      case Double:   d->OffsetCount += sizeof(double);        break;
      case String:   d->OffsetCount += sizeof(quint32);       break;
      case Localized:d->OffsetCount += sizeof(quint32);       break;
      default:
         qWarning() << "Unknown field type!";
         break;
   }
}

bool AbstractDBCFormat::isValid(){
   return d->Version != Invalid;
}

quint32 AbstractDBCFormat::size(){
   return d->OffsetCount;
}

quint32 AbstractDBCFormat::fieldCount(){
   return d->Fields.count();
}

void AbstractDBCFormat::setReader(DBCReader* reader){
   d->Reader = reader;
}

QMap<QString, QVariant> AbstractDBCFormat::readRecord(const QByteArray& data) const{
   QMap<QString, QVariant> record;

   QBuffer dataBuffer;
   dataBuffer.setData(data);
   dataBuffer.open(QBuffer::ReadOnly);

   foreach(const Field& field, d->Fields){
      QVariant variant;

      switch(field.Type){
         case Integer:{
            qint32 val;
            d->Reader->device()->read((char*)&val, sizeof(qint32));

            variant = val;
            break;
         }

         case Reference:{
            qint32 val;
            d->Reader->device()->read((char*)&val, sizeof(qint32));

            variant = val;
            break;
         }

         case Boolean:{
            quint32 val;
            d->Reader->device()->read((char*)&val, sizeof(quint32));

            variant = val;
            break;
         }

         case Float:{
            float val;
            d->Reader->device()->read((char*)&val, sizeof(float));

            variant = val;
            break;
         }

         case Double:{
            double val;
            d->Reader->device()->read((char*)&val, sizeof(double));

            variant = val;
            break;
         }

         case Localized:
         case String:{
            quint32 offset;
            d->Reader->device()->read((char*)&offset, sizeof(quint32));

            variant = d->Reader->d->readString(offset);
            break;
         }

         default:
            qWarning() << "Not implemented field type!";
            return QMap<QString, QVariant>();
      }

      record[field.Id] = variant;
   }

   return record;
}

AbstractDBCFormat& AbstractDBCFormat::operator=(const AbstractDBCFormat& rh){
   d = rh.d;
   return *this;
}
