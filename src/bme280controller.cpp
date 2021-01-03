#include "bme280controller.h"

#include <QDateTime>
#include <QtDebug>

BME280Controller::BME280Controller(const int i2cAddress, const int i2cBus,
                                   const std::chrono::milliseconds msec, QObject *parent):
    SensorController(msec, parent),
    sensor(i2cAddress, i2cBus) { }

BME280Controller::BME280Controller(const int i2cAddress, const int i2cBus,
                                   const long msec, QObject *parent):
    BME280Controller(i2cAddress, i2cBus, std::chrono::milliseconds(msec), parent) { }

bool BME280Controller::isSensorAvailable() {
    return !sensor.obtenirErreur();
}

void BME280Controller::saveData() {
    // Récupération et formatage de l'horodatage des données.
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // Enregistrement des données
    if (isSensorAvailable()) {

        collectedData.timestamp = timestamp;
        collectedData.temp = sensor.obtenirTemperatureEnC();
        collectedData.press = sensor.obtenirPression();
        collectedData.hum = sensor.obtenirHumidite();

        printData();

        emit dataAvailable(collectedData);
    } else
        qCritical()
            << timestamp
            << ": le capteur BME280 n'est pas présent sur le bus I2C";
}
