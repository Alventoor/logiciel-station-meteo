#include "application.h"
#include "fatalexception.h"

#include <cstdio>
#include <cstdlib>

#include <QFile>
#include <QtDebug>

/**
 * @brief Fonction ayant comme rôle l'affichage des messages de log. Tous les
 * messages reçus sont affichés sur la sortie d'erreur.
 * @param type type du message reçu
 * @param context information sur le contexte du message - inutilisé
 * @param msg message reçu
 */
void debugMessageHandler(QtMsgType type, const QMessageLogContext&, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();

    switch(type) {
        case QtMsgType::QtDebugMsg:
            fprintf(stderr, "Debug - %s\n", localMsg.constData());
            break;
        case QtMsgType::QtInfoMsg:
            fprintf(stderr, "Info - %s\n", localMsg.constData());
            break;
        case QtMsgType::QtWarningMsg:
            fprintf(stderr, "Warning - %s\n", localMsg.constData());
            break;
        case QtMsgType::QtCriticalMsg:
            fprintf(stderr, "Erreur - %s\n", localMsg.constData());
            break;
        case QtMsgType::QtFatalMsg:
            fprintf(stderr, "Erreur fatale - %s\n", localMsg.constData());
    }
}

/**
 * @brief Fonction ayant comme rôle l'affichage des messages de log. Tous les
 * messages reçus, à l'exception des QtDebugMsg, sont affichés sur la sortie
 * d'erreur.
 * @param type type du message reçu
 * @param context information sur le contexte du message - inutilisé
 * @param msg message reçu
 */
void verboseMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (type != QtMsgType::QtDebugMsg)
        debugMessageHandler(type, context, msg);
}

/**
 * @brief Fonction ayant comme rôle l'affichage des messages de log. Tous les
 * messages reçus à l'exception des QtInfoMsg et QtDebugMsg, sont affichés sur
 * la sortie d'erreur.
 * @param type type du message reçu
 * @param context information sur le contexte du message - inutilisé
 * @param msg message reçu
 */
void defaultMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (type != QtMsgType::QtInfoMsg)
        verboseMessageHandler(type, context, msg);
}


int main(int argc, char *argv[]) {
    // Configuration du gestionnaire de log
    qInstallMessageHandler(defaultMessageHandler);

    bool debugMode = false;
    QString configName("config.json"); // Nom du fichier de config par défaut
    QString argument;

    // Traitement des paramètres passés au programme
    for (int i = 0; i < argc; i++) {
        argument = argv[i];

        // Récupération du fichier de config
        if ((argument == "-c" || argument == "--config") && i + 1 < argc)
            configName = argv[i + 1];
        // Réglage du système de log en mode verbeux
        else if ((argument == "-v" || argument == "--verbose") && !debugMode)
            qInstallMessageHandler(verboseMessageHandler);
        // Réglage du système de log en mode debug
        else if (argument == "-d" || argument == "--debug") {
            debugMode = true;
            qInstallMessageHandler(debugMessageHandler);
        }
    }

    QFile file(configName);

    try {
        Application app(argc, argv, file);

        return app.exec();
    }  catch (const FatalException &ex) {
        qFatal("%s", ex.what());

        return EXIT_FAILURE;
    }
}
