#include <QCoreApplication>
#include "caninterface.h"

int main(int argc, char *argv[])
{
    qDebug() << "Start monitoring CAN messages";
    QCoreApplication a(argc, argv);
    QString interface = "vcan0";
    CANInterface* canInterface = new CANInterface(interface);
    // canInterface->receiveCANMessage();
    canInterface->monitorCANMessage();
    return a.exec();
}
