#ifndef CLIENT_H
#define CLIENT_H

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QUrl>

#include "weather.h"

/**
 * @brief Structure ServerDesc
 *
 * Regroupe les informations nécessaires à l'établissement d'une connexion avec
 * un serveur distant.
 */
struct ServerDesc {
    /**
     * @brief port du serveur
     */
    int port;
    /**
     * @brief adresse du serveur
     */
    QString address;
    /**
     * @brief chemin d'accès au script du serveur
     */
    QString path;
    /**
     * @brief mot de passe autorisant l'enregistrement des données par le
     * serveur
     */
    QString password;
};

/**
 * @brief Classe Client - hérite de QObject
 *
 * Gère l’envoi des données météorologiques sur un serveur distant.
 *
 * Pour émettre des données en direction du serveur, il suffit d'appeler la
 * méthode uploadData ou alors de la connecter à un signal émettant des données
 * dans le même format.
 *
 * Lors de l'appelle de la méthode uploadData(), les données sont transmises au
 * serveur via la méthode POST et sous la forme d'un fichier JSON similaire à
 * celui-ci :
 *
 * {
 *   "mdp": "foo",
 *   "idImmeuble": 1,
 *   "donnees": [{
 *       "horodatage": "2020-01-01 00:00:00",
 *       "temperature": -1.76,
 *       "pression": 1000.6,
 *       "humidite": 77.0
 *   }, {
 *       "horodatage": "2020-01-01 00:15:00",
 *       "temperature": -2.0,
 *       "pression": 1000.5,
 *       "humidite": 77.3
 *   }, ...]
 * }
 */
class Client : public QObject {
        Q_OBJECT
    private:
        /**
         * @brief Gère l'envoi de données au serveur.
         */
        QNetworkAccessManager manager;

        /**
         * @brief Adresse url du serveur.
         */
        QUrl serverAddress;

        /**
         * @brief Cache des données à envoyer.
         */
        QJsonObject jsonCache;

        /**
         * @brief Enregistre le contenu de data dans le cache.
         * @param data données à enregistrer dans le cache
         */
        void cacheData(const WeatherData &data);

        /**
         * @brief Envoi le contenu du cache au serveur.
         *
         * Si l'envoi est un succès, les données présentes dans le cache sont
         * supprimées.
         */
        void uploadCache();

    public:
        /**
         * @brief Construit un nouveau client.
         * @param serverDesc permet la configuration de la communication avec le
         * serveur
         * @param idBuilding identifiant de l'immeuble auquel appartiennent les
         * données
         * @param parent classe QObject parent de celle-ci
         */
        explicit Client(const ServerDesc &serverDesc, const int idBuilding, QObject *parent = nullptr);

    public slots:
        /**
         * @brief Envoie au serveur le contenu de data ainsi que les précédentes
         * données dont l'envoi avait échoué.
         *
         * @param data nouvelles données à envoyer au serveur
         */
        void uploadData(const WeatherData &data);

    private slots:
        /**
         * @brief Méthode exécutée lors de la fin de transmission d'une requête
         * au serveur.
         *
         * Si l'envoi des données est un succès, elles sont effacées du cache.
         * Sinon un message d'erreur contenant les raisons de l'échec est
         * affiché sur la sortie de qCritical().
         *
         * @param pendingReply réponse du serveur
         */
        void connectionFinished(QNetworkReply *finishedReply);
};

#endif // CLIENT_H
