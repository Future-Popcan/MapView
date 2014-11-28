#include "cascfile.h"
#include "cascstorage_p.h"
#include "cascstorage.h"

#include <QSharedData>
#include <QLocale>
#include <QDebug>
#include <QBuffer>

/*
#define CASC_LOCALE_ALL             0xFFFFFFFF
#define CASC_LOCALE_NONE            0x00000000
#define CASC_LOCALE_UNKNOWN1        0x00000001
#define CASC_LOCALE_ENUS            0x00000002
#define CASC_LOCALE_KOKR            0x00000004
#define CASC_LOCALE_UNKNOWN8        0x00000008
#define CASC_LOCALE_FRFR            0x00000010
#define CASC_LOCALE_DEDE            0x00000020
#define CASC_LOCALE_ZHCN            0x00000040
#define CASC_LOCALE_ESES            0x00000080
#define CASC_LOCALE_ZHTW            0x00000100
#define CASC_LOCALE_ENGB            0x00000200
#define CASC_LOCALE_ENCN            0x00000400
#define CASC_LOCALE_ENTW            0x00000800
#define CASC_LOCALE_ESMX            0x00001000
#define CASC_LOCALE_RURU            0x00002000
#define CASC_LOCALE_PTBR            0x00004000
#define CASC_LOCALE_ITIT            0x00008000
#define CASC_LOCALE_PTPT            0x00010000
 */

class CascFilePrivate : public QSharedData {
public:
      QExplicitlySharedDataPointer<CascStoragePrivate> Storage;
      QString  FileName;
      QLocale  Locale;
      HANDLE   FileHandle;

      CascFilePrivate() : QSharedData(){
         this->Locale = QLocale::system();
         this->FileHandle = 0;
      }

      quint32 cascLocale(){
         switch(this->Locale.language()){
            case QLocale::English:
               switch(this->Locale.country()){
                  case QLocale::UnitedStates:
                  case QLocale::UnitedStatesMinorOutlyingIslands:
                  case QLocale::UnitedStatesVirginIslands:
                     return CASC_LOCALE_ENUS;

                  case QLocale::UnitedKingdom:
                     return CASC_LOCALE_ENGB;

                  case QLocale::China:
                     return CASC_LOCALE_ENCN;

                  case QLocale::Taiwan:
                     return CASC_LOCALE_ENTW;

                  default:
                     return CASC_LOCALE_ENUS;
               }

            case QLocale::German:
               return CASC_LOCALE_DEDE;

            case QLocale::French:
               return CASC_LOCALE_FRFR;

            case QLocale::Italian:
               return CASC_LOCALE_ITIT;

            case QLocale::Russian:
               return CASC_LOCALE_RURU;

            case QLocale::Korean:
            case QLocale::KoreanScript:
               return CASC_LOCALE_KOKR;

            case QLocale::Spanish:
               switch(this->Locale.country()){
                  case QLocale::Spain:
                     return CASC_LOCALE_ESES;

                  case QLocale::Mexico:
                     return CASC_LOCALE_ESMX;

                  default:
                     return CASC_LOCALE_ENUS;
               }

            case QLocale::Zhuang:
               switch(this->Locale.country()){
                  case QLocale::China:
                     return CASC_LOCALE_ZHCN;

                  case QLocale::Taiwan:
                     return CASC_LOCALE_ZHTW;

                  default:
                     return CASC_LOCALE_ENUS;
               }

            default:
               return CASC_LOCALE_ENUS;
         }
      }
};

CascFile::CascFile(QObject* parent)
   : QIODevice(parent),
   d(new CascFilePrivate)
{

}

CascFile::~CascFile(){
   this->close();
}

void CascFile::setStorage(const CascStorage& storage){
   d->Storage = storage.d;
}

CascStorage CascFile::storage(){
   return CascStorage(d->Storage.data());
}

void CascFile::setFileName(const QString& path){
   if(this->isOpen()){
      qWarning() << tr("FileName must be set before open!");
      return;
   }

   d->FileName = path;
}

QString CascFile::fileName() const{
   return d->FileName;
}

void CascFile::setLocale(const QLocale& locale){
   if(this->isOpen()){
      qWarning() << tr("Locale must be set before open!");
      return;
   }

   d->Locale = locale;
}

QLocale CascFile::locale() const{
   return d->Locale;
}

bool CascFile::seek(qint64 pos){
   int loPos = pos & 0xFFFFFFFF;
   int hiPos = pos >> 32 & 0xFFFFFFFF;

   if(CascSetFilePointer(d->FileHandle, loPos, &hiPos, FILE_BEGIN) == CASC_INVALID_POS){
      this->setErrorString(tr("Invalid file pos!"));
      qDebug() << "Pos: " << pos;
      qDebug() << loPos << " / " << hiPos;

      return false;
   }

   return QIODevice::seek(pos);
}

qint64 CascFile::bytesAvailable() const{
   if(!this->isOpen())
      return QIODevice::bytesAvailable();

   DWORD hiSize;
   DWORD loSize = CascGetFileSize(d->FileHandle, &hiSize);

   return qint64(hiSize) << 32 | loSize;
}

bool CascFile::open(OpenMode mode){
   if(mode & WriteOnly){
      this->setErrorString(tr("Writing to casc is unsupported!"));
      return false;
   }

   if(!d->Storage){
      this->setErrorString(tr("Invalid storage provided!"));
      return false;
   }

   if(!d->Storage->hStorage){
      this->setErrorString(tr("Storage not open!"));
      return false;
   }

   if(!QIODevice::open(mode))
      return false;

   if(!CascOpenFile(d->Storage->hStorage,
                    d->FileName.toLatin1().data(),
                    d->cascLocale(),
                    STREAM_FLAG_READ_ONLY,
                    &d->FileHandle)){

      this->setErrorString(d->Storage->translateError(GetLastError()));
      this->close();

      return false;
   }

   return true;
}

void CascFile::close(){
   if(d->FileHandle){
      CascCloseFile(d->FileHandle);
      d->FileHandle = 0;
   }

   QIODevice::close();
}

qint64 CascFile::readData(char *data, qint64 maxlen){
   quint32 readLen = maxlen & 0xFFFFFFFF;
   quint32 read = 0;

   if(!CascReadFile(d->FileHandle, data, readLen, &read)){
      this->setErrorString(d->Storage->translateError(GetLastError()));
      return -1;
   }

   return read;
}

qint64 CascFile::writeData(const char *data, qint64 len){
   Q_UNUSED(data);
   Q_UNUSED(len);

   this->setErrorString(tr("Writing a casc file is not supported!"));
   return -1;
}
