#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cascstorage.h"
#include "cascfile.h"
#include "dbcmap.h"
#include "dbcreader.h"

#include <QDebug>
#include <QApplication>
#include <QShowEvent>
#include <QTreeWidgetItem>
#include <QtConcurrent>
#include <QImage>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   QGraphicsScene* scene = new QGraphicsScene();
   ui->mapView->setScene(scene);

   connect(&this->watcher, SIGNAL(resultReadyAt(int)), this, SLOT(tileReady(int)));
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::showEvent(QShowEvent* event){
   if(!event->spontaneous()){
      if(this->storage.open("/Applications/World of Warcraft/data")){
         qDebug() << "Opened storage!";
      }else
         qDebug() << "Open error: " << this->storage.errorString();

      CascFile mapDbc;
      mapDbc.setStorage(this->storage);
      mapDbc.setFileName("DBFilesClient/Map.dbc");
      if(!mapDbc.open(CascFile::ReadOnly)){
         qFatal("Unabel to open Map.dbc!");
         qApp->quit();
         return;
      }

      DBCReader reader(&mapDbc);
      reader.setFormat(DBCMap());

      QList<QTreeWidgetItem*> treeItems;

      while(!reader.atEnd()){
         QMap<QString, QVariant> entry = reader.nextRow();

         if(entry["ParentMapId"].toInt() == -1){
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, entry["ID"].toString()+" - "+entry["MapName"].toString());
            item->setData(0, Qt::UserRole, entry);

            treeItems << item;
         }else{
            bool fFound = false;

            foreach(QTreeWidgetItem* parentItem, treeItems){
               if(parentItem->data(0, Qt::UserRole).toMap()["ID"].toInt() == entry["ParentMapId"].toInt()){
                  QTreeWidgetItem* item = new QTreeWidgetItem();
                  item->setText(0, entry["ID"].toString()+" - "+entry["MapName"].toString());
                  item->setData(0, Qt::UserRole, entry);
                  parentItem->addChild(item);
                  fFound = true;
                  break;
               }
            }

            if(!fFound){
               qWarning() << entry["MapName"].toString();
               qWarning() << "Parent Map not found!";
               QTreeWidgetItem* item = new QTreeWidgetItem();
               item->setText(0, entry["ID"].toString()+" - "+entry["MapName"].toString());
               item->setData(0, Qt::UserRole, entry);

               treeItems << item;
            }
         }
      }

      ui->mapTree->addTopLevelItems(treeItems);
   }
}

struct TileRequest{
   QString     MapName;
   QPoint      Pos;
   CascStorage Storage;
};

struct BLPHeader{
   char        Magic[4];
   quint32     Type;
   quint8      Compression;
   quint8      AlphaDepth;
   quint8      AlphaType;
   quint8      HasMips;
   quint32     Width;
   quint32     Height;
   quint32     MipmapOffset[16];
   quint32     MipmapLength[16];
   quint32     Palette[256];
};

QMutex mutex;

QRgb toRgb(quint16 color){
   int r = (color & 0xF800) >> 11;
   int g = (color & 0x07E0) >> 5;
   int b = (color & 0x001F);

   r = r * 255 / 31;
   g = g * 255 / 63;
   b = b * 255 / 31;

   return qRgba(r, g, b, 255);
}

