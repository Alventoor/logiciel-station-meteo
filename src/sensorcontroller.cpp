#include "sensorcontroller.h"

#include <QtDebug>

SensorController::SensorController(const std::chrono::milliseconds msec, QObject *parent):
    QObject(parent),
    timer(this)
{
    connect(&timer, &QTimer::timeout, this, &SensorController::saveData);

    updateTimerInterval(msec);
}

SensorController::SensorController(const long msec, QObject *parent):
    SensorController(std::chrono::milliseconds(msec), parent) { }

void SensorController::updateTimerInterval(const std::chrono::milliseconds msec) {
    const long nbTicks = msec.count();

    if (nbTicks == 0)
        timer.stop();
    else if (nbTicks > 0)
        timer.start(msec);
    else
        qCritical() << "L'intervalle du minuteur est invalide (valeur négative) :"
                    << nbTicks;
}

void SensorController::updateTimerInterval(const long msec) {
    updateTimerInterval(std::chrono::milliseconds(msec));
}


void SensorController::printData() {
    qInfo()
        << "Données enregistrées le"
        << collectedData.timestamp << ":"
        << collectedData.temp << "°C"
        << collectedData.press << "hPa"
        << collectedData.hum << "%";

}
