#ifndef APPLICATION_H
#define APPLICATION_H

#include "client.h"
#include "sensorcontroller.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QString>

/**
 * @brief Classe Application
 *
 * Configure et exécute l'application.
 *
 * Afin d’initialiser correctement le programme, la classe Application doit
 * récupérer un certain nombre de paramètres depuis le fichier JSON passé au
 * constructeur. Ce fichier doit être avoir le contenu suivant :
 * {
 *   "adresseI2C": 119,
 *   "busI2C": 1,
 *   "minuteur": 900000,
 *   "idImmeuble": 1,
 *   "serveur": {
 *       "port": 80,
 *       "adresse": "127.0.0.1",
 *       "cheminAcces": "/Foo/Bar",
 *       "mdp": "Baz",
 *   }
 * }
 *
 * Le paramètre idImmeuble est le seul obligatoire, tandis que les autres
 * possèdent comme valeurs par défaut celles de l'exemple. À l'exception de
 * "mdp" et de "cheminAcces" qui sont, par défaut, vides.
}
 */
class Application {
    private:
        // Paramètres par défaut
        const static int defaultI2cAddress = 0x77;
        const static int defaultI2cBus = 1;
        const static int defaultServerPort = 80;
        const static QString defaultServerAddress;
        const static long defaultSensorTimer = 900000; // 15 minutes

        /**
         * @brief Boucle d'événement Qt.
         */
        QCoreApplication qtCore;

        /**
         * @brief Capteur utilisé pour la récupération des données.
         */
        SensorController* sensorController;

        /**
         * @brief Client chargé d'envoyer les données au serveur.
         */
        Client* client;

        /**
         * @brief Initialise l'application depuis un fichier de config JSON.
         *
         * Si une erreur survient lors de la lecture du fichier de config, une
         * FatalException est levée.
         *
         * @throws FatalException
         * @param jsonConfig fichier de configuration JSON
         */
        void init(QJsonDocument &jsonConfig);


    public:
        /**
         * @brief Construit l'application à l'aide des paramètres présents
         * dans configFile.
         *
         * Si une erreur survient lors de la lecture du fichier de config, une
         * FatalException est levée.
         *
         * @throws FatalException
         * @param argc nombre d'arguments passés au programme
         * @param argv tableau des arguments passés au programme
         * @param configFile fichier de configuration JSON
         */
        Application(int &argc, char *argv[], QFile &configFile);

        /**
         * @brief Construit l'application à l'aide des paramètres présents
         * dans configFile.
         *
         * Si une erreur survient lors de la lecture du fichier de config, une
         * FatalException est levée.
         *
         * @throws FatalException
         * @param argc nombre d'arguments passés au programme
         * @param argv tableau des arguments passés au programme
         * @param jsonConfig fichier de configuration JSON
         */
        Application(int &argc, char *argv[], QJsonDocument &jsonConfig);

        Application(const Application &app) = delete;

        /**
          * Destructeur de l'application
          */
        ~Application();

        /**
         * @brief Démarre la boucle d'événement Qt de l'application.
         * @return 0 si l'exécution s'est déroulée sans problème, toute autre
         * valeur indiquant qu'une erreur a eu lieu
         */
        int exec();
};

#endif // APPLICATION_H
