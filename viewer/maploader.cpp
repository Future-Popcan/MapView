#include "maploader.h"

#include <QSharedData>

class MapLoaderPrivate : public QSharedData {

};

MapLoader::MapLoader(QObject *parent) :
   QObject(parent),
   d(new MapLoaderPrivate)
{

}

MapLoader::~MapLoader(){

}

QStringList MapLoader::mapList(){

}
