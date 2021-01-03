#include "application.h"

#include "bme280controller.h"
#include "fatalexception.h"

#include <QJsonObject>

// Paramètre par défaut

const QString Application::defaultServerAddress = "127.0.0.1";


// Méthodes

Application::Application(int &argc, char *argv[], QFile &configFile): qtCore(argc, argv) {
    // Check si le fichier existe et est accessible en lecture
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString errorCode = QString::number(configFile.error());
        QString errorMsg("Échec de l'ouverture du fichier de configuration, code erreur : "+errorCode);

        throw FatalException(errorMsg);
    }

    QJsonDocument jsonConfig = QJsonDocument::fromJson(configFile.readAll());

    init(jsonConfig);
}

Application::Application(int &argc, char *argv[], QJsonDocument &jsonConfig): qtCore(argc, argv) {
    init(jsonConfig);
}

Application::~Application() {
    if (sensorController != nullptr)
        delete sensorController;

    if (client != nullptr)
        delete client;
}

void Application::init(QJsonDocument &jsonConfig) {
    // Check si le fichier est bien au format JSON
    if (jsonConfig.isNull())
        throw FatalException("Le fichier de configuration n'est pas au format JSON");

    // Check si le fichier est vide
    if (jsonConfig.isEmpty())
        throw FatalException("Le fichier de configuration est vide et au moins un des paramètres est nécessaire");

    // Récupération des paramètres
    QJsonObject params = jsonConfig.object();

    const int invalidId = -100;
    int idBuilding = params["idImmeuble"].toInt(invalidId);

    // Check si l'identifiant de l'immeuble du capteur est présent/valide
    if (idBuilding == invalidId) {
        QString errorMsg;
        const QString invalidParam = params["idImmeuble"].toVariant().toString();

        if (invalidParam.isEmpty())
            errorMsg = "Identifiant immeuble absent";
        else
            errorMsg = "Identifiant immeuble invalide : "+invalidParam;

        throw FatalException(errorMsg);
    }

    int i2cAdress = params["adresseI2C"].toInt(this->defaultI2cAddress);
    int i2cBus = params["busI2C"].toInt(this->defaultI2cBus);
    long timer = params["minuteur"].toInt(this->defaultSensorTimer);

    QJsonObject serverParams = params["serveur"].toObject();
    ServerDesc serverDesc {
        serverParams["port"].toInt(this->defaultServerPort), // port
        serverParams["adresse"].toString(this->defaultServerAddress), // address
        serverParams["cheminAcces"].toString(""), // path
        serverParams["mdp"].toString("") // password
    };

    // Configuration du capteur et du client
    this->sensorController = new BME280Controller(i2cAdress, i2cBus, timer, &this->qtCore);
    this->client = new Client(serverDesc, idBuilding, &this->qtCore);

    // Connexion du capteur avec le client
    QObject::connect(this->sensorController, &SensorController::dataAvailable, this->client, &Client::uploadData);

    qDebug() << "Application initialisée";
}

int Application::exec() {
    return qtCore.exec();
}
