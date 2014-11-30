#ifndef CASCFILE_H
#define CASCFILE_H

#include <QIODevice>
#include <QExplicitlySharedDataPointer>

class CascStorage;

class CascFilePrivate;
class CascFile : public QIODevice
{
      Q_OBJECT

   public:
      CascFile(QObject* parent = 0);
      ~CascFile();

      void        setStorage(const CascStorage& storage);
      CascStorage storage();

      void        setFileName(const QString& path);
      QString     fileName() const;

      void        setLocale(const QLocale& locale);
      QLocale     locale() const;

      virtual bool seek(qint64 pos);

      virtual bool atEnd() const;
      virtual qint64 bytesAvailable() const;

      virtual bool open(OpenMode mode);
      virtual void close();

   protected:
      virtual qint64 readData(char *data,          qint64 maxlen);
      virtual qint64 writeData(const char *data,   qint64 len);

   signals:

   public slots:

   private:
      QExplicitlySharedDataPointer<CascFilePrivate> d;

};

#endif // CASCFILE_H
