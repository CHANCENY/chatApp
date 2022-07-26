#include <QCoreApplication>
#include "myserver.h"
#include "interclass.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    myServer server;
    server.makeTables();
    server.startServer();



    return a.exec();
}