MainWindow::MapTile  mapTile(const TileRequest& tile){
   //qDebug() << "Load: " << tile[0];
   CascFile file;
   file.setStorage(tile.Storage);
   file.setFileName(QStringLiteral("World/Minimaps/%1/map%2_%3.blp").arg(tile.MapName).arg(tile.Pos.x()).arg(tile.Pos.y()));

   if(file.open(CascFile::ReadOnly)){
      //QMutexLocker lock(&mutex);

      BLPHeader header;
      file.read((char*)&header, sizeof(BLPHeader));

      if(QLatin1String((char*)&header.Magic, 4) != "BLP2"){
         qWarning() << "Invalid blp header!";
         return MainWindow::MapTile();
      }
      //qDebug() << "Reading: " << tile.Path;

      QImage image(header.Width, header.Height, QImage::Format_ARGB32);
      image.fill(Qt::blue);

      file.seek(header.MipmapOffset[0]);
      QByteArray data = file.read(header.MipmapLength[0]);

      if(header.Compression == 2){
         switch(header.AlphaType){
            case 0:{
               struct DXT1Block{
                     quint16 c0;
                     quint16 c1;
                     quint32 pixels;
               };

               int blocks = data.size() / 8;
               DXT1Block* block = (DXT1Block*)data.data();

               int row=0;
               int col=0;
               int cols = header.Width / 4;

               for(int i=0; i < blocks; i++){
                  QVector<QColor> table;
                  table << toRgb(block[i].c0);
                  table << toRgb(block[i].c1);

                  if(block[i].c0 > block[i].c1){
                     //table[2] = (2.0 / 3.0) * table[0] + (1.0 / 3.0) * table[1];
                     //table[3] = (1.0 / 3.0) * table[0] + (2.0 / 3.0) * table[1];
                     int r2 = (2.0 / 3.0) * table[0].red()     + (1.0 / 3.0) * table[1].red();
                     int g2 = (2.0 / 3.0) * table[0].green()   + (1.0 / 3.0) * table[1].green();
                     int b2 = (2.0 / 3.0) * table[0].blue()    + (1.0 / 3.0) * table[1].blue();

                     int r3 = (1.0 / 3.0) * table[0].red()     + (2.0 / 3.0) * table[1].red();
                     int g3 = (1.0 / 3.0) * table[0].green()   + (2.0 / 3.0) * table[1].green();
                     int b3 = (1.0 / 3.0) * table[0].blue()    + (2.0 / 3.0) * table[1].blue();

                     table << qRgba(r2, g2, b2, 255);
                     table << qRgba(r3, g3, b3, 255);
                  }else{
                     int r2 = 0.5 * table[0].red()     + 0.5 * table[1].red();
                     int g2 = 0.5 * table[0].green()   + 0.5 * table[1].green();
                     int b2 = 0.5 * table[0].blue()    + 0.5 * table[1].blue();

                     table << qRgba(r2, g2, b2, 255);
                     table << Qt::black;
                     //table[2] = 0.5 * table[0] + 0.5 * table[1];
                     //table[3] = 0;
                  }

                  /*qDebug() << "***";
                  qDebug() << QString::number(table[0]);
                  qDebug() << QString::number(table[1]);
                  qDebug() << QString::number(table[2]);
                  qDebug() << QString::number(table[3]);*/

                  quint32 pixels = block[i].pixels;
                  quint32 mask = 0x3;
                  int p=0;

                  //uchar* imgData = image.bits();

                  for(int y=0; y < 4; y++){
                     for(int x=0; x < 4; x++){
                        int index = (pixels & mask) >> p;
                        mask <<= 2;
                        p+=2;


                        //qDebug() << QString::number(r, 10);

                        //qDebug() << r << " / " << g << " / " << b;

                        //uchar* line = image.scanLine((row*4)+y);
                        //imgData[(image.width() * ((row*4)+y) + col*4)+x] = qRgb(r, g, b);
                        image.setPixel((col*4)+x, (row*4)+y, table[index].rgba());
                     }
                  }

                  col++;
                  if(col >= cols){
                     col=0;
                     row++;
                  }
               }

               break;
            }

            case 1:
               qWarning() << "DXT2 not implemented!";
               break;

            case 7:
               qWarning() << "DXT5 not implemented!";
               break;
         }
      }

      CascFile adt;
      adt.setStorage(tile.Storage);
      adt.setFileName(QStringLiteral("World/Maps/%1/%1_%2_%3.adt").arg(tile.MapName).arg(tile.Pos.x()).arg(tile.Pos.y()));

      QImage borderImage(header.Width, header.Height, QImage::Format_ARGB32);
      borderImage.fill(Qt::transparent);

      if(adt.open(CascFile::ReadOnly)){
         struct Chunk{
               quint32 Id;
               quint32 Size;
         };

         int x=0;
         int y=0;

         QPainter borderPainter(&borderImage);
         borderPainter.setBrush(Qt::red);

         while(!adt.atEnd()){
            Chunk chunk;
            adt.read((char*)&chunk, sizeof(Chunk));

            QByteArray data = adt.read(chunk.Size);

            if(chunk.Id == 0x4d434e4b){
               quint32 flags = *(quint32*)data.data();

               if(flags & 0x2)
                  borderPainter.drawRect(x*16, y*16, 16, 16);

               x++;
               if(x >= 16){
                  x=0;
                  y++;
               }
            }
         }
      }

      MainWindow::MapTile tileResult;
      tileResult.Pos    = tile.Pos;
      tileResult.Image  = QPixmap::fromImage(image);
      tileResult.Walls  = QPixmap::fromImage(borderImage);
      return tileResult;
   }

   return MainWindow::MapTile();
}

void MainWindow::tileReady(int index){
   MapTile tile = this->watcher.resultAt(index);

   QGraphicsPixmapItem* item = ui->mapView->scene()->addPixmap(tile.Image);
   item->setPos(tile.Pos.x() * tile.Image.width(), tile.Pos.y() * tile.Image.height());
   item->setToolTip(QString("%1 / %2").arg(tile.Pos.x()).arg(tile.Pos.y()));
   //qDebug() << tile.Pos;

   QGraphicsPixmapItem* wallItem = ui->mapView->scene()->addPixmap(tile.Walls);
   wallItem->setPos(tile.Pos.x() * tile.Image.width(), tile.Pos.y() * tile.Image.height());
   wallItem->setToolTip(QString("%1 / %2").arg(tile.Pos.x()).arg(tile.Pos.y()));
}

void MainWindow::on_mapTree_itemClicked(QTreeWidgetItem *item, int column)
{
   Q_UNUSED(column);

   if(this->watcher.isRunning())
      this->watcher.cancel();

   ui->mapView->scene()->clear();
   ui->mapView->scene()->setSceneRect(QRectF());

   //QString mapPath = QStringLiteral("World/Minimaps/%1/map%2_%3.blp").arg(item->data(0, Qt::UserRole).toMap()["Directory"].toString());

   QList<TileRequest> tiles;
   for(int x=0; x < 100; x++){
      for(int y=0; y < 100; y++){
         TileRequest request;
         //request.Path = mapPath.arg(x, 2, 10, QLatin1Char('0')).arg(y, 2, 10, QLatin1Char('0'));
         request.MapName = item->data(0, Qt::UserRole).toMap()["Directory"].toString();
         request.Pos  = QPoint(x, y);
         request.Storage = this->storage;

         tiles << request;
      }
   }

   QFuture<MapTile> result = QtConcurrent::mapped(tiles, mapTile);
   this->watcher.setFuture(result);
}
