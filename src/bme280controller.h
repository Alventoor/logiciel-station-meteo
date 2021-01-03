#ifndef BME280CONTROLLER_H
#define BME280CONTROLLER_H

#include "BME280/bme280.h"
#include "sensorcontroller.h"

/**
 * @brief Classe BME280Controller - Hérite de SensorController
 *
 * Permet la récupération de données météorologiques depuis un capteur BME280.
 *
 * Ces données peuvent être récupérées de manière périodique, en configurant le
 * minuteur hérité de la classe SensorController, ou alors manuellement en
 * appelant la fonction saveData().
 *
 * Dans chacun des cas, si la récupération des données est un succès, le
 * contrôleur émet un signal dataAvailable() contenant les données météos
 * relevées ainsi que leur date de relevé.
 */
class BME280Controller : public SensorController {
    private:
        /**
         * @brief Capteur BME280.
         */
        bme280 sensor;

    public:
        /**
         * @brief Construit un nouveau contrôleur de capteur BME280.
         *
         * Si msec > 0 les données seront récupérées toutes les msec.
         *
         * @param i2cAddress adresse i2c du capteur
         * @param i2cBus numéro du bus i2c sur lequel est branché le capteur
         * @param msec période de récupération des données (en milliseconde)
         * @param parent classe QObject parent de celle-ci
         */
        explicit BME280Controller(const int i2cAddress, const int i2cBus,
                                  const std::chrono::milliseconds msec, QObject *parent = nullptr);
        /**
         * @brief Construit un nouveau contrôleur de capteur BME280.
         *
         * Si msec > 0 les données seront récupérées toutes les msec.
         *
         * @param i2cAddress adresse i2c du capteur
         * @param i2cBus numéro du bus i2c sur lequel est branché le capteur
         * @param msec période de récupération des données (en milliseconde)
         * @param parent classe QObject parent de celle-ci
         */
        explicit BME280Controller(const int i2cAddress, const int i2cBus,
                                  const long msec, QObject *parent = nullptr);

        /**
         * @brief Permet de savoir si le capteur est disponible et donc s'il est
         * possible de relever des données.
         * @return renvoie vrai si le capteur est disponible, sinon faux
         */
        bool isSensorAvailable() override;

    public slots:
        /**
         * @brief Effectue un relevé des données.
         *
         * En cas de succès, émet le signal dataAvailable.
         */
        void saveData() override;
};

#endif // BME280CONTROLLER_H
