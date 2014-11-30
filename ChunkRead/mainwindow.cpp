#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QtEndian>
#include <QTreeWidgetItem>
#include <QBuffer>
#include <QTextStream>

struct Chunk{
      quint32  Name;
      quint32  Size;
};

#define MCNK 0x4d434e4b


MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   QFile file("Draenor_30_23_obj1.adt");
   if(file.open(QFile::ReadOnly)){
      while(!file.atEnd()){
         Chunk chunk;
         file.read((char*)&chunk, sizeof(Chunk));
         QByteArray chunkData = file.read(chunk.Size);

         uint name = qToBigEndian(chunk.Name);
         QTreeWidgetItem* item = new QTreeWidgetItem();
         item->setText(0, QLatin1String((char*)&name, 4));
         item->setText(1, QString("(%1)").arg(chunk.Size));

         QString strName = QLatin1String((char*)&name, 4);

         if(chunk.Name == MCNK){
            QBuffer buffer;
            buffer.setData(chunkData);
            buffer.open(QBuffer::ReadOnly);

            buffer.seek(128);

            while(!buffer.atEnd()){
               Chunk subChunk;
               buffer.read((char*)&subChunk, sizeof(Chunk));
               buffer.read(subChunk.Size);

               uint name = qToBigEndian(subChunk.Name);
               QTreeWidgetItem* subItem = new QTreeWidgetItem();
               subItem->setText(0, QLatin1String((char*)&name, 4));
               subItem->setText(1, QString("(%1)").arg(subChunk.Size));
               item->addChild(subItem);

            }
         }else if(strName == "MMDX" || strName == "MWMO"){
            QTextStream stream(&chunkData);
            stream.setCodec("UTF-8");

            QString tempName;

            while(!stream.atEnd()){
               QChar chr;
               stream >> chr;

               if(chr.isNull()){
                  QTreeWidgetItem* subItem = new QTreeWidgetItem();
                  subItem->setText(0, tempName);
                  item->addChild(subItem);

                  tempName.clear();
               }else{
                  tempName += chr;
               }
            }
         }
         //out << QLatin1String((char*)&name, 4) << QString("(%1)").arg(chunk.Name, 0, 16) << " - " << chunk.Size << endl;

         ui->treeWidget->addTopLevelItem(item);
      }
   }
}

MainWindow::~MainWindow()
{
   delete ui;
}
