#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cascstorage.h"

#include <QDebug>

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
}

MainWindow::~MainWindow()
{
   delete ui;
}
