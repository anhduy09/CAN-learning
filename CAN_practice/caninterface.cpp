#include "caninterface.h"

CANInterface::CANInterface(const QString &interfaceName) {

    if (initializeCanSocket(interfaceName)) {
        qDebug() << "CAN socket initialized and bound successfully!";
    } else {
        qCritical() << "Failed to initialize CAN socket.";
    }

}
CANInterface:: ~CANInterface() {
   close(sock);
}

bool CANInterface::initializeCanSocket(const QString &interfaceName)
{
    struct ifreq ifr = {};
    struct sockaddr_can addr{};
    // Copy the interface name into the ifreq structure
    strncpy(ifr.ifr_name, interfaceName.toStdString().c_str(), IFNAMSIZ);

    //create CAN socket
    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        qCritical() << "Error creating socket";
        return false;
    }

    // Get the interface index
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        qCritical() << "Error getting interface index";
        close(sock);
        return false;
    }

    // Set up the CAN address structure
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Bind the socket
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        qCritical() << "Error binding socket";
        close(sock);
        return false;
    }

    return true;
}

void CANInterface::receiveCANMessage()
{
    struct can_frame frame{};
    ssize_t nbytes = read(sock, &frame, sizeof(frame));
    if (nbytes > 0) {
        qDebug() << "Received CAN ID:" << frame.can_id << "Data:" << QByteArray((char *)frame.data, frame.can_dlc);
    }
}

void CANInterface::monitorCANMessage()
{
    struct can_frame frame;

    qDebug() << "Listening for CAN messages...";
    while(true) {
        ssize_t nbytes = read(sock, &frame, sizeof(frame));
        if (nbytes > 0) {
            qDebug() << "Received CAN ID:" << frame.can_id << "Data:" << QByteArray((char *)frame.data, frame.can_dlc);
        }

        if (nbytes < sizeof(struct can_frame)) {
            qCritical() << "Incomplete CAN frame received";
            continue;
        }

        // Display the received CAN message
        qDebug() << "Received CAN ID:" << QString::number(frame.can_id, 16)
                 << "DLC:" << frame.can_dlc;

        QString dataPayload;
        for (int i = 0; i < frame.can_dlc; ++i) {
            dataPayload += QString::number(frame.data[i], 16).rightJustified(2, '0').toUpper() + " ";
        }
        qDebug() << "message payload: " + dataPayload.trimmed();
    }
}
