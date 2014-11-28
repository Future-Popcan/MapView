#include "cascstorage.h"
#include "cascstorage_p.h"

CascStoragePrivate::CascStoragePrivate()
   : QSharedData(),
   hStorage(0)
{
}

CascStoragePrivate::~CascStoragePrivate(){

}

bool CascStoragePrivate::openStorage(const QString& path){
   if(CascOpenStorage(path.toLatin1().data(), 0, &this->hStorage)){
      this->lastError.clear();
      this->path = path;
      return true;
   }

   this->path.clear();
   this->lastError = this->translateError(GetLastError());
   return false;
}

void CascStoragePrivate::closeStorage(){
   if(this->hStorage){
      CascCloseStorage(this->hStorage);
      this->hStorage = 0;
   }
}

QString CascStoragePrivate::translateError(int code){
   switch(code){
      case ERROR_SUCCESS:
         return QString();

      case ERROR_FILE_NOT_FOUND:
         return CascStorage::tr("File not found");

      case ERROR_ACCESS_DENIED:
         return CascStorage::tr("Access denied");

      case ERROR_INVALID_HANDLE:
         return CascStorage::tr("Invalid handle");

      case ERROR_NOT_ENOUGH_MEMORY:
         return CascStorage::tr("Out of memory");

      case ERROR_NOT_SUPPORTED:
         return CascStorage::tr("Not supported");

      case ERROR_INVALID_PARAMETER:
         return CascStorage::tr("FInvalid parameter");

      case ERROR_DISK_FULL:
         return CascStorage::tr("Disk full");

      case ERROR_ALREADY_EXISTS:
         return CascStorage::tr("File already existing");

      case ERROR_INSUFFICIENT_BUFFER:
         return CascStorage::tr("Insufficient buffer space");

      case ERROR_BAD_FORMAT:
         return CascStorage::tr("Bad format");

      case ERROR_NO_MORE_FILES:
         return CascStorage::tr("No more files");

      case ERROR_HANDLE_EOF:
         return CascStorage::tr("End of file");

      case ERROR_CAN_NOT_COMPLETE:
         return CascStorage::tr("Can not complete");

      case ERROR_FILE_CORRUPT:
         return CascStorage::tr("File corrupt");
   }

   return QString();
}

CascStorage::CascStorage(CascStoragePrivate* priv)
   : QObject(0),
     d(priv)
{
}

CascStorage::CascStorage(QObject *parent) :
   QObject(parent),
   d(new CascStoragePrivate)
{
}

CascStorage::CascStorage(const CascStorage& other) :
   QObject(0),
   d(other.d)
{
}

CascStorage::~CascStorage(){

}

bool CascStorage::open(const QString& path){
   return d->openStorage(path);
}

void CascStorage::close(){
   d->closeStorage();
}

bool CascStorage::isOpen() const{
   return d->hStorage != 0;
}

QString CascStorage::path() const{
   return d->path;
}

QString CascStorage::errorString() const{
   return d->lastError;
}
