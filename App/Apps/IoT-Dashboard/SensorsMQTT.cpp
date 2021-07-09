//
// Created by hoangqc on 06/11/2020.
//

#include "SensorsMQTT.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>

/*
#ifndef MONGODB
#define MONGODB
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#endif
*/

SensorsMQTT::SensorsMQTT() {

}

SensorsMQTT::~SensorsMQTT() {

}

int SensorsMQTT::initHosting() {
    QFile f(":Data/Database/mongodb.json");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = f.readAll();
    QJsonArray array = QJsonDocument::fromJson(val.toUtf8()).array();
    for(int i = 0; i < array.size(); i++){
        QJsonObject t_obj = array[i].toObject();
        hostnames.push_back(t_obj["hostname"].toString());
//        qDebug()<< i << "-" <<hostnames[i];
    }
    return 0;
}

int SensorsMQTT::loadSensorParameters(int index) {
    QFile file(":Data/Database/devices.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();
    QJsonArray a = QJsonDocument::fromJson(val.toUtf8()).array();

    for (int i = 0; i < a.count(); ++i) {
        QJsonValue v = a[i];
        SensorNode node;
        node.topic_sensor = v["topic-sensor"].toString().toStdString();
        node.topic_control = v["topic-control"].toString().toStdString();
        node.token = v["token"].toString().toStdString();
        sensorNodes.push_back(node);
    }
    return 0;
}

/*
int SensorsMQTT::loadSensorParameters(int index) {
    m_mongodb_uri = "mongodb://" + hostnames[index].toStdString() + ":" + std::to_string(27017);
    std::cout<< m_mongodb_uri <<"\n";
    sensorNodes.clear();
    qDebug() << "initSensorParameters";

    mongocxx::instance instance{};
    mongocxx::uri uri(m_mongodb_uri);
    mongocxx::client client(uri);
    mongocxx::database db = client["iot"];
    mongocxx::collection coll = db["devices"];
    mongocxx::cursor cursor = coll.find({});
    for(auto doc : cursor) {
        SensorNode node;
        node.topic_sensor = doc["topic-sensor"].get_string().value.to_string();
        node.topic_control = doc["topic-control"].get_string().value.to_string();
        node.token = doc["token"].get_string().value.to_string();
        sensorNodes.push_back(node);
//        std::cout << bsoncxx::to_json(doc) << "\n";
//        std::cout << topic_1.get_string().value << "\n";
//        std::cout << topic_2.g`et_string().value << "\n";
    }
    return 0;
}
 */