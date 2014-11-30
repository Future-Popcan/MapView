#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QtConcurrent>
#include <QFutureWatcher>

#include "cascstorage.h"

namespace Ui {
   class MainWindow;
}

class MainWindow : public QMainWindow
{
      Q_OBJECT

   public:
      struct MapTile{
            QPixmap  Image;
            QPoint   Pos;
            QPixmap  Walls;
      };

   public:
      explicit MainWindow(QWidget *parent = 0);
      ~MainWindow();

   protected:
      void showEvent(QShowEvent* event);

   public slots:

   private slots:
      void on_mapTree_itemClicked(QTreeWidgetItem *item, int column);
      void tileReady(int index);

   private:
      CascStorage             storage;
      QFutureWatcher<MapTile>  watcher;
      Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
