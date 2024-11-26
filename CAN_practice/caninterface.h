#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <QString>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <QDebug>

class CANInterface
{
public:
    CANInterface(const QString &interfaceName);
    ~CANInterface();
    bool initializeCanSocket(const QString &interfaceName);
    void receiveCANMessage();
    void monitorCANMessage();

    //void sendCANMessage(int id, const QByteArray &data)

private:
    int sock;
};

#endif // CANINTERFACE_H
