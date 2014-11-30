#include "mapviewermain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MapViewerMain w;
   w.show();

   return a.exec();
}
