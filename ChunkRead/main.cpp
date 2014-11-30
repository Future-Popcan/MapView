#include <QApplication>
#include <QFile>
#include <QtEndian>
#include <QTextStream>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;

   w.show();
   /*QTextStream out(stdout);

   QFile file("Draenor_30_23.adt");
   if(file.open(QFile::ReadOnly)){
      while(!file.atEnd()){
         Chunk chunk;
         file.read((char*)&chunk, sizeof(Chunk));

         uint name = qToBigEndian(chunk.Name);
         if(chunk.Name == MCNK)
            out << "****" << endl;

         out << QLatin1String((char*)&name, 4) << QString("(%1)").arg(chunk.Name, 0, 16) << " - " << chunk.Size << endl;

         if(chunk.Name == MCNK){
            file.seek(file.pos() + 128);
         }else
            file.seek(file.pos() + chunk.Size);
      }
   }*/

   return a.exec();
}
