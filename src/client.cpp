#include "client.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QtDebug>

Client::Client(const ServerDesc &serverDesc, const int idBuilding, QObject *parent): QObject(parent) {
    // Enregistrement des données au format JSON
    jsonCache.insert("mdp", serverDesc.password);
    jsonCache.insert("idImmeuble", idBuilding);

    // Configuration de l'url du serveur
    serverAddress.setHost(serverDesc.address);
    serverAddress.setScheme("http");
    serverAddress.setPort(serverDesc.port);
    serverAddress.setPath(serverDesc.path);

    // Configuration de la connexion au serveur
    connect(&manager, &QNetworkAccessManager::finished, this, &Client::connectionFinished);
    manager.connectToHost(this->serverAddress.host(), static_cast<quint16>(this->serverAddress.port()));
}

void Client::uploadData(const WeatherData &data) {
    cacheData(data);
    uploadCache();
}

void Client::cacheData(const WeatherData &data) {
    // On travaille sur une copie du tableau, Qt ne permettant pas de le
    // modifier directement depuis l'objet SON
    QJsonArray cachedData(jsonCache["donnees"].toArray());

    cachedData.append(QJsonObject {
        {"horodatage", data.timestamp},
        {"temperature", data.temp},
        {"pression", data.press},
        {"humidite", data.hum}
    });

    jsonCache.insert("donnees", cachedData);
}

void Client::uploadCache() {
    qDebug() << "Envoi des données en cache au serveur";

    // Création de la requête d'envoi de données au serveur
    QNetworkRequest request(serverAddress);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Envoie du contenu du cache au serveur
    manager.post(request, QJsonDocument(jsonCache).toJson());
}

void Client::connectionFinished(QNetworkReply *finishedReply) {
    QNetworkReply::NetworkError error = finishedReply->error();

    if (!error) {
        qDebug() << "Succès de l'envoi des données au serveur";

        jsonCache.remove("donnees"); // Efface les données en cache
    } else
        qCritical() << "échec de l'envoie des données au serveur, code erreur :"
                    << static_cast<int>(error);

    // On prévient Qt que l'objet requête peut être supprimé
    finishedReply->deleteLater();
}
