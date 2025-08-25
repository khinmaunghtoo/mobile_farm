#ifndef SCRCPY_PARAMS_H
#define SCRCPY_PARAMS_H

#include <QString>
#include <QRandomGenerator>

/*
   Scrcpy Server Params
   some args are optional, we use default values
   so, they are not shown in the struct
    - codecOptions
    - encoderName
    - crop
*/ 
struct ScrcpyParams
{
    QString deviceSerial;
    const QString serverRemotePath = "/data/local/tmp/scrcpy-server.jar"; // /data/local/tmp/scrcpy-server.jar
    const QString serverVersion = "3.3.1"; // current scrcpy-server version 3.3.1
    const QString logLevel = "info"; // "", "verbose", "debug", "info", "warn", "error"
    int bitRate = 2000000; // 2Mbps
    int maxSize = 0;           // 0 = original size
    int maxFps = 0;            // 0 = original fps
    bool control = true;
    bool tunnelReverse = true; // from 3.0, tunnel reverse is the default
    bool stayAwake = true;

    // device socket name, example: scrcpy_12345678
    QString socketName = "";  
    
    // this value will be generated in ScrcpySession constructor
    quint16 tcpPort = 0;        // local TCP port for forwarding

    // generate our own random scid in stead of using the default scid (-1)
    // then we can use it to identify the scrcpy server process
    const int scid = QRandomGenerator::global()->generate() & 0x7FFFFFFF;

};

#endif // SCRCPY_PARAMS_H
