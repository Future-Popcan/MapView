#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QExplicitlySharedDataPointer>

#include "cascstorage.h"

namespace Ui {
   class MapViewerMain;
}

class MapViewerMainPrivate;
class MapViewerMain : public QMainWindow
{
      Q_OBJECT

   public:
      struct MapTile{
            QPixmap  Image;
            QPoint   Pos;
            QPixmap  Walls;
      };

   public:
      explicit MapViewerMain(QWidget *parent = 0);
      ~MapViewerMain();

   protected:
      void showEvent(QShowEvent* event);

   public slots:

   private slots:
      void on_mapTree_itemClicked(QTreeWidgetItem *item, int column);
      void tileReady(int index);

   private:
      QExplicitlySharedDataPointer<MapViewerMainPrivate> d;

      CascStorage             storage;
      QFutureWatcher<MapTile>  watcher;
      Ui::MapViewerMain *ui;
};

#endif // MAINWINDOW_H
