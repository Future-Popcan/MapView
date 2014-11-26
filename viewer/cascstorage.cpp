#include "cascstorage.h"

#include "../casclib/src/CascLib.h"
#include "../casclib/src/common/Common.h"

class CascStoragePrivate : public QSharedData {
   public:
      HANDLE   hStorage;
      QString  lastError;
      QString  path;

      CascStoragePrivate(){
         this->hStorage = 0;
      }

      ~CascStoragePrivate(){

      }

      bool openStorage(const QString& path){
         if(CascOpenStorage(path.toLatin1().data(), 0, &this->hStorage)){
            this->setError(ERROR_SUCCESS);
            this->path = path;
            return true;
         }

         this->path.clear();
         this->setError(GetLastError());
         return false;
      }

      void closeStorage(){
         if(this->hStorage){
            CascCloseStorage(this->hStorage);
            this->hStorage = 0;
         }
      }

      void setError(int code){
         switch(code){
            case ERROR_SUCCESS:
               this->lastError.clear();
               break;

            case ERROR_FILE_NOT_FOUND:
               this->lastError = CascStorage::tr("File not found");
               break;

            case ERROR_ACCESS_DENIED:
               this->lastError = CascStorage::tr("Access denied");
               break;

            case ERROR_INVALID_HANDLE:
               this->lastError = CascStorage::tr("Invalid handle");
               break;

            case ERROR_NOT_ENOUGH_MEMORY:
               this->lastError = CascStorage::tr("Out of memory");
               break;

            case ERROR_NOT_SUPPORTED:
               this->lastError = CascStorage::tr("Not supported");
               break;

            case ERROR_INVALID_PARAMETER:
               this->lastError = CascStorage::tr("FInvalid parameter");
               break;

            case ERROR_DISK_FULL:
               this->lastError = CascStorage::tr("Disk full");
               break;

            case ERROR_ALREADY_EXISTS:
               this->lastError = CascStorage::tr("File already existing");
               break;

            case ERROR_INSUFFICIENT_BUFFER:
               this->lastError = CascStorage::tr("Insufficient buffer space");
               break;

            case ERROR_BAD_FORMAT:
               this->lastError = CascStorage::tr("Bad format");
               break;

            case ERROR_NO_MORE_FILES:
               this->lastError = CascStorage::tr("No more files");
               break;

            case ERROR_HANDLE_EOF:
               this->lastError = CascStorage::tr("End of file");
               break;

            case ERROR_CAN_NOT_COMPLETE:
               this->lastError = CascStorage::tr("Can not complete");
               break;

            case ERROR_FILE_CORRUPT:
               this->lastError = CascStorage::tr("File corrupt");
               break;
         }
      }
};

CascStorage::CascStorage(QObject *parent) :
   QObject(parent),
   d(new CascStoragePrivate)
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
