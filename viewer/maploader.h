#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <QObject>
#include <QStringList>
#include <QExplicitlySharedDataPointer>

class MapLoaderPrivate;
class MapLoader : public QObject
{
      Q_OBJECT
   public:
      explicit MapLoader(QObject *parent = 0);
      ~MapLoader();

      QStringList mapList();

   signals:

   public slots:

   private:
      QExplicitlySharedDataPointer<MapLoaderPrivate> d;
};

#endif // MAPLOADER_H
