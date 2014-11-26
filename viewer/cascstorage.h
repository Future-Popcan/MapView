#ifndef CASCSTORAGE_H
#define CASCSTORAGE_H

#include <QObject>
#include <QExplicitlySharedDataPointer>

class CascStoragePrivate;
class CascStorage : public QObject
{
      Q_OBJECT
   public:
      explicit CascStorage(QObject *parent = 0);
      ~CascStorage();

      bool     open(const QString& path);
      void     close();
      bool     isOpen() const;

      QString  path() const;
      QString  errorString() const;

   signals:

   public slots:

   private:
      QExplicitlySharedDataPointer<CascStoragePrivate> d;

};

#endif // CASCSTORAGE_H
