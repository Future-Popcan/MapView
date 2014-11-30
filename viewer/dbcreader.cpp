#include "dbcreader.h"
#include "dbcreader_p.h"

#include <QSharedData>
#include <QDebug>
#include <QTextStream>

struct DBCHeader{
      char     Magic[4];
      quint32  RecordCount;
      quint32  FieldCount;
      quint32  RecordSize;
      quint32  StringblockSize;
};

DBCReaderPrivate::DBCReaderPrivate() :
   QSharedData(),
   Device(0),
   Records(-1),
   RecordSize(0),
   FieldCount(0),
   CurrentIndex(0)
{
}

QString DBCReaderPrivate::readString(qint64 offset){
   Q_ASSERT(this->Device);
   if(offset >= this->StringBlock.length()){
      qWarning() << "Offset: " << offset;
      qWarning() << "BlockSize: " << this->StringBlock.length();

      qWarning("Tried to read past string block!");
      return QString();
   }

   return QString::fromUtf8((char*)this->StringBlock.data() + offset);
}

DBCReader::DBCReader(QIODevice *device) :
   d(new DBCReaderPrivate){

   this->setDevice(device);
}

DBCReader::~DBCReader(){
}

void DBCReader::setDevice(QIODevice* device){
   d->Device = device;

   this->readHeader();
}

QIODevice* DBCReader::device(){
   return d->Device;
}

bool DBCReader::atEnd(){
   return d->CurrentIndex >= d->Records-1;
}

bool DBCReader::isValid(){
   return d->Records != -1;
}

void DBCReader::setFormat(const AbstractDBCFormat& format){
   d->Format = format;
   d->Format.setReader(this);
}

QMap<QString, QVariant> DBCReader::nextRow(){
   Q_ASSERT(d->Device->isOpen() && d->Device->isReadable());
   Q_ASSERT(d->Format.isValid());

   if(d->Format.fieldCount() != d->FieldCount){
      qWarning() << "Format fields: " << d->Format.fieldCount();
      qWarning() << "Expected: " << d->FieldCount;
      Q_ASSERT(false);
   }

   if(d->Format.size() != d->RecordSize){
      qWarning() << "Format size: " << d->Format.size();
      qWarning() << "Expected: " << d->RecordSize;
      Q_ASSERT(false);
   }

   if(!d->Device->seek(sizeof(DBCHeader) + d->CurrentIndex * d->RecordSize)){
      qFatal(d->Device->errorString().toLatin1().data());
      return QMap<QString, QVariant>();
   }

   QByteArray buffer = d->Device->read(d->RecordSize);

   if(buffer.length() < d->RecordSize){
      qWarning() << "Insufficient bytes read!";
      return QMap<QString, QVariant>();
   }

   d->CurrentIndex++;

   return d->Format.readRecord(buffer);
}

void DBCReader::readHeader(){
   Q_ASSERT(d->Device->isOpen() && d->Device->isReadable());

   DBCHeader header;
   if(d->Device->read((char*)&header, sizeof(DBCHeader)) != sizeof(DBCHeader)){
      qWarning() << "Not a valid file!";
      d->Records = -1;
      return;
   }

   if(QLatin1String((char*)&header.Magic, 4) == QLatin1Literal("WDBC")){
      d->Records           = header.RecordCount;
      d->RecordSize        = header.RecordSize;
      d->FieldCount        = header.FieldCount;
      int stringblockOffset = d->Records * d->RecordSize + sizeof(DBCHeader);
      d->CurrentIndex      = 0;

      if(!d->Device->seek(stringblockOffset)){
         qFatal(d->Device->errorString().toLatin1().data());
         return;
      }

      d->StringBlock = d->Device->read(header.StringblockSize);
   }else{
      qWarning() << "Invalid DBC header!";
      d->Records = -1;
      return;
   }
}
