#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include <chrono>

#include <QObject>
#include <QTimer>

#include "weather.h"

/**
 * @brief Classe abstraite SensorController - Hérite de QObject
 *
 * Permet d'unifier la récupération de données depuis différents capteurs. La
 * récupération peut alors se faire périodiquement, en passant une période > 0
 * au minuteur, mais aussi manuellement via l'appel à la fonction saveData().
 *
 * Dans chacun des cas, si la récupération des données est un succès, le
 * contrôleur émet un signal dataAvailable() contenant les données météos
 * relevées ainsi que leur date de relevé.
 */
class SensorController : public QObject {
        Q_OBJECT

    private:
        /**
         * @brief Contrôle la récupération périodique des données.
         */
        QTimer timer;

    protected:
        /**
         * @brief Contient les dernières données météorologiques relevées.
         */
        WeatherData collectedData;

        /**
         * @brief Écrit le contenu des dernières données relevées sur la sortie
         * de qInfo().
         */
        void printData();

    public:
        /**
         * @brief Construit un nouveau contrôleur de capteur.
         *
         * Si msec = 0 les données ne seront pas récupérées périodiquement.
         *
         * @param msec période de récupération des données (en milliseconde)
         * @param parent classe QObject parent de celle-ci
         */
        explicit SensorController(const std::chrono::milliseconds msec, QObject *parent = nullptr);

        /**
         * @brief Construit un nouveau contrôleur de capteur.
         *
         * Si msec = 0 les données ne seront pas récupérées périodiquement.
         *
         * @param msec période de récupération des données (en milliseconde)
         * @param parent classe QObject parent de celle-ci
         */
        explicit SensorController(const long msec, QObject *parent = nullptr);

        /**
         * @brief Règle le délai entre chaque récupération de données.
         *
         * Si msec = 0, arrête la récupération périodique des données.
         *
         * @param msec période de récupération des données (en milliseconde)
         */
        void updateTimerInterval(const std::chrono::milliseconds msec);

        /**
         * @brief Règle le délai entre chaque récupération de données.
         *
         * Si msec = 0, arrête la récupération périodique des données.

         * @param msec période de récupération des données (en milliseconde)
         */
        void updateTimerInterval(const long msec);

        /**
         * @brief Permet de savoir si le capteur est disponible et donc s'il est
         * possible de relever des données.
         * @return renvoie vrai si le capteur est disponible, sinon faux
         */
        virtual bool isSensorAvailable() = 0;

    signals:
        /**
         * @brief Signal émis lorsque de nouvelles données sont relevées par le
         * capteur.
         * @param collectedData contient les nouvelles données relevées
         */
        void dataAvailable(const WeatherData &collectedData);

    public slots:
        /**
         * @brief Effectue un relevé des données.
         *
         * En cas de succès, émet le signal dataAvailable.
         */
        virtual void saveData() = 0;
};

#endif // SENSORCONTROLLER_H
