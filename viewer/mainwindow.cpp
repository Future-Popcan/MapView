#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cascstorage.h"
#include "cascfile.h"
#include "dbcmap.h"
#include "dbcreader.h"

#include <QDebug>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   CascStorage storage;
   if(storage.open("/Applications/World of Warcraft/data")){
      qDebug() << "Opened storage!";
   }else
      qDebug() << "Open error: " << storage.errorString();

   CascFile mapDbc;
   mapDbc.setStorage(storage);
   mapDbc.setFileName("DBFilesClient/Map.dbc");
   if(!mapDbc.open(CascFile::ReadOnly)){
      qFatal("Unabel to open Map.dbc!");
      qApp->quit();
      return;
   }

   DBCReader reader(&mapDbc);
   reader.setFormat(DBCMap());

   while(!reader.atEnd()){
      QMap<QString, QVariant> entry = reader.nextRow();
      qDebug() << entry["Directory"] << " / " << entry["MapName"];
   }
}

MainWindow::~MainWindow()
{
   delete ui;
}
