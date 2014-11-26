#ifndef CASCFILE_H
#define CASCFILE_H

#include <QFileDevice>

class CascFile : public QFileDevice
{
      Q_OBJECT
   public:
      explicit CascFile(QObject *parent = 0);

   signals:

   public slots:

};

#endif // CASCFILE_H
