//
// Created by hoangqc on 06/11/2020.
//

#ifndef IOT_DASHBOARD_SENSORSMQTT_H
#define IOT_DASHBOARD_SENSORSMQTT_H

#include <QObject>

class SensorNode
{
public:
    std::string topic_sensor;
    std::string topic_control;
    std::string token;
    std::string mac_address;
};


class SensorsMQTT : public QObject {
    Q_OBJECT

public:
    SensorsMQTT();
    ~SensorsMQTT();
    int initHosting();
    int loadSensorParameters(int index=0);

    std::vector<QString> hostnames;
    std::vector<SensorNode> sensorNodes;

private:
//    std::string dbFile_Login;
//    std::string dbFile_LocalData;
    std::string m_mongodb_uri;



private slots:

};


#endif //IOT_DASHBOARD_SENSORSMQTT_H
